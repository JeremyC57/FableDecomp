// behaviour test for 0x008EC850 CTCDegradableBase::CTCDegradableBase(CThing&) (model copy of the source; CTCBase's ctor is a stub)
#include <cstdio>
#include <cstring>
#include <new>
class CThing { public: int id; };
static CThing* g_baseThing = 0;
class CTCBase {
public:
    CTCBase(CThing& thing) : ParentThing(&thing), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { g_baseThing = &thing; }
    virtual ~CTCBase() { }
    CThing* ParentThing;
    bool ToBeRemoved;
    bool IsInGlobalUpdateTCs;
};
class CDegradableDef;
class CDegradableInfo;
class CThingPhysical;
class CDefPointeeBase;
template <class T> class CDefPointer {                    // Ego_r.pdb: CDefPointeeBase* Object
public:
    CDefPointeeBase* Object;                             // +0x00
    CDefPointer() : Object(0) { }
};
class CTCDegradableBase : public CTCBase {
public:
    CTCDegradableBase(CThing& thing);
    long LastWorldFrameHit;                              // +0x0C (not initialised here)
    CDefPointer<const CDegradableDef> PDefinition;       // +0x10
    CDegradableInfo* PCurrentDegradableInfo;             // +0x14
    CThingPhysical* PParentThingPhysical;                // +0x18
};
CTCDegradableBase::CTCDegradableBase(CThing& thing)
    : CTCBase(thing)
{
}
int main()
{
    CThing t; t.id = 1;
    static double raw[(sizeof(CTCDegradableBase) + 7) / 8];
    std::memset(raw, 0xCD, sizeof(raw));
    CTCDegradableBase* p = new (raw) CTCDegradableBase(t);
    int ok = g_baseThing == &t && p->ParentThing == &t && p->PDefinition.Object == 0;
    if (!ok) { std::printf("008ec850_TEST FAIL\n"); return 1; }
    std::printf("008ec850_TEST PASS\n");
    return 0;
}