#include "league.h"

#include <qdebug.h>

int main()
{
    League l("teamStats.csv");
    l.WeighTeams();
    l.CreateDivisions();
    l.CreateSchedule();
    return 0;
}