// CAIStateGroup_MinionGuardReactToBribe::IsStillValid() -- the hero is still alive and, once the bribe was
// Leaf: ?IsStillValid@CAIStateGroup_MinionGuardReactToBribe@@UAE_NXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Grade: RELOCATION_MATCH (retry 10, RTM 3077) -- the interface lookup written as the ternary entry pick (`((it == last || type < it->type) ? last : it)->component`, the landed 0x00584157 idiom): the component loads straight into ecx
// received, the creature's component 0xB4 still has a non-negative value at +0x10.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00933470, length 123 bytes (+ int3 padding). Vtable slot 5.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (twice: the inlined IS_THING_ALIVE on PHero at
// +0x14, dead bit +0x91), CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 0xB4 = bit 20 of
// the bitset dword at +0x34).
// Shape: the component pointer is a local INITIALISED to null (`xor ecx,ecx` before the bitset test, kept in
// ecx through both paths) and only read when HasRecievedBribe (+0x20, a bool read into al) is set; explicit
// `return true` / `return false` (`mov al,1` / `xor al,al`). PHero / HasRecievedBribe are the engine header
// names (the header flattens the base, so the class is modelled locally).
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CThing;
class CWorld;
class CThingAICreature;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020 -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
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

struct CTCBribeState {                          // interface type 0xB4
    unsigned char _pad[0x10];
    long m_value;                               // +0x10
    long GetValue() const { return m_value; }   // inline accessor: `this` biases the pointer into ecx
};

struct CAIStateGroup_MinionGuardReactToBribe : CAIStateGroupBase {
    CIntelligentPointer<CThing> PHero;              // +0x14 CIntelligentPointer<CThingPlayerCreature>
    long FrameLastReactedToReceiveing;          // +0x1c
    bool HasRecievedBribe;                      // +0x20
    virtual bool IsStillValid();
};

bool CAIStateGroup_MinionGuardReactToBribe::IsStillValid()
{
    if (IS_THING_ALIVE(PHero)) {
        CThing* creature = PBrain->PCreature;
        CTCBribeState* pState = 0;             // declared AFTER the creature load: the creature takes eax, the null ecx
        if (creature->HasTC(TCI_GUARD))
            pState = ((CTCBribeState*)&creature->GetTC(TCI_GUARD));
        if (!HasRecievedBribe)
            return true;
        if (pState->GetValue() >= 0)
            return true;
    }
    return false;
}