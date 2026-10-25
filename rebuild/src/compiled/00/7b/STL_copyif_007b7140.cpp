// _STL::copy_if(list<CIntelligentPointer<CThingMarker> >::const_iterator first, list<CIntelligentPointer<CThingMarker> >::const_iterator last, back_insert_iterator<vector<CIntelligentPointer<CThingMarker> > > dest, IsCrateStackOfType pred) -- append to a vector every marker of a list whose crate-stack def has the wanted stack type
// Leaf: ??$copy_if@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007B7140, 343 bytes (+ int3 padding), `ret 10h` (__fastcall: the hidden
//   back_insert_iterator result pointer in ecx; first, last, dest and the 4-byte predicate on the stack).
// Name: Lionhead's `copy_if` (FableWin.pdb `copy_if<_List_const_iterator<...CIntelligentPointer<CThingMarker>...>,
//   back_insert_iterator<vector<...>>, IsCrateStackOfType>`; the catalog `_Copy_if<...>` is the VC8 Dinkumware helper
//   of the later build). Caller 0x007B6B35.
// Callees: CBaseIntelligentPointer() @ 0x00A01B10, GetPItem @ 0x00A01B50, SetPItem @ 0x00A01B90 (the predicate's
//   by-value CIntelligentPointer<CThingMarker> copy, vtable 0x0125A860, and push_back's in-place copy),
//   CDefClassBase::GetSubDef<CCrateStackDef const>(CDefPointer<...>*) @ 0x0065F7B4 on the marker's PBaseDef (+0x70),
//   ~CBaseIntelligentPointer @ 0x00A01C10 (the predicate argument), vector::_M_insert_overflow @ 0x007B6EF0 (full
//   vector; the `const __false_type&` tag homed in a dead frame word).
// Layout: CThing interface bitset +0x20 (0xB5 = CTCCrateStack: word 5 bit 21, `test [eax+34h],200000h`), interface map
//   +0x44, PBaseDef +0x70; CCrateStackDef::StackType +0x28 (Ego_r.pdb); CDefClassBase RefCount +4, Delete slot 1.
// Shape: `for (; first != last; ++first) if (pred(*first)) *dest++ = *first; return dest;` with the by-value predicate
//   `__forceinline` (VC7.1 otherwise calls it, building the CIntelligentPointer copy as an argument). The predicate looks
//   the CTCCrateStack up (PeekTC: the binary search survives although its result is unused), then fetches the marker
//   pointer in its own statement BEFORE the GetSubDef call (`GetPItem` precedes `push &def` in retail; the one-line
//   `marker->PBaseDef->GetSubDef(&def)` pushes first) and compares the sub-def's StackType. Two same-TU
//   `__declspec(noinline)` STAND-IN bodies (marked below) give VC7.1 the knowledge retail had of the TU's own bodies:
//   GetSubDef writes only its out slot (the def holder then lives in the dead `dest` slot, is not reloaded, and the
//   holder's null test folds away after the StackType load), and _M_insert_overflow never touches its `const
//   __false_type&` tag (homed in the live `first` slot, `lea ecx,[esp+28h]`). Neither stand-in is graded.
extern "C" void* __cdecl malloc(unsigned int size);                     // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) throw() { return where; }

class CTCBase;
struct CBaseObject;

struct CBaseIntelligentPointer {
    void* PData;                                 // +0x04
    CBaseIntelligentPointer();                   // 0x00A01B10
    virtual ~CBaseIntelligentPointer();          // 0x00A01C10
    CBaseObject* GetPItem() const;               // 0x00A01B50
    void SetPItem(CBaseObject* item);            // 0x00A01B90
};

template <class T>
struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x0125A860 for CThingMarker
    __forceinline CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
    __forceinline CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
    T* operator->() const { return (T*)GetPItem(); }
};

template <class T> class CDefPointer;

class CDefClassBase {
public:
    virtual void v00();
    virtual void Delete();                       // slot 1 (+0x04)
    long RefCount;                               // +0x04
    template <class T> bool GetSubDef(CDefPointer<T>* out) const;   // CCrateStackDef: 0x0065F7B4
};

template <class T> class CDefPointer {
public:
    T* Data;
    CDefPointer() : Data(0) { }
    ~CDefPointer()
    {
        if (Data) {
            if (--((CDefClassBase*)Data)->RefCount == 0)
                ((CDefClassBase*)Data)->Delete();
        }
    }
    T* operator->() const { return Data; }
};

// Same-TU stand-in for CDefClassBase::GetSubDef<T> (0x0065F7B4, out of line in retail): it writes only the out slot,
// so VC7.1 knows the def holder's address does not escape (the landed 0x00830F20 / 0x00848730 idiom).
void* FindSubDef_CDefClassBase_GetSubDef(const CDefClassBase* def);   // stand-in callee of the stand-in body
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<T>* out) const
{
    void* def = FindSubDef_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out->Data = (T*)def;
    return true;
}
#pragma auto_inline(on)

class CCrateStackDef : public CDefClassBase {    // Ego_r.pdb, 0x30 bytes
public:
    unsigned char _pad08[0x28 - 0x08];
    long          StackType;                     // +0x28
};

struct STCEntry {
    long     m_kind;
    CTCBase* m_pTC;
};

struct CTCMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*> (STLport vector, 12 bytes)
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    const STCEntry* LowerBound(const unsigned long& kind) const   // 0x004365B0 (inline here)
    {
        const STCEntry* it = m_pFirst;
        int len = int(m_pLast - m_pFirst);
        while (len > 0) {
            int half = len / 2;
            const STCEntry* middle = it + half;
            if (middle->m_kind < (long)kind) {
                it = middle + 1;
                len = len - half - 1;
            } else
                len = half;
        }
        return it;
    }
};

class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                  // +0x20
    unsigned char  _pad40[4];
    CTCMap         m_tcs;                        // +0x44
    unsigned char  _pad50[0x70 - 0x50];
    CDefClassBase* PBaseDef;                     // +0x70 (CDefPointer<const CThingBaseDef>::Data)
    unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    template <class T> __forceinline bool PeekTC(const T** out) const
    {
        if (HasTC(T::INTERFACE_TYPE)) {
            const CTCMap* map = &m_tcs;
            const STCEntry* it = map->LowerBound((unsigned long)T::INTERFACE_TYPE);
            *out = (const T*)((it == map->m_pLast || T::INTERFACE_TYPE < it->m_kind) ? map->m_pLast : it)->m_pTC;
            return true;
        }
        return false;
    }
};

class CThingMarker : public CThing {
};

class CTCCrateStack {
public:
    enum { INTERFACE_TYPE = 0xB5 };
};

class IsCrateStackOfType {
public:
    long Type;                                   // +0x0
    explicit IsCrateStackOfType(long type) : Type(type) { }
    __forceinline bool operator()(CIntelligentPointer<CThingMarker> marker) const
    {
        const CTCCrateStack* stack;
        if (marker->PeekTC(&stack)) {
            CDefPointer<const CCrateStackDef> def;
            const CThingMarker* thing = marker.operator->();
            thing->PBaseDef->GetSubDef(&def);
            return def->StackType == Type;
        }
        return false;
    }
};

namespace _STL {
struct __false_type { __false_type() {} };

template <class _Tp> class allocator {
public:
    allocator() {}
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp value_type;
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend = false);   // 0x007B6EF0
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            new (this->_M_finish) _Tp(__x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};

// Same-TU stand-in for vector::_M_insert_overflow (0x007B6EF0, the TU's own out-of-line body in retail): it never
// touches the `const __false_type&` tag, so VC7.1 may home the tag temporary in the live `first` argument slot.
void InsertOverflow_vector_M_insert_overflow(void* vec, void* position, const void* value, unsigned int count, bool atEnd);   // stand-in callee
#pragma auto_inline(off)
template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend)
{
    InsertOverflow_vector_M_insert_overflow(this, __position, &__x, __fill_len, __atend);
}
#pragma auto_inline(on)

template <class _Container> class back_insert_iterator {
protected:
    _Container* container;
public:
    typedef typename _Container::value_type value_type;
    explicit back_insert_iterator(_Container& __x) : container(&__x) {}
    back_insert_iterator<_Container>& operator=(const value_type& __value)
    {
        container->push_back(__value);
        return *this;
    }
    back_insert_iterator<_Container>& operator*() { return *this; }
    back_insert_iterator<_Container>& operator++() { return *this; }
    back_insert_iterator<_Container> operator++(int) { return *this; }
};

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) {}
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp> struct _Const_traits {
    typedef const _Tp& reference;
};

template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) {}
    _List_iterator(const _List_iterator<_Tp, _Traits>& __x) : _List_iterator_base(__x._M_node) {}
    typename _Traits::reference operator*() const { return ((_Node*)_M_node)->_M_data; }
    _List_iterator<_Tp, _Traits>& operator++() { this->_M_incr(); return *this; }
};

template <class _InputIter, class _OutputIter, class _Predicate>
_OutputIter __fastcall copy_if(_InputIter __first, _InputIter __last, _OutputIter __result, _Predicate __pred)
{
    for ( ; __first != __last; ++__first)
        if (__pred(*__first))
            *__result++ = *__first;
    return __result;
}

typedef CIntelligentPointer<CThingMarker> CMarkerPtr;
template back_insert_iterator<vector<CMarkerPtr> > __fastcall
copy_if(_List_iterator<CMarkerPtr, _Const_traits<CMarkerPtr> >, _List_iterator<CMarkerPtr, _Const_traits<CMarkerPtr> >,
        back_insert_iterator<vector<CMarkerPtr> >, IsCrateStackOfType);
}