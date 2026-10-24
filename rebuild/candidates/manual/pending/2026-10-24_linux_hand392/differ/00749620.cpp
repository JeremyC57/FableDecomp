// std::vector<pair<CIntelligentPointer<const CThing>,NStealth::CDangerStatus> >::push_back(const pair<CIntelligentPointer<const CThing>,NStealth::CDangerStatus>& x) -- copy-construct a danger-thing entry at the end, or grow through _M_insert_overflow
// Grade: DIFFER(98v105) -- register allocation only: retail keeps `this` in ebx and x in edi (late `push edi` in the construct arm), loads end-of-storage into eax before finish (`mov eax,[ebx+8] ; mov esi,[ebx+4] ; cmp esi,eax`) and bumps finish as load/add/store interleaved with the pops; ours puts `this` in edi and x in ebx, `cmp esi,[edi+8]`, `add dword ptr [edi+4],10h`; tried plain vector members, the STLport _Vector_base + _STLP_alloc_proxy end-of-storage, implicit pair copy ctor, direct placement new, reversed compare, base-ctor init list
// Leaf: ?push_back@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00749620, 105 bytes, __thiscall `ret 4`.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, ::GetPItem @ 0x00A01B50, ::SetPItem @ 0x00A01B90
//   (the inlined CIntelligentPointer<const CThing> copy constructor; vtable 0x01237F1C, RTTI
//   `.?AV?$CIntelligentPointer@$$CBVCThing@@@@`), _M_insert_overflow(finish, x, __false_type, 1, true) @ 0x00749420.
// Caller: CTCStealth::SetAsInDanger (0x00747B80; CTCStealth::DangerThings at +0x38, Ego_r.pdb
//   `vector<pair<CIntelligentPointer<const CThing>, NStealth::CDangerStatus> >`).
// Label: the manifest's "CTargetPoint_Vector_PushBack" is wrong about the element: it is the 16-byte danger-thing pair.
// Shape: STLport `if (_M_finish != _M_end_of_storage) { _Construct(_M_finish, x); ++_M_finish; } else
//   _M_insert_overflow(_M_finish, x, __false_type(), 1UL, true);` -- the empty __false_type is homed in the dead x slot.
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

class CThing;

class CBaseIntelligentPointer {
public:
    void* PData;                                         // +0x04 (CBaseObjectPointer*)
    virtual void v00();
    CBaseIntelligentPointer();                           // 0x00A01B10
    void* GetPItem() const;                              // 0x00A01B50
    void SetPItem(const void* item);                     // 0x00A01B90
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
    T* Get() const { return (T*)GetPItem(); }
    virtual void v00();                                  // own vtable (CIntelligentPointer<const CThing> 0x01237F1C)
};

namespace NStealth {
enum EDangerStatus { DANGER_STATUS_NONE = 0 };
class CDangerStatus {                                    // Ego_r.pdb 8 bytes
public:
    EDangerStatus DangerStatus;                          // +0x00
    long          Timer;                                 // +0x04
};
}

namespace _STL {
template <class A, class B> struct pair {
    A first;                                             // +0x00
    B second;                                            // +0x08
    pair(const pair& other) : first(other.first), second(other.second) {}
};
}

typedef _STL::pair<CIntelligentPointer<const CThing>, NStealth::CDangerStatus> TDangerPair;   // 16 bytes

namespace _STL {
struct __false_type { __false_type() {} };

template <class T1, class T2>
inline void _Construct(T1* p, const T2& value)
{
    new (p) T1(value);
}

template <class T> class allocator { };

template <class Value, class T, class MaybeReboundAlloc>
class _STLP_alloc_proxy : public MaybeReboundAlloc {
public:
    Value _M_data;
};

template <class T, class Alloc> class _Vector_base {
public:
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<T*, T, Alloc> _M_end_of_storage;   // +0x08
};

template <class T, class Alloc = allocator<T> > class vector : protected _Vector_base<T, Alloc> {
public:
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend);   // 0x00749420
    void push_back(const T& x);
};

template <class T, class Alloc>
void vector<T, Alloc>::push_back(const T& x)
{
    if (this->_M_finish != this->_M_end_of_storage._M_data) {
        _Construct(this->_M_finish, x);
        ++this->_M_finish;
    }
    else
        _M_insert_overflow(this->_M_finish, x, __false_type(), 1UL, true);
}

template void vector<TDangerPair>::push_back(const TDangerPair&);
}
