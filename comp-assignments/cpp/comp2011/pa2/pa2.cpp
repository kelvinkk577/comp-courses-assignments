#include <iostream>
#include <cstring>
#include "cleaning_robot.h"
using namespace std;

/*
   *    COMP 2011 2021 Fall Programming Assignment 2
   *    Student Name        : Kwok Chun Leung
   *    Student ID          : 20610124
   *    Student ITSC email  : clkwokad@connect.ust.hk
   * 
   * You are not allowed to use extra library
*/

// Please do all your work in this file. You just need to submit this file.

int findMaximumPlace(int robot_x, int robot_y, int robot_energy, int robot_full_energy, char result_map[MAP_HEIGHT][MAP_WIDTH], char temp_map[MAP_HEIGHT][MAP_WIDTH])
{
   if (robot_x > MAP_WIDTH - 1 || robot_y > MAP_HEIGHT - 1 || robot_x < 0 || robot_y < 0)
   {
      return 0;
   }

   if (robot_energy < 0)
   {
      return 0;
   }

   if (result_map[robot_y][robot_x] == ' ' || result_map[robot_y][robot_x] == 'R')
   {
      result_map[robot_y][robot_x] = 'V';
      return 1 + findMaximumPlace(robot_x, robot_y - 1, robot_energy - 1, robot_full_energy, result_map, temp_map) + findMaximumPlace(robot_x + 1, robot_y, robot_energy - 1, robot_full_energy, result_map, temp_map) + findMaximumPlace(robot_x, robot_y + 1, robot_energy - 1, robot_full_energy, result_map, temp_map) + findMaximumPlace(robot_x - 1, robot_y, robot_energy - 1, robot_full_energy, result_map, temp_map);
   }

   if (result_map[robot_y][robot_x] == 'C')
   {
      result_map[robot_y][robot_x] = 'V';
      return 1 + findMaximumPlace(robot_x, robot_y - 1, robot_full_energy - 1, robot_full_energy, result_map, temp_map) + findMaximumPlace(robot_x + 1, robot_y, robot_full_energy - 1, robot_full_energy, result_map, temp_map) + findMaximumPlace(robot_x, robot_y + 1, robot_full_energy - 1, robot_full_energy, result_map, temp_map) + findMaximumPlace(robot_x - 1, robot_y, robot_full_energy - 1, robot_full_energy, result_map, temp_map);
   }

   if (result_map[robot_y][robot_x] == 'B')
   {
      return 0;
   }

   return findMaximumPlace(robot_x, robot_y - 1, robot_energy - 1, robot_full_energy, result_map, temp_map) + findMaximumPlace(robot_x + 1, robot_y, robot_energy - 1, robot_full_energy, result_map, temp_map) + findMaximumPlace(robot_x, robot_y + 1, robot_energy - 1, robot_full_energy, result_map, temp_map) + findMaximumPlace(robot_x - 1, robot_y, robot_energy - 1, robot_full_energy, result_map, temp_map);
}

int findShortestDistance(int robot_x, int robot_y, int target_x, int target_y, int robot_energy, int robot_full_energy, const char map[MAP_HEIGHT][MAP_WIDTH], char temp_map[MAP_HEIGHT][MAP_WIDTH])
{
   if (robot_x < 0 || robot_x > MAP_WIDTH - 1 || robot_y < 0 || robot_y > MAP_HEIGHT - 1)
      return PA2_MAX_PATH;
   if (map[robot_y][robot_x] == 'B')
      return PA2_MAX_PATH;
   if (temp_map[robot_y][robot_x] == 'V')
      return PA2_MAX_PATH;
   if (robot_energy < 0)
      return PA2_MAX_PATH;

   //' ', 'R', 'C'

   if (map[robot_y][robot_x] == 'C')
   {
      temp_map[robot_y][robot_x] = 'V';
      robot_energy = robot_full_energy;
   }

   if (robot_x == target_x && robot_y == target_y)
   {
      temp_map[robot_y][robot_x] = ' ';
      return 1;
   }

   int up = findShortestDistance(robot_x, robot_y - 1, target_x, target_y, robot_energy - 1, robot_full_energy, map, temp_map);
   int right = findShortestDistance(robot_x + 1, robot_y, target_x, target_y, robot_energy - 1, robot_full_energy, map, temp_map);
   int down = findShortestDistance(robot_x, robot_y + 1, target_x, target_y, robot_energy - 1, robot_full_energy, map, temp_map);
   int left = findShortestDistance(robot_x - 1, robot_y, target_x, target_y, robot_energy - 1, robot_full_energy, map, temp_map);

   // All four directions does not lead to target
   if (up >= PA2_MAX_PATH && right >= PA2_MAX_PATH && down >= PA2_MAX_PATH && left >= PA2_MAX_PATH)
   {
      temp_map[robot_y][robot_x] = ' ';
      return PA2_MAX_PATH;
   }

   int shortest = up;
   if (right < shortest)
      shortest = right;
   if (down < shortest)
      shortest = down;
   if (left < shortest)
      shortest = left;

   temp_map[robot_y][robot_x] = ' ';
   return 1 + shortest;
}

int findPathSequence(int robot_x, int robot_y, int target_x, int target_y, int robot_energy, int robot_full_energy, char result_sequence[], const char map[MAP_HEIGHT][MAP_WIDTH], char temp_map[MAP_HEIGHT][MAP_WIDTH])
{
   if (robot_x < 0 || robot_x > MAP_WIDTH - 1 || robot_y < 0 || robot_y > MAP_HEIGHT - 1)
      return PA2_MAX_PATH;
   if (map[robot_y][robot_x] == 'B')
      return PA2_MAX_PATH;
   if (temp_map[robot_y][robot_x] == 'V')
      return PA2_MAX_PATH;
   if (robot_energy < 0)
      return PA2_MAX_PATH;

   //' ', 'R', 'C'

   if (map[robot_y][robot_x] == 'C')
   {
      temp_map[robot_y][robot_x] = 'V';
      robot_energy = robot_full_energy;
   }

   if (robot_x == target_x && robot_y == target_y)
   {
      strcat(result_sequence, "T");
      temp_map[robot_y][robot_x] = ' ';
      return 1;
   }

   char up_seq[MAX_STRING_SIZE] = "", right_seq[MAX_STRING_SIZE] = "", down_seq[MAX_STRING_SIZE] = "", left_seq[MAX_STRING_SIZE] = "";
   strcpy(up_seq, result_sequence);
   strcpy(right_seq, result_sequence);
   strcpy(down_seq, result_sequence);
   strcpy(left_seq, result_sequence);

   int up = findPathSequence(robot_x, robot_y - 1, target_x, target_y, robot_energy - 1, robot_full_energy, strcat(up_seq, "U"), map, temp_map);

   int right = findPathSequence(robot_x + 1, robot_y, target_x, target_y, robot_energy - 1, robot_full_energy, strcat(right_seq, "R"), map, temp_map);

   int down = findPathSequence(robot_x, robot_y + 1, target_x, target_y, robot_energy - 1, robot_full_energy, strcat(down_seq, "D"), map, temp_map);

   int left = findPathSequence(robot_x - 1, robot_y, target_x, target_y, robot_energy - 1, robot_full_energy, strcat(left_seq, "L"), map, temp_map);

   // All four directions does not lead to target
   if (up >= PA2_MAX_PATH && right >= PA2_MAX_PATH && down >= PA2_MAX_PATH && left >= PA2_MAX_PATH)
   {
      temp_map[robot_y][robot_x] = ' ';
      return PA2_MAX_PATH;
   }

   int shortest = up;
   strcpy(result_sequence, up_seq);
   if (right < shortest)
   {
      shortest = right;
      strcpy(result_sequence, right_seq);
   }
   if (down < shortest)
   {
      shortest = down;
      strcpy(result_sequence, down_seq);
   }
   if (left < shortest)
   {
      shortest = left;
      strcpy(result_sequence, left_seq);
   }
   temp_map[robot_y][robot_x] = ' ';
   return 1 + shortest;
}

int findFarthestPossibleCharger(int robot_x, int robot_y, int robot_original_x, int robot_original_y, int &target_x, int &target_y, int robot_energy, int robot_full_energy, const char map[MAP_HEIGHT][MAP_WIDTH], char temp_map[MAP_HEIGHT][MAP_WIDTH])
{
   if (robot_x < 0 || robot_x > MAP_WIDTH - 1 || robot_y < 0 || robot_y > MAP_HEIGHT - 1)
      return -1;
   if (map[robot_y][robot_x] == 'B')
      return -1;
   if (robot_energy < 0)
      return -1;

   //' ', 'R', 'C'

   if (map[robot_y][robot_x] == 'C')
   {
      target_x = robot_x;
      target_y = robot_y;
      return findShortestDistance(robot_original_x, robot_original_y, robot_x, robot_y, robot_full_energy, robot_full_energy, map, temp_map);
   }

   int up_x, up_y, right_x, right_y, down_x, down_y, left_x, left_y;

   int up = findFarthestPossibleCharger(robot_x, robot_y - 1, robot_original_x, robot_original_y, up_x, up_y, robot_energy - 1, robot_full_energy, map, temp_map);

   int right = findFarthestPossibleCharger(robot_x + 1, robot_y, robot_original_x, robot_original_y, right_x, right_y, robot_energy - 1, robot_full_energy, map, temp_map);

   int down = findFarthestPossibleCharger(robot_x, robot_y + 1, robot_original_x, robot_original_y, down_x, down_y, robot_energy - 1, robot_full_energy, map, temp_map);

   int left = findFarthestPossibleCharger(robot_x - 1, robot_y, robot_original_x, robot_original_y, left_x, left_y, robot_energy - 1, robot_full_energy, map, temp_map);

   // No charger is found within the energy constraint
   if (up == -1 && right == -1 && down == -1 && left == -1)
   {
      return -1;
   }

   int farthest = up;
   target_x = up_x;
   target_y = up_y;

   if (right > farthest)
   {
      farthest = right;
      target_x = right_x;
      target_y = right_y;
   }
   if (down > farthest)
   {
      farthest = down;
      target_x = down_x;
      target_y = down_y;
   }
   if (left > farthest)
   {
      farthest = left;
      target_x = left_x;
      target_y = left_y;
   }

   return farthest;
}