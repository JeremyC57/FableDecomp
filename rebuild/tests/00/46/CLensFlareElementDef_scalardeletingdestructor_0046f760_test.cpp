#include "rebuild_abi.h"
#include <cstdio>

class CLandscapeBackgroundPatch;

class CParentDefClassBase {
public:
    virtual ~CParentDefClassBase();
};
CParentDefClassBase::~CParentDefClassBase()
{
    CLandscapeBackgroundPatch* thisptr = (CLandscapeBackgroundPatch*)this; (void)thisptr; (void)thisptr;
}
extern "C" void __cdecl operator_delete_thunk(void* p) { (void)p; }

void* __fastcall CLensFlareElementDef_vector_deleting_dtor(
        CLandscapeBackgroundPatch* thisptr, int edx, unsigned int flags);

int main()
{
    char buf[8];
    void* r = CLensFlareElementDef_vector_deleting_dtor(
                  (CLandscapeBackgroundPatch*)buf, 0, 0u);
    if (r == (void*)buf)
        printf("OK_0046f760\n");
    else
        printf("BAD_0046f760\n");
    return 0;
}