// CScriptGameResourceObjectScriptedThing::FollowThing(const CScriptThing& target, float distance, bool run) -- queue a follow-creature action on a live target for the live scripted thing
// Leaf: ?FollowThing@CScriptGameResourceObjectScriptedThing@@UAEXABVCScriptThing@@M_N@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00904030, length 190 bytes (`ret 0Ch`), vtable 0x0128D86C slot 7.
// PDB: ?FollowThing@CScriptGameResourceObjectScriptedThing@@UAEXABVCScriptThing@@M_N@Z.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PThing at +0x14), CScriptThing::IsAlive (slot 75)
// and GetPThing (slot 11), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inlined
// GetInterface, interface 0x1f = CTCScriptedControl), operator new @ 0x00BFEA1A (0x28 bytes),
// NTCScriptedControl::CActionFollowCreature::CActionFollowCreature(CThing*, float, bool) @ 0x00905180 (vtable
// 0x0128D838), CTCScriptedControl::AddAction @ 0x007137D0.
// Shape: the family guard, then the unchecked GetInterface BEFORE the target thing is fetched (the
// control lands in ebx, the miss arm reads the dead `target` slot); the target thing is a named local
// read after the lookup and before operator new.
// Layout: engine/CScriptGameResourceObjectScriptedThing.h models the vptr as a data word, so it cannot declare this
// virtual member; the class is modelled locally with the header's offsets (ScriptInterface +0x10, PThing +0x14
// CIntelligentPointer<CThing>, ScriptID +0x1c, NullResource +0x20, ActionPriority +0x24; 0x28 bytes).
#include <stddef.h>

void* __cdecl operator new(size_t size);           // 0x00BFEA1A

class CTCBase;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
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
};

#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

namespace NTCScriptedControl {
struct CActionBase {                               // vtable 0x01265504
    virtual void v00();
};
}

class CTCScriptedControl { public:                        // interface 0x1f component
    void AddAction(NTCScriptedControl::CActionBase* action);   // 0x007137D0
};

namespace NTCScriptedControl {
struct CActionFollowCreature : CActionBase {                   // vtable 0x0128D838, 0x28 bytes
    unsigned char _body[0x24];
    CActionFollowCreature(CThing& creature, float distance, bool run);   // 0x00905180 (Ego_r ??0CActionFollowCreature@NTCScriptedControl@@QAE@AAVCThing@@M_N@Z)
};
}

class CGameScriptInterface;
class CScriptGameResourceObjectScriptedThing {     // vtable 0x0128D86C, 0x28 bytes
public:
    virtual ~CScriptGameResourceObjectScriptedThing();
    unsigned char               _pad_0x04[0xc];    // +0x04 CBaseObject / CScriptGameResourceObjectBase members
    CGameScriptInterface*       ScriptInterface;   // +0x10
    CIntelligentPointer<CThing> PThing;            // +0x14
    long                        ScriptID;          // +0x1c
    bool                        NullResource;      // +0x20
    long                        ActionPriority;    // +0x24
    virtual void FollowThing(const CScriptThing& target, float distance, bool run);   // slot 7
};

void CScriptGameResourceObjectScriptedThing::FollowThing(const CScriptThing& target, float distance, bool run)
{
    if (PThing.GetPItem()) {
        if (!PThing.GetPItem()->IsDead()) {
            if (target.IsAlive()) {
                CTCScriptedControl* control;
                PThing.GetPItem()->GetTC(TCI_SCRIPTED_CONTROL, &control);
                CThing* targetThing = target.GetPThing();
                control->AddAction(new NTCScriptedControl::CActionFollowCreature(*targetThing, distance, run));
            }
        }
    }
}