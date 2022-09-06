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
    t->SetDivision(this);
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

void Division::CreateDivisionalMatchups()
{
    for (int ii = 0; ii < divisionTeams.size() - 1; ++ii)
    {
        for (int jj = ii + 1; jj < divisionTeams.size(); ++jj)
        {
            for (int kk = 0; kk < 2; ++kk)
            {
                divisionTeams.at(ii)->teamSchedule.append(divisionTeams.at(jj)
                    ->name);
                divisionTeams.at(jj)->teamSchedule.append(divisionTeams.at(ii)
                    ->name);
            }
        }
    }
}
