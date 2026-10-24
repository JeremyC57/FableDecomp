// CArray<CTVertexSeaBackground>::push_back(const CTVertexSeaBackground& x) -- append a 12-byte sea vertex: placement-copy at the finish pointer when there is room, else the out-of-line _M_insert_overflow @ 0x00B80400
// Leaf: ?push_back@?$CArray@VCTVertexSeaBackground@@@@QAEXABVCTVertexSeaBackground@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE5950, 72 bytes, `__fastcall`, `ret 4`. Callers: four sites in
// CWaterPatchTesselatorBase<CTVertexSeaBackground>::TesselateEdge 0x00BE4FE0 (the BridgeVertexBuffer CArray, Ego_r.pdb
// CWaterPatchTesselatorBase<CTVertexSeaBackground>::BridgeVertexBuffer). The manifest label "Vector_PushBack_3Floats" is descriptive.
// Layout: retail CArray<T> is the 12-byte STLport vector {_M_start, _M_finish, _M_end_of_storage}; CTVertexSeaBackground (Ego_r.pdb
// CGenericVertex<CTVertexSeaBackgroundBase>, 12 bytes: X, Y, Z).
// Shape: STLport `push_back`: `if (_M_finish != _M_end_of_storage) { _Construct(_M_finish, x); ++_M_finish; } else
// _M_insert_overflow(_M_finish, x, __false_type(), 1, true);` -- `this` stays in ecx across the call; the vertex copy is three
// dword moves through one register (a memberwise copy over the three words: the floats move as integers, a float copy
// constructor would go through the FPU); the empty tag temporary has no user constructor, so VC7.1 zero-stores its byte in
// the `push ecx` frame word (`mov byte ptr [esp+17h],0` after the pushes).
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct __false_type { };                           // STLport empty tag, no user constructor (zero-stored temporary)

class CTVertexSeaBackground {                      // 12 bytes; the float components are copied as words
public:
    unsigned long X, Y, Z;                         // +0 / +4 / +8 (float X, Y, Z in the PDB)
    CTVertexSeaBackground(const CTVertexSeaBackground& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

template <class T> inline void _Construct(T* p, const T& value) { new (p) T(value); }

template <class T> class CArray {
public:
    T* _M_start;                                   // +0
    T* _M_finish;                                  // +4
    T* _M_end_of_storage;                          // +8
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend);   // 0x00B80400
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

template void CArray<CTVertexSeaBackground>::push_back(const CTVertexSeaBackground&);