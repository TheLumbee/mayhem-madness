#include "league.h"

#include "division.h"
#include "team.h"

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
        Team* t = new Team(this);
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

    eastDivision = new Division("East");
    for (auto t : bestEast.divisionTeams)
    {
        eastDivision->AddTeam(t);
    }

    westDivision = new Division("West");
    for (auto t : bestWest.divisionTeams)
    {
        westDivision->AddTeam(t);
    }
}

void League::CreateSchedule()
{
    double lowest = 0xffffffff;
    QString bestSchedule;
    while (lowest > 35)
    {
        bool test = false;
        for (auto team : leagueTeams)
        {
            test = team->GenerateMatchups();
        }

        if (test)
        {
            double diff = GetScheduleDiff();
            if (diff < lowest)
            {
                lowest = diff;
                bestSchedule = "";
                for (auto team : leagueTeams)
                {
                    bestSchedule += "\n\n" + team->PrintSchedule();
                }
            }
        }

        for (auto team : leagueTeams)
        {
            team->teamSchedule.clear();
            team->matchups.clear();
        }
    }

    qDebug().noquote() << bestSchedule;
}

Team* League::FindTeam(const QString& name)
{
    for (auto t : leagueTeams)
    {
        if (t->name == name)
        {
            return t;
        }
    }

    return nullptr;
}

void League::PrintSchedule()
{
    for (auto team : leagueTeams)
    {
        qDebug() << "\n";
        team->PrintSchedule();
    }
}

double League::GetScheduleDiff()
{
    std::sort(leagueTeams.begin(), leagueTeams.end(), [](Team* a, Team* b)
    {
        return a->GetScheduleStrength() > b->GetScheduleStrength();
    });
    
    return std::abs(leagueTeams.first()->GetScheduleStrength() - leagueTeams.last()->GetScheduleStrength());
}

