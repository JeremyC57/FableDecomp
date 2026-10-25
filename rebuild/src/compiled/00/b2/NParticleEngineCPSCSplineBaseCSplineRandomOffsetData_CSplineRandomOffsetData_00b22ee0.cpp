// NParticleEngine::CPSCSplineBase::CSplineRandomOffsetData::~CSplineRandomOffsetData() -- free the two offset arrays and both splines' control points
// Leaf: ??1CSplineRandomOffsetData@CPSCSplineBase@NParticleEngine@@QAE@XZ
// Retail: Fable.exe @ 0x00B22EE0, 69 bytes, plain `ret`. Catalog label "CWound::Kill" is propagated; no reference in the
// image (out-of-line copy of the destructor that 0x00B23110, the PDB-listed ??_G, inlines).
// Identification: the constructor 0x00B21DD0 (`InitialSpline` +0, ThisSpline copy at +4 via CArray copy 0x0075B8D0,
// PreviousSpline +0x18 with Tension 0.5f at +0x24, bools +0x2C/+0x2D, two empty CArrays at +0x30 / +0x3C) matches the
// Ego_r.pdb CSplineRandomOffsetData with retail 12-byte CArrays (CCardinalSpline 0x14: ControlPoints +0, Tension +0xC,
// EndPointsConnected +0x10).
// Callees: free 0x00BFEA14 (the CArray buffer).
// Shape: implicit member destructors in reverse order; the last buffer is read into esi.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

struct C3DVector { float X, Y, Z; };

template <class T> struct CArray {                  // retail CArray: STLport vector, three words
    T* m_begin;
    T* m_end;
    T* m_capacity;
    ~CArray() { if (m_begin) free(m_begin); }
};

struct CCardinalSpline {                            // 0x14 bytes retail
    CArray<C3DVector> ControlPoints;                // +0x00
    float             Tension;                      // +0x0C
    bool              EndPointsConnected;           // +0x10
};

namespace NParticleEngine {

class CPSCSplineBase {
public:
    class CSplineRandomOffsetData {
    public:
        ~CSplineRandomOffsetData();
        CCardinalSpline*  InitialSpline;            // +0x00
        CCardinalSpline   ThisSpline;               // +0x04
        CCardinalSpline   PreviousSpline;           // +0x18
        bool              OffsetOnlyAtStartup;      // +0x2C
        bool              ResetEachFrame;           // +0x2D
        CArray<C3DVector> SplinePosOffsetLimits;    // +0x30
        CArray<C3DVector> SplinePosOffsetSpeed;     // +0x3C
    };
};

CPSCSplineBase::CSplineRandomOffsetData::~CSplineRandomOffsetData()
{
}

}