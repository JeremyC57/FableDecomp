// NParticleEngine::CPSCSplineBase::CSplineRandomOffsetData::WriteBinary(CDataOutputStream& out) const -- write the two offset flags, the control-point count, then each point's offset limit and offset speed
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B20630, 119 bytes; `ret 4`. Callees: CDataOutputStream::WriteEBOOL(bool) @ 0x00993E30 (x2),
//          WriteULONG(unsigned long) @ 0x00993EA0 (catalog "WriteCBYTE"; GOTCHAS: it writes a long), Write3DVector(const C3DVector&) @ 0x00993FD0.
// Layout: Ego_r.pdb CSplineRandomOffsetData with retail's 12-byte STLport CArray: InitialSpline +0, ThisSpline +4, PreviousSpline +0x18,
//         OffsetOnlyAtStartup +0x2c, ResetEachFrame +0x2d, SplinePosOffsetLimits +0x30, SplinePosOffsetSpeed +0x3c.
// Shape: the count is the limits array's size (pointer difference / 12, signed); the loop index walks both arrays through
//        `*(begin() + i)` so each begin pointer is re-read per call.
class C3DVector { public: float X, Y, Z; };
template <class T> class CArray { public:                 // STLport vector<T>: start / finish / end of storage
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    long size() const { return _M_finish - _M_start; }
    const T& operator[](unsigned long n) const { return *(begin() + n); }
};
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