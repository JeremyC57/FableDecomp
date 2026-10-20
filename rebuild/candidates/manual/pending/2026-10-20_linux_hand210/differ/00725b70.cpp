// CTCPhysicsControlled::UpdateAdditionalImpulse() -- unless the thing manager pauses entities: gather the creatures (other than the owner) in the body's possible collision area and apply the in-combat-and-too-close impulse and the player-inside-unpushable-creature impulse to each
// Grade: DIFFER(205v201) -- frame and register allocation: retail shares one 0x28 frame between the area box, the filter and both for_each result temporaries and frees the vector through the cached start (edi); ours (0x24 with the search in its own block, 0x34 without) reloads the start for free and moves the result temporaries
// Leaf: UpdateAdditionalImpulse
// Retail: Fable.exe (TLC, VC7.1) @ 0x00725B70, 201 bytes (plain `ret`; PDB ?UpdateAdditionalImpulse@CTCPhysicsControlled@@AAEXXZ).
// Callees: CTCBase::GetThingManager() @ 0x00686AE0 (NonScriptedEntitesPauseMode +0x54, AllEntitesPauseMode +0x55; PDB +0x68/+0x69),
//   virtual GetPossibleCollidedWithArea() const slot 98 (+0x188, C2DBoxI by hidden pointer), CTCBase::PeekThingSearchTools() const
//   @ 0x006AFD40, CThingSearchTools::GetAllThingsInRoughArea<CThingCreatureBase, ...> @ 0x00725F10, std::_For_each<CThingCreatureBase**,
//   CApplyAdditionalImpulseIfInCombatAndTooClose> @ 0x00726610 and <..., CApplyAdditionalPlayerImpulseToPlayerIfInsideUnpushableCreature>
//   @ 0x00726660 (both `__fastcall`, PDB YI; the manifest labels of 0x00725F10/0x00726660 are other instantiations), free @ 0x00BFEA14.
// Tried: chained search call, named area box + memberwise C3DVector copy, cached first/last locals, the search in its own
//   block. Next: lane gap: frame-slot sharing of the by-value result temporaries (QFE/RTM, GOTCHAS 23d/24l) and the start
//   pointer kept in edi across the out-of-line algorithm calls (callee knowledge: the real _For_each bodies above the caller).
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct C2DBoxI {
    long TLX, TLY, BRX, BRY;
    C2DBoxI() { }
};
class CThing;
class CThingCreatureBase;
template <class P> struct CAllocProxy {                 // STLport _STLP_alloc_proxy<P, ...> over an empty allocator
    P _M_data;
    CAllocProxy(P p) : _M_data(p) { }
};
template <class T> struct vector {                      // STLport vector: _Vector_base, 12 bytes
    T* _M_start;
    T* _M_finish;
    CAllocProxy<T*> _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector() { if (_M_start) free(_M_start); }
    T* begin() const { return _M_start; }
    T* end() const { return _M_finish; }
};
struct CThingFilter_IsNot {
    CThing* Thing;
    CThingFilter_IsNot(CThing* thing) : Thing(thing) { }
};
struct CThingSearchTools {
    template <class T, class TContainer, class TFilter>
    void GetAllThingsInRoughArea(const C2DBoxI& box, const TFilter& filter, TContainer& out) const;   // 0x00725F10
};
struct CThingManager {
    unsigned char _pad00[0x54];
    bool NonScriptedEntitesPauseMode;                   // +0x54 (PDB +0x68)
    bool AllEntitesPauseMode;                           // +0x55
};
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
};
class CApplyAdditionalPlayerImpulseToPlayerIfInsideUnpushableCreature {
public:
    CTCPhysicsControlled* Physics;
    CApplyAdditionalPlayerImpulseToPlayerIfInsideUnpushableCreature(CTCPhysicsControlled* physics) : Physics(physics) { }
};
namespace std {
    CApplyAdditionalImpulseIfInCombatAndTooClose __fastcall _For_each(CThingCreatureBase** first, CThingCreatureBase** last,
        CApplyAdditionalImpulseIfInCombatAndTooClose func);   // 0x00726610
    CApplyAdditionalPlayerImpulseToPlayerIfInsideUnpushableCreature __fastcall _For_each(CThingCreatureBase** first, CThingCreatureBase** last,
        CApplyAdditionalPlayerImpulseToPlayerIfInsideUnpushableCreature func);   // 0x00726660
}
struct CTCPhysicsBase {
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
    CThing*   ParentThing;                              // +0x04 (CTCBase)
    bool      ToBeRemoved;                              // +0x08
    bool      IsInGlobalUpdateTCs;                      // +0x09
    unsigned char _pad0a[2];
    C3DVector Position;                                 // +0x0c
    C3DVector OldPosition;                              // +0x18
    long      OldPositionLastFrameSet;                  // +0x24
    C3DVector Velocity;                                 // +0x28
    float     Radius;                                   // +0x34
    CThingManager* GetThingManager();                   // 0x00686AE0 (CTCBase)
    const CThingSearchTools* PeekThingSearchTools() const;   // 0x006AFD40
};
class CTCPhysicsControlled : public CTCPhysicsBase {
public:
    void UpdateAdditionalImpulse();
};

void CTCPhysicsControlled::UpdateAdditionalImpulse()
{
    if (GetThingManager()->NonScriptedEntitesPauseMode)
        return;
    if (GetThingManager()->AllEntitesPauseMode)
        return;
    vector<CThingCreatureBase*> creatures;
    {
        C2DBoxI area = GetPossibleCollidedWithArea();
        PeekThingSearchTools()->GetAllThingsInRoughArea<CThingCreatureBase>(area, CThingFilter_IsNot(ParentThing), creatures);
    }
    CThingCreatureBase** first = creatures.begin();
    CThingCreatureBase** last = creatures.end();
    std::_For_each(first, last, CApplyAdditionalImpulseIfInCombatAndTooClose(Position, Radius, this));
    std::_For_each(first, last, CApplyAdditionalPlayerImpulseToPlayerIfInsideUnpushableCreature(this));
}
