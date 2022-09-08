#ifndef LEAGUE_H
#define LEAGUE_H

#include <qlist.h>
#include <qset.h>
#include <qstring.h>

class Division;
class Team;

class League
{
public:
    League(const QString& teamStatsFile);
    QString PrintTeams();
    void WeighTeams();
    void CreateDivisions();
    void CreateSchedule();
    QList<Team*> leagueTeams;
    Division* eastDivision = nullptr;
    Division* westDivision = nullptr;

private:
    QList<QList<QPair<QString, QString>>> leagueSchedule;
    Team* FindTeam(const QString& name);
    void PrintSchedule();
    double GetScheduleDiff();
};

#endif // LEAGUE_H
