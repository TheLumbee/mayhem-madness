#include "league.h"

#include <algorithm>
#include <cmath>
#include <QDebug>
#include <QFile>
#include <QRandomGenerator>
#include <QTextStream>

League::League(const QString& teamStatsFile)
{
    QFile* dataFile = new QFile(teamStatsFile);
    if (!dataFile->open(QIODevice::ReadOnly))
    {
        return;
    }

    QTextStream in(dataFile);
    while (!in.atEnd())
    {
        QStringList data = in.readLine().split(',');
        Team* t = new Team();
        t->name = data.at(0);
        t->avgFinishingPosition = data.at(1).toDouble();
        t->avgWinPerc = data.at(2).toDouble();
        t->avgPointsFor = data.at(3).toDouble();
        leagueTeams.append(t);
    }
}

QString League::PrintTeams()
{
    QString data = "\n";
    for (auto ii : leagueTeams)
    {
        data += ii->PrintTeam();
        data += '\n';
    }

    return data;
}

void League::WeighTeams()
{
    for (auto ii : leagueTeams)
    {
        ii->GetTeamWeight();
    }

    std::sort(leagueTeams.begin(), leagueTeams.end(), [](Team* a, Team* b)
    {
        return a->GetTeamWeight() > b->GetTeamWeight();
    });
}

void League::CreateDivisions()
{
    double lowestMargin = 0xffffffff;
    Division bestEast("Best East");
    Division bestWest("Best West");
    for (int ii = 0; ii < 1'000'000; ++ii)
    {
        QList<Team*> allTeams = leagueTeams;
        Division east("East");
        Division west("West");
        for (int jj = 0; jj < 6; ++jj)
        {
            int num = QRandomGenerator::global()->bounded(allTeams.size());
            Team* chosen = allTeams.at(num);
            east.AddTeam(chosen);
            allTeams.removeOne(chosen);
        }

        for (int jj = 0; jj < allTeams.size(); ++jj)
        {
            west.AddTeam(allTeams.at(jj));
        }

        double margin = std::abs(east.GetAverageTeamStrength() - west.GetAverageTeamStrength());
        if (margin < lowestMargin)
        {
            lowestMargin = margin;
            bestEast = east;
            bestWest = west;
        }
    }

    eastDivision = new Division(bestEast);
    eastDivision->CreateDivisionalMatchups();
    westDivision = new Division(bestWest);
    westDivision->CreateDivisionalMatchups();
}

Team* League::FindTeam(const QString& teamName)
{
    for (auto t : leagueTeams)
    {
        if (t->name == teamName)
        {
            return t;
        }
    }

    return nullptr;
}

Team* League::GetLastTeam(const QSet<QString>& otherTeams, Team* currentTeam)
{
    for (auto ii :leagueTeams)
    {
        if (!otherTeams.contains(ii->name) &&
            ii != currentTeam)
        {
            return ii;
        }
    }

    return nullptr;
}

void League::SetNonDivisionalMatchups()
{
    qDebug().noquote() << "Setting non-divisional matchups...";
    Division east(*eastDivision);
    Division west(*westDivision);
    for (int ii = 0; ii < east.divisionTeams.size(); ii++)
    {
        if (ii <= 2)
        {
            for (int jj = 0; jj < 3; jj++)
            {
                east.divisionTeams.at(ii)->nonDivisionMatchups.insert
                    (west.divisionTeams.at(jj)->name, false);
                west.divisionTeams.at(jj)->nonDivisionMatchups.insert
                    (east.divisionTeams.at(ii)->name, false);
                east.divisionTeams.at(ii)->teamSchedule.append(west.divisionTeams.at(jj)->name);
                west.divisionTeams.at(jj)->teamSchedule.append(east.divisionTeams.at(ii)->name);
            }
        }

        else
        {
            for (int jj = 3; jj < west.divisionTeams.size(); jj++)
            {
                east.divisionTeams.at(ii)->nonDivisionMatchups.insert
                    (west.divisionTeams.at(jj)->name, false);
                west.divisionTeams.at(jj)->nonDivisionMatchups.insert
                    (east.divisionTeams.at(ii)->name, false);
                east.divisionTeams.at(ii)->teamSchedule.append(west.divisionTeams.at(jj)->name);
                west.divisionTeams.at(jj)->teamSchedule.append(east.divisionTeams.at(ii)->name);
            }
        }
    }
}

void League::CreateWeekMatchups()
{
    QList<QPair<QString, QString>> matchups;
    QSet<QString> teamsDecided;
    for (int ii = 0; ii < leagueTeams.size(); ii++)
    {
        Team* currentTeam = leagueTeams.at(ii);
        if (teamsDecided.contains(currentTeam->name))
        {
            continue;
        }

        int rand = QRandomGenerator::global()->bounded(currentTeam->teamSchedule.size());
        Team* otherTeam = FindTeam(currentTeam->teamSchedule.at(rand));
        int temp = 0;
        while (teamsDecided.contains(otherTeam->name))
        {
            if (temp == currentTeam->teamSchedule.size())
            {
                otherTeam = GetLastTeam(teamsDecided, currentTeam);
                if (!currentTeam->IsTeamInDivision(otherTeam))
                {
                    if (!currentTeam->nonDivisionMatchups.contains(otherTeam->name))
                    {
                        if (!currentTeam->RemoveNonPlayedTeam())
                        {
                            qDebug() << "No team to remove.";
                            std::exit(1);
                        }

                        if (!otherTeam->nonDivisionMatchups.contains(currentTeam->name))
                        {
                            if (!otherTeam->RemoveNonPlayedTeam())
                            {
                                qDebug() << "Other team unable to remove";
                                std::exit(1);
                            }

                            otherTeam->nonDivisionMatchups.insert(currentTeam->name, false);
                        }

                        otherTeam->nonDivisionMatchups[currentTeam->name] = true;
                        currentTeam->nonDivisionMatchups.insert(otherTeam->name, true);
                    }
                }

                break;
            }

            temp++;
            rand = QRandomGenerator::global()->bounded(currentTeam->teamSchedule.size());
            otherTeam = FindTeam(currentTeam->teamSchedule.at(rand));
        }

        matchups.append(qMakePair(currentTeam->name, otherTeam->name));
        teamsDecided.insert(currentTeam->name);
        teamsDecided.insert(otherTeam->name);
        currentTeam->teamSchedule.removeOne(otherTeam->name);
        otherTeam->teamSchedule.removeOne(currentTeam->name);
        qDebug().noquote() << currentTeam->name << " vs " << otherTeam->name;
    }

    leagueSchedule.append(matchups);
}

void League::CreateSchedule()
{
    qDebug().noquote() << "Creating schedule... ";
    for (int ii = 0; ii < 13; ii++)
    {
        qDebug().noquote() << QString("\n\n *** Week %1 ***").arg(ii + 1);
        CreateWeekMatchups();
    }
}

