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

    class TaskInfo
    {
        public:

        TaskState       state;
        unsigned long   executionIntervalMicros;
        unsigned long   lastExecutionMicros;
        unsigned long   nextExecutionMicros;
        unsigned long   maxExecutionDurationMicros;
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
            maxExecutionDurationMicros(0),
            executions(0),
            id(0),
            priority(0),
            isIntervalFunction(false) {}
    };

    // Use this to declare a task without a setup or loop
    inline bool DUMMY_FUNCTION(const unsigned long& timeMicros, const TaskInfo& taskInfo)
    {
        return true;
    }

    class Task
    {
        public:  
             
        TaskInfo _taskInfo;
        std::function<bool(const unsigned long&, const TaskInfo&)> _setup;
        std::function<bool(const unsigned long&, const TaskInfo&)> _loop;

        // Interval task constructor
        Task(
            std::function<bool(const unsigned long&, const TaskInfo&)> setup,
            std::function<bool(const unsigned long&, const TaskInfo&)> loop,
            unsigned long executionIntervalMicros
        ) :
        _taskInfo(TaskInfo()),
        _setup(setup),
        _loop(loop)
        {
            _taskInfo.executionIntervalMicros = executionIntervalMicros;
            _taskInfo.isIntervalFunction = true;
        };

        // Interval task constructor with priority
        Task(
            std::function<bool(const unsigned long&, const TaskInfo&)> setup,
            std::function<bool(const unsigned long&, const TaskInfo&)> loop,
            unsigned long executionIntervalMicros,
            int priority
        ) :
        _taskInfo(TaskInfo()),
        _setup(setup),
        _loop(loop)
        {
            _taskInfo.executionIntervalMicros = executionIntervalMicros;
            _taskInfo.isIntervalFunction = true;
            _taskInfo.priority = priority;
        };

        // Idle task constructor
        Task(
            std::function<bool(const unsigned long&, const TaskInfo&)> setup,
            std::function<bool(const unsigned long&, const TaskInfo&)> loop
        ) :
        _taskInfo(TaskInfo()),
        _setup(setup),
        _loop(loop)
        {
            _taskInfo.isIntervalFunction = false;
        };

        // Idle task constructor with priority
        Task(
            std::function<bool(const unsigned long&, const TaskInfo&)> setup,
            std::function<bool(const unsigned long&, const TaskInfo&)> loop,
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
        Task() : Task(DUMMY_FUNCTION, DUMMY_FUNCTION) {};
    };
} // namespace HT_TASK


#endif // __HT_TASK__