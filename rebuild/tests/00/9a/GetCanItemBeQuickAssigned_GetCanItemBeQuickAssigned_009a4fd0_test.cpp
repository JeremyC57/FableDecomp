// Leaf: ?IsMouseButton1Pressed@CSystemManager@@QAE_NXZ
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CSYSTEMMANAGER_MOUSE
#include "engine/CSystemManager.h"

bool CSystemManager::IsMouseButton1Pressed()
{
    return MouseButton1Pressed;
}

#include <stdio.h>
int main() {
    static unsigned long storage[sizeof(CSystemManager)/4+1];
    unsigned char* state=reinterpret_cast<unsigned char*>(storage);
    CSystemManager* manager=reinterpret_cast<CSystemManager*>(storage);
    for(unsigned mask=0;mask<32;++mask) {
        for(unsigned i=0;i<5;++i)state[0xdd+i]=(mask>>i)&1;
        if(manager->IsMouseButton1Pressed()!=((mask&(1u<<1))!=0))return 1;
    }
    puts("009a4fd0_TEST PASS");return 0;
}
