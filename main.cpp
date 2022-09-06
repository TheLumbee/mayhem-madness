#include "league.h"

#include <QDebug>

int main()
{
    League l("teamStats.csv");
    l.WeighTeams();
    l.CreateDivisions();
    l.SetNonDivisionalMatchups();
    l.CreateSchedule();
    return 0;
}