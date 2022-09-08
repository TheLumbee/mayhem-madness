#include "team.h"

#include "division.h"
#include "league.h"

#include <algorithm>
#include <qdebug.h>
#include <qrandom.h>
#include <random>
#include <time.h>

const double FINAL_STANDING_WEIGHT = 0.20;
const double WIN_PERC_WEIGHT = 0.20;
const double POINTS_FOR_WEIGHT = 0.60;

Team::Team(League* l) :
    teamLeague(l)
{
    teamSchedule.resize(13);
}

double Team::GetTeamWeight()
{
    if (!teamWeight)
    {
        teamWeight = (FINAL_STANDING_WEIGHT / avgFinishingPosition) + ((avgWinPerc * 100) * 
            WIN_PERC_WEIGHT) + (avgPointsFor * POINTS_FOR_WEIGHT);
    }

    return teamWeight;
}

QString Team::PrintTeam()
{
    return QString("%1: %2")
        .arg(name).arg(teamWeight);
}

void Team::SetDivision(Division* d)
{
    teamDivision = d;
}

bool Team::IsTeamInDivision(Team* otherTeam)
{
    for (Team* team : teamDivision->divisionTeams)
    {
        if (team == otherTeam)
        {
            return true;
        }
    }

    return false;
}

QString Team::PrintSchedule()
{
    QString temp;
    for (int ii = 0; ii < teamSchedule.size(); ii++)
    {
        temp += QString("Week %1: %2 vs %3\n").arg(ii + 1)
            .arg(name).arg(teamSchedule.at(ii)->name);
    }

    temp += QString::number(scheduleStrength);
    return temp;
}

bool Team::GenerateMatchups()
{
    Division* otherDivision = (teamDivision == teamLeague->eastDivision) ?
        teamLeague->westDivision : teamLeague->eastDivision;
    int rand = 0;
    for (qsizetype ii = 0; ii < teamSchedule.size(); ii++)
    {
        if (teamSchedule.at(ii))
        {
            continue;
        }

        int temp = 0;
        while (1)
        {
            temp++;
            if (temp >= 100000)
            {
                return false;
            }

            bool inDivision = QRandomGenerator::global()->generateDouble() <= 0.85;
            rand = QRandomGenerator::global()->bounded(6);
            Team* otherTeam = inDivision ?
                teamDivision->divisionTeams.at(rand) :
                otherDivision->divisionTeams.at(rand);
            if (otherTeam == this)
            {
                continue;
            }

            if (teamSchedule.size() >= 3)
            {
                // No 3 games in a row
                if (ii >= 2 &&
                    teamSchedule.at(ii - 1) == otherTeam &&
                    teamSchedule.at(ii - 2) == otherTeam)
                {
                    continue;
                }

                // Each team only has 2 back to backs at most
                if (ii >= 1 &&
                    teamSchedule.at(ii - 1) == otherTeam &&
                    numOfBackToBacks == 2)
                {
                    continue;
                }
            }

            // If other team already has game here
            if (otherTeam->teamSchedule.at(ii))
            {
                continue;
            }

            // If it's in division, no more than 3 matchups
            if (inDivision && matchups.value(otherTeam) == 3)
            {
                continue;
            }

            // If not in division, no more than 2 matchups
            if (!inDivision && matchups.value(otherTeam) == 2)
            {
                continue;
            }

            teamSchedule[ii] = otherTeam;
            otherTeam->teamSchedule[ii] = this;
            matchups[otherTeam]++;
            otherTeam->matchups[this]++;
            if (ii >= 1 &&
                teamSchedule.at(ii - 1) == otherTeam)
            {
                numOfBackToBacks++;
            }

            break;
        }
    }

    return true;
}

double Team::GetScheduleStrength()
{
    if (!scheduleStrength)
    {
        for (auto team : teamSchedule)
        {
            scheduleStrength += team->GetTeamWeight();
        }
    }

    return scheduleStrength / teamSchedule.size();
}
