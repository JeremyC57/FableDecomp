// CAIStateGroup_WanderAroundShops::FindPrettyThingInShop(CThing* shop) -- collect the live stock-item things the shop owns and return one picked with the world's random seed (null for a dead shop or an empty pick)
// Grade: DIFFER(356v354) -- residue: the pushed item temporary takes a frame slot of its own ([esp+10h], frame 0x18 v 0x14) where retail homes it in the dead `shop` argument slot after the GetTC key, and the PDB `bool CThing::HasTC` gives `shr ecx,14h ; test cl,1` for TC 0x54 where retail tests the dword (`test dword ptr [eax+28h],100000h`; the 2026-10-21 model's `unsigned long` HasTC gave that form) | tried: 2026-10-21 out-param GetInterface, named GetTC owner, named item local, two overflow stand-ins; 2026-10-28 real-name rewrite (GetTC<CTCThingOwner>(T**) with the key as a temporary: the key now shares the `shop` slot as in retail, CVectorMap, STLport list ObjectList, GFROR13 returning long), ThingCast<CThing> by-value push, HasTC then GetTC, early-return guard (365), `?:` / implicit-conversion HasTC bodies, split alive/HasTC tests, guarded GetTC statement | next: a CThing::HasTC overload or bitset::test form that keeps the dword test, then the item temporary slot (VC7.1 packs the pushed temporary into the argument slot only when no condition temporary of the enclosing if is still in scope)
// Leaf: ?FindPrettyThingInShop@CAIStateGroup_WanderAroundShops@@AAEPAVCThing@@PAV2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00947110, length 354 bytes, `ret 4`. Ego_r.pdb name (private).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (inline CThing::GetTC<CTCThingOwner> 0x21, bit 1 of +0x24), CBaseIntelligentPointer::
//   GetPItem 0x00A01B50 (IS_THING_ALIVE twice, HasTC 0x54 = bit 20 of +0x28, the pushed item), vector<CThing*>::
//   _M_insert_overflow 0x00436AC0, CAIStateGroupBase::GetPWorldSeed 0x008FCFD0, GFROR13 0x00497890 (Ego_r `?GFROR13@@YIJK@Z`, returns long; GFRandom `?GFRandom@@YIKKPAK@Z` inline),
//   free 0x00BFEA14 (the vector's storage).
// Shape: a same-TU noinline _M_insert_overflow stand-in that never reads the __true_type tag (the landed 0x0062E840 device for
//   the real retail callee 0x00436AC0).
// Layout: Ego_r.pdb CTCThingOwner (ObjectList, an STLport list<CIntelligentPointer<CThing> > header pointer, at +0x0C);
//   CThing interface bitset +0x20, map +0x44, Dead bit 0 of +0x91. No members of `this` are touched.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

namespace std {
struct __true_type { __true_type() {} };
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector { public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~vector() { if (_M_start) free(_M_start); }
    unsigned long size() const { return _M_finish - _M_start; }
    bool empty() const { return _M_start == _M_finish; }
    T& operator[](unsigned long n) { return _M_start[n]; }
protected:
    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned int fill_len, bool atend);
public:
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
}

class CTCBase;
enum ETCInterfaceType { TC_THING_OWNER = 0x21, TC_PRETTY = 0x54 };
namespace std {
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
}
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class C = CKeyPairCompareLess<K, V> >
class CVectorMap : public std::vector<std::pair<K, V> > {
public:
    typedef std::pair<K, V> value_type;
    typedef value_type* iterator;
    iterator LowerBound(const K& key);                     // 0x0040F020
    iterator end() { return this->_M_finish; }
};
class CThing { public:
    virtual ~CThing();
    unsigned char _pad_0x04[0x20 - 0x04];
    unsigned long UsingComponents[9];                      // +0x20
    CVectorMap<ETCInterfaceType, CTCBase*> Components;     // +0x44
    unsigned char _pad_0x50[0x91 - 0x50];
    bool Dead : 1;                                         // +0x91 bit 0
    bool IsDead() const { return Dead != 0; }
    bool HasTC(ETCInterfaceType type) const { return (UsingComponents[type >> 5] & (1UL << (type & 31))) != 0; }
    template <class T> __forceinline bool GetTC(T** out)
    {
        if (!(UsingComponents[T::InterfaceType / 32] & (1UL << (T::InterfaceType % 32))))
            return false;
        CVectorMap<ETCInterfaceType, CTCBase*>& components = Components;
        CVectorMap<ETCInterfaceType, CTCBase*>::iterator p = components.LowerBound((ETCInterfaceType)T::InterfaceType);
        if (p == components.end() || (ETCInterfaceType)T::InterfaceType < p->first)
            p = components.end();
        *out = (T*)p->second;
        return true;
    }
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer { public:                 // vtable 0x0129C95C
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
private:
    CBaseObjectPointer* PData;                          // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    T* operator->() const { return GetPItem(); }
};
template <class T> inline bool IS_THING_ALIVE(const T& thing)
{
    if (thing != 0) {
        if (!thing->IsDead())
            return true;
    }
    return false;
}
template <class T> inline bool operator!=(const CIntelligentPointer<T>& p, int) { return p.GetPItem() != 0; }
namespace std {
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class T> struct _List_node : _List_node_base { T _M_data; };
template <class T> class list {                         // STLport: the one header-node pointer
public:
    _List_node_base* _M_node;
};
}
class CTCThingOwner { public:
    enum { InterfaceType = TC_THING_OWNER };
    unsigned char _pad00[0x0c];
    std::list<CIntelligentPointer<CThing> > ObjectList;  // +0x0C
};

// vector<CThing*>::_M_insert_overflow @ 0x00436AC0, compiled in this TU but kept out of line as in retail.
template <class T, class A> __declspec(noinline) void std::vector<T, A>::_M_insert_overflow(T* position, const T& x, const __true_type&, unsigned int fill_len, bool atend)
{
    if (atend && fill_len == 1) {
        _M_start = position;
        _M_finish = position + (x != 0);
    }
}

long __fastcall GFROR13(unsigned long value);            // 0x00497890
inline unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return range ? *seed % range : 0;
}

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
protected:
    unsigned long* GetPWorldSeed() const;               // 0x008FCFD0
};
class CAIStateGroup_WanderAroundShops : public CAIStateGroupBase {
    CThing* FindPrettyThingInShop(CThing* shop);
};

CThing* CAIStateGroup_WanderAroundShops::FindPrettyThingInShop(CThing* shop)
{
    CTCThingOwner* owner;
    if (IS_THING_ALIVE(shop) && shop->GetTC(&owner)) {
        std::vector<CThing*> pretty_things;
        for (std::_List_node_base* node = owner->ObjectList._M_node->_M_next; node != owner->ObjectList._M_node; node = node->_M_next) {
            CIntelligentPointer<CThing>& item = ((std::_List_node<CIntelligentPointer<CThing> >*)node)->_M_data;
            if (IS_THING_ALIVE(item) && item->HasTC(TC_PRETTY))
                pretty_things.push_back(item.GetPItem());
        }
        if (!pretty_things.empty())
            return pretty_things[GFRandom(pretty_things.size(), GetPWorldSeed())];
    }
    return 0;
}
