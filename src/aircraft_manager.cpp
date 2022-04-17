#include "aircraft_manager.hpp"

void AircraftManager::move(float delta)
{
    assert(delta >= 0.0f && "delta must be positive");
    // sorting
    std::sort(aircrafts.begin(), aircrafts.end(),
              [](const auto& a, const auto& b)
              {
                  if (a->has_terminal())
                  {
                      return true;
                  }
                  if (b->has_terminal())
                  {
                      return false;
                  }
                  return a->get_fuel() < b->get_fuel();
              });

    const auto& pred = [this, delta](const auto& a)
    {
        if (a->must_remove())
        {
            return true;
        }
        try
        {
            a->move(delta);
            return false;
        } catch (const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            crashed_aircrafts++;
            return true;
        }
    };
    aircrafts.erase(std::remove_if(aircrafts.begin(), aircrafts.end(), pred), aircrafts.end());
}

void AircraftManager::add_aircraft(std::unique_ptr<Aircraft>& aircraft)
{
    assert(aircraft != nullptr && "aircraft is nullptr");
    aircrafts.emplace_back(std::move(aircraft));
}

int AircraftManager::get_required_fuel() const
{
    return std::accumulate(aircrafts.begin(), aircrafts.end(), 0,
                           [](int acc, const auto& a)
                           {
                               if (!a->is_at_terminal || !a->is_low_on_fuel())
                               {
                                   return acc;
                               }
                               return acc + (Aircraft::MAX_FUEL - a->get_fuel());
                           });
}
int AircraftManager::get_crashed_aircrafts() const
{
    return crashed_aircrafts;
}
