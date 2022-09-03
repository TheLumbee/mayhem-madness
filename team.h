#ifndef TEAM_H
#define TEAM_H

#include <QSet>

class Team
{
public:
    Team(){}
    ~Team(){}
    double avgFinishingPosition = 0,
        avgWinPerc = 0,
        avgPointsFor = 0;
    QList<QString> teamSchedule;
    QSet<QString> nonDivisionMatchups;
    QString name;
    double GetTeamWeight();

    bool operator <(const Team& otherTeam) const
    {
        return this->teamWeight < otherTeam.teamWeight;
    }

    bool operator >(const Team& otherTeam) const
    {
        return this->teamWeight > otherTeam.teamWeight;
    }
    
    QString PrintTeam();

private:
    double teamWeight = 0;
};

#endif // TEAM_H