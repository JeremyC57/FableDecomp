// std::vector::vector(const vector& x) -- (vector<CCountedPointer<CCombatSequenceBase> >) STLport copy constructor: allocate x.size() elements (none for an empty source), set start/finish/end of storage, copy-construct the elements through the out-of-line __uninitialized_copy
// Leaf: ??0?$vector@V?$CCountedPointer@VCCombatSequenceBase@@@@V?$allocator@V?$CCountedPointer@VCCombatSequenceBase@@@@@std@@@std@@QAE@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006F19F0, 87 bytes (+ int3 padding to 96), thiscall, `ret 4` (&x), returns this.
// Identity (proven): std::vector<CCountedPointer<CCombatSequenceBase> >::vector(const vector&); evidence: 0x006F3E50 is the CAttackingCombatantInfo copy constructor (it copies Ego_r.pdb's Attacked, AttackerNumber, CombatWheelRingIndex, PreferredAttackDollySet, PreferredAttackDollyTimer, PreferredAttackDollyVector, UsingCombatWheel and CurrentSequence word by word at +0..+0x20, then hands +0x24 -- CombatSequences, vector<CCountedPointer<CCombatSequenceBase> > -- to this constructor), and the CCombatantInfo copy constructor 0x006F4990 does the same for its embedded AttackerInfo (+4, so +0x28); the element copy 0x006EFE80 copies {Data, Info} and bumps Info->RefCount; both PDBs name ??0?$vector@V?$CCountedPointer@VCCombatSequenceBase@@@@...@QAE@ABV01@@Z.
// Label: the manifest's "CCombatantInfo::InitActiveScripts" is propagated (family J011: six copy constructors over 8-byte non-POD elements;
//   retail has no identical-code folding, so each address is one element type).
// Callees: malloc 0x00BFEA0E (the STLport malloc allocator), __uninitialized_copy<const CCountedPointer<CCombatSequenceBase>*, CCountedPointer<CCombatSequenceBase>*>(first,
//   last, result, const __false_type&) 0x006EFE80 (out of line, `__fastcall`; STLport's type traits give every class
//   __false_type, so it copy-constructs element by element).
// Shape: STLport 4.6 `vector(const _Self& __x) : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator()) { _M_finish =
//   __uninitialized_copy((const_pointer)__x._M_start, (const_pointer)__x._M_finish, _M_start, _IsPODType()); }`; the
//   three zero stores share the size compare's zero register and the `__false_type` temporary lands in the dead `x` slot.
// Model: the landed 0x00566CD0 (vector<pair<float, CWideString> >).
extern "C" void* __cdecl malloc(unsigned int size);                       // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                // 0x00BFEA14

class CCombatSequenceBase;
class CCPPointerInfo;                                               // Ego_r.pdb: RefCount, DeleteFunc, Data
template <class T> class CCountedPointer {                        // 8 bytes (Ego_r.pdb)
public:
    T*              Data;                                         // +0x0
    CCPPointerInfo* Info;                                         // +0x4 (RefCount at +0)
};

namespace std {
struct __false_type { __false_type() { } };

template <class _Tp> class allocator {                                    // STLport allocator over the malloc allocator: empty
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {                     // the end-of-storage word over the empty allocator
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(unsigned int __n, const _Alloc& __a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    _Tp* _M_start;                                                        // +0x0
    _Tp* _M_finish;                                                       // +0x4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;               // +0x8
};

template <class _InputIter, class _ForwardIter>                            // 0x006EFE80 for this element
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                             const __false_type&);

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef const _Tp* const_pointer;
    typedef _Alloc allocator_type;
    allocator_type get_allocator() const { return allocator_type(); }
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    vector(const vector& __x) : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const_pointer)__x._M_start, (const_pointer)__x._M_finish, this->_M_start,
                                               __false_type());
    }
};
}

template std::vector<CCountedPointer<CCombatSequenceBase> >::vector(const std::vector<CCountedPointer<CCombatSequenceBase> >&);