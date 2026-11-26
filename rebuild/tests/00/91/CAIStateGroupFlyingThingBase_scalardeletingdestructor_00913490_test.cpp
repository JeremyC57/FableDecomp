#include <cstdio>
#include <cstdlib>
#include <new>
// CAIStateGroup_FlyingThingBase::`scalar deleting destructor' (retail 0x00913490): the implicit destructor chain (Name +0x0C, then ~CBaseClass),
//   then operator delete only when bit 0 of the flags is set (a virtual delete passes 1, an explicit destructor call none).
// NF472: self-contained -- the row is now a thin member of its owner's TU (the ??_G is compiler-generated), so the test models the retail classes
//   with IMPLICIT destructors and lets VC7.1 generate the deleting destructor here, as it does in the row's object.
static int g_dtor = 0, g_delete = 0, g_order = 0;
class CBaseClass { public: virtual ~CBaseClass(); };
class CCharString { public: ~CCharString(); void* PString; };
class CAIBrain;
class CAIStateGroupBase : public CBaseClass { public: CAIBrain* PBrain; long ScriptAIPriority; CCharString Name; bool Terminated; unsigned char Priority; bool Enabled; };
CBaseClass::~CBaseClass() { g_order = g_order * 10 + 2; ++g_dtor; }
CCharString::~CCharString() { g_order = g_order * 10 + 1; }
class CAIStateGroup_FlyingThingBase : public CAIStateGroupBase { public: float MinHeightToKeepAboveGround, MinHeightToKeepAboveGroundInFlight, MaxHeightAboveGround, FlightRange; };
void operator delete(void* p) { ++g_delete; std::free(p); }
int main()
{
    CBaseClass* p = new CAIStateGroup_FlyingThingBase;
    delete p;                                                   // ??_G with flags 1
    if (g_dtor != 1 || g_delete != 1 || g_order != 12) { std::printf("BAD_DELETE %d %d %d\n", g_dtor, g_delete, g_order); return 1; }
    static double buf[16];
    CAIStateGroup_FlyingThingBase* q = new (buf) CAIStateGroup_FlyingThingBase;
    g_order = 0;
    q->CAIStateGroup_FlyingThingBase::~CAIStateGroup_FlyingThingBase();      // no delete
    if (g_dtor != 2 || g_delete != 1 || g_order != 12) { std::printf("BAD_DTOR %d %d %d\n", g_dtor, g_delete, g_order); return 1; }
    std::printf("PASS_00913490\n"); return 0;
}
