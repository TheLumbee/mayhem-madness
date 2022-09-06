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
    void CreateDivisionalMatchups();
    QList<Team*> divisionTeams;

private:
    double averageTeamStrength = 0;
    QString name;
};

#endif // DIVISION_H