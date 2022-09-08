#ifndef DIVISION_H
#define DIVISION_H

#include <qlist.h>
#include <qstring.h>

class Team;

class Division
{
public:
    Division(const QString& name);
    double GetAverageTeamStrength();
    void AddTeam(Team* t);
    QString PrintDivision();
    QList<Team*> divisionTeams;

private:
    double averageTeamStrength = 0;
    QString name;
};

#endif // DIVISION_H