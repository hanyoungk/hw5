#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#endif

// add or remove necessary headers as you please
#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool scheduleHelper(const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    vector<int> workerShifts,
    int row, int col);


// Add your implementation of schedule() and other helper functions here


/**
 * @brief Produces a work schedule given worker availability,
 *        the number of needed workers per day, and the maximum 
 *        shifts any single worker is allowed. Returns true
 *        and the valid schedule if a solution exists, and false
 *        otherwise. 
 * 
 * @param [in]  avail n x k vector of vectors (i.e. matrix) of the availability
 *                    of the k workers for each of the n days
 * @param [in]  dailyNeed Number of workers needed per day (aka d)
 * @param [in]  maxShifts Maximum shifts any worker is allowed over 
 *                        the n day period (aka m)
 * @param [out] sched n x d vector of vectors indicating the d workers
 *                    who are scheduled to work on each of the n days
 * @return true if a solution exists; sched contains the solution
 * @return false if no solution exists; sched is undefined (can be anything)
 */
bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below

    //keep track of row and col of avail matrix (vector of vector)
    int row = 0; //day #
    int col = 0; //worker ID

    //create a data structure to keep track of # shifts each worker has worked
    // worker IDs:         0  1  2  3  4
    // # of shifts worked [0][0][0][0][0]
    vector<int> workerShifts;
    for(size_t i = 0; i < avail[0].size(); i++) {
        workerShifts.push_back(0);
    }

    //set of sched for # of days (resizing)
    sched.resize(avail.size());


    return scheduleHelper(avail, dailyNeed, maxShifts, sched, workerShifts, row, col);
}

bool scheduleHelper(const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    vector<int> workerShifts,
    int row, int col
)
{

    int currentWorker = 0;

    //base case: reached the end of matrix
    if(row == avail.size()) {
        return true;
    }

    //if in the last column or 
    if(col > avail[0].size() - 1 || sched[row].size() == dailyNeed) {
        //go to the next row & reset the column
        return scheduleHelper(avail, dailyNeed, maxShifts, sched, workerShifts, row+1, 0);
    }


    for(size_t id = 0; id < avail[0].size(); id++) { //for the entire row
        //if worker ID(col) has LESS than m && # worker for day(row) has LESS than dailyNeed

        // if worker is available & worker's shift is < m & # workers assigned for day has LESS than dailyNeed
        if(avail[row][id] && workerShifts[id] < maxShifts && sched[row].size() < dailyNeed) {

            // check if the worker is alr in the scheudle
            if(find(sched[row].begin(), sched[row].end(), id)  == sched[row].end())
            {
                //add the worker to the shift
                sched[row].push_back(id);
                //update the worker's shift amount
                workerShifts[id]++;
                currentWorker = id;
                if(scheduleHelper(avail, dailyNeed, maxShifts, sched, workerShifts, row, col+1))
                    return true;
                sched[row].pop_back();
                workerShifts[currentWorker]--;
            }
        }
    }
    return false;

    


}