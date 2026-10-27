// CThingSearchTools::GetAllThings(const CThingFilter_None& filter, std::vector<const CThing*>& things) const -- appends every searchable thing of the class list (scan-included, activated, not dead, not in limbo) to the vector
// Grade: DIFFER(122v125) -- retail has an extra `xor al,al ; jne skip` (a constant-false test VC7.1 left unfolded) after the InLimbo test, and homes the pushed value in the dead `things` argument slot ([esp+14h]) where ours opens a `push ecx` frame slot; flag tests, list walk and push_back otherwise match
// Leaf: ??$GetAllThings@$$CBVCThing@@V?$vector@PBVCThing@@V?$allocator@PBVCThing@@@std@@@std@@VCThingFilter_None@@@CThingSearchTools@@QBEXABVCThingFilter_None@@AAV?$vector@PBVCThing@@V?$allocator@PBVCThing@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004AD810, 125 bytes, thiscall `ret 8`.
// Identity: Ego_r.pdb ??$GetAllThings@$$CBVCThing@@V?$vector@PBVCThing@@...@VCThingFilter_None@@@CThingSearchTools@@QBEX...@Z:
//   the body walks CThingSearchTools::PeekClassList() const (0x0049C770, protected; list<CClass*>), reads no filter state
//   (the first argument is never touched -- CThingFilter_None) and pushes `const CThing*` into a vector whose growth path
//   is vector<const CThing*>::_M_insert_overflow 0x0049C370 (bound in reloc_bindings with the __true_type tag).
//   Only caller: 0x004A0520 (CWorld code).
// Layout: retail CThing flag byte +0x91 (Ego_r.pdb +0xA1): bit 0 Dead, bit 2 IncludeInThingScans, bit 3 Activated,
//   bit 5 InLimbo (as the landed sources read it). No engine header for CThing.
// Shape: STLport list walk (`node = header->_M_next` until the header, re-read through the list reference each pass);
//   the four flag tests in source order; the filter call is an inline `return true` whose negation VC7.1 leaves as
//   `xor al,al ; jne`; STLport 4.6 `push_back` (placement-new null test, `++_M_finish` in memory) with the value homed in
//   the dead `things` argument slot and the empty __true_type tag pointing at the same slot.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)

namespace std {
struct __true_type { __true_type() { } };
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp> struct _List_iterator {
    _List_node<_Tp>* _M_node;
    _List_iterator(_List_node<_Tp>* __x) : _M_node(__x) { }
    _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) { }
    const _Tp& operator*() const { return _M_node->_M_data; }
    _List_iterator& operator++() { _M_node = (_List_node<_Tp>*)_M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp, class _Alloc>
class _List_base {
public:
    _STLP_alloc_proxy<_List_node<_Tp>*, _List_node<_Tp>, allocator<_List_node<_Tp> > > _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_iterator<_Tp> const_iterator;
    const_iterator begin() const { return const_iterator((_List_node<_Tp>*)this->_M_node._M_data->_M_next); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
};

template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    __forceinline void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __true_type(), 1, true);
    }
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, unsigned int __fill_len, bool __atend);   // 0x0049C370
};
} // namespace std

class CClass {
public:
    virtual ~CClass();
};

class CThing : public CClass {
public:
    unsigned char _pad004[0x91 - 0x4];
    unsigned char Dead : 1;                     // +0x91 bit 0
    unsigned char CreatedByOwnerThingCreation : 1;
    unsigned char IncludeInThingScans : 1;      // bit 2
    unsigned char Activated : 1;                // bit 3
    unsigned char GloballyLoaded : 1;
    unsigned char InLimbo : 1;                  // bit 5
    unsigned char KillLocked : 1;
    unsigned char CreatedBySerialisation : 1;
    bool IsAlive() const { return !Dead; }
    bool IsInLimbo() const { return InLimbo; }
};

class CThingFilter_None {
public:
    bool operator()(const CThing&) const { return true; }
};

class CThingSearchTools {
public:
    template <class T, class TContainer, class TFilter>
    void GetAllThings(const TFilter& filter, TContainer& things) const
    {
        const std::list<CClass*>& classList = PeekClassList();
        for (std::list<CClass*>::const_iterator it = classList.begin(); it != classList.end(); ++it) {
            T* thing = static_cast<T*>(*it);
            if (!thing->IncludeInThingScans || !thing->Activated || !thing->IsAlive() || thing->IsInLimbo())
                continue;
            if (!filter(*thing))
                continue;
            {
                T* value = thing;
                things.push_back(value);
            }
        }
    }
protected:
    const std::list<CClass*>& PeekClassList() const;   // 0x0049C770
};

template void CThingSearchTools::GetAllThings<const CThing, std::vector<const CThing*>, CThingFilter_None>(
    const CThingFilter_None&, std::vector<const CThing*>&) const;
