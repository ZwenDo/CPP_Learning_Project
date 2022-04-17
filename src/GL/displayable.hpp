#pragma once

#include <vector>
#include <algorithm>
#include <iostream>
#include <cassert>

namespace GL {

// is displayed before whom ;]

// a displayable object can be displayed and has a z-coordinate indicating who
class Displayable;

inline std::vector<const Displayable*> display_queue;

class Displayable
{
protected:
    float z = 0;
    long index;

public:
    Displayable(const float z_) : z { z_ } {
        index = display_queue.size();
        display_queue.push_back(this);
    }
    virtual ~Displayable() {
        display_queue.erase(std::find(display_queue.begin(), display_queue.end(), this));
    }

    virtual void display() const = 0;

    float get_z() const { return z; }
};

struct disp_z_cmp
{
    bool operator()(const Displayable* a, const Displayable* b) const
    {
        assert (a != nullptr && "a is nullptr");
        assert (b != nullptr && "b is nullptr");
        const auto az = a->get_z();
        const auto bz = b->get_z();
        return (az == bz) ? (a > b) : (az > bz);
    }
};

} // namespace GL
