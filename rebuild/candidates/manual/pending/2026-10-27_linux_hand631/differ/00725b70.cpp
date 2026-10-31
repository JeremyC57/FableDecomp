// CTCPhysicsControlled::UpdateAdditionalImpulse() -- unless the thing manager pauses entities: gather the creatures (other than the owner) in the body's possible collision area and apply the player-too-close impulse and the player-inside-unpushable-creature impulse to each
// Grade: DIFFER(201v201) -- frame slots only: retail 0x28 frame with the area box at +0x24 and the first for_each result temporary at +0x10 over the dead vector (its start/finish already in edi/ebp); ours 0x24 frame, box at +0x1c and the result temporary on the box
// Leaf: ?UpdateAdditionalImpulse@CTCPhysicsControlled@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00725B70, 201 bytes (plain `ret`; PDB ?UpdateAdditionalImpulse@CTCPhysicsControlled@@AAEXXZ).
// Callees: CTCBase::GetThingManager() 0x00686AE0 (PDB IAE, CThingManager&; NonScriptedEntitesPauseMode +0x54, AllEntitesPauseMode
//   +0x55 in retail), virtual GetPossibleCollidedWithArea() const slot 98 (+0x188, C2DBoxI by hidden pointer), CTCBase::
//   PeekThingSearchTools() const 0x006AFD40 (PDB IBE, const CThingSearchTools&), CThingSearchTools::GetAllThingsInRoughArea
//   <CThingCreatureBase, vector, CThingFilter_IsNot> 0x00725F10, std::for_each<CThingCreatureBase**, CApplyAdditionalPlayerImpulseIfTooClose>
//   0x00726610 and std::for_each<CThingCreatureBase**, CApplyAdditionalPlayerImpulseToPlayerIfInsideUnpushableCreature> 0x00726660
//   (both __fastcall, hidden result in ecx, first in edx), free 0x00BFEA14.
// Names (chunk 631): the functors are proven from Ego_r.pdb layouts: the 0x14-byte {Position, Radius, Owner} functor 0x00726610
//   copies is CApplyAdditionalPlayerImpulseIfTooClose (ctor (const C3DVector&, float, const CTCPhysicsBase&)), the 4-byte {Player}
//   one is CApplyAdditionalPlayerImpulseToPlayerIfInsideUnpushableCreature (ctor (CTCPhysicsBase&)); CApplyAdditionalImpulseIfInCombat
//   AndTooClose is the 0x18-byte Navigator functor (0x00727500). The earlier attempt had the first two swapped.
// Shape: a same-TU noinline stand-in for GetAllThingsInRoughArea (as the landed sibling CTCPhysicsNavigator::UpdateAdditionalImpulse
//   0x00726BE0) keeps the vector from escaping, so its start stays in edi for the freeing destructor (fixed the 205v201 residue);
//   the area box in its own block. Tried after that: box at function scope (0x34 frame: the result temp gets a fresh slot), box
//   declared before the vector, the area as a temporary in the call (197), begin()/end() inline in both calls (same frame).
//   Next: whatever makes VC7.1 treat the vector's slot as dead after begin()/end() (retail overlaps it with the result temp).
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
class C3DVector {
public:
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
class C2DBoxI {
public:
    long TLX, TLY, BRX, BRY;
    C2DBoxI() { }
};
class CThing;
class CThingCreatureBase;
namespace std {
template <class T> class allocator { public: allocator() { } };
template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector() { if (_M_start) free(_M_start); }
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
};
template <class InputIter, class Function>
Function __fastcall for_each(InputIter first, InputIter last, Function f);
}
class CThingFilter_IsNot {
public:
    CThing* Thing;
    CThingFilter_IsNot(CThing* thing) : Thing(thing) { }
};
bool SearchRoughArea_00725f10(const void* tools, const C2DBoxI* area, const CThingFilter_IsNot* filter,
                              CThingCreatureBase** first, CThingCreatureBase** last, CThingCreatureBase** end);
CThingCreatureBase** SearchResultFirst_00725f10();
CThingCreatureBase** SearchResultLast_00725f10();
CThingCreatureBase** SearchResultEnd_00725f10();
class CThingSearchTools {
public:
    template <class T, class TContainer, class TFilter>
    void GetAllThingsInRoughArea(const C2DBoxI& box, const TFilter& filter, TContainer& out) const;   // 0x00725F10
};
template <class T, class TContainer, class TFilter>
__declspec(noinline) void CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& box, const TFilter& filter, TContainer& out) const
{
    SearchRoughArea_00725f10(this, &box, &filter, out._M_start, out._M_finish, out._M_end_of_storage);
    out._M_start = SearchResultFirst_00725f10();
    out._M_finish = SearchResultLast_00725f10();
    out._M_end_of_storage = SearchResultEnd_00725f10();
}
class CThingManager {
public:
    unsigned char _pad00[0x54];
    bool NonScriptedEntitesPauseMode;                   // +0x54
    bool AllEntitesPauseMode;                           // +0x55
};
class CTCBase {
public:
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void v048();
    virtual void v049();
    virtual void v050();
    virtual void v051();
    virtual void v052();
    virtual void v053();
    virtual void v054();
    virtual void v055();
    virtual void v056();
    virtual void v057();
    virtual void v058();
    virtual void v059();
    virtual void v060();
    virtual void v061();
    virtual void v062();
    virtual void v063();
    virtual void v064();
    virtual void v065();
    virtual void v066();
    virtual void v067();
    virtual void v068();
    virtual void v069();
    virtual void v070();
    virtual void v071();
    virtual void v072();
    virtual void v073();
    virtual void v074();
    virtual void v075();
    virtual void v076();
    virtual void v077();
    virtual void v078();
    virtual void v079();
    virtual void v080();
    virtual void v081();
    virtual void v082();
    virtual void v083();
    virtual void v084();
    virtual void v085();
    virtual void v086();
    virtual void v087();
    virtual void v088();
    virtual void v089();
    virtual void v090();
    virtual void v091();
    virtual void v092();
    virtual void v093();
    virtual void v094();
    virtual void v095();
    virtual void v096();
    virtual void v097();
    virtual C2DBoxI GetPossibleCollidedWithArea() const;   // slot 98 (+0x188)
    CThing*   ParentThing;                              // +0x04
    bool      ToBeRemoved;                              // +0x08
    bool      IsInGlobalUpdateTCs;                      // +0x09
protected:
    CThingManager& GetThingManager();                   // 0x00686AE0
    const CThingSearchTools& PeekThingSearchTools() const;   // 0x006AFD40
};
class CTCPhysicsBase : public CTCBase {
public:
    C3DVector Position;                                 // +0x0c
    C3DVector OldPosition;                              // +0x18
    long      OldPositionLastFrameSet;                  // +0x24
    C3DVector Velocity;                                 // +0x28
    float     Radius;                                   // +0x34
};
class CApplyAdditionalPlayerImpulseIfTooClose {         // 0x14 bytes (PDB)
public:
    C3DVector Position;
    float     Radius;
    const CTCPhysicsBase* Owner;
    CApplyAdditionalPlayerImpulseIfTooClose(const C3DVector& position, float radius, const CTCPhysicsBase& owner)
        : Position(position), Radius(radius), Owner(&owner) { }
    CApplyAdditionalPlayerImpulseIfTooClose(const CApplyAdditionalPlayerImpulseIfTooClose& other)
        : Position(other.Position), Radius(other.Radius), Owner(other.Owner) { }
};
class CApplyAdditionalPlayerImpulseToPlayerIfInsideUnpushableCreature {   // 4 bytes (PDB)
public:
    CTCPhysicsBase* Player;
    CApplyAdditionalPlayerImpulseToPlayerIfInsideUnpushableCreature(CTCPhysicsBase& player) : Player(&player) { }
};
class CTCPhysicsControlled : public CTCPhysicsBase {
private:
    void UpdateAdditionalImpulse();
};

void CTCPhysicsControlled::UpdateAdditionalImpulse()
{
    if (GetThingManager().NonScriptedEntitesPauseMode)
        return;
    if (GetThingManager().AllEntitesPauseMode)
        return;
    std::vector<CThingCreatureBase*> creatures;
    {
        C2DBoxI area = GetPossibleCollidedWithArea();
        PeekThingSearchTools().GetAllThingsInRoughArea<CThingCreatureBase>(area, CThingFilter_IsNot(ParentThing), creatures);
    }
    CThingCreatureBase** first = creatures.begin();
    CThingCreatureBase** last = creatures.end();
    std::for_each(first, last, CApplyAdditionalPlayerImpulseIfTooClose(Position, Radius, *this));
    std::for_each(first, last, CApplyAdditionalPlayerImpulseToPlayerIfInsideUnpushableCreature(*this));
}
