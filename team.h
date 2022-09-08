#ifndef TEAM_H
#define TEAM_H

#include <qhash.h>

class Division;
class League;

class Team
{
public:
    Team(League* l);
    ~Team(){}
    double avgFinishingPosition = 0,
        avgWinPerc = 0,
        avgPointsFor = 0;
    QHash<Team*, int> matchups;
    QList<Team*> teamSchedule;
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

    bool operator !=(const Team& otherTeam) const
    {
        return this->name != otherTeam.name;
    }
    
    QString PrintTeam();
    void SetDivision(Division* d);
    bool IsTeamInDivision(Team* otherTeam);
    QString PrintSchedule();
    bool GenerateMatchups();
    double GetScheduleStrength();
    QString PrintWeek(); 

private:
    double teamWeight = 0;
    Division* teamDivision = nullptr;
    League* teamLeague = nullptr;
    double scheduleStrength = 0;
    quint8 numOfBackToBacks = 0;
};

#endif // TEAM_H