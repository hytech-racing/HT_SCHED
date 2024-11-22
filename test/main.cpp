#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <stdio.h>
#include <functional>
#include "ht_sched.hpp"

auto start_time = std::chrono::high_resolution_clock::now();

unsigned long stdMicros()
{
    auto now = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - start_time).count();
    return static_cast<unsigned long>(elapsed);
}

bool task1F(const unsigned long& sysMicros, const HT_TASK::TaskInfo& taskInfo)
{
    std::cout << "task1 exec " << taskInfo.executions << "\n";
    return true;
}

bool task2F(const unsigned long& sysMicros, const HT_TASK::TaskInfo& taskInfo)
{
    std::cout << "task2 exec " << taskInfo.executions << "\n";
    return true;
}

bool task3F(const unsigned long& sysMicros, const HT_TASK::TaskInfo& taskInfo)
{
    // std::cout << "task3 filtered duration " << taskInfo.filteredExecutionDurationMicros << "\n";
    // std::cout << "task3 last execution " << taskInfo.lastExecutionMicros << "\n";
    return true;
}

HT_SCHED::Scheduler scheduler = HT_SCHED::Scheduler(stdMicros);

HT_TASK::Task task1 = HT_TASK::Task(HT_TASK::DUMMY_FUNCTION, task1F, 20000UL, 2); // 20000us is 50hz
HT_TASK::Task task2 = HT_TASK::Task(HT_TASK::DUMMY_FUNCTION, task2F, 20000UL, 1); // task2 is higher priority
HT_TASK::Task task3 = HT_TASK::Task(HT_TASK::DUMMY_FUNCTION, task3F); // task3 is an idle task

HT_TASK::Task schedMon = HT_TASK::Task(
    std::bind(&HT_SCHED::Scheduler::initSchedMon, std::ref(scheduler), std::placeholders::_1, std::placeholders::_2), 
    std::bind(&HT_SCHED::Scheduler::schedMon, std::ref(scheduler), std::placeholders::_1, std::placeholders::_2), 
    20000UL
);

int main(void)
{
    scheduler.schedule(task1);
    scheduler.schedule(task2);
    scheduler.schedule(task3);
    scheduler.schedule(schedMon);

    while (std::chrono::high_resolution_clock::now() - start_time < std::chrono::seconds(1))
    {
        scheduler.run();
    }

    return 0;
}