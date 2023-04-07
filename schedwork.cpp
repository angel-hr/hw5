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
//static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here

bool helper(const AvailabilityMatrix& a, const size_t d, 
            const size_t m, DailySchedule& s, size_t& row);
/*
bool helper(const AvailabilityMatrix& a, DailySchedule& sched,
            const size_t d, size_t col, size_t row, const size_t m);
*/

bool scheduleBuilder(DailySchedule& s, size_t& row, size_t& col,
                    const size_t d, const size_t m); //row corresponds to the day, and column corresponds to worker

bool valid(Worker_T worker, const DailySchedule& sched, const size_t d,
           const size_t m, size_t& count, size_t &row);

// Add your implementation of schedule() and other helper functions here

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
    for (size_t i =0; i < avail.size(); i++){  //allocate row to sched for the days
        vector<Worker_T> day;
        sched.push_back(day);
    }

    

    //return helper(avail, sched, dailyNeed, 0, 0, maxShifts);
    size_t row = 0;
    return helper(avail, dailyNeed, maxShifts, sched, row);  
}

/*
bool helper(const AvailabilityMatrix& a, DailySchedule& sched,
            const size_t d, size_t col, size_t row, const size_t m)
{
    if (row == a.size()) {return true;}
    if (col == a[row].size()) { //if we move to a stack where the col == d then we can go the next row
        //clear the row, and return false
        if (sched[row].size() < d){
            sched[row].clear();
        }
        else {
            if (col != 0){
                
            }
        }
        return false;
    } 

    if (a[row][col] == true){
        bool fit = scheduleBuilder(sched, row, col, d, m);  
        if (fit){
            if (sched[row].size() == d){
                return helper(a,sched, d, 0, row+1, m);
            }
            
        }
    }

    return helper(a, sched, d, col +1, row, m);
}
*/


bool helper(const AvailabilityMatrix& a, const size_t d, 
            const size_t m, DailySchedule& s, size_t& row)
{
    if (row == a.size()){return true;} 

    unsigned int d_count = 0;
    for (size_t col = 0; col < a[row].size(); col++){ // one loop recursive search
        if (a[row][col] == true){
            bool fit = scheduleBuilder(s, row, col, d, m); 
            if (fit){d_count++;}
            if (d_count == d){
                size_t row2 = row + 1;
                bool status = helper(a, d, m, s, row2);
                if (status){return true;} //if false continue
                else {
                    d_count = 0;
                    //s[row].pop_back();
                    s[row].clear();
                    if (col  != 0){
                       --col; //go back a column
                    }
                    //return false;
                }
            }
        }
    }

    s[row].clear();
    return false; //if we go through the entire column without filling the nxd sched row then we don't have a valid solution
}


bool scheduleBuilder(DailySchedule& sched, size_t& row, size_t& col, 
                    const size_t d, const size_t m)
{  
    if (sched[row].size() == d) {return false;} //this means that all columns in the sched matrix are filled so we were not able to put a new woker in the row

    Worker_T worker = col;
    sched[row].push_back(worker);
    size_t count =0, row2 =0;
    bool okay = valid(worker, sched, d, m, count, row2); //check if valid, start at the first row
    if (!okay){
        sched[row].pop_back();
        //sched[row].clear();
        return false;
    }
    return true;
}


bool valid(Worker_T worker, const DailySchedule& sched, const size_t d,
           const size_t m, size_t& count, size_t &row)
{
    if (count > m){return false;}
    if (row == sched.size()) {return true;}

    for (size_t i =0; i < sched[row].size(); i++){
        if (sched[row][i] == worker){
            return valid(worker, sched, d, m, ++count, ++row); //there can only be one instance of a worker in each row
        }
    }
    return valid(worker, sched, d, m, count, ++row);    //if we traverse the row then move to the next row
}