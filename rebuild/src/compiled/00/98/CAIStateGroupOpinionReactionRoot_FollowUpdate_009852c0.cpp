// CAIStateGroup_OpinionReactionRoot::FollowUpdate(float distance, bool run) -- once the reacted-to thing is farther than `distance`, turn to face it and walk (or run) to a wander position within 0.9 x distance of it; true while following is fine
// Leaf: ?FollowUpdate@CAIStateGroup_OpinionReactionRoot@@AAE_NM_N@Z
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local GetMagnitudeSquared respelled GetSquaredMagnitude (the header / FableWin.pdb method, same computation).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009852C0, 337 bytes (`ret 8`). Ego_r.pdb ?FollowUpdate@CAIStateGroup_OpinionReactionRoot@@AAE_NM_N@Z
//   (private; the parked file emitted it on a derived "...Methods" stand-in).
// Callees (Ego_r.pdb spellings): CBaseIntelligentPointer::GetPItem() const 0x00A01B50 (returns CBaseObject*; ThingReactingTo
//   +0x6B), CThing::GetPos() const 0x004C73D0, CAIStateGroup_OpinionReactionRoot::TurnToFace() 0x0095EDF0 (private),
//   CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (inside the inline CThing::GetTC<CTCCreatureNavigation>,
//   interface 7 = CTCCreatureNavigation::StatGetInterfaceType; the miss path reads the dead `run` slot that VC7.1 made the
//   out pointer's home), CTCCreatureNavigation::GetNearbyWanderPosition(const C3DVector&, float, C3DVector&, bool) const
//   0x006CB6E0, CAIStateGroupBase::AIAction_MoveToPosProximity (protected; EAIActionRepeat, C3DVector by value, float,
//   bool&, EMovementType, bool, bool, bool, const CThing*, bool, float) 0x0097E890. Inline: CAIStateGroupBase::GetCreature()
//   const and CAIBrain::GetCreature() const (both Ego_r names, CThingAICreature&).
// Globals: 0.9f @ 0x01253AF0.
// Shape: `target - creature` through a three-float ctor (Z first), its length through the sqrt intrinsic; the movement
//   type is a named `EMovementType movement = run ? 5 : 3` local declared before `reached` (setne + lea; inlined into the
//   argument it colours the run byte cl/edx instead of retail bl/eax); the result is `MoveToPosProximity(...) == 2`.
// Layout: engine/CAIStateGroupBase.h and engine/CAIBrain.h through the layout-rename include (the generated headers
//   flatten the base, so CAIStateGroupBase's members cannot be reached on engine/CAIStateGroup_OpinionReactionRoot.h);
//   the OpinionReactionRoot members up to ThingReactingTo +0x6B are copied from that header (pack(1)).
#include "fable_compiler.h"
#define CAIStateGroupBase CAIStateGroupBaseLayout
#include "engine/CAIStateGroupBase.h"
#undef CAIStateGroupBase
#define CAIBrain CAIBrainLayout
#include "engine/CAIBrain.h"
#undef CAIBrain
extern "C" int __cdecl puts(const char* text);              // 0x00BFEAF2 (MSVCR71 import thunk)
extern "C" __declspec(noreturn) void __cdecl abort(void);   // 0x00BFEAEC
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
namespace std {
template <unsigned int _Nb>
class bitset {                                              // STLport 4.6 bitset<274>: 9 words, test() range-checked (no exceptions)
public:
    enum { _Words = (_Nb + 31) / 32 };
    unsigned long _M_w[_Words];
    static unsigned int _S_whichword(unsigned int __pos) { return __pos / 32; }
    static unsigned int _S_whichbit(unsigned int __pos) { return __pos % 32; }
    static unsigned long _S_maskbit(unsigned int __pos) { return 1UL << _S_whichbit(__pos); }
    unsigned long _M_getword(unsigned int __pos) const { return _M_w[_S_whichword(__pos)]; }
    bool _Unchecked_test(unsigned int __pos) const { return (_M_getword(__pos) & _S_maskbit(__pos)) != 0; }
    bool test(unsigned int __pos) const
    {
        if (__pos >= _Nb) {
            puts("bitset");
            abort();
        }
        return _Unchecked_test(__pos);
    }
};
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};
}
enum ETCInterfaceType { TC_INTERFACE_NONE = 0 };
enum EAIActionReturn { };
enum EAIActionRepeat { };
enum EMovementType { };
class CTCBase;
class CBaseObject;
template <class K, class V> class CKeyPairCompareLess { public: CKeyPairCompareLess() { } };
template <class K, class V, class C = CKeyPairCompareLess<K, V> >
class CVectorMap : public std::vector<std::pair<K, V> > {
public:
    typedef std::pair<K, V> value_type;
    value_type* LowerBound(const K& key);                   // 0x0040F020
    C Compare;                                              // +0x0C
    bool Dirty;                                             // +0x0D
};
#include "engine/C3DVector.h"
class CThing { public:
    unsigned char _pad0[0x20];
    std::bitset<274> UsingComponents;                       // +0x20
    CVectorMap<ETCInterfaceType, CTCBase*> Components;      // +0x44
    const C3DVector& GetPos() const;                        // 0x004C73D0
    template <class T> FABLE_FORCEINLINE bool GetTC(T** out)
    {
        bool has = UsingComponents.test(T::StatGetInterfaceType());
        if (has) {
            ETCInterfaceType type = T::StatGetInterfaceType();
            CVectorMap<ETCInterfaceType, CTCBase*>* map = &Components;
            CVectorMap<ETCInterfaceType, CTCBase*>::value_type* it = map->LowerBound(type);
            if (it == map->_M_finish || T::StatGetInterfaceType() < it->first)
                it = map->_M_finish;
            *out = (T*)it->second;
        }
        return has;
    }
};
class CThingAICreature : public CThing { public: };
class CTCCreatureNavigation { public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)7; }
    bool GetNearbyWanderPosition(const C3DVector& centre, float radius, C3DVector& result, bool onMap) const;   // 0x006CB6E0
};
class CBaseIntelligentPointer { public:
    void* __vftable;
    CBaseObject* PItem;                                     // +0x04
    CBaseObject* GetPItem() const;                          // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* operator->() const { return (T*)GetPItem(); }
};
class CAIBrain : public CAIBrainLayout { public:
    CThingAICreature& GetCreature() const { return *Creature; }
};
class CAIStateGroupBase : public CAIStateGroupBaseLayout { public:
protected:
    CThingAICreature& GetCreature() const { return ((CAIBrain*)PBrain)->GetCreature(); }
    EAIActionReturn AIAction_MoveToPosProximity(EAIActionRepeat repeat, C3DVector pos, float proximity, bool& reached,
        EMovementType movement, bool a, bool b, bool c, const CThing* avoid, bool d, float speed);   // 0x0097E890
};
#pragma pack(push, 1)
class CAIStateGroup_OpinionReactionRoot : public CAIStateGroupBase {
    bool TurnToFace();                                      // 0x0095EDF0
    bool FollowUpdate(float distance, bool run);
public:
    void*         PTCTalk;                                  // +0x14
    unsigned char CurrentReactionType[0x4];                 // +0x18 EOpinionReactionType
    bool          ReactionFlags[79];                        // +0x1C
    CIntelligentPointer<CThing> ThingReactingTo;            // +0x6B
};
#pragma pack(pop)

bool CAIStateGroup_OpinionReactionRoot::FollowUpdate(float distance, bool run)
{
    CThingAICreature& creature = GetCreature();
    CThing* target = ThingReactingTo.operator->();
    const C3DVector& from = creature.GetPos();
    float lengthSquared;
    {
        C3DVector delta = target->GetPos() - from;
        lengthSquared = delta.GetSquaredMagnitude();
    }
    if ((float)sqrt(lengthSquared) > distance) {
        if (!TurnToFace())
            return false;
        CTCCreatureNavigation* navigation;
        GetCreature().GetTC(&navigation);
        C3DVector wander;
        if (navigation->GetNearbyWanderPosition(ThingReactingTo->GetPos(), distance * 0.9f, wander, false)) {
            EMovementType movement = (EMovementType)(run ? 5 : 3);
            bool reached;
            return AIAction_MoveToPosProximity((EAIActionRepeat)1, wander, 0.5f, reached, movement,
                true, false, true, 0, true, 0.0f) == 2;
        }
    }
    return true;
}