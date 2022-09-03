#ifndef DIVISION_H
#define DIVISION_H

#include "team.h"

class Division
{
public:
    Division(const QString& name);
    double GetAverageTeamStrength();
    void AddTeam(Team* t);
    QString PrintDivision();

private:
    double averageTeamStrength = 0;
    QList<Team*> divisionTeams;
    QString name;
};

#endif // DIVISION_H