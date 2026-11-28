// Leaf: ?SetPriority@CProcessedInput@@QAEXW4EGameEventPriority@@@Z
// Flags:
#define FABLE_RETAIL_PROCESS_ROLLING_PASS30
#define FABLE_RETAIL_EXPRESSION_FRAME
#include "engine/CProcessedInput.h"
void CProcessedInput::SetPriority(EGameEventPriority priority)
{
    Priority = priority;
}
