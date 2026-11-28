// Leaf: ?ProcessAnalogJoystickAxis@CInputProcessGameBase@@IBEXAAM@Z
// Flags:
#define FABLE_RETAIL_PROCESS_ROLLING_PASS30
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_COMBAT_INPUT_PASS26
#include "engine/CInputProcessGameBase.h"
void CInputProcessGameBase::ProcessAnalogJoystickAxis(float&) const
{
}

#include <stdio.h>
#include <string.h>
struct FixtureAxis : CInputProcessGameBase {
    void process(float& value) { ProcessAnalogJoystickAxis(value); }
};
int main() {
    unsigned long store[16]={0};
    unsigned long bits[]={0,0x80000000UL,0x3f800000UL,0xbf800000UL,0x7fc01234UL,0x7f800000UL};
    for(int i=0;i<6;++i) {
        float f;
        memcpy(&f,&bits[i],4);
        reinterpret_cast<FixtureAxis*>(store)->process(f);
        if(memcmp(&f,&bits[i],4))return 1;
    }
    puts("00687a80_TEST PASS"); return 0;
}
