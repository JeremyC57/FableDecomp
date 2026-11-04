// CTCPhysicsControlled::UpdateAdditionalImpulse() -- unless the thing manager pauses entities: gather the creatures (other than the owner) in the body's possible collision area and apply the in-combat-and-too-close impulse and the player-inside-unpushable-creature impulse to each
// Leaf: ?UpdateAdditionalImpulse@CTCPhysicsControlled@@AAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00725B70, 201 bytes (plain `ret`; PDB ?UpdateAdditionalImpulse@CTCPhysicsControlled@@AAEXXZ).
// Callees: CTCBase::GetThingManager() @ 0x00686AE0 (NonScriptedEntitesPauseMode +0x54, AllEntitesPauseMode +0x55; PDB +0x68/+0x69),
//   virtual GetPossibleCollidedWithArea() const slot 98 (+0x188, C2DBoxI by hidden pointer), CTCBase::PeekThingSearchTools() const
//   @ 0x006AFD40, CThingSearchTools::GetAllThingsInRoughArea<CThingCreatureBase, ...> @ 0x00725F10, std::_For_each<CThingCreatureBase**,
//   CApplyAdditionalImpulseIfInCombatAndTooClose> @ 0x00726610 and <..., CApplyAdditionalPlayerImpulseToPlayerIfInsideUnpushableCreature>
//   @ 0x00726660 (both `__fastcall`, PDB YI; the manifest labels of 0x00725F10/0x00726660 are other instantiations), free @ 0x00BFEA14.
// Shape: the area box is a function-scope named local; the same-TU bodies of the GetAllThingsInRoughArea instantiation (a
//   stand-in: the real one is 0x00725F10 of this TU, not graded) and of the two std::_For_each instantiations let VC7.1 see that
//   the vector's address never escapes, so begin/end stay in edi/ebp across both calls, the first functor result temporary
//   reuses the vector's words and the storage is freed through edi.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_SEARCHTOOLS
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_CTHINGMANAGER_ALLPAUSE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C2DBoxI.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CThingManager.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingSearchTools.h"
class CThingFilter_IsNot { public:
    const CThing* Thing;
    CThingFilter_IsNot(const CThing* thing) : Thing(thing) { }
    bool operator()(const CThing& thing) const;         // Ego_r.pdb ??RCThingFilter_IsNot@@QBE_NABVCThing@@@Z
};
// Stand-in body for the same-TU GetAllThingsInRoughArea<CThingCreatureBase, vector, CThingFilter_IsNot> instantiation (retail
// 0x00725F10, the map-cell walk; not graded): it touches the result vector only through its inline members, so VC7.1 sees that
// the caller's vector address does not escape the call (the chunk 756 GetAllLeafThings idiom).
template <class T, class Container, class Filter>
__declspec(noinline) void CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& area, const Filter& filter, Container& result) const
{
    const std::list<CClass*>& things = PeekTypeList(area.TLX);   // stand-in: the real body walks the map cells of the area
    for (std::list<CClass*>::const_iterator it = things.begin(); it != things.end(); ++it) {
        T* thing = static_cast<T*>(*it);
        if (thing->IsIncludedInThingScans() && filter(*thing))
            result.push_back(thing);
    }
}
class CTCPhysicsControlled;
class CApplyAdditionalImpulseIfInCombatAndTooClose {   // 0x14 bytes, passed by value
public:
    C3DVector Position;
    float     Radius;
    CTCPhysicsControlled* Physics;
    CApplyAdditionalImpulseIfInCombatAndTooClose(const C3DVector& position, float radius, CTCPhysicsControlled* physics)
        : Position(position), Radius(radius), Physics(physics) { }
    CApplyAdditionalImpulseIfInCombatAndTooClose(const CApplyAdditionalImpulseIfInCombatAndTooClose& other)
        : Position(other.Position), Radius(other.Radius), Physics(other.Physics) { }
    void operator()(CThing* thing) const;               // Ego_r.pdb ??RCApplyAdditionalImpulseIfInCombatAndTooClose@@QBEXPAVCThing@@@Z
};
class CApplyAdditionalPlayerImpulseToPlayerIfInsideUnpushableCreature {
public:
    CTCPhysicsControlled* Physics;
    CApplyAdditionalPlayerImpulseToPlayerIfInsideUnpushableCreature(CTCPhysicsControlled* physics) : Physics(physics) { }
    void operator()(CThing* thing) const;
};
namespace std {
// STLport's for_each worker, instantiated in this TU (0x00726610 / 0x00726660; Ego_r.pdb
// ??$_For_each@PAPAVCThingCreatureBase@@V...@@@std@@YI?AV...@PAPAVCThingCreatureBase@@0V1@@Z, __fastcall).
template <class _InputIter, class _Function>
__declspec(noinline) _Function __fastcall _For_each(_InputIter __first, _InputIter __last, _Function __f)
{
    for ( ; __first != __last; ++__first)
        __f(*__first);
    return __f;
}
}
class CTCPhysicsControlled : public CTCPhysicsBase {
private:
    virtual void UpdateOnSurface(std::vector<CThing*>& things, C3DVector& pos);   // slot 97 0x00725290 (Ego_r.pdb, EAE)
public:
    virtual C2DBoxI GetPossibleCollidedWithArea() const;   // slot 98 (+0x188) Ego_r.pdb ?GetPossibleCollidedWithArea@CTCPhysicsControlled@@UBE?AVC2DBoxI@@XZ
private:
    void UpdateAdditionalImpulse();
};

void CTCPhysicsControlled::UpdateAdditionalImpulse()
{
    if (GetThingManager().IsNonScriptedEntitesPauseMode())
        return;
    if (GetThingManager().IsAllEntitesPauseMode())
        return;
    std::vector<CThingCreatureBase*> creatures;
    C2DBoxI area = GetPossibleCollidedWithArea();
    GetThingSearchTools().GetAllThingsInRoughArea<CThingCreatureBase>(area, CThingFilter_IsNot(ParentThing), creatures);
    std::_For_each(creatures.begin(), creatures.end(), CApplyAdditionalImpulseIfInCombatAndTooClose(Position, Radius, this));
    std::_For_each(creatures.begin(), creatures.end(), CApplyAdditionalPlayerImpulseToPlayerIfInsideUnpushableCreature(this));
}