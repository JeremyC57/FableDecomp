// NParticleEngine::CPSCSplineBase::CSplineRandomOffsetData::`scalar deleting destructor'(unsigned int) -- inline destructor, then free the object when bit 0 is set
// Leaf: ??_GCSplineRandomOffsetData@CPSCSplineBase@NParticleEngine@@QAEPAXI@Z
// Retail: Fable.exe @ 0x00B23110, 89 bytes, `ret 4`. Catalog label "CWound::Kill" is propagated; Ego_r.pdb lists
// ??_GCSplineRandomOffsetData@CPSCSplineBase@NParticleEngine@@QAEPAXI@Z (non-virtual: the class has no vtable). No
// reference in the image.
// Callees: free 0x00BFEA14 (the four CArray buffers), operator delete 0x00BFE9BC.
// Layout: as the out-of-line destructor 0x00B22EE0 (ThisSpline.ControlPoints +4, PreviousSpline.ControlPoints +0x18,
// SplinePosOffsetLimits +0x30, SplinePosOffsetSpeed +0x3C).
// Shape: a NON-polymorphic class deleted with `delete` makes VC7.1 emit `??_G` (GOTCHAS wave 24m); the implicit inline
// destructor is expanded inside it, `this` kept in esi for the delete.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14
void __cdecl operator delete(void* block);          // 0x00BFE9BC

class C3DVector { public: float X, Y, Z; };

template <class T> class CArray { public:                  // retail CArray: STLport vector, three words
    T* m_begin;
    T* m_end;
    T* m_capacity;
    ~CArray() { if (m_begin) free(m_begin); }
};

class CCardinalSpline { public:                            // 0x14 bytes retail
    CArray<C3DVector> ControlPoints;                // +0x00
    float             Tension;                      // +0x0C
    bool              EndPointsConnected;           // +0x10
};

namespace NParticleEngine {

class CPSCSplineBase {
public:
    class CSplineRandomOffsetData {
    public:
        CCardinalSpline*  InitialSpline;            // +0x00
        CCardinalSpline   ThisSpline;               // +0x04
        CCardinalSpline   PreviousSpline;           // +0x18
        bool              OffsetOnlyAtStartup;      // +0x2C
        bool              ResetEachFrame;           // +0x2D
        CArray<C3DVector> SplinePosOffsetLimits;    // +0x30
        CArray<C3DVector> SplinePosOffsetSpeed;     // +0x3C
    };
};

void DeleteSplineRandomOffsetData(CPSCSplineBase::CSplineRandomOffsetData* data)   // emits the ??_G
{
    delete data;
}

}