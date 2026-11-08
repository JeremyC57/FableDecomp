// NTCScriptedControl::CActionPickUpGenericBox::Update() -- refresh the box handle; once the pick-up action is plugged in, finish when the creature is idle or runs another action; otherwise hand the creature a pick-up-generic-box action (finishing at once when the box is dead), and when the creature refuses it put the box straight into the carry slot named by its carryable def, add the carrying mode (34) and finish
// Leaf: ?Update@CActionPickUpGenericBox@NTCScriptedControl@@UAEXXZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007153B0, 424 bytes (+ int3 padding), plain `ret`. Slot 1 of the action vtable
//   (Ego_r.pdb ?Update@CActionPickUpGenericBox@NTCScriptedControl@@UAEXXZ).
// Callees: CScriptThing virtual slots 77 IsValid (+0x134), 71 Refresh (+0x11C), 75 IsAlive (+0x12C) and 11 GetPThing
//   (+0x2C) on PBox (as the landed CActionFireProjectileWeapon::Update 0x007152F0), CThingCreatureBase::
//   IsPerformingAnyAction @ 0x006622A0 / SetCurrentAction(const CCreatureActionBase&) @ 0x006644F0,
//   CCreatureActionBase::GetActionID @ 0x00692620 (manifest "CWorld::DrawGetEnvironment" propagated),
//   CCreatureAction_PickUpGenericBox(CThingCreatureBase&, CThing&) @ 0x00843700 (manifest
//   "CCreatureAction_AddRealObjectToInventory" propagated; Ego_r.pdb names this signature), ~CBaseIntelligentPointer
//   @ 0x00A01C10, ~CCreatureActionBase @ 0x00693EF0 (manifest "~CPhysicsMeshInfo" propagated; landed name in
//   0x00914F00), CVectorMap::LowerBound @ 0x0040F020 (the 0x46 CTCCarrying and 0x31 mode-manager lookups, both
//   unchecked), CTCCarrying::RemoveThingFromPrimarySlot(bool) @ 0x006A1E00 / PutThingInCarrySlot @ 0x006A1F80,
//   CDefClassBase::GetSubDef<CCarryableDef> @ 0x005CC10E, CTCCreatureModeManager::AddMode(unsigned long) @ 0x0073AE10.
// Layout: Ego_r.pdb CActionPickUpGenericBox = retail: PBox (CScriptThing) +0x0C, ActionID +0x18, ActionPluggedIn +0x1C;
//   CActionBase PTCScript +0x04, Terminated +0x09; retail CThingCreatureBase PCurrentAction +0x100, CThing def +0x70;
//   the pick-up action temporary is 0xB0 bytes (CCreatureActionBase 0xA8 + the box intelligent pointer).
// Shape: the action is a temporary bound to SetCurrentAction's const reference (its implicit destructor inlined, the
//   result held in bl); the refused path follows the landed CCreatureAction_PickUpGoods::PerformAction 0x00848730
//   model: unchecked GetInterface lookups (the miss path reads the shared frame word), the carryable def a CDefPointer
//   local released at the end (null test folded after the dereference), and the same-TU GetSubDef / LowerBound
//   stand-ins that carry retail's whole-program knowledge (def kept in ebp, key and def share one frame word); the box
//   is taken into a local before GetSubDef (else &def is pushed before the GetPThing call); the mode manager comes from
//   an inline accessor returning an unset local over the ternary GetTC (the out-pointer GetInterface form leaves the
//   pointer in eax and adds `mov ecx,eax`).
#include "fable_compiler.h"
class CTCBase;
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
static FABLE_FORCEINLINE bool DecRefToZero(long& count) { return --count == 0; }
class CCarryableDef { public:
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
    unsigned char _pad08[0x28 - 0x08];
    long CarrySlot;                                     // +0x28
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (DecRefToZero(Object->RefCount)) Object->Destroy(); } }
    T* operator->() const { return Object; }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                if (--Object->RefCount == 0)
                    Object->Destroy();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->RefCount;
        }
        return *this;
    }
};
class CDefinitionManager { public:
    CDefPointer<CCarryableDef> GetPDefFromGlobalIndex(long index);   // 0x009ADA10 (hidden result pointer)
};
class CDefClassBase { public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10();
    virtual void v14(); virtual void v18(); virtual void v1c(); virtual void v20(); virtual void v24();
    virtual void v28(); virtual void v2c(); virtual void v30(); virtual void v34();
    virtual long* GetSubDefIndex(const CCharString& name);   // slot 14 (+0x38)
    unsigned char _pad04[0x1c - 4];
    CDefinitionManager* m_pManager;                     // +0x1c
    bool GetSubDef(CDefPointer<CCarryableDef>* out);   // 0x005CC10E
};
#pragma auto_inline(off)
// Same-TU stand-in for GetSubDef<CCarryableDef> 0x005CC10E (the landed body, as in 0x00848730): VC7.1 then knows it
// does not keep the def pointer's address.
bool CDefClassBase::GetSubDef(CDefPointer<CCarryableDef>* out)
{
    long* pIndex;
    {
        CCharString name("CCarryableDef");
        pIndex = GetSubDefIndex(name);
    }
    if (pIndex) {
        CDefPointer<CCarryableDef> result = m_pManager->GetPDefFromGlobalIndex(*pIndex);
        *out = result;
        return true;
    }
    return false;
}
#pragma auto_inline(on)
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing {
public:
    unsigned char  _pad0[0x20];
    unsigned long  m_presentBits[9];                    // +0x20
    CInterfaceMap  m_interfaces;                        // +0x44
    unsigned char  _pad54[0x1c];
    CDefClassBase* m_pDef;                              // +0x70
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
    template<class T> FABLE_FORCEINLINE T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
        return (T*)((it == map->end() || wanted < it->first) ? map->end() : it)->second;
    }
};

class CTCCarrying {
public:
    CThing* RemoveThingFromPrimarySlot(bool immediately);                    // 0x006A1E00
    void    PutThingInCarrySlot(CThing* thing, long slot, bool immediately); // 0x006A1F80
};
class CTCCreatureModeManager {
public:
    bool AddMode(unsigned long mode);                   // 0x0073AE10
};

#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CCreatureActionBase {                             // retail sizeof 0xa8
public:
    virtual ~CCreatureActionBase();                     // 0x00693EF0
    long GetActionID() const;                           // 0x00692620
    unsigned char _pad04[0xa8 - 4];
};
class CThingCreatureBase;
class CCreatureAction_PickUpGenericBox : public CCreatureActionBase {   // retail sizeof 0xb0
public:
    CCreatureAction_PickUpGenericBox(CThingCreatureBase& creature, CThing& box);   // 0x00843700
    CIntelligentPointer<CThing> PBox;                   // +0xa8
};

class CThingCreatureBase : public CThing {
public:
    unsigned char        _pad74[0x100 - 0x74];
    CCreatureActionBase* PCurrentAction;                // +0x100 (retail)
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
public:
    bool IsPerformingAnyAction() const;   // 0x006622A0
};

#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

class CTCScriptedControl { public:
    void*               __vftable;
    CThingCreatureBase* ParentThing;                    // +0x04
};

namespace NTCScriptedControl {

enum { CREATURE_MODE_CARRYING_GENERIC_BOX = 0x22 };     // descriptive

class CActionBase {
public:
    virtual ~CActionBase();
    virtual void Update();                              // slot 1
    CTCScriptedControl* PTCScript;                      // +0x04
    bool                Activated;                      // +0x08
    bool                Terminated;                     // +0x09
};

class CActionPickUpGenericBox : public CActionBase {
public:
    virtual void Update();
    FABLE_FORCEINLINE CTCCreatureModeManager* GetModeManager() const
    {
        CTCCreatureModeManager* modes;
        CThing* thing = PTCScript->ParentThing;
        if (thing->m_presentBits[0x31 >> 5] & (1UL << (0x31 & 31)))
            modes = thing->GetTC<CTCCreatureModeManager>(0x31);
        return modes;
    }
    CScriptThing PBox;                                  // +0x0c
    long         ActionID;                              // +0x18
    bool         ActionPluggedIn;                       // +0x1c
};

void CActionPickUpGenericBox::Update()
{
    if (!PBox.IsNull())
        PBox.UpdateThingAttachment();
    if (ActionPluggedIn) {
        if (!PTCScript->ParentThing->IsPerformingAnyAction()
            || PTCScript->ParentThing->PCurrentAction->GetActionID() != ActionID)
            Terminated = true;
        return;
    }
    if (!PBox.IsAlive()) {
        Terminated = true;
        return;
    }
    CThingCreatureBase* creature = PTCScript->ParentThing;
    if (creature->SetCurrentAction(CCreatureAction_PickUpGenericBox(*creature, *PBox.GetPThing()))) {
        ActionPluggedIn = true;
        ActionID = PTCScript->ParentThing->PCurrentAction->GetActionID();
        return;
    }
    CTCCarrying* carrying;
    PTCScript->ParentThing->GetInterface(&carrying, 0x46);
    carrying->RemoveThingFromPrimarySlot(false);
    CDefPointer<CCarryableDef> carryDef;
    CThing* box = PBox.GetPThing();
    box->m_pDef->GetSubDef(&carryDef);
    carrying->PutThingInCarrySlot(PBox.GetPThing(), carryDef->CarrySlot, true);
    GetModeManager()->AddMode(CREATURE_MODE_CARRYING_GENERIC_BOX);
    Terminated = true;
}
}