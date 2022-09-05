#include "team.h"

const double FINAL_STANDING_WEIGHT = 0.20;
const double WIN_PERC_WEIGHT = 0.20;
const double POINTS_FOR_WEIGHT = 0.60;

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
