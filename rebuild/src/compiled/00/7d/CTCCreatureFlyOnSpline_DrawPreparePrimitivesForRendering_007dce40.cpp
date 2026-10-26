// CTCCreatureFlyOnSpline::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) -- debug-draw the flight spline: one persistent yellow line primitive per engine handle, each joining consecutive spline points sampled at equal progress steps from 0 to 1
// Leaf: DrawPreparePrimitivesForRendering
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DCE40, 336 bytes (`ret 4`).
// Callees: CCardinalSpline::GetSplinePosition(C3DVector*, float) @ 0x00A567A0, CEnginePrimitiveLine::CEnginePrimitiveLine(...)
// @ 0x004C3090 (returns `this`), CIEngine::SetPersistentPrimitive(CEnginePrimitiveHandle&, const CEnginePrimitive&, unsigned
// long) (slot 23, +0x5c). Globals: 2^32 @ 0x0122DCB4 (unsigned size to float), 1.0f @ 0x0122DED8, 0.0f @ 0x0122DEDC.
// Layout: as the landed SetSpline 0x007DCFD0 (PSpline +0x0c, EngineHandles: retail STLport vector of 8-byte handles +0x14);
// CThing::UniqueID (unsigned __int64) +0x18; CEnginePrimitivePrepareInfo::Engine +0x34 (Ego_r.pdb).
// Shape: `if (size() != 0) { colour; do {...} while (++i != size()); }` with the index declared before the test (a `for` loop
// keeps the colour stores inside the loop); the colour (R, G, B, A = 255, 255, 0, 255) is built through CRGBColour's inline
// Set (stores R, G, B, A); the step is `1.0f / size()` (unsigned) added to the running progress, clamped to [0, 1] every step;
// the thing's id is a named local read first; the handle is `*(begin() + i)` (operator[] forms the address before the
// line constructor call, retail keeps only the base across it).
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
};
class CRGBColour { public:
    unsigned char B, G, R, A;
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { Set(r, g, b, a); }
};
class CEnginePrimitiveHandle { public: unsigned long m_data[2]; };
class CEnginePrimitive { public: };
class CEnginePrimitiveLine : public CEnginePrimitive { public:
    unsigned char _body[0x3c];
    CEnginePrimitiveLine(unsigned __int64 id, const C3DVector& start, const C3DVector& end, const CRGBColour& colour,
                         float width, long blendMode, unsigned long flags, bool depthTest, long interpolation,
                         long blendOp);                                     // 0x004C3090
};
class CIEngine { public:
#define VF(n) virtual void v##n();
    VF(00) VF(01) VF(02) VF(03) VF(04) VF(05) VF(06) VF(07) VF(08) VF(09) VF(10) VF(11) VF(12) VF(13) VF(14) VF(15) VF(16)
    VF(17) VF(18) VF(19) VF(20) VF(21) VF(22)
#undef VF
    virtual void SetPersistentPrimitive(CEnginePrimitiveHandle& handle, const CEnginePrimitive& primitive,
                                        unsigned long flags);              // slot 23 (+0x5c)
};
class CEnginePrimitivePrepareInfo { public:
    unsigned char _pad00[0x34];
    CIEngine* Engine;                                   // +0x34
};
class CThing { public:
    unsigned char _pad00[0x18];
    unsigned __int64 UniqueID;                          // +0x18
};
class CCardinalSpline { public:
    void GetSplinePosition(C3DVector* position, float progress);    // 0x00A567A0
};
struct SCountBlock;
template <class T> class CCountedPointer { public:
    T* Data;                                            // +0
    SCountBlock* m_pCount;                              // +4
    T* operator->() const { return Data; }
};
struct CEnginePrimitiveHandleVector {                   // STLport vector<CEnginePrimitiveHandle>
    CEnginePrimitiveHandle* _M_start;
    CEnginePrimitiveHandle* _M_finish;
    CEnginePrimitiveHandle* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    CEnginePrimitiveHandle& operator[](unsigned long n) { return _M_start[n]; }
    CEnginePrimitiveHandle* begin() { return _M_start; }
};
class CTCCreatureFlyOnSpline { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
    CCountedPointer<CCardinalSpline> PSpline;           // +0x0c
    CEnginePrimitiveHandleVector EngineHandles;         // +0x14
    float Progress;                                     // +0x20
    float DeltaProgress;                                // +0x24
    bool  Looping;                                      // +0x28
    void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);
};

void CTCCreatureFlyOnSpline::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
{
    if (!PSpline.Data)
        return;
    float progress = 0.0f;
    C3DVector lastPos;
    PSpline->GetSplinePosition(&lastPos, 0.0f);
    unsigned long i = 0;
    if (EngineHandles.size() != 0) {
        CRGBColour colour(255, 255, 0, 255);
        do {
            progress += 1.0f / EngineHandles.size();
            if (progress < 0.0f)
                progress = 0.0f;
            else if (progress > 1.0f)
                progress = 1.0f;
            C3DVector pos;
            PSpline->GetSplinePosition(&pos, progress);
            unsigned __int64 id = ParentThing->UniqueID;
            info.Engine->SetPersistentPrimitive(*(EngineHandles.begin() + i),
                CEnginePrimitiveLine(id, lastPos, pos, colour, 0.05f, 0, 0, true, 0, 0), 0);
            lastPos = pos;
        } while (++i != EngineHandles.size());
    }
}