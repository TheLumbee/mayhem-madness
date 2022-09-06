#ifndef TEAM_H
#define TEAM_H

#include <QHash>

class Division;

class Team
{
public:
    Team(){}
    ~Team(){}
    double avgFinishingPosition = 0,
        avgWinPerc = 0,
        avgPointsFor = 0;
    QList<QString> teamSchedule;
    QHash<QString, bool> nonDivisionMatchups;
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
    void SetDivision(Division* d);
    bool IsTeamInDivision(Team* otherTeam);
    bool RemoveNonPlayedTeam();

private:
    double teamWeight = 0;
    Division* teamDivision = nullptr;
};

#endif // TEAM_H