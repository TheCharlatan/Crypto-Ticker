#include <iostream>
#include "learner.h"

Learner::Learner()
{
    china_move = false;
    usd_move = false;
    score_china1 = 0;
    score_china2 = 0;
    score_usd1 = 0;
    score_usd2 = 0;
}
//constant data feed timed in ~seconds.
void Learner::data_feeder(double china1_current, double china2_current, double usd1_current, double usd2_current)
{
    //check if the there is a significant discrimination between the markets.
    if (!china_move)
    {
      if ((china1_current-china2_current > 0.1) | (china1_current-china2_current < -0.1))
        china_move = true;
    }
    //Only set it to false again, once it reaches a certain capacity and is subject to no movement between them.
    if ((china1.size() == 11) && (zero_movement(china1,china2)))
        china_move = false;
    if (china_move)
    {
        push_vector(china1_current, china1);
        push_vector(china2_current, china2);
    }
    if ((usd1_current-usd2_current > 0.01) | (usd1_current-usd2_current < -0.01))
        usd_move = true;
    if ((usd1.size() == 11) && (zero_movement(usd1,usd2)))
        usd_move = false;
    if (usd_move)
    {
        push_vector(usd1_current, usd1);
         push_vector(usd2_current, usd2);
    }
    if (usd_move & china_move && (china1.size() >= 11))
        get_direction();
    if (china_measure || usd_measure)
        set_score();
}

bool Learner::zero_movement(std::vector<double> &once, std::vector<double> &twice)
{
    //check if there has been no difference since more than 10 seconds
    if (once.size() >= 11)
        return false;
    for (unsigned short c(0); c <= 10; c++)
    {
        if ((once[c]-twice[c] > 0.1) | (once[c]-twice[c] < -0.1))
            return false;
    }
    return true;
}

void Learner::push_vector(double current, std::vector<double> &new_storage)
{
    //push data into a vector up until size 12
    if (new_storage.size() <= 12)
        new_storage.push_back(current);
    else
    {
        for (unsigned short c(0); c < 11; c++)
        {
            new_storage[c] = new_storage[c+1];
        }
        new_storage[11] = current;
    }
}

void Learner::get_direction()
{
    std::cout << "I am looking for direction";
    //assess if the market is moving up or down, true is encoded for up, false is for down
    if ((china1.front()-china1.back() > 0.1))
        china1_direction = true;
    else
        china1_direction = false;
    if ((china2.front()-china2.front()) > 0.1)
        china2_direction = true;
    else
        china2_direction =false;
    if ((usd1.front() - usd1.back() > 0.1))
        usd1_direction = true;
    else
        usd1_direction = false;
    if ((usd2.back()- usd2.front()) > 0.1)
        usd2_direction = true;
    else
        usd2_direction = false;
    //set the measurement record when a deviation is first detected, do not touch it until said deviation has been resolved.
    if ((china1_direction && !china2_direction) || (!china1_direction && china2_direction))
    {
        if (!china_measure)
        {
            china1_measure.set_measurements(china1_direction, china1.back());
            china2_measure.set_measurements(china2_direction, china2.back());
            china_measure = true;
            std::cout << "I have found some directions!\n";
        }
    }
    if ((usd1_direction && !usd2_direction) || (!usd1_direction && usd2_direction))
    {
        if (!usd_measure)
        {
            usd1_measure.set_measurements(usd1_direction, usd1.back());
            usd2_measure.set_measurements(usd2_direction, usd2.back());
            usd_measure = true;
            std::cout << "I have found some directions!\n";
        }
    }
}
//once it resolves, give the market a score to
void Learner::set_score()
{
    std::cout << "I am looking at the score!";
    if (china_measure)
    {
        if (china1_measure.direction == china2_direction)
            score_china1 += pow(china1_measure.price - china1.front(), 2);
        if (china2_measure.direction == china1_direction)
            score_china2 += pow(china2_measure.price - china2.front(), 2);
    }
    if (usd_measure)
    {
        if (usd1_measure.direction == usd2_direction)
            score_usd1 += pow(usd1_measure.price - usd1.front(), 2);
        if (usd2_measure.direction == usd1_direction)
            score_usd2 += pow(usd2_measure.price - usd2.front(), 2);
    }
}
