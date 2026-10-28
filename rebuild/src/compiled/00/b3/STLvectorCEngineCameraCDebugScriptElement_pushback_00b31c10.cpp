// std::vector::push_back(const CDebugScriptElement& x) -- STLport push_back of the camera's debug-script element vector: placement-copy at _M_finish when there is room, else _M_insert_overflow @ 0x00B31B10 (one element, at the end)
// Leaf: ?push_back@?$vector@UCDebugScriptElement@CEngineCamera@@@std@@QAEXABUCDebugScriptElement@CEngineCamera@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B31C10, length 94 bytes, thiscall `ret 4`.
// Identity: the only caller is the landed CEngineCamera::ConsoleAddDebugScript 0x00B317A0 (call 0x00B31830), pushing one
//   CEngineCamera::CDebugScriptElement onto DebugScript (+0x208); the callee is the landed
//   std::vector<CEngineCamera::CDebugScriptElement>::_M_insert_overflow 0x00B31B10 (whose model this row shares).
//   Descriptive: no PDB symbol exists for this instantiation (the PDB name sets only hold CArray<CDebugScriptElement>'s ctor;
//   whether retail emitted CArray<T>::push_back or vector<T>::push_back is not decidable from the bytes). The catalog name
//   CRetailVector_SRecord32::PushBack and the manifest CScriptModuleTransitionBetweenPoints::OnInitLeaf were a shape name and
//   a propagation.
// Shape: STLport 4.6 `if (_M_finish != _M_end_of_storage._M_data) { _Construct(_M_finish, x); ++_M_finish; } else
//   _M_insert_overflow(_M_finish, x, __false_type(), 1, true);`. The element's C3DVector members have a user copy
//   constructor, so the placement copy is eight dword moves through esi (a POD element would be a `rep movsd`); the tag
//   temporary is homed in this function's dead argument slot (`lea edx,[esp+0Ch]` after the two `push 1`).
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E (allocator, unused here)
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14 (allocator, unused here)
inline void* __cdecl operator new(unsigned int, void* where) throw() { return where; }

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

class CEngineCamera {
public:
    struct CDebugScriptElement {                           // PDB sizeof 0x20
        C3DVector WorldPos;                                 // +0x00
        C3DVector LookVector;                               // +0x0C
        float     FOV;                                      // +0x18
        float     TimeToNextElement;                        // +0x1C
    };
};

namespace std {
struct __false_type { __false_type() {} };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class T> class allocator {
public:
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    void deallocate(T* p, unsigned int) { if (p != 0) free(p); }
};
template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
};
template <class T> class _Vector_base {
public:
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T> _M_end_of_storage;
};

template <class T> class vector : public _Vector_base<T> {
public:
    protected:
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend);   // 0x00B31B10
    public:
    void push_back(const T& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1, true);
    }
};
}

template void std::vector<CEngineCamera::CDebugScriptElement>::push_back(const CEngineCamera::CDebugScriptElement&);
