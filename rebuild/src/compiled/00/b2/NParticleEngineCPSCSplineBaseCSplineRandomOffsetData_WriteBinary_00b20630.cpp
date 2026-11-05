// NParticleEngine::CPSCSplineBase::CSplineRandomOffsetData::WriteBinary(CDataOutputStream& out) const -- write the two offset flags, the control-point count, then each point's offset limit and offset speed
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B20630, 119 bytes; `ret 4`. Callees: CDataOutputStream::WriteEBOOL(bool) @ 0x00993E30 (x2),
//          WriteULONG(unsigned long) @ 0x00993EA0 (catalog "WriteCBYTE"; GOTCHAS: it writes a long), Write3DVector(const C3DVector&) @ 0x00993FD0.
// Layout: Ego_r.pdb CSplineRandomOffsetData with retail's 12-byte STLport CArray: InitialSpline +0, ThisSpline +4, PreviousSpline +0x18,
//         OffsetOnlyAtStartup +0x2c, ResetEachFrame +0x2d, SplinePosOffsetLimits +0x30, SplinePosOffsetSpeed +0x3c.
// Shape: the count is the limits array's size (pointer difference / 12, signed); the loop index walks both arrays through
//        `*(begin() + i)` so each begin pointer is re-read per call.
#include "engine/C3DVector.h"
#include "engine/CArray.h"
class CCardinalSpline { public:                           // retail 20 bytes
    CArray<C3DVector> ControlPoints;
    float             Tension;
    bool              EndPointsConnected;
};
class CDataOutputStream { public:
    void WriteEBOOL(bool value);                   // 0x00993E30
    void WriteULONG(unsigned long value);          // 0x00993EA0
    void Write3DVector(const C3DVector& value);    // 0x00993FD0
};
namespace NParticleEngine {
class CPSCSplineBase { public:
    class CSplineRandomOffsetData { public:
        CCardinalSpline*  InitialSpline;           // +0x00
        CCardinalSpline   ThisSpline;              // +0x04
        CCardinalSpline   PreviousSpline;          // +0x18
        bool              OffsetOnlyAtStartup;     // +0x2c
        bool              ResetEachFrame;          // +0x2d
        CArray<C3DVector> SplinePosOffsetLimits;   // +0x30
        CArray<C3DVector> SplinePosOffsetSpeed;    // +0x3c
        void WriteBinary(CDataOutputStream& out) const;
    };
};

void CPSCSplineBase::CSplineRandomOffsetData::WriteBinary(CDataOutputStream& out) const
{
    out.WriteEBOOL(OffsetOnlyAtStartup);
    out.WriteEBOOL(ResetEachFrame);
    unsigned long count = SplinePosOffsetLimits.size();
    out.WriteULONG(count);
    for (unsigned long i = 0; i < count; ++i) {
        out.Write3DVector(SplinePosOffsetLimits[i]);
        out.Write3DVector(SplinePosOffsetSpeed[i]);
    }
}
}