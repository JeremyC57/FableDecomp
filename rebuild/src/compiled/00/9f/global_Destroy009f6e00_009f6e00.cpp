// _global::Destroy_009f6e00(CInputEventPackage* first, CInputEventPackage* last, const SAllocatorTag&) -- STLport __destroy_aux<CInputEventPackage*>: run each input-event package's destructor on [first, last) (destroy its event vector's elements, free the storage)
// Leaf: Destroy_009f6e00
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F6E00, length 75 bytes, `ret 4`.
// Callers: the landed CInputManager::~CInputManager 0x009F5C80 (declares exactly this `Destroy_009f6e00(first, last,
//   const SAllocatorTag&)` for its LoadedEventPackages vector) and 0x009F6F88 (vector insert-range 0x009F6ED0).
// Callees: CInputEvent::~CInputEvent @ 0x00A03B30 (an out-of-line empty destructor), free @ 0x00BFEA14.
// Identity: the catalog label `_Uninit_copy<_Vector_const_iterator<..unsigned int..>, ...>` is propagated (`ret 4` with no
//   destination is the STLport destroy helper, GOTCHAS chunk 358). Ego_r.pdb CInputEventPackage: Events
//   vector<CInputEvent> +0x00, TimeStamp +0x0C (retail 12-byte vector), Checksum +0x10, MousePos +0x20, JoystickPos +0x28
//   = 0x30 bytes; CInputEvent is 0x34 bytes. Only Events has a non-trivial destructor.
// Shape: `__fastcall` (first in ecx -> edi, last in edx -> ebp, the tag reference on the stack). Each package's implicit
//   destructor inlines ~vector (`_Destroy(_M_start, _M_finish)`: the element loop calling ~CInputEvent with finish in ebx,
//   start in esi) and ~_Vector_base (`deallocate`: a null-tested free of the re-read start).
extern "C" void __cdecl free(void* block);      // 0x00BFEA14

struct SAllocatorTag { SAllocatorTag() { } };   // STLport __false_type: empty, by const reference (one slot)

struct CInputEvent {                            // 0x34 bytes
    unsigned char _body[0x34];
    ~CInputEvent();                             // 0x00A03B30
};

template <class T> inline void _Destroy(T* p) { p->~T(); }
template <class T> inline void _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        _Destroy(first);
}

template <class T> struct _Vector_base {        // retail 12-byte STLport vector base
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~_Vector_base() { if (_M_start) free(_M_start); }   // _M_end_of_storage.deallocate(_M_start, n)
};
template <class T> struct vector : _Vector_base<T> {
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};

struct CChecksum { unsigned long Words[4]; };   // 16 bytes
#pragma pack(push, 1)
struct C2DVector { float X, Y; };
#pragma pack(pop)

struct CInputEventPackage {                     // 0x30 bytes (retail)
    vector<CInputEvent> Events;                 // +0x00
    long                TimeStamp;              // +0x0C
    CChecksum           Checksum;               // +0x10
    C2DVector           MousePos;               // +0x20
    C2DVector           JoystickPos;            // +0x28
};

void __fastcall Destroy_009f6e00(CInputEventPackage* first, CInputEventPackage* last, const SAllocatorTag&)
{
    for (; first != last; ++first)
        _Destroy(first);
}