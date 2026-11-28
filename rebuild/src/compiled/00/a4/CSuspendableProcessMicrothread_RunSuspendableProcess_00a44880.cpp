// Flags: 
// 00a44880 CSuspendableProcessMicrothread::RunSuspendableProcess
// Leaf: ?RunSuspendableProcess@CSuspendableProcessMicrothread@@UAEXXZ
#define FABLE_RETAIL_PASS22_SCOREBOARD_ORIGINAL_OVERFLOW_SCOPE
#define FABLE_RETAIL_STLP_MICROTHREAD_MALLOC_PASS30
#define FABLE_RETAIL_MICROTHREAD_PROCESS_STACK_PASS30
#define FABLE_RETAIL_CASUSPENDABLEPROCESS_ACTIVE_PASS27
#include "engine/CSuspendableProcessMicrothread.h"
double FABLE_FASTCALL GFGetTime();
#ifdef _MSC_VER
#pragma optimize("", off)
#endif
void CSuspendableProcessMicrothread::RunSuspendableProcess()
{
    if (PActiveProcess != 0) {
        CASuspendableProcess* pold_process = PActiveProcess;
        ProcessStack.push_back(static_cast<CSuspendableProcessMicrothread*>(PActiveProcess));
        ProcessStack.push_back(this);
        InternalSuspend();
        PActiveProcess = pold_process;
    } else {
        ProcessStack.push_back(this);
        while (!ProcessStack.empty()) {
            CSuspendableProcessMicrothread* process = ProcessStack.back();
            ProcessStack.pop_back();
            TimeProcessSliceStarted = (float)GFGetTime();
            InternalSwitchToProcess(*process);
        }
    }
}
#ifdef _MSC_VER
#pragma optimize("", on)
#endif
