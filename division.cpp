#include "division.h"

Division::Division(const QString& name)
{
    this->name = name;
}

double Division::GetAverageTeamStrength()
{
    if (!averageTeamStrength)
    {
        double temp = 0;
        for (auto ii : divisionTeams)
        {
            temp += ii->GetTeamWeight();
        }

        averageTeamStrength = temp / divisionTeams.size();
    }

    return averageTeamStrength;
}

void Division::AddTeam(Team* t)
{
    divisionTeams.append(t);
}

QString Division::PrintDivision()
{
    QString teams;
    for (auto ii : divisionTeams)
    {
        teams += ii->name;
        teams += '\n';
    }

    return QString("%1 Strength: %2\n%3")
        .arg(name).arg(GetAverageTeamStrength()).arg(teams);
}
