#ifndef LEAGUE_H
#define LEAGUE_H

#include "division.h"

class League
{
public:
    League(const QString& teamStatsFile);
    QString PrintTeams();
    void WeighTeams();

private:
    QPair<Division*, Division*> leagueDivisions;
    QList<QPair<QString, QString> > leagueSchedule;
    QList<Team*> leagueTeams;
    void CreateDivisions();
};

#endif // LEAGUE_H
