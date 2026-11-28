// Leaf: ?SetPriority@CProcessedInput@@QAEXW4EGameEventPriority@@@Z
// Flags:
#define FABLE_RETAIL_PROCESS_ROLLING_PASS30
#define FABLE_RETAIL_EXPRESSION_FRAME
#include "engine/CProcessedInput.h"
void CProcessedInput::SetPriority(EGameEventPriority priority)
{
    Priority = priority;
}

#include <stdio.h>
#include <string.h>
int main() {
    unsigned long buf[46];
    for(int i=0;i<6;++i) {
        memset(buf,0x5a,sizeof(buf));
        reinterpret_cast<CProcessedInput*>(buf+1)->SetPriority(static_cast<EGameEventPriority>(i));
        for(int j=0;j<46;++j) if(buf[j]!=(j==44? (unsigned long)i : 0x5a5a5a5aUL))return 1;
    }
    puts("00a0d330_TEST PASS"); return 0;
}
