// Behaviour test for 0x009EDC50 CCountedPointer<CConsoleInputBase>::DeleteData (NF81, round 7, q10984: was the AutoTiny stand-in
// DeleteData(AutoTinyVirtualObject*)): a null pointer deletes nothing; an object is destroyed through its virtual destructor.
// The header's static member is used under its FableWin.pdb name; CConsoleInputBase (no shared header) as in the row.
#include <cstdio>
#include "engine/CCountedPointer.h"
class CConsoleInputBase {
public:
    virtual ~CConsoleInputBase();
};
static int g_ConsoleInputDestructorCalls = 0;
CConsoleInputBase::~CConsoleInputBase()
{
    ++g_ConsoleInputDestructorCalls;
}
int main()
{
    CCountedPointer<CConsoleInputBase>::DeleteData(0);
    if (g_ConsoleInputDestructorCalls != 0)
        return 1;
    CConsoleInputBase* object = new CConsoleInputBase;
    CCountedPointer<CConsoleInputBase>::DeleteData(object);
    if (g_ConsoleInputDestructorCalls != 1)
        return 1;
    std::printf("AUTO_TINY_009edc50_TEST PASS\n");
    return 0;
}
