#ifndef AtomicData_h
#define AtomicData_h

#include <atomic>


struct AtomicData{
    std::atomic<bool>   loker_state_;
    std::atomic<int>    temperature_;
    std::atomic<int>    relative_hum_;

};


#endif // AtomicData_h