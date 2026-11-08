// CThingSearchTools::GetAllThings(const CFilter_Bind4<...>& filter, std::list<CThingGameObject*>& result) const -- appends every scan-visible game object that is not the striker, is strike-to-strike with it, whose hit-location bounding sphere overlaps the strike sphere and that it can collide or interact with
// Leaf: ??$GetAllThings@VCThingGameObject@@V?$list@PAVCThingGameObject@@V?$allocator@PAVCThingGameObject@@@std@@@std@@V?$CFilter_Bind4@VCThingFilter_IsNot@@VCThingFilter_CombatIsStrikeToStrike@?A0x
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0085B5B0, 365 bytes (+3 int3), thiscall `ret 8`; one caller 0x00859392.
// Name (Ego_r.pdb name set, donor 0x0083B380): ??$GetAllThings@VCThingGameObject@@V?$list@PAVCThingGameObject@@...@std@@
//   V?$CFilter_Bind4@VCThingFilter_IsNot@@VCThingFilter_CombatIsStrikeToStrike@?A0xad05d24c@@VCThingFilter_CombatOverlappingSphere@@
//   VCThingFilter_CombatCanCollideOrInteractWithThing@3@@@@CThingSearchTools@@QBEXABV...@AAV?$list@...@@Z -- public const member
//   template of CThingSearchTools (the parked CThingSearchToolsMethods / CResultList / non-const spelling was wrong). The two
//   anonymous-namespace filters mangle with this TU's own ?A0x hash (retail ?A0xad05d24c), so the Leaf is a prefix, as for the
//   landed out-of-line filter 0x0085B160.
// Callees: CThingSearchTools::PeekClassList() const 0x0049C770 (landed, Ego_r IBE), CThingFilter_IsNot::operator() 0x006A76A0,
//   CThingFilter_CombatIsStrikeToStrike::operator() 0x008565E0, the const interface-map LowerBound 0x004365B0 (same-TU stand-in
//   body as in the landed 0x0085B160), CTCHitLocations::GetBoundingSphere(C3DVector&, float&, bool) const 0x00815420 (Ego_r),
//   CThingFilter_CombatCanCollideOrInteractWithThing::operator() 0x00857250, malloc 0x00BFEA0E (the 12-byte list node).
// Shape: the landed list-result sibling GetAllThings<CThingObject, list<CThing*>, CThingFilter_HasDefName> 0x008AC660 (PeekClassList
//   walked with STLport const_iterators, IsIncludedInThingScans, IsThingOfType mask copy, push_back = insert(end())) with the
//   landed 0x0085B160 filter model inlined (__forceinline Bind4 operator(), the out-parameter GetInterface and references to
//   GetBoundingSphere load the component into ecx before the argument pushes, as retail). CThingFilter_CombatOverlappingSphere is
//   NOT in the anonymous namespace (the PDB name has no ?A0x on it).
// Layout: real CClass prefix (engine/CClass.h); CThing interface bits +0x20, interface map +0x44, type flags +0x6c, state byte
//   +0x91 are retail offsets (the shared CThing header is the Ego_r layout, 0x10 off from +0x6c on); CThingSearchTools is the
//   shared header layout renamed under a #define so the class can declare its member templates (header declares none).
#include "fable_compiler.h"
#include <new>
#include <stdlib.h>
#include "engine/CClass.h"
#define CThingSearchTools CThingSearchTools_HeaderLayout
#include "engine/CThingSearchTools.h"
#undef CThingSearchTools
class CTCBase;
struct CTCInterfaceEntry {
    long     type;
    CTCBase* component;
};
class CTCInterfaceMap {
public:
    CTCInterfaceEntry* first;
    CTCInterfaceEntry* last;
    CTCInterfaceEntry* end;
    CTCInterfaceEntry* LowerBound(const long& type) const;   // 0x004365B0
};
#pragma auto_inline(off)
__declspec(noinline) CTCInterfaceEntry* CTCInterfaceMap::LowerBound(const long& type) const
{
    CTCInterfaceEntry* lo = first;
    long count = (long)(last - first);
    while (count > 0) {
        long half = count / 2;
        CTCInterfaceEntry* mid = lo + half;
        if (mid->type < type) {
            lo = mid + 1;
            count -= half + 1;
        } else {
            count = half;
        }
    }
    return lo;
}
#pragma auto_inline(on)
class CThing : public CClass {
public:
    unsigned char   Pad11[0x20 - 0x11];
    unsigned long   m_tcPresent[9];                     // +0x20
    CTCInterfaceMap m_tcs;                              // +0x44
    unsigned char   Pad50[0x6c - 0x50];
    unsigned long   ThingTypeFlags;                     // +0x6c
    unsigned char   Pad70[0x21];
    unsigned char   StateBits;                          // +0x91
    bool IsIncludedInThingScans() const { unsigned char f = StateBits; unsigned char nf = ~f; if ((f & 4) && (f & 8) && (nf & 1) && !(f & 0x20)) return true; return false; }
    FABLE_FORCEINLINE unsigned long HasTC(long type) const { return m_tcPresent[type >> 5] & (1UL << (type & 31)); }
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long type) const
    {
        if (HasTC(type)) {
            long key = type;
            const CTCInterfaceMap* map = &m_tcs;
            CTCInterfaceEntry* it = map->LowerBound(key);
            *ppOut = (T*)((it == map->last || type < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
class CThingPhysical : public CThing { public: enum { ThingTypeMask = 3 }; };
class CThingGameObject : public CThingPhysical { public: enum { ThingTypeMask = 7 }; };
#include "engine/C3DVector.h"
class CTCHitLocations {
public:
    void GetBoundingSphere(C3DVector& centre, float& radius, bool flag) const;   // 0x00815420
};
class CThingFilter_IsNot {
public:
    CThing* Thing;
    bool operator()(const CThing& thing) const;         // 0x006A76A0
};
namespace {
class CThingFilter_CombatIsStrikeToStrike {
public:
    CThing* ThingStriking;
    bool operator()(const CThing& thing) const;         // 0x008565E0
};
class CThingFilter_CombatCanCollideOrInteractWithThing {
public:
    class CTCPhysicsBase* TCPhysics;
    bool ExcludeCreatures;
    bool ExcludeDeadCreatures;
    bool operator()(const CThing& thing) const;         // 0x00857250
};
}
class CThingFilter_CombatOverlappingSphere {
public:
    C3DVector SpherePos;
    float     Radius;
    FABLE_FORCEINLINE bool operator()(const CThing& thing) const
    {
        const CTCHitLocations* hitLocations;
        if (thing.GetInterface(&hitLocations, 0x7f)) {
            C3DVector centre;
            float radius;
            hitLocations->GetBoundingSphere(centre, radius, true);
            float dx = centre.X - SpherePos.X;
            float dy = centre.Y - SpherePos.Y;
            float dz = centre.Z - SpherePos.Z;
            float r = radius + Radius;
            if (dx * dx + dy * dy + dz * dz < r * r)
                return true;
        }
        return false;
    }
};
template <class F1, class F2, class F3, class F4> class CFilter_Bind4 {
public:
    const F1* PF1;
    const F2* PF2;
    const F3* PF3;
    const F4* PF4;
    template <class T> FABLE_FORCEINLINE bool operator()(const T& thing) const
    {
        return (*PF1)(thing) && (*PF2)(thing) && (*PF3)(thing) && (*PF4)(thing);
    }
};
namespace std {
template<class T>class allocator{};
struct _List_node_base {_List_node_base* _M_next;_List_node_base* _M_prev;};template<class T>struct _List_node:public _List_node_base {T _M_data;};
template<class T>struct _Const_traits {typedef const T& reference;};template<class T>struct _Nonconst_traits {typedef T& reference;};
template<class T,class Traits>struct _List_iterator {typedef typename Traits::reference reference;_List_node_base* _M_node;_List_iterator(_List_node_base* p):_M_node(p){} _List_iterator(const _List_iterator& p):_M_node(p._M_node){}reference operator*()const{return ((_List_node<T>*)_M_node)->_M_data;} _List_iterator& operator++(){_M_node=_M_node->_M_next;return *this;}bool operator!=(const _List_iterator& p)const{return _M_node!=p._M_node;}};
template<class T,class A=allocator<T> >class list {public:_List_node_base* _M_node;typedef _List_iterator<T,_Nonconst_traits<T> > iterator;typedef _List_iterator<T,_Const_traits<T> > const_iterator;
iterator end(){return iterator(_M_node);}const_iterator begin()const{return const_iterator(_M_node->_M_next);}const_iterator end()const{return const_iterator(_M_node);}
_List_node<T>* _M_create_node(const T& value){_List_node<T>* p=(_List_node<T>*)malloc(sizeof(_List_node<T>));new(&p->_M_data)T(value);return p;}
iterator insert(iterator position,const T& value){_List_node<T>* tmp=_M_create_node(value);_List_node_base* n=position._M_node;_List_node_base* prev=n->_M_prev;tmp->_M_next=n;tmp->_M_prev=prev;prev->_M_next=tmp;n->_M_prev=tmp;return iterator(tmp);}
void push_back(const T& value){insert(end(),value);}
};
}
template<class T>inline bool IsThingOfType(const CThing* thing){unsigned long mask=T::ThingTypeMask;return (thing->ThingTypeFlags&mask)==mask;}
class CThingSearchTools:public CThingSearchTools_HeaderLayout {protected:const std::list<CClass*>& PeekClassList()const;public:template<class TThing,class TContainer,class TFilter>void GetAllThings(const TFilter&,TContainer&)const;};
template<class TThing,class TContainer,class TFilter>void CThingSearchTools::GetAllThings(const TFilter& filter,TContainer& result)const{const std::list<CClass*>& things=PeekClassList();for(std::list<CClass*>::const_iterator it=things.begin();it!=things.end();++it){CThing* thing=(CThing*)*it;if(thing->IsIncludedInThingScans() && IsThingOfType<TThing>(thing) && filter(*(TThing*)thing))result.push_back((TThing*)thing);}}
typedef CFilter_Bind4<CThingFilter_IsNot, CThingFilter_CombatIsStrikeToStrike, CThingFilter_CombatOverlappingSphere, CThingFilter_CombatCanCollideOrInteractWithThing> CStrikeFilter;
template void CThingSearchTools::GetAllThings<CThingGameObject, std::list<CThingGameObject*>, CStrikeFilter>(const CStrikeFilter&, std::list<CThingGameObject*>&) const;