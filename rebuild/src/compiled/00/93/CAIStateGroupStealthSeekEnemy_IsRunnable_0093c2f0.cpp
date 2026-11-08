// CAIStateGroup_StealthSeekEnemy::IsRunnable() -- runnable when the creature perceives a live enemy it is willing to attack, that it can see (extended line of sight) or is aware of, and that its targeting component accepts; latches it as PEnemy and remembers where it was last perceived
// Leaf: ?IsRunnable@CAIStateGroup_StealthSeekEnemy@@UAE_NXZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0093C2F0, 303 bytes (virtual). Ego_r.pdb ?IsRunnable@CAIStateGroup_StealthSeekEnemy@@UAE_NXZ.
// Callees (Ego_r.pdb spellings): CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const 0x004365B0 (inside the inline
//   CThing::PeekTC<const CTCPerceiveThing> / PeekTC<const CTCTargetingAI>, both Ego_r names; interface 0x34 = bit 20 of
//   +0x24, interface 8 = bit 8 of +0x20), CTCPerceiveThing::GetPerceivedThing() const 0x007446A0,
//   CTCEnemy::AreThingsEnemies(const CThing&, const CThing&) 0x0076D2F0 (static __fastcall),
//   NCreatureAI::IsWillingToAttackTarget(const CThingAICreature*, const CThing*) 0x008648A0 (__fastcall),
//   CTCPerceiveThing::IsAwareOfThing() const 0x00643B3B, CTCTargetingAI::IsThingValidTarget(const CThing&) const 0x007F6170,
//   CIntelligentPointer<CThing>::CIntelligentPointer(CThing*) 0x004A96F0 (stores vtable 0x01238C6C, RTTI
//   .?AV?$CIntelligentPointer@VCThing@@@@), CIntelligentPointer<CThing>::operator=(const CIntelligentPointer<CThing>&)
//   0x004A9760, CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10.
// Layout: CTCPerceiveThing through the layout-rename include (ExtendedLineOfSight +0x2e, LastPerceivedPos +0x20);
//   CAIStateGroupBase polymorphic over the header's size (the generated header models the vptr as a member);
//   PEnemy +0x14 / DetectedPos +0x1c copied from engine/CAIStateGroup_StealthSeekEnemy.h; retail CThing Dead = bit 0 of +0x91.
// Shape: CThing::PeekTC<T> as `if (HasTC(type)) { lookup; return true; } return false;` (the mask test on the bitset word);
//   a same-TU auto_inline(off) stand-in for the const LowerBound (GOTCHAS chunk 240) lets the CIntelligentPointer temporary
//   overlay the dead key slot (sub esp,8); the creature for AreThingsEnemies through the inline
//   CAIStateGroupBase::GetCreature() accessor and the other three reads straight through PBrain->Creature (this mix sets
//   retail's scratch rotation edx/eax/ecx); `PEnemy = const_cast<CThing*>(target)` is an implicit-conversion temporary.
#include "fable_compiler.h"
#include "engine/CBaseClass.h"
#define CAIStateGroupBase CAIStateGroupBaseLayout
#include "engine/CAIStateGroupBase.h"
#undef CAIStateGroupBase
#define CAIBrain CAIBrainLayout
#include "engine/CAIBrain.h"
#undef CAIBrain
#define CTCPerceiveThing CTCPerceiveThingLayout
#include "engine/CTCPerceiveThing.h"
#undef CTCPerceiveThing
class CAIBrain;
extern "C" int __cdecl puts(const char* text);              // 0x00BFEAF2 (MSVCR71 import thunk)
extern "C" __declspec(noreturn) void __cdecl abort(void);   // 0x00BFEAEC
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
class CTCBase;
class CBaseObject;
template <class K, class V> class CKeyPairCompareLess { public: CKeyPairCompareLess() { } };
template <class K, class V, class C = CKeyPairCompareLess<K, V> > class CVectorMap;
template <>
class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >
    : public std::vector<std::pair<ETCInterfaceType, CTCBase*> > {
public:
    typedef std::pair<ETCInterfaceType, CTCBase*> value_type;
    const value_type* LowerBound(const ETCInterfaceType& key) const;   // 0x004365B0
    CKeyPairCompareLess<ETCInterfaceType, CTCBase*> Compare;           // +0x0C
    bool Dirty;                                                        // +0x0D
};
// Same-TU stand-in for the const LowerBound (not the retail body; GOTCHAS chunk 240: the CIntelligentPointer temporary
// overlays the dead lookup-key slot only when the TU holds a body for the LowerBound copy it calls).
#pragma auto_inline(off)
__declspec(noinline) const CVectorMap<ETCInterfaceType, CTCBase*>::value_type*
CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound(const ETCInterfaceType& key) const
{
    const value_type* it = _M_start;
    long count = _M_finish - it;
    while (count > 0) {
        long half = count / 2;
        if (it[half].first < key) { it += half + 1; count -= half + 1; }
        else count = half;
    }
    return it;
}
#pragma auto_inline(on)
#include "engine/C3DVector.h"
class CThing { public:
    unsigned char _pad0[0x20];
    std::bitset<274> UsingComponents;                       // +0x20
    CVectorMap<ETCInterfaceType, CTCBase*> Components;      // +0x44
    unsigned char _pad54[0x3d];
    unsigned char Dead : 1;                                 // +0x91 bit 0
    unsigned char _bits91 : 7;
    unsigned char _pad92[0x6];
    bool HasTC(ETCInterfaceType type) const { return UsingComponents.test(type); }
    template <class T> FABLE_FORCEINLINE bool PeekTC(T** out) const
    {
        if (HasTC(T::StatGetInterfaceType())) {
            ETCInterfaceType type = T::StatGetInterfaceType();
            const CVectorMap<ETCInterfaceType, CTCBase*>* map = &Components;
            const CVectorMap<ETCInterfaceType, CTCBase*>::value_type* it = map->LowerBound(type);
            if (it == map->_M_finish || T::StatGetInterfaceType() < it->first)
                it = map->_M_finish;
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CThingAICreature : public CThing { public: };
template <class T> bool __fastcall IS_THING_ALIVE(const T& p)
{
    if (p != 0) {
        if (!p->Dead)
            return true;
    }
    return false;
}
class CTCEnemy { public:
    static bool __fastcall AreThingsEnemies(const CThing& a, const CThing& b);   // 0x0076D2F0
};
namespace NCreatureAI {
bool __fastcall IsWillingToAttackTarget(const CThingAICreature* creature, const CThing* target);   // 0x008648A0
}
class CTCPerceiveThing : public CTCPerceiveThingLayout { public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)0x34; }
    const CThing* GetPerceivedThing() const;                // 0x007446A0
    bool IsAwareOfThing() const;                            // 0x00643B3B
};
class CTCTargetingAI { public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)8; }
    bool IsThingValidTarget(const CThing& thing) const;     // 0x007F6170
};
#pragma pack(push, 1)
class CBaseIntelligentPointer { public:
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* PItem;                                     // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer(T* item);                           // 0x004A96F0 (T = CThing)
    CIntelligentPointer& operator=(const CIntelligentPointer& other);   // 0x004A9760 (T = CThing)
};
#pragma pack(pop)
class CAIBrain : public CAIBrainLayout { public:
    CThingAICreature& GetCreature() const { return *Creature; }
};
#pragma pack(push, 1)
class CAIStateGroupBase : public CBaseClass {               // polymorphic: the generated header models the vptr as a member
protected:
    CAIBrain* PBrain;                                       // +0x04
    unsigned char unobservedGroupState[sizeof(CAIStateGroupBaseLayout) - 8];
    CThingAICreature& GetCreature() const { return ((CAIBrain*)PBrain)->GetCreature(); }
public:
    virtual bool IsRunnable();
};
class CAIStateGroup_StealthSeekEnemy : public CAIStateGroupBase {
public:
    virtual bool IsRunnable();
    CIntelligentPointer<CThing> PEnemy;                     // +0x14
    C3DVector DetectedPos;                                  // +0x1c
};
#pragma pack(pop)

bool CAIStateGroup_StealthSeekEnemy::IsRunnable()
{
    const CTCPerceiveThing* perceive;
    if (PBrain->Creature->PeekTC(&perceive)) {
        const CThing* target = perceive->GetPerceivedThing();
        if (IS_THING_ALIVE(target) && CTCEnemy::AreThingsEnemies(GetCreature(), *target)
                && NCreatureAI::IsWillingToAttackTarget(PBrain->Creature, target)
                && (perceive->ExtendedLineOfSight || perceive->IsAwareOfThing())) {
            const CTCTargetingAI* targeting;
            if (PBrain->Creature->PeekTC(&targeting) && targeting->IsThingValidTarget(*target)) {
                PEnemy = const_cast<CThing*>(target);
                DetectedPos = *reinterpret_cast<const C3DVector*>(&perceive->LastPerceivedPos_X);
                return true;
            }
        }
    }
    return false;
}