//
// Created by Accawi, Gina K. on 6/17/16.
//

#include "PSATResult.h"
#include "calculator/MotorShaftPower.h"
#include "calculator/PumpShaftPower.h"
#include "calculator/PumpEfficiency.h"
#include "calculator/AnnualCost.h"
#include "calculator/AnnualEnergy.h"
#include "calculator/OptimalPumpEfficiency.h"
#include "calculator/OptimalPumpShaftPower.h"
#include "calculator/OptimalMotorShaftPower.h"
#include "calculator/OptimalMotorPower.h"
#include "calculator/OptimalMotorSize.h"


double PSATResult::calculateExisting() {

    MotorShaftPower motorShaftPower(motor_.getMotorRatedPower(), fieldData_.getMotorPower(), motor_.getMotorRpm(), motor_.getLineFrequency(),
                                    motor_.getEfficiencyClass(), motor_.getSpecifiedEfficiency(), motor_.getMotorRatedVoltage(), motor_.getFullLoadAmps(),
                                    fieldData_.getVoltage(), fieldData_.getLoadEstimationMethod(),
                                    fieldData_.getMotorAmps());
    existing_.motorShaftPower_ = motorShaftPower.calculate();
    existing_.motorCurrent_ = motorShaftPower.calculateCurrent();
    existing_.motorPowerFactor_ = motorShaftPower.calculatePowerFactor();
    existing_.motorEfficiency_ = motorShaftPower.calculateEfficiency();
    //existing_.motorPower_ = motorShaftPower.calculateElectricPower();
    existing_.motorRatedPower_ = motor_.getMotorRatedPower();
    existing_.motorPower_ = motorShaftPower.calculatePower();
    existing_.estimatedFLA_ = motorShaftPower.calculateEstimatedFLA();
    // Calculate PumpShaftPower
    PumpShaftPower pumpShaftPower(existing_.motorShaftPower_, pump_.getDrive());
    existing_.pumpShaftPower_ = pumpShaftPower.calculate();

    // Calculate Pump Efficiency
    PumpEfficiency pumpEfficiency(pump_.getSg(), fieldData_.getFlowRate(), fieldData_.getHead(),
                                  existing_.pumpShaftPower_);
    existing_.pumpEfficiency_ = pumpEfficiency.calculate();

    // Calculate Annual Energy
    AnnualEnergy annualEnergy(existing_.motorPower_, financial_.getOperatingFraction());
    existing_.annualEnergy_ = annualEnergy.calculate();

    // Calculate Annual Cost
    AnnualCost annualCost(existing_.annualEnergy_, financial_.getUnitCost());
    existing_.annualCost_ = annualCost.calculate();

    return 0;
}

double PSATResult::calculateOptimal() {
    /*
    Calculate optimal pump efficiency, fluid power and pump shaft power
    If a belt drive is specified, calculate the motor shaft power
    If direct drive, motor shaft power = pump shaft power
    Select motor size based on required motor shaft power
    Develop 25% interval motor performance data for EE motor of the selected size *
    Do curve fitting of current from 25% to 1% intervals
    Do curve fitting of efficiency in 1% intervals
    Using current and efficiency 1% interval data, calculate balance of motor data in 1% intervals
    Calculate required power, motor eff., current, pf from shaft power
    Calculate annual energy and energy cost
    Calculate annual savings potential and optimization rating
     */

    OptimalPumpEfficiency optimalPumpEfficiency(pump_.getStyle(),pump_.getAchievableEfficiency(), pump_.getRpm(),pump_.getKviscosity(),pump_.getStageCount(),fieldData_.getFlowRate(),fieldData_.getHead());
    optimal_.pumpEfficiency_ = optimalPumpEfficiency.calculate();
    OptimalPumpShaftPower optimalPumpShaftPower(fieldData_.getFlowRate(),fieldData_.getHead(),pump_.getSg(),optimal_.pumpEfficiency_);
    optimal_.pumpShaftPower_ = optimalPumpShaftPower.calculate();
    OptimalMotorShaftPower optimalMotorShaftPower(optimal_.pumpShaftPower_,pump_.getDrive());
    optimal_.motorShaftPower_ = optimalMotorShaftPower.calculate();
    OptimalMotorSize optimalMotorSize(optimal_.motorShaftPower_,motor_.getSizeMargin());
    optimal_.motorRatedPower_ = optimalMotorSize.calculate();
    OptimalMotorPower optimalMotorPower(optimal_.motorRatedPower_, fieldData_.getMotorPower(), motor_.getMotorRpm(), motor_.getLineFrequency(),
                                        motor_.getEfficiencyClass(), motor_.getSpecifiedEfficiency(), motor_.getMotorRatedVoltage(), motor_.getFullLoadAmps(),
                                        fieldData_.getVoltage(), fieldData_.getLoadEstimationMethod(),
                                        fieldData_.getMotorAmps(), optimal_.motorShaftPower_);
    optimalMotorPower.calculate();
    optimal_.motorCurrent_ = optimalMotorPower.getMotorCurrent();
    optimal_.motorEfficiency_ = optimalMotorPower.getMotorEff();
    optimal_.motorPower_ = optimalMotorPower.getMotorPower();
    optimal_.motorPowerFactor_ = optimalMotorPower.getMotorPf();
    // Calculate Annual Energy
    AnnualEnergy annualEnergy(optimal_.motorPower_, financial_.getOperatingFraction());
    optimal_.annualEnergy_ = annualEnergy.calculate();

    // Calculate Annual Cost
    AnnualCost annualCost(optimal_.annualEnergy_, financial_.getUnitCost());
    optimal_.annualCost_ = annualCost.calculate();

    // Annual Savings potential
    annualSavingsPotential_ = existing_.annualCost_ - optimal_.annualCost_;
    // Optimization Rating
    optimizationRating_ = optimal_.motorPower_/existing_.motorPower_;
    return 0;
}
