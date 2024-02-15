#include "main.h"

/////
// For installation, upgrading, documentations and tutorials, check out our website!
// https://ez-robotics.github.io/EZ-Template/
/////

// Chassis constructor
ez::Drive chassis(
    // Left Chassis Ports (negative port will reverse it!)
    //   the first port is used as the sensor
    {-13, 14, -17}

    // Right Chassis Ports (negative port will reverse it!)
    //   the first port is used as the sensor
    ,
    {11, -12, 16}

    // IMU Port
    ,
    18

    // Wheel Diameter (Remember, 4" wheels without screw holes are actually 4.125!)
    ,
    4

    // Cartridge RPM
    ,
    600

    // External Gear Ratio (MUST BE DECIMAL) This is WHEEL GEAR / MOTOR GEAR
    // eg. if your drive is 84:36 where the 36t is powered, your RATIO would be 84/36 which is 2.333
    // eg. if your drive is 60:36 where the 36t is powered, your RATIO would be 60/36 which is 0.6
    // eg. if your drive is 36:60 where the 60t is powered, your RATIO would be 36/60 which is 0.6
    ,
    0.57143);

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize()
{
  // Print our branding over your terminal :D
  ez::ez_template_print();

  pros::delay(500); // Stop the user from doing anything while legacy ports configure

  // Configure your chassis controls
  chassis.opcontrol_curve_buttons_toggle(true); // Enables modifying the controller curve with buttons on the joysticks
  chassis.opcontrol_drive_activebrake_set(0);   // Sets the active brake kP. We recommend 0.1.
  chassis.opcontrol_curve_default_set(0, 0);    // Defaults for curve. If using tank, only the first parameter is used. (Comment this line out if you have an SD card!)
  default_constants();                          // Set the drive to your own constants from autons.cpp!

  // These are already defaulted to these buttons, but you can change the left/right curve buttons here!
  // chassis.opcontrol_curve_buttons_left_set (pros::E_CONTROLLER_DIGITAL_LEFT, pros::E_CONTROLLER_DIGITAL_RIGHT); // If using tank, only the left side is used.
  // chassis.opcontrol_curve_buttons_right_set(pros::E_CONTROLLER_DIGITAL_Y,    pros::E_CONTROLLER_DIGITAL_A);

  // Initialize chassis and auton selector
  chassis.initialize();
  master.rumble(".");
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled()
{
  // . . .
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize()
{
  // . . .
}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous()
{
  chassis.pid_targets_reset();               // Resets PID targets to 0
  chassis.drive_imu_reset();                 // Reset gyro position to 0
  chassis.drive_sensor_reset();              // Reset drive sensors to 0
  chassis.drive_brake_set(MOTOR_BRAKE_HOLD); // Set motors to hold.  This helps autonomous consistency

  //auton();
  //defAuton();
  //offAuton();
  //AWPoffAuton();
  skillsAuton();
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol()
{
    // old cata variables
    /*
    bool cataOff = true;
    long launchTrack = pros::millis();
    bool launchFlip = true;
    int_least8_t oldRot;
    rot.set_position(0);
    */
    pros::Motor intake(15, pros::E_MOTOR_GEARSET_06);
    pros::Motor cata(10, pros::E_MOTOR_GEARSET_36);
    // pros::Rotation rot(22);
    
    pros::ADIDigitalOut blocker('A');
    pros::ADIDigitalOut rWing('B');
    pros::ADIDigitalOut lWing('C');

    // pneumatic switch booleans
    bool blockerSwitch = false;
    bool lWingSwitch = false;
    bool rWingSwitch = false;

    // cata variables
    int cataSpeed = 127;
    const int cataAdjust = 6;

    // drive variables
    const float dBand = 5;

    // stick variables to calulate speeds with curve
    double power;
    double powerC;
    double turn;
    double turnC;

    // curve out of 10
    // modelled after https://www.desmos.com/calculator/toufp2r8qb
    int pCurve = 0;   // curve for fwd/back
    int tCurve = 0.3; // curve for turn

    double e = exp(1); // Euler's constant

    chassis.drive_brake_set(MOTOR_BRAKE_COAST);

  while (true)
  {

    // PID Tuner
    // After you find values that you're happy with, you'll have to set them in auton.cpp
    if (!pros::competition::is_connected())
    {
      // Enable / Disable PID Tuner
      //  When enabled:
      //  * use A and Y to increment / decrement the constants
      //  * use the arrow keys to navigate the constants
      if (master.get_digital_new_press(DIGITAL_X))
        chassis.pid_tuner_toggle();

      // Trigger the selected autonomous routine
      if (master.get_digital_new_press(DIGITAL_B))
        autonomous();

      chassis.pid_tuner_iterate(); // Allow PID Tuner to iterate
    }

      // driver code
      //  variable for # of motors per side
      const int sideMotors = 3;
      // get stick values
      if (master.get_analog(ANALOG_LEFT_Y) > dBand || master.get_analog(ANALOG_LEFT_Y) < -dBand)
      {
        // calculates power curve for joystick
        power = master.get_analog(ANALOG_LEFT_Y);
        powerC = power * (pow(e, -(pCurve / 10)) + pow(e, (abs(power) - 127) / 10) * (1 - pow(e, -(pCurve / 10))));
      }
      else
      {
        powerC = 0;
      }

      // gets turn value and calculates curve
      if (master.get_analog(ANALOG_RIGHT_X) > dBand || master.get_analog(ANALOG_RIGHT_X) < -dBand)
      {
        turn = -master.get_analog(ANALOG_RIGHT_X);
        turnC = turn * (pow(e, -(tCurve / 10)) + pow(e, (abs(turn) - 127) / 10) * (1 - pow(e, -(tCurve / 10))));
      }
      else
      {
        turnC = 0;
      }

      for (int i = 0; i < sideMotors; i++)
      {
        chassis.left_motors[i].move(powerC - turnC);
        chassis.right_motors[i].move(powerC + turnC);
      }

      // intake code
      if (master.get_digital(DIGITAL_R2)) // outtake
      {
        intake.move(-127);
      }
      else if (master.get_digital(DIGITAL_L2)) // intake
      {
        intake.move(127);
      }
      else
      {
        intake.move(0);
      }

      // cata speed adjustment code
      if (master.get_digital_new_press(DIGITAL_UP))
      {
        cataSpeed += cataAdjust;
      }
      if (master.get_digital_new_press(DIGITAL_DOWN))
      {
        cataSpeed -= cataAdjust;
      }

      // caps catapult speed at a certain range
      if (cataSpeed > 127)
      {
        cataSpeed = 127;
      }
      if (cataSpeed < 70)
      {
        cataSpeed = 70;
      }

      // new cata code
      if (master.get_digital(DIGITAL_A))
      {
        cata.move(cataSpeed);
      }
      else
      {
        cata.move(0);
      }

      // toggleable blocker
      if (master.get_digital_new_press(DIGITAL_L1))
      {
        if (blockerSwitch == false)
        {
          blockerSwitch = true;
          blocker.set_value(true);
        }
        else
        {
          blockerSwitch = false;
          blocker.set_value(false);
        }
      }

      // wings
      if (master.get_digital_new_press(DIGITAL_R1))
      {
        if (lWingSwitch == true && rWingSwitch == true) // if both wings are down, both come up
        {
          lWingSwitch = false;
          rWingSwitch = false;

          lWing.set_value(false);
          rWing.set_value(false);
        }
        else if (lWingSwitch == true || rWingSwitch == true) // if one wing is down, both come down
        {
          lWingSwitch = true;
          rWingSwitch = true;

          lWing.set_value(true);
          rWing.set_value(true);
        }
        else // if both wings are up, both come down
        {
          lWingSwitch = true;
          rWingSwitch = true;

          lWing.set_value(true);
          rWing.set_value(true);
        }
      }

      // left wing individual controls
      if (master.get_digital_new_press(DIGITAL_LEFT))
      {
        if (lWingSwitch == false)
        {
          lWingSwitch = true;
          lWing.set_value(true);
        }
        else
        {
          lWingSwitch = false;
          lWing.set_value(false);
        }
      }

      // right wing individual controls
      if (master.get_digital_new_press(DIGITAL_RIGHT))
      {
        if (rWingSwitch == false)
        {
          rWingSwitch = true;
          rWing.set_value(true);
        }
        else
        {
          rWingSwitch = false;
          rWing.set_value(false);
        }
      }

      // master.rumble(".");

      // ez::print_to_screen("Rotation Angle: " + std::to_string(rotDeg), 3);
      // master.set_text(1, 1, "Rot: " + std::to_string(rotDeg));
      // ez::print_to_screen("CataSpeed = " + std::to_string(cataSpeed), 3);
      // ez::print_to_screen("Linear Speed: " + std::to_string(power), 4);
      // ez::print_to_screen("Drive Motor Temp: " + std::to_string(static_cast<int>(chassis.left_motors[0].get_temperature())), 2);
      //  master.set_text(1, 1, std::to_string(static_cast<int>(chassis.left_motors[0].get_temperature())) + "power = " + std::to_string(power));

      /*
      // archive old cata
      //toggle switch
      if (master.get_digital_new_press(DIGITAL_B))
      {
        if (cataOff == true)
        {
          cataOff = false;
          cata.move(0);
        }
        else
        {
          cataOff = true;
          cata.move(0);
        }
      }

      int_least8_t rotDeg = rot.get_position() / 100; // sets rotations sensor to an integer
      if (cataOff == false)
      {
        if (rotDeg < 41 || master.get_digital(DIGITAL_A)) // moves down if button is pressed or rotation is in correct position
        {
          cata.move(127);
          cata.set_brake_modes(MOTOR_BRAKE_COAST);
        }
        else // stops cata once distance has been reached
        {
          cata.move(0);
          cata.set_brake_modes(MOTOR_BRAKE_HOLD);
        }
      }

      if (rotDeg < 0) // resets rotation to correct position upon firing, ensuring no negatives for more acurrate reloads
      {
        rot.set_position(0);
      }


      //keeps track of the old rotation sensor's position
      if (launchTrack + 500 < pros::millis()) //running if statement every half second
      {
        launchTrack = pros::millis();
        oldRot = rot.get_position() / 100;
      }

      if (oldRot > rotDeg + 40) //checks if cata fires
      {
        rot.set_position(0);
      }
      */

      /*
      //X-Drive Code
      pros::Motor M1(1); //Top left motor
      pros::Motor M2(1); //Bottom left motor
      pros::Motor M3(1); //Top right motor
      pros::Motor M4(1); //bottom right motor

      M1 = master.get_analog(ANALOG_LEFT_Y) + master.get_analog(ANALOG_LEFT_X) - master.get_analog(ANALOG_RIGHT_X);
      M2 = master.get_analog(ANALOG_LEFT_Y) - master.get_analog(ANALOG_LEFT_X) + master.get_analog(ANALOG_RIGHT_X);
      M3 = master.get_analog(ANALOG_LEFT_Y) + master.get_analog(ANALOG_LEFT_X) + master.get_analog(ANALOG_RIGHT_X);
      M4 = master.get_analog(ANALOG_LEFT_Y) - master.get_analog(ANALOG_LEFT_X) - master.get_analog(ANALOG_RIGHT_X);
      */

      pros::delay(ez::util::DELAY_TIME); // This is used for timer calculations!  Keep this ez::util::DELAY_TIME
    }
}
