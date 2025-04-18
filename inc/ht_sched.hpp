#ifndef __HT_SCHED__
#define __HT_SCHED__

#include <functional>
#include "ht_task.hpp"
#include <climits>

#define HT_SCHED_MAX_TASKS 32

namespace HT_SCHED
{
    class Scheduler
    {
        public:

        // Singleton patterning
        static Scheduler& getInstance()
        {
            static Scheduler instance;
            return instance;
        }

        void setTimingFunction(std::function<unsigned long()>);
        bool schedule(HT_TASK::Task& task);
        void run();

        // init function for SchedMon task
        HT_TASK::TaskResponse initSchedMon(const unsigned long& sysMicros, const HT_TASK::TaskInfo& taskInfo);
        // loop for SchedMon task
        HT_TASK::TaskResponse schedMon(const unsigned long& sysMicros, const HT_TASK::TaskInfo& taskInfo);
        const float& getPeriodicUtilization();
        const float& getIdleUtilization();
        const float& getSchedulerUtilization();
        
        private:
        
        Scheduler();

        HT_TASK::Task*                  _taskQueue[HT_SCHED_MAX_TASKS]; // ordered list of task object pointers
        int                             _numTasks;
        std::function<unsigned long()>  _microsFunction;
        unsigned long                   _timeOfNextExec;        // time at which the next scheduled function must trigger
        unsigned long                   _intervalExecTimer;     // accumulation of time spent executing scheduled tasks
        unsigned long                   _idleExecTimer;         // accumulation of time spent executing idle tasks
        unsigned long                   _schedulerExecTimer;    // accumulation of time spent in the scheduler

        // variables for SchedMon
        float periodicUtilization;  // CPU utilization of periodic tasks
        float idleUtilization;      // CPU utilization of idle tasks
        float schedulerUtilization; // CPU utilization of the scheduler
    };
} // namespace HT_SCHED


#endif // __HT_SCHED__
