#include "league.h"

#include <QDebug>

int main()
{
    League l("teamStats.csv");
    l.WeighTeams();
    return 0;
}