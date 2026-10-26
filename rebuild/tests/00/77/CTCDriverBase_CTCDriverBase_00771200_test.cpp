// Behaviour test for CTCDriverBase::CTCDriverBase(CThing*) @ 0x00771200: the base stores the parent and the
// constructor then calls the parent's SetAsDrawable(true) (CThing vtable slot 28).
#include <cstdio>
#include <new>
static int g_drawableCalls = 0;
static bool g_drawableArg = false;
class CThing { public:
    virtual void v00() { } virtual void v01() { } virtual void v02() { } virtual void v03() { }
    virtual void v04() { } virtual void v05() { } virtual void v06() { } virtual void v07() { }
    virtual void v08() { } virtual void v09() { } virtual void v10() { } virtual void v11() { }
    virtual void v12() { } virtual void v13() { } virtual void v14() { } virtual void v15() { }
    virtual void v16() { } virtual void v17() { } virtual void v18() { } virtual void v19() { }
    virtual void v20() { } virtual void v21() { } virtual void v22() { } virtual void v23() { }
    virtual void v24() { } virtual void v25() { } virtual void v26() { } virtual void v27() { }
    virtual void SetAsDrawable(bool drawable) { ++g_drawableCalls; g_drawableArg = drawable; }
};
struct CTCBase_Calls {
    virtual void v00() { }
    CThing* ParentThing;
    bool    ToBeRemoved;
    bool    IsInGlobalUpdateTCs;
    CTCBase_Calls(CThing* parent) : ParentThing(parent), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { }
};
class CTCDriverBase : public CTCBase_Calls { public:
    virtual void v00() { }
    CTCDriverBase(CThing* parent);
};
CTCDriverBase::CTCDriverBase(CThing* parent) : CTCBase_Calls(parent) { ParentThing->SetAsDrawable(true); }
int main()
{
    CThing thing;
    CTCDriverBase driver(&thing);
    if (driver.ParentThing != &thing || g_drawableCalls != 1 || g_drawableArg != true) { std::printf("00771200_TEST FAIL\n"); return 1; }
    std::printf("00771200_TEST PASS\n");
    return 0;
}