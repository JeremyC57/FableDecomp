// AddToRelevantBuilding(CThingAICreature* creature, std::map<CThingBuilding*, std::list<CThingAICreature*> >& occupants, std::vector<CThingBuilding*>& buildings, const CDefinitionManager* defs, CWorld& world) -- region population: give a villager a home: the first multi-family house nobody lives in yet, else the free building with the most room, where an empty one-family house takes a man (or a creature of type 1, or a coin flip) and a mixed household takes whichever sex it is short of; the creature's HomeBuilding is set, it joins the building's occupant list, and a now-full building leaves the candidate list
// Leaf: ?AddToRelevantBuilding@@YIXPAVCThingAICreature@@AAV?$map@PAVCThingBuilding@@V?$list@PAVCThingAICreature@@V?$allocator@PAVCThingAICreature@@@std@@@std@@U?$less@PAVCThingBuilding@@@3@V?$allocator@U?$pair@QAVCThingBuilding@@V?$list@PAVCThingAICreature@@V?$allocator@PAVCThingAICreature@@@std@@@std@@@std@@@3@@std@@AAV?$vector@PAVCThingBuilding@@V?$allocator@PAVCThingBuilding@@@std@@@3@PBVCDefinitionManager@@AAVCWorld@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00502B00, 906 bytes, `ret 0Ch` (`__fastcall`: creature in ecx, the occupant map in edx,
//   the building vector, the (unused) definition manager and the world on the stack).
// Identity (proven): Ego_r.pdb / FableWin.pdb name set (the only PDB function with this argument list: the creature, the
//   building->occupants map, the building vector, a const CDefinitionManager* and a CWorld&); the home-building twin of the
//   landed AddToWorkBuildingList 0x005003E0 (LiveSpaces +0x44, the creature's HomeBuilding +0x168).
// Callees: CWorld::GetWorldSeed() const 0x0049D810 and GFROR13 0x00497890 (the inline GFRandom(2, seed)),
//   CThingBuilding::PeekPSimDefinition() const 0x0082D2D0 (manifest "PeekPMaterial" is propagated), STLport
//   map<CThingBuilding*, list<CThingAICreature*> >::operator[](CThingBuilding* const&) 0x0051BF90 (manifest
//   "std::map::insert"), CThingCreatureBase::GetCreatureType() const 0x00662000 and GetSex() const 0x00661FF0 (both
//   [+0xE0 def] reads: CThingCreatureDef Sex +0xC0 / CreatureType +0xC4, Ego_r adjacent pair), CBaseIntelligentPointer
//   ctor 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10 (CIntelligentPointer<CThingBuilding>
//   vtable 0x01244344, RTTI), malloc 0x00BFEA0E (the list node), memmove 0x00BFEAE6 (vector::erase).
// Shape: GFRandom(2, seed) inline (the coin rides in the dead world slot); the def pointer's release zeroes the pointer
//   (the store survives only where the loop passes the slot again); the two sex-balance tests are one `||` condition (one
//   shared break block); the assignment tail sits at function scope after `if (chosen == 0) return;` -- inside an
//   `if` block VC7.1 packs its def pointer into the dead `buildings` argument slot (frame 0x1C) instead of retail's own
//   word (+0x24, frame 0x20); vector::erase is STLport's `__copy_trivial` (memmove) with `end() != position + 1`.
#include <stddef.h>

extern "C" void* __cdecl malloc(unsigned int size);                 // 0x00BFEA0E
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int n);   // 0x00BFEAE6
inline void* operator new(unsigned int, void* p) { return p; }

long __fastcall GFROR13(unsigned long value);                       // 0x00497890
__forceinline unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed)   // 0x004CDAE0 (inline here)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return range ? *seed % range : 0;
}

class CDefinitionManager;
#define CWorld CWorldLayout
#include "engine/CWorld.h"
#undef CWorld
class CWorld {
public:
    unsigned long* GetWorldSeed() const;                            // 0x0049D810
};

struct CRefCounted {
    long Count;
    void DecRef() { --Count; }
    long GetRefCount() const { return Count; }
};

class CSimBuildingDef {                             // Ego_r.pdb names, retail offsets
public:
    virtual void v00();
    virtual void Destroy() const;                   // slot 1 (+0x04)
    mutable CRefCounted Ref;                        // +0x04
    unsigned char _pad08[0x3c - 0x08];
    long Property;                                  // +0x3C
    long WorkSpaces;                                // +0x40
    long LiveSpaces;                                // +0x44
    __forceinline void ReleaseTemporary() const { Ref.DecRef(); if (Ref.GetRefCount() == 0) Destroy(); }
};

template <class T> class CDefPointer {
public:
    T* Object;
    __forceinline ~CDefPointer() { if (Object) { Object->ReleaseTemporary(); Object = 0; } }
    T* operator->() const { return Object; }
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                      // 0x00A01B10
    virtual ~CBaseIntelligentPointer();             // 0x00A01C10
    CBaseObject* GetPItem() const;                  // 0x00A01B50
    void SetPItem(CBaseObject* item);               // 0x00A01B90
private:
    CBaseObjectPointer* PData;                      // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CThingBuilding {
public:
    CDefPointer<const CSimBuildingDef> PeekPSimDefinition() const;   // 0x0082D2D0
};

enum ESex { SEX_NONE = 0, SEX_MALE = 1, SEX_FEMALE = 2 };
enum ECreatureType { };

#define CThingCreatureBase CThingCreatureBaseLayout
#include "engine/CThingCreatureBase.h"
#undef CThingCreatureBase
class CThingCreatureBase {
public:
    ESex GetSex() const;                            // 0x00661FF0
    ECreatureType GetCreatureType() const;          // 0x00662000
};

#define CThingAICreature CThingAICreature_Layout
#undef FABLE_ENGINE_CTHINGAICREATURE_H
#include "engine/CThingAICreature.h"
#undef CThingAICreature

class CThingAICreature : public CThingCreatureBase {
public:
    unsigned char _pad00[0x168];
    CIntelligentPointer<CThingBuilding> HomeBuilding;   // +0x168
};
FABLE_STATIC_ASSERT(offsetof(CThingAICreature_Layout, HomeBuilding) == 0x168);

namespace std {

template <class T1, class T2>
inline void _Construct(T1* p, const T2& value)
{
    new (p) T1(value);
}

template <class T> class allocator {
public:
    allocator() { }
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};

template <class T> struct less {
    bool operator()(const T& a, const T& b) const { return a < b; }
};

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};

template <class V, class A> struct _STLP_alloc_proxy : public A {
    V _M_data;
};

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class T> struct _List_node : public _List_node_base {
    T _M_data;
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* x) : _M_node(x) { }
};

template <class T> struct _List_iterator : public _List_iterator_base {
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) { }
    _List_iterator(const _List_iterator& x) : _List_iterator_base(x._M_node) { }
    T& operator*() const { return ((_List_node<T>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& x) const { return _M_node != x._M_node; }
};

template <class T, class A = allocator<T> > class list {
public:
    typedef _List_node<T> _Node;
    typedef _List_iterator<T> iterator;
    _STLP_alloc_proxy<_Node*, allocator<_Node> > _M_node;
    iterator begin() const { return iterator(_M_node._M_data->_M_next); }
    iterator end() const { return iterator(_M_node._M_data); }
    unsigned int size() const
    {
        unsigned int n = 0;
        for (iterator first = begin(), last = end(); first._M_node != last._M_node; first._M_node = first._M_node->_M_next)
            ++n;
        return n;
    }
    _Node* _M_create_node(const T& x)
    {
        _Node* p = _M_node.allocate(1);
        _Construct(&p->_M_data, x);
        return p;
    }
    iterator insert(iterator position, const T& x)
    {
        _Node* tmp = _M_create_node(x);
        _List_node_base* n = position._M_node;
        _List_node_base* p = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = p;
        p->_M_next = tmp;
        n->_M_prev = tmp;
        return tmp;
    }
    void push_back(const T& x) { insert(end(), x); }
};

template <class K, class T, class Compare = less<K>, class A = allocator<pair<const K, T> > > class map {
public:
    void* _M_header;
    unsigned int _M_node_count;
    Compare _M_key_compare;
    T& operator[](const K& k);                      // 0x0051BF90
};

inline void* __copy_trivial(const void* first, const void* last, void* result)
{
    memmove(result, first, (const char*)last - (const char*)first);
    return (char*)result + ((const char*)last - (const char*)first);
}
template <class T, class A = allocator<T> > class vector {
public:
    typedef T* iterator;
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    iterator erase(iterator position)
    {
        if (end() != position + 1)
            __copy_trivial(position + 1, _M_finish, position);
        --_M_finish;
        return position;
    }
};

}

void __fastcall AddToRelevantBuilding(CThingAICreature* creature,
    std::map<CThingBuilding*, std::list<CThingAICreature*> >& occupants, std::vector<CThingBuilding*>& buildings,
    const CDefinitionManager* defs, CWorld& world)
{
    std::vector<CThingBuilding*>::iterator chosen = 0;
    long mostFree = 0;
    bool coin = GFRandom(2, world.GetWorldSeed()) == 0;
    std::vector<CThingBuilding*>::iterator it;
    for (it = buildings.begin(); it != buildings.end(); ++it) {
        CDefPointer<const CSimBuildingDef> def = (*it)->PeekPSimDefinition();
        if (def->LiveSpaces > 1 && occupants[*it].size() == 0) {
            chosen = it;
            break;
        }
    }
    if (chosen == 0) {
        for (it = buildings.begin(); it != buildings.end(); ++it) {
            CDefPointer<const CSimBuildingDef> def = (*it)->PeekPSimDefinition();
            long spaces = def->LiveSpaces - occupants[*it].size();
            if (spaces > 0) {
                if (occupants[*it].size() == 0) {
                    if (def->LiveSpaces > 1 || creature->GetCreatureType() == 1 || coin) {
                        chosen = it;
                        break;
                    }
                }
                long males = 0;
                long females = 0;
                for (std::list<CThingAICreature*>::iterator c = occupants[*it].begin(); c != occupants[*it].end(); ++c) {
                    if ((*c)->GetSex() == SEX_MALE)
                        ++males;
                    else if ((*c)->GetSex() == SEX_FEMALE)
                        ++females;
                }
                if ((females < males && creature->GetSex() == SEX_FEMALE) || (females > males && creature->GetSex() == SEX_MALE)) {
                    chosen = it;
                    break;
                }
                if (spaces > mostFree) {
                    mostFree = spaces;
                    chosen = it;
                }
            }
        }
    }
    if (chosen == 0)
        return;
    creature->HomeBuilding = CIntelligentPointer<CThingBuilding>(*chosen);
    occupants[*chosen].push_back(creature);
    CDefPointer<const CSimBuildingDef> def = (*chosen)->PeekPSimDefinition();
    if (occupants[*chosen].size() == def->LiveSpaces)
        buildings.erase(chosen);
}