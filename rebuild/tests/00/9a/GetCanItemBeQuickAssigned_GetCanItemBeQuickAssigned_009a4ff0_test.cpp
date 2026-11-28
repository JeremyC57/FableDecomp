// Leaf: ?IsMouseButton3Pressed@CSystemManager@@QAE_NXZ
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CSYSTEMMANAGER_MOUSE
#include "engine/CSystemManager.h"

bool CSystemManager::IsMouseButton3Pressed()
{
    return MouseButton3Pressed;
}

#include <stdio.h>
int main() {
    static unsigned long storage[sizeof(CSystemManager)/4+1];
    unsigned char* state=reinterpret_cast<unsigned char*>(storage);
    CSystemManager* manager=reinterpret_cast<CSystemManager*>(storage);
    for(unsigned mask=0;mask<32;++mask) {
        for(unsigned i=0;i<5;++i)state[0xdd+i]=(mask>>i)&1;
        if(manager->IsMouseButton3Pressed()!=((mask&(1u<<3))!=0))return 1;
    }
    puts("009a4ff0_TEST PASS");return 0;
}
