// CAIStateGroup_MinionWalkToMarker::IsRunnable() -- runnable when the creature's walk-to-marker component (0xAA) is in
// Leaf: ?IsRunnable@CAIStateGroup_MinionWalkToMarker@@UAE_NXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// mode 8 or 9 and a thing with the component's marker script name exists.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00921630, length 175 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 0xAA, bit 10 of the dword at +0x34 ->
// `test ch,4`; miss path reads the never-written slot), CCharString copy ctor @ 0x0099EC30 (twice),
// CAIStateGroupBase::PeekThingSearchTools @ 0x008FD000, CThingSearchTools::GetPFirstThing<CThingFilter_HasScriptName>
// @ 0x0056D050 (filter, out thing), ~CCharString @ 0x0099EAE0 (twice).
// Shape: a NAMED CCharString copy of the component's marker name, then the filter holding its own copy (inline copy
// ctor) in an inner block; the bool result is declared outside it (kept in bl across both destructors). No engine header for this class.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CThing;
class CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x004365B0.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CThingSearchTools;
class CAIStateGroupBase {
protected:
    const CThingSearchTools& PeekThingSearchTools() const;   // 0x008FD000 (Ego_r.pdb ?PeekThingSearchTools@CAIStateGroupBase@@IBEABVCThingSearchTools@@XZ, protected)
private: public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual bool IsStillValid();                // slot 5  (+0x14)
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void InitStateGroup();              // slot 13 (+0x34)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CBaseIntelligentPointerCtor {            // CBaseIntelligentPointer (8 bytes: vptr + item)
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointerCtor();              // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointerCtor();             // 0x00A01C10
};

// `PThing = thing` is the converting-temporary + inline `SetPItem(other.GetPItem())` assignment.
struct CIntelligentPointerThing : CBaseIntelligentPointerCtor {
    __forceinline CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointerCtor() { SetPItem(item); }
    __forceinline CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

struct CTCWalkToMarker {                        // the interface-0xAA component
    unsigned char _pad[0x38];
    long Mode;                                  // +0x38
    unsigned char _pad_0x3c[0x14];
    CCharString MarkerName;                     // +0x50
};

class CThingFilter_HasScriptName { public:
    CCharString ScriptName;
    CThingFilter_HasScriptName(const CCharString& name) : ScriptName(name) { }
};

class CThingSearchTools { public:
    bool GetPFirstThing(const CThingFilter_HasScriptName& filter, CThing** ppThing) const;   // 0x0056D050
};

struct CAIStateGroup_MinionWalkToMarker : CAIStateGroupBase {
    virtual bool IsRunnable();
};

bool CAIStateGroup_MinionWalkToMarker::IsRunnable()
{
    CTCWalkToMarker* walk;
    PBrain->PCreature->GetTC(TCI_AI_SCRATCHPAD, &walk);
    if (walk->Mode == 8 || walk->Mode == 9) {
        bool found;
        {
            CCharString name(walk->MarkerName);
            CThingFilter_HasScriptName filter(name);
            CThing* marker;
            found = PeekThingSearchTools().GetPFirstThing(filter, &marker);
        }
        if (found)
            return true;
    }
    return false;
}