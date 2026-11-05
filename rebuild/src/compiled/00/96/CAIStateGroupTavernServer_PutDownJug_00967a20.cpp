// CAIStateGroup_TavernServer::PutDownJug() -- put the empty jug down on the bar (PUT_EMPTY_JUG_ON_BAR, mode 0x29)
// Leaf: ?PutDownJug@CAIStateGroup_TavernServer@@QAE_NXZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// and free the bar point; on success clear the jug's holder component, forget the jug and the serving point.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00967A20, length 288 bytes.
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CCreatureAction_PutDownThingAtTable
// (creature, const CCharString&, mode) @ 0x00842A20, AIAction_PerformActionAndWait @ 0x0097DB70, ~CCreatureActionBase
// @ 0x00693EF0, CAIStateGroup_TavernKeeper::FreeBarPoint @ 0x00967110, CBaseIntelligentPointer GetPItem/ctor/SetPItem/
// dtor (PJug; null temporary vtable 0x0125A858), CVectorMap::LowerBound @ 0x0040F020 (0xBB = bit 27 of +0x34).
// Shape: CCreatureAction_PutDownJug (vtable 0x012964DC) is a derived action whose inline constructor builds the
// animation-name CCharString as an IMPLICIT conversion temporary inside its base mem-initializer (built before
// any argument push) (so the string dies BEFORE the derived vtable store);
// the action is a named local in its own block with the `!= 2` result in bl across its destructor and FreeBarPoint.
// Engine header names (rebuild/include/engine/CAIStateGroup_TavernServer.h; flattened base, modelled locally).
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound (this TU's copy) -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem((CBaseObject*)(((CThing*)other.GetPItem()))); return *this; }
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!((CThing*)p.GetPItem())->IsDead())
            return true;
    }
    return false;
}

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

long __fastcall GFGetCurrentFrame();            // 0x0049D870
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void v13();
    virtual long GetTimeToInitiate();           // slot 14 (+0x38)
    virtual void v15(); virtual void v16(); virtual void v17();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    // the landed GetRandomObject random step: seed = ror13(seed*0x24A1 + 0x24DF), r = seed % range
    unsigned long Random(unsigned long range)
    {
        unsigned long* seed = GetPWorldSeed();
        unsigned long next = *seed * 0x24A1U + 0x24DFU;
        *seed = next;
        unsigned long rotated = GFROR13(next);
        *seed = rotated;
        return rotated % range;
    }
};

// vtable padding helpers: V4/V16 declare 4/16 placeholder virtual slots
#define V4(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3();
#define V16(n) V4(n##a) V4(n##b) V4(n##c) V4(n##d)
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
class CThingCreatureBase;

class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _body[0xa4];                      // +0x04..+0xa7
    virtual ~CCreatureActionBase();                         // 0x00693EF0
};
class CCreatureAction_PutDownThingAtTable : public CCreatureActionBase { public:
    long m_thing;                                   // +0xa8
    CCreatureAction_PutDownThingAtTable(CThingCreatureBase* owner, const CCharString& animation, long mode);   // 0x00842A20
};
class CCreatureAction_PutDownJug : public CCreatureAction_PutDownThingAtTable { public:   // vtable 0x012964DC
    virtual void v00();
    CCreatureAction_PutDownJug(CThingCreatureBase* owner)
        : CCreatureAction_PutDownThingAtTable(owner, "PUT_EMPTY_JUG_ON_BAR", 0x29) { }
};
struct CTCJugHolder {                               // interface 0xBB on the jug
    unsigned char _pad[0x0c];
    CThing* m_pHolder;                              // +0x0c
};
struct CIntelligentPointerObject : CBaseIntelligentPointer {   // CIntelligentPointer<CThingObject> (vtable 0x0125A858)
    virtual void v00();
    CIntelligentPointerObject(CThing* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointerObject& operator=(const CIntelligentPointerObject& other) { SetPItem((CBaseObject*)(((CThing*)other.GetPItem()))); return *this; }
};

struct CAIStateGroup_TavernServer : CAIStateGroupBase {
    CIntelligentPointerObject PJug;                 // +0x14
    CBaseIntelligentPointer PBar;                   // +0x1c
    CBaseIntelligentPointer PTable;                 // +0x24
    CBaseIntelligentPointer PTankard;               // +0x2c
    CBaseIntelligentPointer PPub;                   // +0x34
    long JugIndex;                                  // +0x3c
    long ServingActionPointIndex;                   // +0x40
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool interruptible);   // 0x0097DB70
    void FreeBarPoint();                            // 0x00967110
    bool PutDownJug();
};

bool CAIStateGroup_TavernServer::PutDownJug()
{
    CThingCreatureBase* creature = (CThingCreatureBase*)PBrain->PCreature;
    bool failed;
    {
        CCreatureAction_PutDownJug action(creature);
        failed = AIAction_PerformActionAndWait(&action, false) != 2;
    }
    FreeBarPoint();
    if (failed)
        return false;
    if (IS_THING_ALIVE(PJug)) {
        CThing* jug = ((CThing*)PJug.GetPItem());
        if (jug->HasTC(TCI_TAVERN_JUG)) {
            CTCJugHolder* holder;
            jug->GetTC(TCI_TAVERN_JUG, &holder);
            holder->m_pHolder = 0;
        }
    }
    PJug = CIntelligentPointerObject(0);
    ServingActionPointIndex = -1;
    return true;
}