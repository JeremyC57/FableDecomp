// CThing::AddTC(const CCharString& type, bool override, const CTCParamsBase* pparams_base) -- create the named thing component and attach it: refused unless TCs may be added; an interface already present is kept (the new TC deleted) unless overriding; the physics / appearance / driver shortcuts are updated; an overridden TC is dropped from the update list and the pending-removal list and deleted, otherwise the TC is added to the sorted interface map; then the presence bit is set, the params applied, the TC initialised and queued for updates when it wants them
// Leaf: ?AddTC@CThing@@QAEPAVCTCBase@@ABVCCharString@@_NPBVCTCParamsBase@@@Z
// Flags: /d2inlS60
// Inline-budget: /d2inlS60 (EXTRA_RULES 2026-11-08 preference 2, source untouched) -- retail expands CThing::HasTC (the bitset<274>::test range check, `cmp eax,112h ; jb`, puts / abort, then the word test, 0x004C9D9D..0x004C9DCA) at the head of this function; at the default budget VC7.1 calls ?HasTC@CThing@@QBE_NW4ETCInterfaceType@@@Z out of line (461 v 504 bytes). Sweep: default / /d2inlS20 / 30 / 45 DIFFER 461v504, /d2inlS60 RELOCATION_MATCH.
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C9D60, 504 bytes, thiscall `ret 0Ch`.
// Name: Ego_r.pdb 0x004CC0F0 + FableWin.pdb 0x01CF4F30 ?AddTC@CThing@@QAEPAVCTCBase@@ABVCCharString@@_NPBVCTCParamsBase@@@Z. Ego_d.pdb
//   (python3 -I egod_locals.py) names the parameters type / override / pparams_base and the locals pbase, inserted_in_place, then
//   component_it and remove_tc_it in nested scopes.
// Callees: CTCBase::AllocateTC 0x00686F10 (__fastcall; q11188: the landed spelling ?CreateTC@@YI.. is wrong), puts / abort (the inline
//   bitset<274>::test range check), CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020, list<CTCBase*>::remove 0x004CC150,
//   ~CBaseIntelligentPointer 0x00A01C10 + free (the inline list erase of CThing::TCsToBeRemoved 0x013B8A24),
//   vector<pair<ETCInterfaceType,CTCBase*> >::_M_insert_overflow 0x004CCCE0, std::sort 0x004CD5D0 (__fastcall), bitset<274>::set
//   0x004CC090, CThing::InitTC 0x004C73E0 (private), list<CTCBase*>::push_back 0x004CC820; virtual slots 0 (scalar deleting dtor),
//   3 ConstructFromParams, 8 HasFrameUpdate, 21 GetInterfaceType, 24 IsDriver.
// Casts: FableWin.pdb ??$GFDebugPDynamicCast@PAVCTCBase@@PAVCTCPhysicsBase@@@@YI.. (0x01CFF390) and ..PAVCTCDriverBase.. (0x01CFF550)
//   sit beside CThing's own debug COMDATs (AddTC 0x01CF4F30); the appearance one is ..PAVCTCGraphicAppearance@NGraphicAppearance.. .
// Shape: Components.Insert(key, value) then Components.Sort() in place (`mov byte ptr [Dirty],1`, std::sort with no Dirty test, Dirty
//   cleared): FableWin.pdb has ?Insert@?$CVectorMap@W4ETCInterfaceType@@..@@QAEXABW4ETCInterfaceType@@ABQAVCTCBase@@@Z (0x01CFA690) and
//   ?Sort@..@@QAEXXZ (0x01CFA5F0) beside AddTC and no Update for that map. The inline push_back stores the pair through the
//   placement-new null test.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_REGISTRY
#define FABLE_RETAIL_CTCBASE_REGISTRY_LOOKUP
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_INITTC
#define FABLE_RETAIL_CTHING_TCSTOBEREMOVED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE
#define FABLE_RETAIL_CVECTORMAP_INSERT_VALUE_TYPE
#define FABLE_RETAIL_CVECTORMAP_SORT_INLINE
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_REMOVE_DECL
#define FABLE_RETAIL_STLP_LIST_PUSHBACK_NOINLINE
#define FABLE_RETAIL_STLP_BITSET_SET_DECL
#define FABLE_RETAIL_CTCPHYSICSBASE
#include "fable_string.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCDriverBase.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/GFDebugPDynamicCast.h"

CTCBase* CThing::AddTC(const CCharString& type, bool override, const CTCParamsBase* pparams_base)
{
    if (!AddingTCsPermitted)
        return 0;
    CTCBase* pbase = CTCBase::AllocateTC(type, *this);
    if (!pbase)
        return 0;
    if (HasTC(pbase->GetInterfaceType()) && !override) {
        delete pbase;
        return 0;
    }
    if (pbase->GetInterfaceType() == TCI_PHYSICS)
        PhysicsTC = GFDebugPDynamicCast(pbase, (CTCPhysicsBase*)0);
    else if (pbase->GetInterfaceType() == TCI_GRAPHIC_APPEARANCE_NEW)
        PTCGraphicAppearance = GFDebugPDynamicCast(pbase, (NGraphicAppearance::CTCGraphicAppearance*)0);
    if (pbase->IsDriver())
        PTCDriver = GFDebugPDynamicCast(pbase, (CTCDriverBase*)0);
    bool inserted_in_place = false;
    if (override) {
        CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >::iterator component_it =
            Components.Find(pbase->GetInterfaceType());
        if (component_it != Components.end()) {
            UpdateComponentsList.remove(component_it->second);
            std::list<std::pair<CIntelligentPointer<CThing>, CTCBase*> >::iterator remove_tc_it = TCsToBeRemoved.begin();
            while (remove_tc_it != TCsToBeRemoved.end()) {
                if (remove_tc_it->second == component_it->second)
                    remove_tc_it = TCsToBeRemoved.erase(remove_tc_it);
                else
                    ++remove_tc_it;
            }
            delete component_it->second;
            component_it->second = pbase;
            inserted_in_place = true;
        }
    }
    if (!inserted_in_place) {
        Components.Insert(pbase->GetInterfaceType(), pbase);
        Components.Sort();
    }
    UsingComponents.set(pbase->GetInterfaceType());
    if (pparams_base)
        pbase->ConstructFromParams(*pparams_base);
    InitTC(pbase);
    if (pbase->HasFrameUpdate())
        UpdateComponentsList.push_back(pbase);
    return pbase;
}
