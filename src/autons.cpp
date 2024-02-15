#include "main.h"

/////
// For installation, upgrading, documentations and tutorials, check out our website!
// https://ez-robotics.github.io/EZ-Template/
/////

// These are out of 127
const int DRIVE_SPEED = 110;  
const int TURN_SPEED = 90;
const int SWING_SPEED = 90;

///
// Constants
///
void default_constants() {
  chassis.pid_heading_constants_set(3, 0, 20);
  chassis.pid_drive_constants_set(22, 0, 50);
  chassis.pid_turn_constants_set(3, 0, 0);
  chassis.pid_swing_constants_set(5, 0, 30);

  chassis.pid_turn_exit_condition_set(300_ms, 3_deg, 500_ms, 7_deg, 750_ms, 750_ms);
  chassis.pid_swing_exit_condition_set(300_ms, 3_deg, 500_ms, 7_deg, 750_ms, 750_ms);
  chassis.pid_drive_exit_condition_set(300_ms, 1_in, 500_ms, 3_in, 750_ms, 750_ms);

  chassis.slew_drive_constants_set(7_in, 80);
}

void auton() {
  chassis.pid_drive_set(20, DRIVE_SPEED, true);
  chassis.pid_wait();
}

const int defDriveSpeed = 110;
const int defTurnSpeed = 110;

  pros::ADIDigitalOut blocker('A');
  pros::ADIDigitalOut rWing('B');
  pros::ADIDigitalOut lWing('C');

  pros::Motor intake(15, pros::E_MOTOR_GEARSET_06);
      pros::Motor cata(10, pros::E_MOTOR_GEARSET_36);
    int intakeTime = 320;

void defAuton()
{
  // outtake preload
  intake = -70;

  pros::delay(300);
  intake = -127;
  pros::delay(intakeTime);
  intake = 0;

  // turn onto first triball]
  chassis.pid_turn_set(-70, defTurnSpeed);
  chassis.pid_wait();

  // drive to and intake it, while staying on line
  intake = 127;

  chassis.pid_drive_set(-114, defDriveSpeed, true);
  chassis.pid_wait();

  intake = 30;

  // turn to other ball to hit over with wing
  chassis.pid_turn_set(-179, defTurnSpeed);
  chassis.pid_wait();

  // drop left wing
  lWing.set_value(true);

  // drive at and hit over triball
  chassis.pid_drive_set(45, defDriveSpeed, true);
  chassis.pid_wait();

  // retrack wing
  lWing.set_value(false);

  // back up for turn
  chassis.pid_drive_set(-20, defDriveSpeed, true);
  chassis.pid_wait();

  // turn to point where triball will be dropped off
  chassis.pid_turn_set(-240, defTurnSpeed);
  chassis.pid_wait();

  // drive to outtake point there
  chassis.pid_drive_set(-55, defDriveSpeed, true);
  chassis.pid_wait();

  // turn to outtake
  chassis.pid_turn_set(-313, defTurnSpeed);
  chassis.pid_wait();

  // outtake middle ball
  intake = -127;
  pros::delay(intakeTime);
  intake = 0;

  // turn back to line up with back wall
  chassis.pid_turn_set(-10, 127);
  chassis.pid_wait();

  // drive there
  chassis.pid_drive_set(14, defDriveSpeed, true);
  chassis.pid_wait();

  // turn to get lined up with matchload zone
  chassis.pid_turn_set(-90, 127);
  chassis.pid_wait();

  // swing to grab corner ball
  chassis.pid_swing_set(RIGHT_SWING, -135, defTurnSpeed);
  chassis.pid_wait();

  // drop right wing to clear
  rWing.set_value(true);

  // clear
  chassis.pid_drive_set(14, defDriveSpeed, true);
  chassis.pid_wait();

  // close wing
  rWing.set_value(false);

  // swing to line up and clear
  chassis.pid_swing_set(RIGHT_SWING, -180, defTurnSpeed);
  chassis.pid_wait();

  // drive forward slightly
  chassis.pid_drive_set(27, defDriveSpeed, true);
  chassis.pid_wait();

  // left wing drops
  lWing.set_value(true);

  // clear side
  chassis.pid_drive_set(50, defDriveSpeed, true);
  chassis.pid_wait();
}

// runs 15 offensive autonomous routine for the game.
void offAuton()
{
// intake and move to first triball
  intake.move(127);
  chassis.pid_drive_set(-163, 127, true);
  chassis.pid_wait();
  intake.move(30);

  // swing to outake
  chassis.pid_turn_set(-53, 127);
  chassis.pid_wait();

  lWing.set_value(true);

  // slam
  chassis.pid_drive_set(90, 127, true);
  chassis.pid_wait();

  // back
  chassis.pid_drive_set(-35, 127, true);
  chassis.pid_wait();

  lWing.set_value(false);

  chassis.pid_turn_set(-233, 127);
  chassis.pid_wait();

  intake.move(0);

  chassis.pid_drive_set(-54, 127, true);
  chassis.pid_wait();

  // back up slightly
  chassis.pid_drive_set(30, 127, true);
  chassis.pid_wait();

  // turn to snatch
  chassis.pid_turn_set(-79, 127);
  chassis.pid_wait();

  // grab 3rd ball
  intake.move(127);
  chassis.pid_drive_set(-68, 127, true);
  chassis.pid_wait();
  intake.move(30);

  // swing to outtake into corner
  chassis.pid_turn_set(-198, 127);
  chassis.pid_wait();

  // drive to drop off point
  chassis.pid_drive_set(-64, 127, true);
  chassis.pid_wait();

  // turn to outtake to corner
  chassis.pid_turn_set(-250, 127);
  chassis.pid_wait();

  // outtake
  intake.move(-127);
  pros::delay(250);
  intake.move(0);

  // turn to grab preload and corner ball
  chassis.pid_turn_set(-330, 127);
  chassis.pid_wait();

  // drive up
  chassis.pid_drive_set(40, 127, true);
  chassis.pid_wait();

  /*
    // turn
    chassis.pid_turn_set(-420, 127);
    chassis.pid_wait();
  */
  // swing to grab corner ball
  chassis.pid_swing_set(RIGHT_SWING, -466, 127);
  chassis.pid_wait();

  // right wing extend
  rWing.set_value(true);
  // extend left wing to grab pre load
  lWing.set_value(true);
  // drive and clear
  chassis.pid_drive_set(25, 127, true);
  chassis.pid_wait();

  // close corner wing
  rWing.set_value(false);

  chassis.pid_drive_set(32, 127, true);
  chassis.pid_wait();

  // swing to slam in the three triballs
  chassis.pid_swing_set(RIGHT_SWING, -510, 127);
  chassis.pid_wait();

  // slam in triballs
  chassis.pid_drive_set(32, 127, true);
  chassis.pid_wait();

  // backup
  chassis.pid_drive_set(-15, 127, true);
  chassis.pid_wait();

  // double slam
  chassis.pid_drive_set(20, 127, true);
  chassis.pid_wait();

  // close left wing
  lWing.set_value(false);

  while (1)
  {
    pros::delay(100);
  }

  chassis.pid_drive_set(-10, 127, true);
  chassis.pid_wait();

  chassis.pid_swing_set(RIGHT_SWING, -239, 127);
  chassis.pid_wait();

  chassis.pid_drive_set(50, 127, true);
  chassis.pid_wait();

  lWing.set_value(true);

  chassis.pid_drive_set(40, 127, true);
  chassis.pid_wait();
}
/*
  Psuedo-Code (Skills)
  - Allign With Location For Match Loads
  - Match Load Certain Ammount Using a Delay (Or Millis if Necessary) With Cata Going
  - Allign and Drive Forward on an Arc To Allign Perp With Middle Bar
  - Extend Pnuematic Arm and Grab Corner Ball
  - Close Arm and Ram in On Arc
  - Back Out and Arc To Far Side
  - Both Arms Extend And Push On A Slight Angle
  - Back Up and Arc to Final Position
  - Push Other Side In To Finish
*/

void AWPoffAuton()
{
  intake.move(-120);
  pros::delay(400);
  intake.move(127);

  chassis.pid_drive_set(-62, 127, true);
  chassis.pid_wait();

  pros::delay(200);

  intake.move(30);

  chassis.pid_drive_set(98, 127, true);
  chassis.pid_wait();

  // swing to grab corner ball
  chassis.pid_swing_set(RIGHT_SWING, -45, 127);
  chassis.pid_wait();

  // right wing extend
  rWing.set_value(true);
  // extend left wing to grab pre load
  lWing.set_value(true);
  // drive and clear
  chassis.pid_drive_set(27, 127, true);
  chassis.pid_wait();

  // close corner wing
  rWing.set_value(false);

  chassis.pid_drive_set(19, 127, true);
  chassis.pid_wait();

  // swing to slam in the three triballs
  chassis.pid_swing_set(RIGHT_SWING, -90, 127);
  chassis.pid_wait();

  // slam in triballs
  chassis.pid_drive_set(42, 127, true);
  chassis.pid_wait();

  // backup
  chassis.pid_drive_set(-25, 127, true);
  chassis.pid_wait();

  lWing.set_value(false);

  chassis.pid_turn_set(90, 127);
  chassis.pid_wait();

  pros::delay(150);
  intake.move(-127);
  pros::delay(200);
  intake.move(0);

    chassis.pid_turn_set(-90, 127);
  chassis.pid_wait();

  // slam
  chassis.pid_drive_set(33, 127, true);
  chassis.pid_wait();

  chassis.pid_drive_set(-20, 127, true);
  chassis.pid_wait();

  //AWP
  /*
  chassis.pid_turn_set(184, 127);
  chassis.pid_wait();

  chassis.pid_drive_set(90, 127);
  chassis.pid_wait();

  lWing.set_value(true);

  chassis.pid_drive_set(64, 127, true);
  chassis.pid_wait();
  */

   chassis.pid_turn_set(18, 127);
  chassis.pid_wait();

    intake.move(127);

    chassis.pid_drive_set(-143, 127, true);
  chassis.pid_wait();

  intake.move(30);

     chassis.pid_turn_set(132, 127);
  chassis.pid_wait();

      chassis.pid_drive_set(-70, 127, true);
  chassis.pid_wait();

       chassis.pid_turn_set(180, 127);
  chassis.pid_wait();

    intake.move(-40);

          chassis.pid_drive_set(-35, 127, true);
  chassis.pid_wait();

    intake.move(0);

          chassis.pid_drive_set(20, 127, true);
  chassis.pid_wait();

    //possible 6 ball
  /*

  intake.move(-127);
  pros::delay(200);
  intake.move(127);

       chassis.pid_turn_set(86, 127);
  chassis.pid_wait();

      chassis.pid_drive_set(-60, 127);
  chassis.pid_wait();

    intake.move(30);

       chassis.pid_turn_set(0, 127);
  chassis.pid_wait();

  lWing.set_value(true);
  rWing.set_value(true);

        chassis.pid_drive_set(110, 127);
  chassis.pid_wait();

        chassis.pid_drive_set(-20, 127);
  chassis.pid_wait();

  lWing.set_value(false);
  rWing.set_value(false);

         chassis.pid_turn_set(180, 127);
  chassis.pid_wait();

  intake.move(0);

        chassis.pid_drive_set(-25, 127);
  chassis.pid_wait();
  */


}

// skills autonomous routine //no while loop is needed as it is preset to 1 minute
void skillsAuton()
{
  /*
  // static pros::Motor cataleft(12, pros::E_MOTOR_GEARSET_36);
  // static pros::Motor cataright(-19, pros::E_MOTOR_GEARSET_36); // comment out with prog
  //  static pros::Motor cataright(9000, pros::E_MOTOR_GEARSET_36); // comment out with real bort

  static pros::Motor cata(3, pros::E_MOTOR_GEARSET_36);

  // static pros::Motor_Group cata({cataleft, cataright});
  // pros::Rotation rot(2);

  pros::ADIDigitalOut blocker('A');
  pros::ADIDigitalOut rWing('B');
  pros::ADIDigitalOut lWing('C');

  // rot.set_position(0);

  int turnChange = 23;

  cata.move(127);
  pros::delay(41300);

  // pros::delay(2600);
  cata.move(0);

  chassis.pid_turn_set(-110, 127);
  chassis.pid_wait();

  chassis.pid_drive_set(65, 127);
  chassis.pid_wait_until(65);

  chassis.pid_turn_set(-80, 127);
  chassis.pid_wait();

  chassis.pid_drive_set(130, 127);
  chassis.pid_wait_until(45);

  chassis.pid_drive_set(-130, 127);
  chassis.pid_wait_until(-25);

  chassis.pid_turn_set(60, 127);
  chassis.pid_wait();

  chassis.pid_drive_set(180, 127);
  chassis.pid_wait_until(140);

  chassis.pid_turn_set(23, 127);
  chassis.pid_wait();

  chassis.pid_drive_set(330, 127);
  chassis.pid_wait_until(290);

  // Wings.set_value(false);

  chassis.pid_turn_set(turnChange + -50, 90);
  // chassis.pid_drive_set(120, 127);
  chassis.pid_wait();

  // Wings.set_value(true);

  chassis.pid_drive_set(220, 127);
  chassis.pid_wait_until(150);

  // Wings.set_value(false);

  chassis.pid_drive_set(-220, 127);
  chassis.pid_wait_until(-20);

  chassis.pid_turn_set(turnChange + -160, 90);
  // chassis.pid_drive_set(120, 127);
  chassis.pid_wait();

  chassis.pid_drive_set(220, 127);
  chassis.pid_wait_until(120);

  chassis.pid_turn_set(turnChange + -40, 127);
  chassis.pid_wait();

  lWing.set_value(true);
  rWing.set_value(true);

  chassis.pid_drive_set(200, 127);
  chassis.pid_wait_until(123);

  lWing.set_value(false);
  rWing.set_value(false);

  chassis.pid_turn_set(turnChange + 0, 127);
  chassis.pid_wait();

  chassis.pid_drive_set(-150, 127);
  chassis.pid_wait_until(-100);

  chassis.pid_turn_set(turnChange + 90, 127);
  chassis.pid_wait();

  chassis.pid_drive_set(-150, 127);
  chassis.pid_wait_until(-100);

  chassis.pid_turn_set(turnChange + 20, 127);
  chassis.pid_wait();

  lWing.set_value(true);
  rWing.set_value(true);

  chassis.pid_drive_set(160, 127);
  chassis.pid_wait_until(120);

  lWing.set_value(false);
  rWing.set_value(false);

  chassis.pid_turn_set(turnChange + -20, 127);
  chassis.pid_wait();

  chassis.pid_drive_set(-160, 127);
  chassis.pid_wait_until(-100);

  chassis.pid_turn_set(turnChange + 0, 127);
  chassis.pid_wait();

  lWing.set_value(true);
  rWing.set_value(true);

  chassis.pid_drive_set(160, 127);
  chassis.pid_wait_until(120);

  while (1)
  {
    pros::delay(100);
  }
  */

  intake.move(-100);

  chassis.pid_drive_set(40, 127, true);
  chassis.pid_wait();

  intake.move(0);

  chassis.pid_swing_set(LEFT_SWING, 45, 127);
  chassis.pid_wait();

  rWing.set_value(true);

  chassis.pid_drive_set(45, 127, true);
  chassis.pid_wait();

  rWing.set_value(false);

  chassis.pid_drive_set(-40, 127, true);
  chassis.pid_wait();

    chassis.pid_swing_set(LEFT_SWING, 0, 127);
  chassis.pid_wait();

    chassis.pid_drive_set(-13, 127, true);
  chassis.pid_wait();

  chassis.pid_turn_set(-64, 127);
  chassis.pid_wait();

  lWing.set_value(true);

  cata.move(115);

  pros::delay(27000);

  cata.move(0);

  lWing.set_value(false);

  chassis.pid_turn_set(-180, 127);
  chassis.pid_wait();

  chassis.pid_drive_set(24, 127, true);
  chassis.pid_wait();

  chassis.pid_swing_set(RIGHT_SWING, -225, 127);
  chassis.pid_wait();

  chassis.pid_drive_set(243, 127, true);
  chassis.pid_wait();

  lWing.set_value(true);

  chassis.pid_swing_set(RIGHT_SWING, -270, 127);
  chassis.pid_wait();

  chassis.pid_drive_set(53, 127, true);
  chassis.pid_wait();

  chassis.pid_swing_set(RIGHT_SWING, -315, 127);
  chassis.pid_wait();

  chassis.pid_drive_set(34, 127, true);
  chassis.pid_wait();

  chassis.pid_drive_set(-24, 127, true);
  chassis.pid_wait();

  chassis.pid_drive_set(28, 127, true);
  chassis.pid_wait();

  chassis.pid_drive_set(-21, 127, true);
  chassis.pid_wait();

  lWing.set_value(false);

  chassis.pid_turn_set(-393, 127);
  chassis.pid_wait();

  chassis.pid_drive_set(145, 127, true);
  chassis.pid_wait();

  chassis.pid_turn_set(-250, 127);
  chassis.pid_wait();

  lWing.set_value(true);
  rWing.set_value(true);

  chassis.pid_drive_set(104, 127, true);
  chassis.pid_wait();

  chassis.pid_turn_set(-225, 127);
  chassis.pid_wait();

  chassis.pid_drive_set(-25, 127, true);
  chassis.pid_wait();

  chassis.pid_drive_set(29, 127, true);
  chassis.pid_wait();

  chassis.pid_drive_set(-10, 127, true);
  chassis.pid_wait();

  lWing.set_value(false);
  rWing.set_value(false);

  chassis.pid_turn_set(-214, 127);
  chassis.pid_wait();

  chassis.pid_drive_set(-96, 127, true);
  chassis.pid_wait();

  chassis.pid_turn_set(-135, 127);
  chassis.pid_wait();

  chassis.pid_drive_set(-110, 127, true);
  chassis.pid_wait();

  chassis.pid_swing_set(RIGHT_SWING, -213, 127);
  chassis.pid_wait();

  lWing.set_value(true);
  rWing.set_value(true);

  chassis.pid_drive_set(103, 127, true);
  chassis.pid_wait();

  chassis.pid_turn_set(-225, 127);
  chassis.pid_wait();

  chassis.pid_drive_set(-26, 127, true);
  chassis.pid_wait();

  chassis.pid_drive_set(30, 127, true);
  chassis.pid_wait();

  chassis.pid_drive_set(-10, 127, true);
  chassis.pid_wait();

  lWing.set_value(false);
  rWing.set_value(false);

  while (1)
  {
    pros::delay(100);
  }

  chassis.pid_turn_set(147, 127);
  chassis.pid_wait();

  chassis.pid_drive_set(-150, 127, true);
  chassis.pid_wait();

  chassis.pid_turn_set(225, 127);
  chassis.pid_wait();

  chassis.pid_drive_set(148, 127, true);
  chassis.pid_wait();

  chassis.pid_turn_set(135, 127);
  chassis.pid_wait();

  lWing.set_value(true);
  rWing.set_value(true);

  chassis.pid_drive_set(80, 127, true);
  chassis.pid_wait();

  chassis.pid_drive_set(-20, 127, true);
  chassis.pid_wait();

  chassis.pid_drive_set(25, 127, true);
  chassis.pid_wait();

  chassis.pid_drive_set(-15, 127, true);
  chassis.pid_wait();

  lWing.set_value(false);
  rWing.set_value(false);
}