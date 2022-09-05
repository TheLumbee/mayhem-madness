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
    westDivision = new Division(bestWest);
}

