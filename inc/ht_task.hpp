#ifndef __HT_TASK__
#define __HT_TASK__

#include <functional>

namespace HT_TASK
{    
    enum TaskState
    {
        UNINITIALIZED   = 0,
        RUNNING         = 1,
        KILLED          = 2,
        NumStates
    };

    enum TaskResponse
    {
        YIELD           = 0, // If the task should run again (not permanently end)
        EXIT            = 1, // If the task should permanently end
    };

    class TaskInfo
    {
        public:

        TaskState       state;
        unsigned long   executionIntervalMicros;
        unsigned long   lastExecutionMicros;
        unsigned long   nextExecutionMicros;
        unsigned long   filteredExecutionDurationMicros;
        unsigned long   executions;
        unsigned int    id;
        // use priority to enforce ordering when multiple tasks execute on the same scheduling cycle
        unsigned int    priority;   // 0 is highest priority.
        bool            isIntervalFunction;

        TaskInfo():
            state(TaskState::UNINITIALIZED),
            executionIntervalMicros(1),
            lastExecutionMicros(0),
            nextExecutionMicros(0),
            filteredExecutionDurationMicros(0),
            executions(0),
            id(0),
            priority(0),
            isIntervalFunction(false) {}
    };

    // Use this to declare a task without a setup or loop
    inline TaskResponse DUMMY_FUNCTION(const unsigned long& timeMicros, const TaskInfo& taskInfo)
    {
        return TaskResponse::YIELD;
    }

    class Task
    {
        public:  
             
        TaskInfo _taskInfo;
        std::function<TaskResponse(const unsigned long&, const TaskInfo&)> _setup;
        std::function<TaskResponse(const unsigned long&, const TaskInfo&)> _loop;

        // Interval task constructor
        Task(
            std::function<TaskResponse(const unsigned long&, const TaskInfo&)> setup,
            std::function<TaskResponse(const unsigned long&, const TaskInfo&)> loop,
            int priority,
            unsigned long executionIntervalMicros
        ) :
        _taskInfo(TaskInfo()),
        _setup(setup),
        _loop(loop)
        {
            _taskInfo.executionIntervalMicros = executionIntervalMicros;
            _taskInfo.isIntervalFunction = true;
            _taskInfo.priority = priority;
        };

        // Idle task constructor with priority
        Task(
            std::function<TaskResponse(const unsigned long&, const TaskInfo&)> setup,
            std::function<TaskResponse(const unsigned long&, const TaskInfo&)> loop,
            int priority
        ) :
        _taskInfo(TaskInfo()),
        _setup(setup),
        _loop(loop)
        {
            _taskInfo.isIntervalFunction = false;
            _taskInfo.priority = priority;
        };

        // default constructor
        Task() : Task(DUMMY_FUNCTION, DUMMY_FUNCTION, 0) {};
    };
} // namespace HT_TASK


#endif // __HT_TASK__