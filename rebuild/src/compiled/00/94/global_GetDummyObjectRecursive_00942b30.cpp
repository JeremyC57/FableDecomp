// GetDummyObjectRecursive(const CThing* thing, const CCharString& name, long index, unsigned long flags, C3DVector& pos, CRightHandedSet& set) -- find a named dummy object on the thing's graphic appearance, else recursively on every live thing it owns (CTCThingOwner); true when found (pos/set filled by GetDummyObject)
// Leaf: ?GetDummyObjectRecursive@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00942B30, 223 bytes (224 with padding), __fastcall (thing in ecx, name in edx; index,
//   flags, pos, set on the stack, `ret 10h`), returns bool.
// Identity: Ego_r.pdb ?GetDummyObjectRecursive@@YI_NPBVCThing@@ABVCCharString@@JKAAVC3DVector@@AAVCRightHandedSet@@@Z (a free
//   function); callers: itself (0x00942BE9) and 0x009433F2. The catalog label is right.
// Callees: NGraphicAppearance::CTCGraphicAppearance::GetDummyObject(const CCharString&, long, unsigned long, C3DVector&,
//   CRightHandedSet&, float) const 0x004C08C0 (scale 1.0f), CVectorMap::LowerBound const 0x004365B0 (inlined GetInterface,
//   interface 0x21 = CTCThingOwner, bit 1 of +0x24), CBaseIntelligentPointer::GetPItem 0x00A01B50 (three times per object).
// Layout (retail): CThing PTCGraphicAppearance +0x64, the interface bitset +0x20 / vector map +0x44, Dead = bit 0 of +0x91;
//   CTCThingOwner::ObjectList (STLport list<CIntelligentPointer<CThing> >, one sentinel pointer) at +0x0C, the node data at +8
//   (landed CTCTavern::GetFreeSeat 0x007BABE0).
// Shape: `if (appearance && appearance->GetDummyObject(...)) return true;`, then the out-parameter GetInterface and a
//   const_iterator walk with IS_THING_ALIVE && the recursive call. The lookup key and the owner share ONE frame word -- the
//   dead `set` argument slot (set lives in ebx) -- and the owner is re-read from it each pass; VC7.1 packs them only when
//   the const LowerBound body is visible in the TU (same-TU auto_inline(off) stand-in), otherwise the owner gets its own
//   `push ecx` slot.
class CTCBase;
class CThing;
class CCharString;
class C3DVector;
class CRightHandedSet;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const; // 0x004365B0
};
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound const (STLport lower_bound over the 8-byte {type, component} entries): compiled
// above the caller so VC7.1 knows the key's address does not escape and packs the key and the owner into one frame word
// (the landed 0x007BABE0 / 0x00848730 idiom).
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)

namespace NGraphicAppearance {
class CTCGraphicAppearance {
public:
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector& pos, CRightHandedSet& set,
                        float scale) const;     // 0x004C08C0
};
}

class CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long UsingComponents[9];           // +0x20 bitset<274>
    CInterfaceMap Components;                   // +0x44
    unsigned char _pad4c[0x64 - 0x4c];
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;   // +0x64
    unsigned char _pad68[0x91 - 0x68];
    unsigned char Dead : 1;                     // +0x91 bit 0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {

public:
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
public:
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};

inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->Dead)
            return true;
    }
    return false;
}

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                // +0x08
};
template <class _Tp> struct _List_const_iterator {
    const _List_node_base* _M_node;
    _List_const_iterator(const _List_node_base* __x) : _M_node(__x) { }
    const _Tp& operator*() const { return ((const _List_node<_Tp>*)_M_node)->_M_data; }
    const _Tp* operator->() const { return &((const _List_node<_Tp>*)_M_node)->_M_data; }
    _List_const_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_const_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp> class list {               // STLport list: one sentinel pointer
public:
    typedef _List_const_iterator<_Tp> const_iterator;
    _List_node_base* _M_node;
    const_iterator begin() const { return const_iterator(_M_node->_M_next); }
    const_iterator end() const { return const_iterator(_M_node); }
};

class CTCThingOwner {                           // interface 0x21
public:
    unsigned char _pad00[0x0c];
    list<CIntelligentPointer<CThing> > ObjectList;   // +0x0C
};

bool __fastcall GetDummyObjectRecursive(const CThing* thing, const CCharString& name, long index, unsigned long flags,
                                        C3DVector& pos, CRightHandedSet& set)
{
    if (thing->PTCGraphicAppearance && thing->PTCGraphicAppearance->GetDummyObject(name, index, flags, pos, set, 1.0f))
        return true;
    const CTCThingOwner* owner;
    if (thing->GetInterface(&owner, 0x21)) {
        for (list<CIntelligentPointer<CThing> >::const_iterator it = owner->ObjectList.begin(); it != owner->ObjectList.end(); ++it) {
            if (IS_THING_ALIVE(*it) && GetDummyObjectRecursive(it->GetPItem(), name, index, flags, pos, set))
                return true;
        }
    }
    return false;
}