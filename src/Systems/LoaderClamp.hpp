#pragma once

#include "pros/adi.hpp"
#include "../Constants.hpp"

using namespace pros::adi;
using namespace Constants;

class LoaderClamp {
    private:
        DigitalOut clampLock = DigitalOut(clamp_p, true);
        bool stateClamp = true;

    public:
        LoaderClamp(){}
        void toggleClampLock(){stateClamp=!stateClamp; clampLock.set_value(stateClamp);}
};