#include "rebuild_abi.h"
#include <cstdio>

class CLandscapeBackgroundPatch { public: int dummy; };

extern "C" void __fastcall CCombatSequenceDef_dtor(CLandscapeBackgroundPatch* thisptr){ (void)thisptr; }
extern "C" void __cdecl LBP_operator_delete(void* p){ (void)p; }

void* __fastcall CCombatSequenceDef_vector_deleting_dtor(
    CLandscapeBackgroundPatch* thisptr, int, unsigned int flags);

int main()
{
    CLandscapeBackgroundPatch obj;
    obj.dummy = 42;
    void* r = CCombatSequenceDef_vector_deleting_dtor(&obj, 0, 0u);
    if (r == (void*)&obj)
        printf("OK_004588c6\n");
    else
        printf("FAIL_004588c6\n");
    return 0;
}