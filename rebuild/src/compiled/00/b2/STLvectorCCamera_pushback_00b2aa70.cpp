// std::vector::push_back(const CCamera& x) -- STLport push_back of the engine's camera vectors: copy-construct at _M_finish (out-of-line CCamera copy constructor) when there is room, else _M_insert_overflow @ 0x00B29840 (one element, at the end)
// Leaf: ?push_back@?$vector@VCCamera@@@std@@QAEXABVCCamera@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B2AA70, length 59 bytes, thiscall `ret 4`.
// Identity: callees are the landed CCamera::CCamera(const CCamera&) 0x0049D380 (Ego_r.pdb CCamera, 0x38 bytes = the 0x38
//   stride) and the landed std::vector<CCamera>::_M_insert_overflow 0x00B29840 (whose model and namespace this row
//   shares); the other push_back sites of that vector (CEngine::LoadNewScene 0x00B277A0, CEngine::CutsceneAddCameraAngle
//   0x00B27D50, Ego_r.pdb CArray<CCamera> PreloadCameras) inline the same body. This out-of-line copy has no direct xref.
//   Descriptive: no PDB symbol for this instantiation (CArray<T>::push_back vs vector<T>::push_back not decidable from the
//   bytes). The catalog name PushBack_CElement56_0049d380 and the manifest CTCCameraPointScriptedSpline_InsertKeyCamera were
//   a shape name and a propagation.
// Shape: STLport 4.6 `if (_M_finish != _M_end_of_storage._M_data) { _Construct(_M_finish, x); ++_M_finish; } else
//   _M_insert_overflow(_M_finish, x, __false_type(), 1, true);`; the placement new null-tests before the copy constructor
//   call, and the tag temporary is homed in this function's dead argument slot (`lea edx,[esp+10h]` after the two `push 1`).
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E (allocator, unused here)
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14 (allocator, unused here)
inline void* __cdecl operator new(unsigned int, void* where) throw() { return where; }

class C3DVector {
public:
    float X, Y, Z;
};

class CRightHandedSet {
public:
    C3DVector Up;                                           // +0x00
    C3DVector Forward;                                      // +0x0C
};

class CCamera {                                             // PDB sizeof 0x38
public:
    C3DVector       WorldPosition;                          // +0x00
    CRightHandedSet RHSet;                                  // +0x0C
    bool            HeightLocked;                           // +0x24
    bool            ViewVecZLocked;                         // +0x25
    unsigned long   FOVFlags;                               // +0x28
    float           HorizontalFOV;                          // +0x2C
    float           VerticalFOV;                            // +0x30
    float           Zoom;                                   // +0x34
    CCamera(const CCamera& other);                          // 0x0049D380
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
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend);   // 0x00B29840
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

template void std::vector<CCamera>::push_back(const CCamera&);
