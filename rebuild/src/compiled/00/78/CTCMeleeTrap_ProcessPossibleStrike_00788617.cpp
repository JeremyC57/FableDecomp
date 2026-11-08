// CTCMeleeTrap::ProcessPossibleStrike(float hitTolerance, float damage, const CPointPair& previous, const CPointPair& current, CThing& thing) -- when the thing has hit locations: find the first hit location the triangle (current pair, previous point) intersects, hit the thing there along the trail direction with the trap's damage, and remember it in HitThings for a second and a half
// Leaf: ?ProcessPossibleStrike@CTCMeleeTrap@@AAEXMMABU?$pair@VC3DVector@@V1@@std@@0AAVCThing@@@Z
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: the local C2DCoordI removed too (engine/C3DVector.h includes engine/C2DCoordI.h, Ego_r TPI 0xa59e: public long X/Y, size 8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00788617, 423 bytes, thiscall, `ret 14h`, the biased /Os frame (`lea ebp,[esp-64h]`).
// Name: Ego_r.pdb `?ProcessPossibleStrike@CTCMeleeTrap@@AAEXMMABU?$pair@VC3DVector@@V1@@std@@0AAVCThing@@@Z` (private); the only
//   caller is PerformPossibleTrapDamage 0x0078891D (call 0x00788A90) which passes its own two floats and consecutive trail pairs.
// Callees: bitset<274>::test 0x00410DE0 + CVectorMap::LowerBound 0x0040F020 (inline GetTC<CTCHitLocations>, type 0x7f),
//   vector<C3DVector>::push_back 0x0066E630 (x3), CTCHitLocations::GetPCombatFirstHitLocationIntersectionWithTriangle
//   (const vector<C3DVector>&, C3DVector&, C3DVector&, float) 0x008169B0 with 0.25f (0x0123078C), free 0x00BFEA14 (the triangle's
//   inline destructor), C3DVector::AccurateNormalise 0x00A14440, CHitParameters::CHitParameters 0x00789238 (nine arguments),
//   CHitParameters::AddDamageWithAttribute 0x00868CA0, CThing virtual slot 65 OnHit(CHitParameters&) (+0x104),
//   CIntelligentPointer<CThing>::CIntelligentPointer(CThing*) 0x004A96F0 and copy 0x004A9730 (both store the
//   CIntelligentPointer<CThing> vtable 0x01238C6C; the <const CThing> instantiation is 0x0056C860 / vtable 0x01237F1C),
//   pair<CIntelligentPointer<CThing>,long>::pair(const pair<CIntelligentPointer<CThing>,float>&) 0x00788D68,
//   vector<pair<CIntelligentPointer<CThing>,long>>::push_back 0x00789515, ~CBaseIntelligentPointer 0x00A01C10 (x3),
//   ~CHitParameters 0x00789358. Globals: DELTA_NOTIONAL_ZERO 0x0129BA3C, g_framesPerSecond 0x01375550, 1.5f 0x0126B7E0.
// Layout: as PerformPossibleTrapDamage (HitThings +0xc = Ego_r vector<pair<CIntelligentPointer<CThing>, long>>; the landed
//   0x00788D68 file spells the pair `<const CThing>`, the stored vtable 0x01238C6C proves `<CThing>`).
// Shape: the triangle vector lives in its own block (freed right after the intersection call, its slot reused by the direction);
//   direction = current.first - previous.first, normalised when its squared length exceeds DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO (landed 0x00805A90 form);
//   the hit is built with the trap's parent as the hitting thing and no weapon, knock-down set, the other flags clear; the
//   remembered entry is pair<CIntelligentPointer<CThing>, float>(&thing, g_framesPerSecond * 1.5f) converted on push_back.
#include "fable_compiler.h"
#pragma optimize("s", on)
#include <stddef.h>
#include <new>
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
extern const float DELTA_NOTIONAL_ZERO;                                   // 0x0129BA3C ?DELTA_NOTIONAL_ZERO@@3MB (ESTABLISHED_SPELLINGS; naming wave 20) (1e-4f)
extern long g_framesPerSecond;                                // 0x01375550

#include "engine/C3DVector.h"
class C2DBoxI { public:
    long TLX, TLY, BRX, BRY;
    C2DBoxI(const C2DCoordI& centre, long radius);            // 0x00434740
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) {}
    pair(const _T1& a, const _T2& b) : first(a), second(b) {}
    pair(const pair& p) : first(p.first), second(p.second) {}
    template <class _U1, class _U2> pair(const pair<_U1, _U2>& p) : first(p.first), second((_T2)p.second) {}
};
template <class _Tp> class allocator { public:
    allocator() {}
    void deallocate(_Tp* p, unsigned int) { if (p != 0) free(p); }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public:
    _Tp _M_data;
    _STLP_alloc_proxy(const _Alloc& a, _Tp p) : _Alloc(a), _M_data(p) {}
};
template <class _Tp, class _Alloc> class _Vector_base { public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
    _Vector_base(const _Alloc& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (_Tp*)0) {}
    ~_Vector_base() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> { public:
    typedef _Tp value_type;
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    vector() : _Vector_base<_Tp, _Alloc>(_Alloc()) {}
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Tp& operator[](size_type n) { return *(begin() + n); }
    void push_back(const _Tp& x);                             // out of line in retail
};
template <class _Container> class back_insert_iterator { public:
    _Container* container;
    explicit back_insert_iterator(_Container& c) : container(&c) {}
};
template <class _Container> inline back_insert_iterator<_Container> back_inserter(_Container& c) { return back_insert_iterator<_Container>(c); }
template <class _InIt, class _OutIt, class _Fn1> _OutIt __fastcall _Transform(_InIt first, _InIt last, _OutIt dest, _Fn1 func);
template <class _InIt, class _OutIt, class _Fn1> inline _OutIt transform(_InIt first, _InIt last, _OutIt dest, _Fn1 func) { return _Transform(first, last, dest, func); }
template <class _Predicate> class unary_negate { public:
    _Predicate _M_pred;
    explicit unary_negate(const _Predicate& p) : _M_pred(p) {}
};
template <class _Predicate> inline unary_negate<_Predicate> not1(const _Predicate& p) { return unary_negate<_Predicate>(p); }
}
typedef std::pair<C3DVector, C3DVector> CPointPair;
typedef std::vector<CPointPair> CPointPairVector;

class CThing; class CHitParameters; class CTCBase; class CWorld; class CThingSearchTools; class CTrapDef;
enum ETCInterfaceType { TC_INTERFACE_WEAPON_TRAIL = 0x4c, TC_INTERFACE_HIT_LOCATIONS = 0x77 /* also the excluded type */, TC_INTERFACE_HIT_LOCATIONS_2 = 0x7f };
enum EDamageAttribute { DAMAGE_ATTRIBUTE_NONE = 0 };

class CBaseIntelligentPointer { public:
    virtual ~CBaseIntelligentPointer();                       // 0x00A01C10
    void* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer(T* item);                             // 0x004A96F0 for <CThing> (stores vtable 0x01238C6C)
    CIntelligentPointer(const CIntelligentPointer& other);    // 0x004A9730
};

namespace std { template <unsigned int N> class bitset { public: unsigned long _M_w[(N + 31) / 32]; bool test(unsigned int pos) const; }; }   // 0x00410DE0 for <274>
template <class K, class V, class C> class CVectorMap;
template <class K, class V> class CKeyPairCompareLess;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& key);          // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
class CThing { public:                                        // retail CThing (landed models): bitset +0x20, interface map +0x44
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF(60) VF(61) VF(62) VF(63) VF(64)
    virtual void OnHit(CHitParameters& hit);                  // slot 65 (+0x104)
    unsigned char _pad04[0x20 - 0x04];
    std::bitset<274> UsingComponents;                         // +0x20
    CInterfaceMap    Components;                              // +0x44
    const C3DVector& GetPos() const;                          // 0x004C73D0
    bool HasTC(ETCInterfaceType type) const { return UsingComponents.test(type); }
    template <class T> FABLE_FORCEINLINE bool GetTC(T** out)
    {
        long type = T::StatGetInterfaceType();
        if (HasTC((ETCInterfaceType)type)) {
            ETCInterfaceType key = (ETCInterfaceType)type;
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(key);
            if (it == map->last || type < it->first)
                it = map->last;
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};
#undef VF
#undef VF10
class CThingGameObject : public CThing {};
class CTCBase { public:
    virtual ~CTCBase();
    CThing* ParentThing;                                      // +0x04
protected:
    CWorld& GetWorld() const;                                 // 0x00686A70 Ego_r ?GetWorld@CTCBase@@IBEAAVCWorld@@XZ
};
class CTCWeaponTrail : public CTCBase { public:
    static ETCInterfaceType StatGetInterfaceType() { return TC_INTERFACE_WEAPON_TRAIL; }
    void GetAllTrailsLastWeaponTrailPoints(std::vector<CPointPairVector>& points) const;   // 0x0076EA60
};
namespace NHitLocations { class CHitLocation; }
class CTCHitLocations : public CTCBase { public:
    static ETCInterfaceType StatGetInterfaceType() { return TC_INTERFACE_HIT_LOCATIONS_2; }
    NHitLocations::CHitLocation* GetPCombatFirstHitLocationIntersectionWithTriangle(const std::vector<C3DVector>& triangle, C3DVector& hitA, C3DVector& hitB, float tolerance);   // 0x008169B0
};
class CHitParameters { public:
    unsigned char Body[0x60];
    CHitParameters(NHitLocations::CHitLocation* hitLocation, CThing* thingHitting, CThing* weapon, const C3DVector& position,
                   const C3DVector& direction, bool knockDown, bool blockable, bool flourish, bool blockCounter);   // 0x00789238
    ~CHitParameters();                                        // 0x00789358
    void AddDamageWithAttribute(EDamageAttribute attribute, float damage);   // 0x00868CA0
};
class CTrapDef { public:                                      // Ego_r CTrapDef, retail -4 (12-byte map): TriggerRadius +0x3c, TrapDamage +0x44
    unsigned char _pad00[0x3c];
    float TriggerRadius;                                      // +0x3c
    unsigned char _pad40[0x4];
    float TrapDamage;                                         // +0x44
};
template <class T> class CDefPointer { public: const T* Object; const T* operator->() const { return Object; } };
class CFadeInFadeOutBase { public: unsigned char Body[0x1c]; };
enum ETrapType {};
class CThingSearchTools;
template <class T> struct scoped_ptr { T* Ptr; T* operator->() const { return Ptr; } T& operator*() const { return *Ptr; } };
class CWorld { public:                                        // Ego_r CWorld: PThingSearchTools (scoped_ptr) +0x20
    unsigned char _pad00[0x20];
    scoped_ptr<CThingSearchTools> PThingSearchTools;          // +0x20
    CThingSearchTools& GetThingSearchTools() const { return *PThingSearchTools; }
};

class CThingFilter_IsNotList { public:                        // Ego_r layout: PNotList +0, PNotList2 +4
    std::vector<const CThing*>* PNotList;
    std::vector<CIntelligentPointer<CThing> >* PNotList2;
    CThingFilter_IsNotList(const std::vector<const CThing*>& list) : PNotList((std::vector<const CThing*>*)&list), PNotList2(0) {}
};
class CThingFilter_HasTC { public:
    ETCInterfaceType Type;
    CThingFilter_HasTC(ETCInterfaceType type) : Type(type) {}
};
namespace {
struct CThingFilter_IsAbleToBeAttacked { CThingFilter_IsAbleToBeAttacked() {} };
struct CThingFilter_IsAffectedByTraps { CThingFilter_IsAffectedByTraps() {} };
struct CGetThingFromIntelligentPointer { CGetThingFromIntelligentPointer() {} };
}
template <class A, class B> class CFilter_Bind2 { public:
    const A* m_pFirst; const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) {}
};
template <class A, class B, class C> class CFilter_Bind3 { public:
    const A* m_pFirst; const B* m_pSecond; const C* m_pThird;
    CFilter_Bind3(const CFilter_Bind2<A, B>& ab, const C& c) : m_pFirst(ab.m_pFirst), m_pSecond(ab.m_pSecond), m_pThird(&c) {}
};
template <class A, class B, class C, class D> class CFilter_Bind4 { public:
    const A* m_pFirst; const B* m_pSecond; const C* m_pThird; const D* m_pFourth;
    CFilter_Bind4(const CFilter_Bind3<A, B, C>& abc, const D& d) : m_pFirst(abc.m_pFirst), m_pSecond(abc.m_pSecond), m_pThird(abc.m_pThird), m_pFourth(&d) {}
};
template <class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
template <class A, class B, class C> inline CFilter_Bind3<A, B, C> operator&&(const CFilter_Bind2<A, B>& ab, const C& c) { return CFilter_Bind3<A, B, C>(ab, c); }
template <class A, class B, class C, class D> inline CFilter_Bind4<A, B, C, D> operator&&(const CFilter_Bind3<A, B, C>& abc, const D& d) { return CFilter_Bind4<A, B, C, D>(abc, d); }
typedef CFilter_Bind4<CThingFilter_IsNotList, CThingFilter_IsAbleToBeAttacked, std::unary_negate<CThingFilter_HasTC>, CThingFilter_IsAffectedByTraps> CTrapTargetFilter;

class CThingSearchTools { public:
    template <class T, class TContainer, class TFilter> void GetAllThingsInRoughArea(const C2DBoxI& area, const TFilter& filter, TContainer& result) const;   // 0x0078971D
};

class CTCTrapBase : public CTCBase { public:                   // Ego_r CTCTrapBase (0x60 retail: 12-byte vectors)
    unsigned char _pad08[0x4];
    std::vector<std::pair<CIntelligentPointer<CThing>, long> > HitThings;   // +0x0c (Ego_r; vtable 0x01238C6C = CIntelligentPointer<CThing>)
    std::vector<CIntelligentPointer<CThing> > ImmuneThings;   // +0x18
    CIntelligentPointer<CThing> PObstruction;                 // +0x24
    std::vector<CIntelligentPointer<CThing> > TriggerThings;  // +0x2c
    CFadeInFadeOutBase TrapOpener;                            // +0x38
    CDefPointer<CTrapDef> PTrapDef;                           // +0x54
    bool Active;                                              // +0x58
    bool Triggered;                                           // +0x59
    bool LastTriggerOnProximity;                              // +0x5a
    ETrapType TrapType;                                       // +0x5c
};
class CTCMeleeTrap : public CTCTrapBase { public:
    bool PlayingContinuous;                                   // +0x60
    bool StrikeAnimation;                                     // +0x61
private:
    void ProcessPossibleStrike(float hitTolerance, float damage, const CPointPair& previous, const CPointPair& current, CThing& thing);   // 0x00788617
    void PerformPossibleTrapDamage(float hitTolerance, float damage);   // 0x0078891D
};

void CTCMeleeTrap::ProcessPossibleStrike(float hitTolerance, float damage, const CPointPair& previous, const CPointPair& current, CThing& thing)
{
    CTCHitLocations* hitLocations;
    if (!thing.GetTC(&hitLocations))
        return;
    NHitLocations::CHitLocation* hitLocation;
    C3DVector hitPosition;
    C3DVector hitNormal;
    {
        std::vector<C3DVector> triangle;
        triangle.push_back(current.first);
        triangle.push_back(current.second);
        triangle.push_back(previous.first);
        hitLocation = hitLocations->GetPCombatFirstHitLocationIntersectionWithTriangle(triangle, hitPosition, hitNormal, 0.25f);
    }
    if (hitLocation != 0) {
        C3DVector direction(current.first.X - previous.first.X, current.first.Y - previous.first.Y, current.first.Z - previous.first.Z);
        float lengthSquared = direction.X * direction.X + direction.Y * direction.Y + direction.Z * direction.Z;
        if (lengthSquared > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO)
            direction.AccurateNormalise();
        CHitParameters hit(hitLocation, ParentThing, 0, hitPosition, direction, true, false, false, false);
        hit.AddDamageWithAttribute(DAMAGE_ATTRIBUTE_NONE, damage);
        thing.OnHit(hit);
        float frames = (float)g_framesPerSecond * 1.5f;
        HitThings.push_back(std::pair<CIntelligentPointer<CThing>, float>(CIntelligentPointer<CThing>(&thing), frames));
    }
}