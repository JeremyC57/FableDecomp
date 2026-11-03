// CTCVillage::AddThingToVillage(CThing&) -- register a thing with its village: attach its CTCVillageMember to this village, then
// sort it by type: villagers (counting guards, LaggyLimbo villagers put in persistent limbo), buildings (physics mesh info, LaggyLimbo
// door locking, the house lights of the building's owned objects switched on by tavern/day or the def's HouseLightsOn/OffTime),
// objects (torches lit by night, crops with a pick-crops task when the def lists task type 2, horns, gates, cages, campfires),
// patrol track starts on the def's PatrolTrackName (a patrol task when the def lists task type 1), markers, and dummy villagers.
// Leaf: ?AddThingToVillage@CTCVillage@@QAEXAAVCThing@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065B854, 1502 bytes, `ret 4`. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees (retail): std::bitset<274>::test 0x00410DE0 (inline CThing::HasTC), CVectorMap<..>::LowerBound 0x0040F020 (inline
//   CVectorMap::Find), CTCVillageMember::GetPVillage 0x007EE540 / SetPVillage 0x007EE270, CThing::SetInLimbo 0x004C8CF0, the STLport
//   list<..>::push_back COMDATs 0x004BA4E0 / 0x006604D1 / 0x006604EB / 0x00660505 / 0x00660553 / 0x0066056D / 0x006605BB / 0x00660FC9,
//   CThingCreatureBase::GetProperty 0x00662010, CThing::HasScriptThing 0x004C7E70, CThingBuilding::LockAllDoors 0x0082DB00,
//   IS_THING_ALIVE<CIntelligentPointer<CThing> > 0x004CC340, CGameTimeManager::IsDay 0x006BB2C0 / IsTimeBetween 0x006BB390,
//   CTCLight::SetActive 0x006412B0, std::find<const EVillageTaskType*,EVillageTaskType> 0x0065E5A4, CDefString::GetString 0x00415DD0,
//   CBasicString<char>::operator== 0x004A93C0, CVillageTaskPickCrops ctor 0x0065EEFF, CVillageTaskPatrol ctor 0x0065EE66,
//   CCountedPointer ctors 0x0065E198 / 0x0065E1DB, CIntelligentPointer<T>(T*) ctors 0x004A96F0 / 0x0050F380 / 0x0050F3F0 /
//   0x0065E133 / 0x0065E15D / 0x0065E21E, CBaseIntelligentPointer::GetPItem 0x00A01B50 / dtor 0x00A01C10, CCharString dtor 0x0099EAE0,
//   CCountedPointer dtors 0x0065D507 / 0x0065D533, CTCBase::PeekWorld 0x00686A80, operator new 0x00BFEA1A.
// Levers: implicit-conversion CIntelligentPointer temporaries in each push_back (`VillageDummies.push_back(&thing)`: each gets its own
//   slot and is re-read with lea, a named local makes sibling arms share slots); CTCVillage::GetPDef() __forceinline (keeps the two
//   CDefPointer copies in registers, frame 0x7C); `(bool)((props >> n) & 1)` for the property bits (retail `shr eax,n; test al,1`);
//   the object case as an if/break chain (retail's tail-merge placement); the patrol track name compared through CDefString's
//   implicit operator CCharString() over the out-of-line GetString (retail re-reads the [ebp-4] temporary).
// Header debt: CThingObject is a TU-local model whose virtuals 4..47 are positional placeholders (VtableSlotNN) so that
//   GetProperties() lands at vtable slot 48; CVillageDef / CThingTrackNode / CGameTimeManager / CTCLight have no engine header yet.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CBASICSTRING_EQ_OUTOFLINE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_GETSTRING
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_VILLAGE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_PROPERTY
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGBUILDING
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCTHINGOWNER
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_MEMBERS
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_GAMETIME
#define FABLE_RETAIL_CVILLAGETASKPATROL_CTOR
#define FABLE_RETAIL_CVILLAGETASKPICKCROPS_CTOR
#define FABLE_RETAIL_CVILLAGETASKSECURITYSWEEP_CTOR
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CTCBase.h"
#include "engine/CThing.h"
#include "engine/CDefClassBase.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CDefString.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBuilding.h"
#include "engine/CThingMarker.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCThingOwner.h"
#include "engine/CTCVillageMember.h"
#include "engine/CTCVillage.h"
#include "engine/CWorld.h"
// ---- TU-local models (no engine header / no retail branch for these classes) ----
class CGameTimeManager {                                 // Ego_r.pdb CGameTimeManager (methods only)
public:
    bool IsDay() const;                                  // 0x006BB2C0 ?IsDay@CGameTimeManager@@QBE_NXZ
    bool IsTimeBetween(long start, long end) const;      // 0x006BB390 ?IsTimeBetween@CGameTimeManager@@QBE_NJJ@Z (Ego_r / FableWin; reads Time +0x8)
};
class CTCLight : public CTCBase {                        // RTTI .?AVCTCLight@@ : CTCBase (interface TCI_LIGHT)
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_LIGHT; }   // FableWin ?StatGetInterfaceType@CTCLight@@SI..
    void SetActive(bool active);                         // 0x006412B0 ?SetActive@CTCLight@@QAEX_N@Z
};
enum EVillageTaskType { };                               // Ego_r.pdb CVillageDef::Tasks element type (enumerator names unproven)
class CVillageDef : public CSubDefClassBase {            // Ego_r.pdb CVillageDef (retail STLport offsets; HouseLights +0xCC/+0xD0)
public:
    std::vector<EVillageTaskType> Tasks;                 // +0x28
    CDefString     PatrolTrackName;                      // +0x34
    long           DefaultStanceToHero;                  // +0x38 EVillageStanceToHero
    long           SecondsSecuritySweepLasts;            // +0x3c
    long           WarehouseCrateDef;                    // +0x40
    std::vector<long> StoryStartTimes;                   // +0x44
    long           StoryDuration;                        // +0x50
    std::vector<long> SchoolStartTimes;                  // +0x54
    long           SchoolDuration;                       // +0x60
    std::vector<long> WorkStartTimes;                    // +0x64
    long           WorkDuration;                         // +0x70
    std::vector<long> CookStartTimes;                    // +0x74
    long           CookDuration;                         // +0x80
    std::vector<long> EatStartTimes;                     // +0x84
    long           EatDuration;                          // +0x90
    std::vector<long> TavernStartTimes;                  // +0x94
    long           TavernDuration;                       // +0xa0
    std::vector<long> GamingStartTimes;                  // +0xa4
    long           GamingDuration;                       // +0xb0
    long           SleepAdultStartTime;                  // +0xb4
    long           SleepAdultEndTime;                    // +0xb8
    long           SleepChildStartTime;                  // +0xbc
    long           SleepChildEndTime;                    // +0xc0
    long           SleepElderlyStartTime;                // +0xc4
    long           SleepElderlyEndTime;                  // +0xc8
    long           HouseLightsOnTime;                    // +0xcc
    long           HouseLightsOffTime;                   // +0xd0
};
// CThingObject: GetProperties is CThing-hierarchy virtual slot 48 (retail `call [eax+0C0h]`; FableWin ?GetProperties@CThingObject@@UBEKXZ).
// Header debt: no engine header models the CThing vtable (slots 3..47 of the retail chain), so the slots in between are
// positional placeholders here (CThingGameObject's retail branch already puts OnHit at slot 3).
class CThingObject : public CThingGameObject {
public:
    virtual void VtableSlot04();
    virtual void VtableSlot05();
    virtual void VtableSlot06();
    virtual void VtableSlot07();
    virtual void VtableSlot08();
    virtual void VtableSlot09();
    virtual void VtableSlot10();
    virtual void VtableSlot11();
    virtual void VtableSlot12();
    virtual void VtableSlot13();
    virtual void VtableSlot14();
    virtual void VtableSlot15();
    virtual void VtableSlot16();
    virtual void VtableSlot17();
    virtual void VtableSlot18();
    virtual void VtableSlot19();
    virtual void VtableSlot20();
    virtual void VtableSlot21();
    virtual void VtableSlot22();
    virtual void VtableSlot23();
    virtual void VtableSlot24();
    virtual void VtableSlot25();
    virtual void VtableSlot26();
    virtual void VtableSlot27();
    virtual void VtableSlot28();
    virtual void VtableSlot29();
    virtual void VtableSlot30();
    virtual void VtableSlot31();
    virtual void VtableSlot32();
    virtual void VtableSlot33();
    virtual void VtableSlot34();
    virtual void VtableSlot35();
    virtual void VtableSlot36();
    virtual void VtableSlot37();
    virtual void VtableSlot38();
    virtual void VtableSlot39();
    virtual void VtableSlot40();
    virtual void VtableSlot41();
    virtual void VtableSlot42();
    virtual void VtableSlot43();
    virtual void VtableSlot44();
    virtual void VtableSlot45();
    virtual void VtableSlot46();
    virtual void VtableSlot47();
    virtual unsigned long GetProperties() const;          // slot 48
};
class CThingTrackNode : public CThingGameObject {        // Ego_r.pdb CThingTrackNode (retail CThingGameObject 0xD0 -> members from +0xD0)
public:
    unsigned char    NodeLinks[0x8];                     // +0xd0 Ego_r unions CThingTrackNode_u_232 / _u_236 (unmodelled)
    unsigned __int64 NextNodeUID;                        // +0xd8
    unsigned __int64 PreviousNodeUID;                    // +0xe0
    CCharString      TrackName;                          // +0xe8
    bool             IsStart;                            // +0xec
    bool             IsEnd;                              // +0xed
};
#include "engine/CVillageTaskPatrol.h"
#include "engine/CVillageTaskPickCrops.h"
// The TU's own out-of-line template bodies (retail COMDATs): IS_THING_ALIVE<CIntelligentPointer<CThing> > 0x004CC340,
// std::find<const EVillageTaskType*, EVillageTaskType> 0x0065E5A4 -> std::__find 0x0065D9EE (STLport 4.6 _algobase.c).
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool __fastcall IS_THING_ALIVE(const T& thing);
template <> __declspec(noinline) bool __fastcall IS_THING_ALIVE<CIntelligentPointer<CThing> >(const CIntelligentPointer<CThing>& thing)
{
    return thing.Get() && !thing->IsDead();
}
namespace std {
template <class _RandomAccessIter, class _Tp>
__declspec(noinline) _RandomAccessIter __fastcall __find(_RandomAccessIter __first, _RandomAccessIter __last, const _Tp& __val,
                                                         const random_access_iterator_tag&)
{
    int __trip_count = (__last - __first) >> 2;
    for (; __trip_count > 0; --__trip_count) {
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
    }
    switch (__last - __first) {
    case 3:
        if (*__first == __val) return __first;
        ++__first;
    case 2:
        if (*__first == __val) return __first;
        ++__first;
    case 1:
        if (*__first == __val) return __first;
    case 0:
    default:
        return __last;
    }
}
template <class _InputIter, class _Tp>
__declspec(noinline) _InputIter __fastcall find(_InputIter __first, _InputIter __last, const _Tp& __val)
{
    return __find(__first, __last, __val, random_access_iterator_tag());
}
}
#pragma auto_inline(on)
void CTCVillage::AddThingToVillage(CThing& thing)
{
    CTCVillageMember* villageMember;
    thing.GetTC(&villageMember);
    if (villageMember->GetPVillage() == this)
        return;
    villageMember->SetPVillage(this);
    switch (thing.GetType()) {
    case 1: {                                            // AI creature
        CThingAICreature& creature = static_cast<CThingAICreature&>(thing);
        VillagePeople.push_back(&creature);
        if (creature.GetProperty() == 3)
            ++NumGuards;
        if (LaggyLimbo) {
            if (!thing.HasScriptThing() || !thing.HasTC(TCI_SCRIPTED_CONTROL)) {
                thing.SetInLimbo(true);
                thing.SetPersistOnUnloadWhenInLimbo(true);
            }
        }
        break;
    }
    case 3: {                                            // building
        CThingBuilding& building = static_cast<CThingBuilding&>(thing);
        VillageBuildings.push_back(&building);
        building.GetPhysicsTC()->GetPhysicsMeshInfo();
        if (LaggyLimbo) {
            building.SetOverrideAutomaticLocking(true);
            building.LockAllDoors(true, false);
        }
        CTCThingOwner* owner;
        if (thing.GetTC(&owner)) {
            for (std::list<CIntelligentPointer<CThing> >::iterator it = owner->GetObjectList().begin();
                 it != owner->GetObjectList().end(); ++it) {
                if (IS_THING_ALIVE(*it) && (*it)->GetType() == 5) {
                    CThingObject* object = static_cast<CThingObject*>(it->Get());
                    CTCLight* light;
                    if (object->GetTC(&light)) {
                        if ((bool)((object->GetProperties() >> 8) & 1) || (bool)((object->GetProperties() >> 9) & 1)) {
                            bool active;
                            if (thing.HasTC(TCI_TAVERN))
                                active = !PeekWorld().PeekGameTimeManager().IsDay();
                            else
                                active = PeekWorld().PeekGameTimeManager().IsTimeBetween(GetPDef()->HouseLightsOnTime,
                                                                                             GetPDef()->HouseLightsOffTime);
                            light->SetActive(active);
                        }
                    }
                }
            }
        }
        break;
    }
    case 5: {                                            // object
        CThingObject& object = static_cast<CThingObject&>(thing);
        if ((bool)((object.GetProperties() >> 9) & 1) || (bool)((object.GetProperties() >> 8) & 1)) {
            VillageTorches.push_back(&thing);
            if (!thing.IsDead()) {
                CTCLight* light;
                if (thing.GetTC(&light)) {
                    if (PeekWorld().PeekGameTimeManager().IsDay())
                        light->SetActive(false);
                    else
                        light->SetActive(true);
                }
            }
            break;
        }
        if ((bool)((object.GetProperties() >> 1) & 1)) {
            VillageCrops.push_back(&object);
            if (std::find(PDef->Tasks.begin(), PDef->Tasks.end(), (EVillageTaskType)2) != PDef->Tasks.end())
                CropTasks.push_back(CCountedPointer<CVillageTaskPickCrops>(new CVillageTaskPickCrops(object)));
            break;
        }
        if ((bool)((object.GetProperties() >> 24) & 1)) {
            VillageHorns.push_back(&thing);
            break;
        }
        if ((bool)((object.GetProperties() >> 28) & 1)) {
            VillageGates.push_back(&thing);
            break;
        }
        if ((bool)((object.GetProperties() >> 3) & 1)) {
            VillageCages.push_back(&thing);
            break;
        }
        if ((bool)((object.GetProperties() >> 14) & 1)) {
            VillageCampfires.push_back(&thing);
            break;
        }
        if ((bool)((object.GetProperties() >> 2) & 1)) {
        }
        break;
    }
    case 10: {                                           // track node
        CThingTrackNode& node = static_cast<CThingTrackNode&>(thing);
        if (node.IsStart && node.TrackName == PDef->PatrolTrackName) {
            if (std::find(PDef->Tasks.begin(), PDef->Tasks.end(), (EVillageTaskType)1) != PDef->Tasks.end()) {
                VillagePatrols.push_back(&node);
                PatrolTasks.push_back(CCountedPointer<CVillageTaskPatrol>(new CVillageTaskPatrol(node)));
            }
        }
        break;
    }
    case 11:                                             // marker
        VillageMarkers.push_back(&static_cast<CThingMarker&>(thing));
        break;
    case 13:
        break;
    default:
        if (thing.HasTC(TCI_DUMMY_VILLAGER))
            VillageDummies.push_back(&thing);
        break;
    }
}
