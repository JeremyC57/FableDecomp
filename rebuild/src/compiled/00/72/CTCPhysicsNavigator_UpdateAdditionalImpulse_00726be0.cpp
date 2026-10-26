// CTCPhysicsNavigator::UpdateAdditionalImpulse() -- clear the additional impulse; when the feature is on and the owner creature is busy with an action, pushable and pushable by self-avoidance, gather the other things in the area it could reach this frame and let each one that is in combat and too close add to its impulse
// Leaf: ?UpdateAdditionalImpulse@CTCPhysicsNavigator@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00726BE0, 254 bytes, plain `ret`.
// Callees: CThingCreatureBase::IsPerformingAnyAction() const 0x006622A0, the physics TC's virtual slot 83 (+0x14c,
//   IsPushableByHero as the landed 0x00715710), NCreatureMode::IsPhysicsPushableBySelfAvoidance(const CThing&) 0x0085FE60
//   (__fastcall), CTCPhysicsNavigator::GetPossibleCollidedWithAreaWithImpulse 0x007267C0 (landed), CTCBase::
//   PeekThingSearchTools() const 0x006AFD40, CThingSearchTools::GetAllThingsInRoughArea<CThingFilter_IsNot> 0x00725F10,
//   std::_For_each<CThingCreatureBase**, CApplyAdditionalImpulseIfInCombatAndTooClose> 0x00727500 (__fastcall, hidden
//   result in ecx, first in edx, the 0x18-byte functor by value), free 0x00BFEA14.
// Globals: the additional-impulse enable byte 0x0137574A (name descriptive).
// Layout: Ego_r.pdb CTCPhysicsNavigator (pack 1: AdditionalImpulse +0x95); CTCPhysicsBase Position +0x0c, Radius +0x34;
//   CThing physics TC +0x60; CApplyAdditionalImpulseIfInCombatAndTooClose {Position, Radius, Owner, OwnerCreature}.
// Shape: the impulse is cleared by a chained `X = Y = Z = 0` (Z stored first, one zero register); the functor is built in
//   place by an inline (position, float radius, owner) ctor that takes the owner creature from owner->ParentThing; the
//   C3DVector copy is memberwise; GetAllThingsInRoughArea has a same-TU stand-in (as the landed 0x0092E950) so the result
//   vector does not escape and its start stays in esi for the freeing destructor.
class CThing;
class CThingCreatureBase;
class CTCPhysicsBase;

class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
class C2DCoordI { public: long X, Y; };
class C2DBoxI { public:
    C2DCoordI TopLeft;
    C2DCoordI BottomRight;
};
template <class T> struct vector {                  // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector() { if (_M_start) free(_M_start); }
    T* begin() const { return _M_start; }
    T* end() const { return _M_finish; }
};
extern "C" void __cdecl free(void* block);          // 0x00BFEA14

class CThingFilter_IsNot { public:
    CThing* Thing;
    CThingFilter_IsNot(CThing* thing) : Thing(thing) { }
};
class CThingSearchTools { public:
    bool GetAllThingsInRoughArea(const C2DBoxI& area, const CThingFilter_IsNot& filter, vector<CThingCreatureBase*>& out);   // 0x00725F10
};

class CTCPhysicsBaseVirtuals {
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
    virtual bool IsPushableByHero();                // slot 83 (+0x14c)
};
class CThingCreatureBase {
public:
    unsigned char _pad00[0x60];
    CTCPhysicsBaseVirtuals* PhysicsTC;              // +0x60
    bool IsPerformingAnyAction() const;             // 0x006622A0
};
namespace NCreatureMode {
bool __fastcall IsPhysicsPushableBySelfAvoidance(const CThing& thing);   // 0x0085FE60
}

#pragma pack(push, 1)
class CTCPhysicsBase {
public:
    virtual void v00();
    CThing*       ParentThing;                      // +0x04 (CTCBase)
    unsigned char _pad08[0x0c - 0x08];
    C3DVector     Position;                         // +0x0c
    unsigned char _pad18[0x34 - 0x18];
    float         Radius;                           // +0x34
};
#pragma pack(pop)
class CApplyAdditionalImpulseIfInCombatAndTooClose { public:   // 0x18 bytes
    C3DVector           Position;                   // +0x00
    float               Radius;                     // +0x0c
    CTCPhysicsBase*     Owner;                      // +0x10
    CThingCreatureBase* OwnerCreature;              // +0x14
    __forceinline CApplyAdditionalImpulseIfInCombatAndTooClose(const C3DVector& position, float radius, CTCPhysicsBase* owner)
        : Position(position), Radius(radius), Owner(owner),
          OwnerCreature((CThingCreatureBase*)owner->ParentThing) { }
};
namespace std {
template <class TIterator, class TFunction>
TFunction __fastcall _For_each(TIterator first, TIterator last, TFunction function);   // 0x00727500 (this instantiation)
}

extern bool g_AdditionalImpulseEnabled;             // 0x0137574A

#pragma pack(push, 1)
class CTCPhysicsNavigator : public CTCPhysicsBase {
public:
    unsigned char _pad38[0x95 - 0x38];
    C3DVector     AdditionalImpulse;                // +0x95
    const CThingSearchTools* PeekThingSearchTools() const;   // 0x006AFD40 (CTCBase)
    C2DBoxI GetPossibleCollidedWithAreaWithImpulse(const C3DVector& impulse) const;   // 0x007267C0
    void UpdateAdditionalImpulse();
};
#pragma pack(pop)

bool SearchRoughArea_00725f10(CThingSearchTools* tools, const C2DBoxI* area, const CThingFilter_IsNot* filter,
                              CThingCreatureBase** first, CThingCreatureBase** last, CThingCreatureBase** end);
CThingCreatureBase** SearchResultFirst_00725f10();
CThingCreatureBase** SearchResultLast_00725f10();
CThingCreatureBase** SearchResultEnd_00725f10();
// Same-TU stand-in for the GetAllThingsInRoughArea<IsNot> instantiation 0x00725F10 (not a reconstruction of it, as the
// landed 0x0092E950): the area and filter go by address, but the result vector's three words are read and rewritten by
// value, so VC7.1 knows the vector does not escape and keeps its start in a register across the for_each call.
#pragma auto_inline(off)
__declspec(noinline) bool CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& area, const CThingFilter_IsNot& filter,
                                                                     vector<CThingCreatureBase*>& out)
{
    bool found = SearchRoughArea_00725f10(this, &area, &filter, out._M_start, out._M_finish, out._M_end_of_storage);
    out._M_start = SearchResultFirst_00725f10();
    out._M_finish = SearchResultLast_00725f10();
    out._M_end_of_storage = SearchResultEnd_00725f10();
    return found;
}
#pragma auto_inline(on)

void CTCPhysicsNavigator::UpdateAdditionalImpulse()
{
    AdditionalImpulse.X = AdditionalImpulse.Y = AdditionalImpulse.Z = 0.0f;
    if (g_AdditionalImpulseEnabled) {
        CThingCreatureBase* creature = (CThingCreatureBase*)ParentThing;
        if (creature->IsPerformingAnyAction() && creature->PhysicsTC->IsPushableByHero()
            && NCreatureMode::IsPhysicsPushableBySelfAvoidance(*(CThing*)creature)) {
            vector<CThingCreatureBase*> creatures;
            C2DBoxI area = GetPossibleCollidedWithAreaWithImpulse(C3DVector(1.0f, 1.0f, 0.0f));
            ((CThingSearchTools*)PeekThingSearchTools())->GetAllThingsInRoughArea(area, CThingFilter_IsNot(ParentThing), creatures);
            std::_For_each(creatures.begin(), creatures.end(),
                           CApplyAdditionalImpulseIfInCombatAndTooClose(Position, Radius, this));
        }
    }
}