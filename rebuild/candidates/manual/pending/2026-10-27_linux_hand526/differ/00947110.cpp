// CAIStateGroup_WanderAroundShops::FindPrettyThingInShop(CThing* shop) -- collect the live stock-item things the shop owns and return one picked with the world's random seed (null for a dead shop or an empty pick)
// Grade: DIFFER(354v354) -- frame 0x18 v 0x14: the pushed item temporary takes a slot of its own ([esp+10h]) where retail homes it in the dead `shop` argument slot with the lookup key and the overflow's __true_type tag; every other byte matches (tried: out-param GetInterface, named GetTC owner, a named item local, two overflow stand-ins)
// Leaf: ?FindPrettyThingInShop@CAIStateGroup_WanderAroundShops@@AAEPAVCThing@@PAV2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00947110, length 354 bytes, `ret 4`. Ego_r.pdb name (private).
// Callees: CVectorMap::LowerBound 0x0040F020 (inlined HasTC + GetTC 0x21 = CTCThingOwner, bit 1 of +0x24), CBaseIntelligentPointer::
//   GetPItem 0x00A01B50 (IS_THING_ALIVE twice, HasTC 0x54 = bit 20 of +0x28, the pushed item), vector<CThing*>::
//   _M_insert_overflow 0x00436AC0, CAIStateGroupBase::GetPWorldSeed 0x008FCFD0, GFROR13 0x00497890 (GFRandom inline),
//   free 0x00BFEA14 (the vector's storage).
// Shape: the pushed item temporary and the overflow's __true_type tag share the dead `shop` argument slot (a same-TU
//   noinline _M_insert_overflow stand-in that never reads the tag, as the landed 0x0062E840); the owner is a named
//   GetTC result (an out-parameter lookup keeps it address-taken and gives the item temporary a slot of its own).
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
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char StateBits;                            // +0x91 bit 0: Dead
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
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
};
__forceinline bool IS_THING_ALIVE(const CThing* thing)
{
    if (thing != 0) {
        if (!(thing->StateBits & 1))
            return true;
    }
    return false;
}
__forceinline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->StateBits & 1))
            return true;
    }
    return false;
}
struct CThingPointerNode {                              // STLport list<CIntelligentPointer<CThing> > node
    CThingPointerNode* _M_next;
    CThingPointerNode* _M_prev;
    CIntelligentPointer<CThing> _M_data;
};
class CTCThingOwner { public:                           // interface 0x21
    unsigned char _pad00[0x0c];
    CThingPointerNode* ObjectList;                      // +0x0C list header node
};

// vector<CThing*>::_M_insert_overflow @ 0x00436AC0, compiled in this TU but kept out of line as in retail.
template <class T, class A> __declspec(noinline) void std::vector<T, A>::_M_insert_overflow(T* position, const T& x, const __true_type&, unsigned int fill_len, bool atend)
{
    if (atend && fill_len == 1) {
        _M_start = position;
        _M_finish = position + (x != 0);
    }
}

unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890
inline unsigned long GFRandom(unsigned long range, unsigned long* seed)
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
    if (IS_THING_ALIVE(shop) && shop->HasTC(0x21)) {
        CTCThingOwner* owner = shop->GetTC<CTCThingOwner>(0x21);
        std::vector<CThing*> pretty_things;
        for (CThingPointerNode* node = owner->ObjectList->_M_next; node != owner->ObjectList; node = node->_M_next) {
            if (IS_THING_ALIVE(node->_M_data) && node->_M_data.GetPItem()->HasTC(0x54))
                pretty_things.push_back(node->_M_data.GetPItem());
        }
        if (!pretty_things.empty())
            return pretty_things[GFRandom(pretty_things.size(), GetPWorldSeed())];
    }
    return 0;
}
