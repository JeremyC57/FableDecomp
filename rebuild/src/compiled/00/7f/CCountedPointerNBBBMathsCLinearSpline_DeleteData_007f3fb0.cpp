// CCountedPointer<NBBBMaths::CLinearSpline>::DeleteData(void* data) -- the counted pointer's per-type deleter: delete the linear spline (its processed-point and control-point vectors' storage, then the object)
// Leaf: ?DeleteData@?$CCountedPointer@VCLinearSpline@NBBBMaths@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F3FB0, 49 bytes, static `__fastcall` (data in ecx). FableWin.pdb
//   `?DeleteData@?$CCountedPointer@VCLinearSpline@NBBBMaths@@@@SIXPAX@Z`; stored as the destroy function of the count block
//   built by the landed CCountedPointer Reset 0x007F4780 (CTCLightningOrb::PSpline, +0x2c, created in Initialise
//   0x007F26B0 with a zero-filled 0x18-byte `new CLinearSpline`). Catalog label `operator_delete_array` was a guess.
// Callees: free 0x00BFEA14 (STLport malloc allocator deallocate, twice), operator delete 0x00BFE9BC.
// Layout: CLinearSpline (Ego_r.pdb with retail 12-byte STLport vectors, as the landed GetPointWithParam 0x00A57F70):
//   ControlPoints +0x00, ProcessedPoints +0x0c; C3DVector elements (trivial destructor).
// Shape: `delete (T*)data` -- null test, the inlined non-virtual destructor (members in reverse order: each vector frees its
//   storage when non-null), then operator delete.
extern "C" void __cdecl free(void* block);                       // 0x00BFEA14
void __cdecl operator delete(void* block);                       // 0x00BFE9BC

class C3DVector { public: float X, Y, Z; };

template <class T> struct CRetailVector {                        // STLport vector over the malloc allocator
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~CRetailVector()
    {
        if (_M_start != 0)
            free(_M_start);
    }
};

namespace NBBBMaths {
class CLinearSpline {
public:
    CRetailVector<C3DVector> ControlPoints;                     // +0x00
    CRetailVector<C3DVector> ProcessedPoints;                   // +0x0c
};
}

template <class T> class CCountedPointer {
public:
    T*    Data;
    void* Count;
    static void __fastcall DeleteData(void* data) { delete (T*)data; }
};

template class CCountedPointer<NBBBMaths::CLinearSpline>;