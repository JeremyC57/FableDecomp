// CEngineScenePrimitiveStatsEnumerator::ProcessPrimitive(CEngineInternalPrimitiveBase& primitive) -- count a primitive into the stats when it lies in the enumerator's 2D area: by its world position when it has one (half-open containment), else by its world bounding box (overlap of the XY extents)
// Leaf: ProcessPrimitive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BACA70, length 270 bytes (`ret 4`, virtual `?ProcessPrimitive@CEngineScenePrimitiveStatsEnumerator@@UAEXAAVCEngineInternalPrimitiveBase@@@Z`).
// Callees: the primitive's virtual slots 15 (+0x3c, GetWorldPosition(C3DVector&) -> bool), 17 (+0x44, GetBoundingBoxWorldSpace
// (C3DBoundingBox&) -> bool) and 2 (+0x08, the stats accumulation, (stats, a, b) from the enumerator's +0x04/+0x18/+0x1c).
// Layout (descriptive, no PDB layout): the enumerator's stats record +0x04, the area C2DBoxF +0x08 (Min X/Y, Max X/Y), two
// accumulation arguments +0x18/+0x1c.
// Shape: an if/else with early returns and ONE accumulation call after it; the position arm tests through an inline
// `Contains` (if/return true/return false), which keeps its own failing epilogue (written as one `&&` with the call in the
// `if`, VC7.1 cross-jumps the last position test into the box arm's final compare). `x >= min` is `test ah,1 ; jne`,
// `x < max` is `test ah,5 ; jp`; the overlap uses named float locals `lo = box.Min > area.Min ? box.Min : area.Min`,
// `hi = box.Max < area.Max ? box.Max : area.Max` and `lo < hi` (`fcomp st(1)` + `fstp`).
class C3DVector { public: float X, Y, Z; };
class C3DBoundingBox { public: C3DVector Min, Max; };
class C2DBoxF { public:
    float MinX, MinY, MaxX, MaxY;
    bool Contains(const C3DVector& p) const
    {
        if (p.X >= MinX && p.X < MaxX && p.Y >= MinY && p.Y < MaxY)
            return true;
        return false;
    }
};
class CEngineSceneRenderStats;

class CEngineInternalPrimitiveBase {
public:
    virtual void v00(); virtual void v01();
    virtual void AccumulateStats(CEngineSceneRenderStats* stats, long a, long b);   // slot 2 (+0x08)
    virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual bool GetWorldPosition(C3DVector& position);           // slot 15 (+0x3c)
    virtual void v16();
    virtual bool GetBoundingBoxWorldSpace(C3DBoundingBox& box);   // slot 17 (+0x44)
};

class CEngineScenePrimitiveStatsEnumerator {
public:
    CEngineSceneRenderStats* Stats;                // +0x04
    C2DBoxF                  Area;                 // +0x08
    long                     ArgA;                 // +0x18
    long                     ArgB;                 // +0x1c
    virtual void ProcessPrimitive(CEngineInternalPrimitiveBase& primitive);
};

void CEngineScenePrimitiveStatsEnumerator::ProcessPrimitive(CEngineInternalPrimitiveBase& primitive)
{
    C3DVector position;
    if (primitive.GetWorldPosition(position)) {
        if (!Area.Contains(position))
            return;
    } else {
        C3DBoundingBox box;
        if (!primitive.GetBoundingBoxWorldSpace(box))
            return;
        float lowX = box.Min.X > Area.MinX ? box.Min.X : Area.MinX;
        float highX = box.Max.X < Area.MaxX ? box.Max.X : Area.MaxX;
        if (!(lowX < highX))
            return;
        float lowY = box.Min.Y > Area.MinY ? box.Min.Y : Area.MinY;
        float highY = box.Max.Y < Area.MaxY ? box.Max.Y : Area.MaxY;
        if (!(lowY < highY))
            return;
    }
    primitive.AccumulateStats(Stats, ArgA, ArgB);
}