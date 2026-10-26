// Test for 0x00686830 CTCBase::~CTCBase: destroying a component runs the derived, CTCBase and base destructors in order.
#include <stdio.h>
static char g_order[8]; static int g_n = 0;
class CBaseClassNonCopyable {
protected:
    CBaseClassNonCopyable() { }
    virtual ~CBaseClassNonCopyable() { g_order[g_n++] = 'B'; }
};
class CTCBase : public CBaseClassNonCopyable {
public:
    virtual ~CTCBase();
protected:
    void* ParentThing;
    bool  ToBeRemoved;
    bool  IsInGlobalUpdateTCs;
};
CTCBase::~CTCBase()
{
    g_order[g_n++] = 'T';
}
class CTCDerived : public CTCBase {
public:
    ~CTCDerived() { g_order[g_n++] = 'D'; }
};
int main()
{
    CTCBase* p = new CTCDerived;
    delete p;
    bool ok = g_n == 3 && g_order[0] == 'D' && g_order[1] == 'T' && g_order[2] == 'B';
    if (!ok) { printf("00686830_TEST FAIL\n"); return 1; }
    printf("00686830_TEST PASS\n");
    return 0;
}
