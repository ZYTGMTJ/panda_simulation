/*
 * File:   MRAC_controller.cpp
 * Author: Corrado Pezzato, TU Delft, DCSC
 *
 * Created on April 14th, 2019
 *
 * This node allows to control the 7DOF Franka Emika Panda robot arm through
 * the new promisin theory called Active Inference proposed by Karl Friston.
 *
 * The robot moves to the desired position specified in desiredPos performing
 * free-energy minimization and actiove inference using gradient descent.
 * The robot is equipped with proprioceptive sensors for joints position and
 * velocity and a camera for the end-effector pose estimation.
 * The control is in joint space.
 *
 */

#include "MRAC.h"

int main(int argc, char **argv)
{
  ros::init(argc, argv, "MRAC_controller_node");
  // Variables to regulate the flow (Force to read once every 1ms the sensors)
  int count = 0;
  // Variable for desired position, set here the goal for the Panda for each joint
  std::vector<double> desiredPos(7);

  desiredPos[0] = 0.0;
  desiredPos[1] = 0.5;
  desiredPos[2] = 0.0;
  desiredPos[3] = -1.1;
  desiredPos[4] = 0.0;
  desiredPos[5] = 2;
  desiredPos[6] = 0.0;

  // Object of the class MRAC which will take care of everything
  MRAC MRAC_controller;
  // Set desired position in the MRAC class
  MRAC_controller.setGoal(desiredPos);

  // Main loop
  ros::Rate rate(1000);
  while (ros::ok()){
    // Manage all the callbacks and so read sensors
    ros::spinOnce();

    // Skip only first cycle to allow reading the sensory input first
    if ((count!=0)&&(MRAC_controller.dataReady()==1)){
      // Control using MRAC
      MRAC_controller.computeControlInput();
    }
    else
      count ++;

    rate.sleep();
  }
  return 0;
}