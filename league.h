#ifndef LEAGUE_H
#define LEAGUE_H

#include "division.h"

class League
{
public:
    League(const QString& teamStatsFile);
    QString PrintTeams();
    void WeighTeams();
    void CreateDivisions();
    void SetNonDivisionalMatchups();
    void CreateWeekMatchups();
    void CreateSchedule();

private:
    Division* eastDivision = nullptr;
    Division* westDivision = nullptr;
    QList<QPair<QString, QString> > leagueSchedule;
    QList<Team*> leagueTeams;
    Team* FindTeam(const QString& teamName);
};

#endif // LEAGUE_H
