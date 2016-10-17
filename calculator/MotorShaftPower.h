//
// Created by Accawi, Gina K. on 6/17/16.
//

#ifndef AMO_LIBRARY_MOTORSHAFTPOWER_H
#define AMO_LIBRARY_MOTORSHAFTPOWER_H

#include <cmath>
#include<iostream>
#include "../motor.h"
#include "../FieldData.h"

class MotorShaftPower {
public:
    double tempLoadFraction_ = 0.01;
    double power = 0.0, powerE1 = 0.0, powerE2 = 0.0;
    double lf1 = 0.0, lf2 = 0.0;
    double eff = 0.0, eff1 = 0.0, eff2 = 0.0;
    double current = 0.0, current1 = 0.0, current2 = 0.0;
    double pf = 0.0, pf1 = 0.0, pf2 = 0.0;
    double estimatedFLA;

    MotorShaftPower(double motorRatedPower, double motorMeasuredPower, int motorRPM,
                    Motor::LineFrequency lineFrequency,
                    Motor::EfficiencyClass efficiencyClass,
                    double specifiedEfficiency,
                    double ratedVoltage, double fullLoadAmps, double fieldVoltage,
                    FieldData::LoadEstimationMethod loadEstimationMethod, double fieldCurrent)
            :
            fieldPower_(motorMeasuredPower),
            motorRatedPower_(motorRatedPower), motorRPM_(motorRPM), lineFrequency_(lineFrequency),
            efficiencyClass_(efficiencyClass), specifiedEfficiency_(specifiedEfficiency),
            ratedVoltage_(ratedVoltage), fullLoadAmps_(fullLoadAmps), fieldVoltage_(fieldVoltage),
            loadEstimationMethod_(loadEstimationMethod), fieldCurrent_(fieldCurrent) {};

    double calculate();

    double calculateCurrent();

    double calculateEfficiency();

    double calculatePowerFactor();

    double calculatePower();

    double calculateEstimatedFLA();

private:
    double motorRatedPower_ = 0.0;
    double fieldPower_ = 0.0;
    int motorRPM_ = 0;
    Motor::LineFrequency lineFrequency_;
    double ratedVoltage_ = 0.0;
    double fullLoadAmps_ = 0.0;
    double fieldVoltage_ = 0.0;
    Motor::EfficiencyClass efficiencyClass_;

    double specifiedEfficiency_;
    double motorShaftPower_ = 0.0;
    double fieldCurrent_ = 0.0;
    FieldData::LoadEstimationMethod loadEstimationMethod_;
};

#endif //AMO_LIBRARY_MOTORSHAFTPOWER_H
