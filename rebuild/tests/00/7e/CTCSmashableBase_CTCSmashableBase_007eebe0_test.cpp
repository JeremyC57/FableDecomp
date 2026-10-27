// behaviour test for 0x007EEBE0 CTCSmashableBase::CTCSmashableBase(CThing&) (model copy of the source; CTCBase's ctor is a stub)
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
class CSmashableDef;
class CDefPointeeBase;
template <class T> class CDefPointer {                    // Ego_r.pdb: CDefPointeeBase* Object
public:
    CDefPointeeBase* Object;                             // +0x00
    CDefPointer() : Object(0) { }
};
class CTCSmashableBase : public CTCBase {
public:
    CTCSmashableBase(CThing& thing);
    long LastWorldFrameHit;                              // +0x0C (not initialised here)
    bool Smashed;                                        // +0x10
    CDefPointer<const CSmashableDef> Definition;         // +0x14
};
CTCSmashableBase::CTCSmashableBase(CThing& thing)
    : CTCBase(thing)
{
}
int main()
{
    CThing t; t.id = 1;
    static double raw[(sizeof(CTCSmashableBase) + 7) / 8];
    std::memset(raw, 0xCD, sizeof(raw));
    CTCSmashableBase* p = new (raw) CTCSmashableBase(t);
    int ok = g_baseThing == &t && p->ParentThing == &t && p->Definition.Object == 0;
    if (!ok) { std::printf("007eebe0_TEST FAIL\n"); return 1; }
    std::printf("007eebe0_TEST PASS\n");
    return 0;
}