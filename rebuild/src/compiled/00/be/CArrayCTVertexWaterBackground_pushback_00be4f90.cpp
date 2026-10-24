// CArray<CTVertexWaterBackground>::push_back(const CTVertexWaterBackground& x) -- append a 56-byte water vertex: block-copy it to the finish pointer when there is room, else the out-of-line _M_insert_overflow @ 0x00B7FFC0
// Leaf: ?push_back@?$CArray@VCTVertexWaterBackground@@@@QAEXABVCTVertexWaterBackground@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE4F90, 72 bytes, `__fastcall`, `ret 4`. Callers: four sites in the water
// tesselator 0x00BE4660 (CWaterPatchTesselatorBase<CTVertexWaterBackground>, its BridgeVertexBuffer CArray in Ego_r.pdb).
// The manifest label "Vector_PushBack_14Elements" is descriptive. Sibling: the sea-vertex push_back 0x00BE5950.
// Layout: retail CArray<T> is the 12-byte STLport vector {_M_start, _M_finish, _M_end_of_storage}; CTVertexWaterBackground (Ego_r.pdb
// CGenericVertex<CTVertexWaterBackgroundBase>, 56 bytes: short X, short Y, float Z, float ShoreLookUpArray[12]) is a POD.
// Shape: STLport `push_back`: `if (_M_finish != _M_end_of_storage) { _Construct(_M_finish, x); ++_M_finish; } else
// _M_insert_overflow(_M_finish, x, __false_type(), 1, true);` -- the POD's implicit copy is `mov ecx,0Eh ; rep movsd` through
// edi = _M_finish (the end pointer read first into ecx, `this` moved to eax); the empty tag temporary has no user constructor,
// so VC7.1 zero-stores its byte in the `push ecx` frame word.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct __false_type { };                           // STLport empty tag, no user constructor (zero-stored temporary)

class CTVertexWaterBackground {                    // 56 bytes, POD
public:
    short X;                                       // +0x00
    short Y;                                       // +0x02
    float Z;                                       // +0x04
    float ShoreLookUpArray[12];                    // +0x08
};

template <class T> inline void _Construct(T* p, const T& value) { new (p) T(value); }

template <class T> class CArray {
public:
    T* _M_start;                                   // +0
    T* _M_finish;                                  // +4
    T* _M_end_of_storage;                          // +8
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend);   // 0x00B7FFC0
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            _Construct(_M_finish, x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};

template void CArray<CTVertexWaterBackground>::push_back(const CTVertexWaterBackground&);