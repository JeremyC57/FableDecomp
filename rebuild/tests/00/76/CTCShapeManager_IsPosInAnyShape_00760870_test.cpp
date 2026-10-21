// Behaviour test for 00760870: src/00760870.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CTCShapeManager::IsPosInAnyShape(const C2DVector& pos)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCShapeManager::IsPosInAnyShape(const C2DVector& pos) -- false outside the closed-shape bounds box; otherwise true when some persistent shape of type 0 contains the position
// Leaf: IsPosInAnyShape
// Retail: Fable.exe 0x00760870, 151 bytes (`ret 4`).
// Callees: NShape::CShape (vtable 0x0126A208) virtual slot 1 (+0x04, shape type as a long; 0 = closed area) and
// slot 4 (+0x10, bool contains(const C2DVector&)).
// Layout: retail CTCShapeManager (Ego_r.pdb less the 4 bytes of the 16-byte debug vector): PersistantShapes
// (vector<CCountedPointer<NShape::CShape>>, 8-byte elements) +0x0C, SelectedShape +0x18, Activated +0x1C,
// ClosedShapeBounds (C2DBoxF: TopLeft +0x20, BottomRight +0x28).
// Shape: the whole search sits inside `if (ClosedShapeBounds.Contains(pos)) { ... } return false;` so the miss paths
// share one epilogue (all four registers pushed up front); Contains takes the C2DVector BY VALUE with a user copy ctor
// (Y copied into the 8-byte frame, X forwarded on the FPU stack); the loop re-reads the vector end each pass.
struct C2DVector { float X, Y; C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) { } };
namespace NShape {
struct CShape {
    virtual void v00();
    virtual long GetShapeType() const;                  // slot 1 (+0x04)
    virtual void v02(); virtual void v03();
    virtual bool IsPosInShape(const C2DVector& pos) const;   // slot 4 (+0x10)
};
}
struct CCPPointerInfo;
struct CShapePointer {                                  // CCountedPointer<NShape::CShape>
    NShape::CShape* Data;
    CCPPointerInfo* Info;
};
struct C2DBoxF {
    C2DVector TopLeft;
    C2DVector BottomRight;
    bool Contains(C2DVector p) const
    {
        return p.X >= TopLeft.X && p.X < BottomRight.X && p.Y >= TopLeft.Y && p.Y < BottomRight.Y;
    }
};
struct CTCShapeManager {
    virtual void v00();
    void*          ParentThing;                         // +0x04 (CTCBase)
    bool           ToBeRemoved;                         // +0x08
    bool           IsInGlobalUpdateTCs;                 // +0x09
    CShapePointer* PersistantShapes_first;              // +0x0c
    CShapePointer* PersistantShapes_last;               // +0x10
    CShapePointer* PersistantShapes_end;                // +0x14
    CShapePointer* SelectedShape;                       // +0x18
    bool           Activated;                           // +0x1c
    bool           NewShapeActivated;                   // +0x1d
    bool           LeavingShape;                        // +0x1e
    C2DBoxF        ClosedShapeBounds;                   // +0x20
    bool IsPosInAnyShape(const C2DVector& pos) const;
};

bool CTCShapeManager::IsPosInAnyShape(const C2DVector& pos) const
{
    if (ClosedShapeBounds.Contains(pos)) {
        for (CShapePointer* it = PersistantShapes_first; it != PersistantShapes_last; ++it) {
            NShape::CShape* shape = it->Data;
            if (shape->GetShapeType() == 0 && shape->IsPosInShape(pos))
                return true;
        }
    }
    return false;
}

// ---- stubs and driver ----
void NShape::CShape::v00() { }
long NShape::CShape::GetShapeType() const { return -1; }
void NShape::CShape::v02() { }
void NShape::CShape::v03() { }
bool NShape::CShape::IsPosInShape(const C2DVector&) const { return false; }
void CTCShapeManager::v00() { }
static int g_typeCalls = 0, g_inCalls = 0; static const C2DVector* g_lastPos = 0;
struct CTestShape : NShape::CShape {
    long type; float minX, maxX;
    virtual long GetShapeType() const { ++g_typeCalls; return type; }
    virtual bool IsPosInShape(const C2DVector& p) const { ++g_inCalls; g_lastPos = &p; return p.X >= minX && p.X <= maxX; }
};
static C2DVector& Vec(float* f) { return *(C2DVector*)f; }
int main()
{
    CTestShape s0; s0.type = 1; s0.minX = 0; s0.maxX = 100;     // wrong type: never asked
    CTestShape s1; s1.type = 0; s1.minX = 0; s1.maxX = 5;
    CTestShape s2; s2.type = 0; s2.minX = 6; s2.maxX = 9;
    CShapePointer ptrs[3]; ptrs[0].Data = &s0; ptrs[1].Data = &s1; ptrs[2].Data = &s2;
    static double mgrMem[(sizeof(CTCShapeManager) + 7) / 8]; CTCShapeManager& m = *(CTCShapeManager*)mgrMem;   // no default ctor (C2DVector has only a copy ctor); the vtable is not used
    m.PersistantShapes_first = ptrs; m.PersistantShapes_last = ptrs + 3;
    float tl[2] = { 0, 0 }, br[2] = { 10, 10 };
    memcpy(&m.ClosedShapeBounds.TopLeft, tl, 8); memcpy(&m.ClosedShapeBounds.BottomRight, br, 8);
    float in2[2] = { 7, 3 }, in1[2] = { 2, 3 }, gap[2] = { 5.5f, 3 }, outX[2] = { 10, 3 }, outY[2] = { 2, -1 }, outY2[2] = { 2, 10 }, outX2[2] = { -0.5f, 3 };
    CHECK(m.IsPosInAnyShape(Vec(in2)));
    CHECK(g_typeCalls == 3 && g_inCalls == 2 && g_lastPos == &Vec(in2));   // the caller's vector, by reference
    g_typeCalls = g_inCalls = 0;
    CHECK(m.IsPosInAnyShape(Vec(in1)) && g_typeCalls == 2 && g_inCalls == 1);
    g_typeCalls = g_inCalls = 0;
    CHECK(!m.IsPosInAnyShape(Vec(gap)) && g_typeCalls == 3 && g_inCalls == 2);
    g_typeCalls = g_inCalls = 0;
    // outside the bounds box (BottomRight exclusive, TopLeft inclusive): no shape is asked
    CHECK(!m.IsPosInAnyShape(Vec(outX)) && !m.IsPosInAnyShape(Vec(outY)) && !m.IsPosInAnyShape(Vec(outY2)) && !m.IsPosInAnyShape(Vec(outX2)));
    CHECK(g_typeCalls == 0 && g_inCalls == 0);
    float edge[2] = { 0, 0 };
    CHECK(m.IsPosInAnyShape(Vec(edge)));
    // no shapes
    m.PersistantShapes_last = ptrs;
    CHECK(!m.IsPosInAnyShape(Vec(in1)));
    if (fails == 0) printf("00760870_TEST PASS\n");
    return fails;
}
