// DeleteSplineRandomOffsetData(CSplineRandomOffsetData* data) -- the counted-pointer deleter of CSplineRandomOffsetData: null test, the inlined destructor frees its four C3DVector arrays, then operator delete
// Leaf: DeleteSplineRandomOffsetData
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B231E0, length 82 bytes (+ int3 padding).
// Callees: free @ 0x00BFEA14 four times (the inline CArray destructors), operator delete @ 0x00BFE9BC.
// Evidence: the address is stored as the destroy function (+4) of the 12-byte count block built by the
// CCountedPointer Reset @ 0x00B23240 (`mov dword ptr [eax+4],0xB231E0`; called with the object in ecx), whose callers
// allocate 0x48 bytes and run CSplineRandomOffsetData::CSplineRandomOffsetData @ 0x00B21DD0 on them. The catalog label
// "CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip" is propagated.
// Layout: Ego_r.pdb CSplineRandomOffsetData with the retail 12-byte CArray: InitialSpline +0, ThisSpline (CCardinalSpline,
// retail 0x14: ControlPoints, Tension, EndPointsConnected) +0x04, PreviousSpline +0x18, OffsetOnlyAtStartup +0x2c,
// ResetEachFrame +0x2d, SplinePosOffsetLimits +0x30, SplinePosOffsetSpeed +0x3c (retail size 0x48). No engine header.
// Shape: a `__fastcall` deleter (object in ecx, kept in esi) written as the expanded `delete` (`if (data) { ~T; operator
// delete }`): a plain `delete data` of this non-polymorphic class calls an out-of-line `??_G`; the destructors are
// `__forceinline`, the arrays are freed in reverse order (+0x3c, +0x30, +0x18, +0x04).
extern "C" void __cdecl free(void* block);         // 0x00BFEA14
void __cdecl operator delete(void* block);         // 0x00BFE9BC

struct C3DVector { float X, Y, Z; };

template <class T>
struct CArray {                                    // retail CArray<T>: first / last / end (0x0c bytes)
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    __forceinline ~CArray() { if (m_pFirst) free(m_pFirst); }
};

struct CCardinalSpline {                           // retail 0x14 bytes
    CArray<C3DVector> ControlPoints;               // +0x00
    float             Tension;                     // +0x0c
    bool              EndPointsConnected;          // +0x10
};

struct CSplineRandomOffsetData {
    CCardinalSpline*  InitialSpline;               // +0x00
    CCardinalSpline   ThisSpline;                  // +0x04
    CCardinalSpline   PreviousSpline;              // +0x18
    bool              OffsetOnlyAtStartup;         // +0x2c
    bool              ResetEachFrame;              // +0x2d
    CArray<C3DVector> SplinePosOffsetLimits;       // +0x30
    CArray<C3DVector> SplinePosOffsetSpeed;        // +0x3c
};

void __fastcall DeleteSplineRandomOffsetData(CSplineRandomOffsetData* data)
{
    if (data) {
        data->~CSplineRandomOffsetData();
        operator delete(data);
    }
}
