// CAIStateGroup_MinionReactToUnconsciousPerson::IsRunnable() -- runnable when there is an unconscious person to
// Leaf: ?IsRunnable@CAIStateGroup_MinionReactToUnconsciousPerson@@UAE_NXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// react to (an undiscovered one unless the creature is already in life state 3); remember it in PPerson.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00937D20, length 122 bytes (+ int3 padding). Vtable slot 11.
// Callees: the creature's life-state getter @ 0x00662010 (`[this+0E0h]+0F0h`), FindUnconsciousPerson @ 0x00937C20
// (state 3) / FindUndiscoveredUnconsciousPerson @ 0x00937B10, then the converting CIntelligentPointer temporary
// (vtable 0x0123E7D8 = CIntelligentPointer<CThingCreatureBase>): CBaseIntelligentPointer::CBaseIntelligentPointer
// @ 0x00A01B10, SetPItem @ 0x00A01B90 (twice), GetPItem @ 0x00A01B50, ~CBaseIntelligentPointer @ 0x00A01C10.
// Shape: `person = (state == 3) ? FindUnconsciousPerson() : FindUndiscoveredUnconsciousPerson();` (the `this`
// reload sits between the compare and the branch), the result tested for null and the dead bit (+0x91 bit 0),
// `PPerson = person` through the temporary in the 8-byte frame, explicit `return true` / `return false`
// (`mov al,1` / `xor al,al`). No engine header for this class; PPerson is +0x14.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
#include "fable_compiler.h"
class CTCBase;
class CThing;
class CWorld;
class CThingAICreature;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020 -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

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
    CWorld* GetPWorld();                        // 0x008FCFC0 (PBrain->+0x1c->+0x24; manifest "CPlayer::GetCurrentMode" is propagated)
    unsigned long* GetPWorldSeed();             // 0x008FCFD0 (world random seed)
};

long __fastcall GFGetCurrentFrame();            // 0x0049D870 (manifest "GFGetPreMainMemoryUsed" is propagated)
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

struct CBaseIntelligentPointerCtor {            // the full CBaseIntelligentPointer interface (converting temporaries)
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointerCtor();              // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointerCtor();             // 0x00A01C10
};

// `PThing = thing` is the converting-temporary + inline `SetPItem(other.GetPItem())` assignment (the landed
// 0x00902045 shape): base constructor, the derived vtable store, SetPItem on the temporary, GetPItem from it,
// SetPItem on the member, destructor.
struct CIntelligentPointerThing : CBaseIntelligentPointerCtor {
    FABLE_FORCEINLINE CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointerCtor() { SetPItem(item); }
    FABLE_FORCEINLINE CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

struct CThingCreatureState {
    long GetLifeState();                        // 0x00662010
};

struct CAIStateGroup_MinionReactToUnconsciousPerson : CAIStateGroupBase {
    CIntelligentPointerThing PPerson;           // +0x14 CIntelligentPointer<CThingCreatureBase>
    CThing* FindUndiscoveredUnconsciousPerson();    // 0x00937B10
    CThing* FindUnconsciousPerson();            // 0x00937C20
    virtual bool IsRunnable();
};

bool CAIStateGroup_MinionReactToUnconsciousPerson::IsRunnable()
{
    CThing* person;
    if (((CThingCreatureState*)PBrain->PCreature)->GetLifeState() == 3)
        person = FindUnconsciousPerson();
    else
        person = FindUndiscoveredUnconsciousPerson();
    if (person != 0 && !person->IsDead()) {
        PPerson = person;
        return true;
    }
    return false;
}