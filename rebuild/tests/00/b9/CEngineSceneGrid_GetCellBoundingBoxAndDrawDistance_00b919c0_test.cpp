// Behaviour test for CEngineSceneGrid::GetCellBoundingBoxAndDrawDistance @ 0x00B919C0
#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct CAABox { C3DVector Min, Max; };
struct SDrawRange { float Near, Far; };
struct Primitive {
    CAABox Box; float Far; bool HasBox; Primitive* Next;
    SDrawRange GetDrawRange() const { SDrawRange r; r.Near = 0; r.Far = Far; return r; }
    bool GetBoundingBox(CAABox* box) const { if (HasBox) *box = Box; return HasBox; }
};
struct Cell { Primitive* Primitives; CAABox Box; float DrawDistance; unsigned char Flags; };
struct Grid {
    Grid* Child; long Side; Cell** Cells;
    bool Get(CAABox& box, float& drawDistance, long x, long y)
    {
        Cell* cell = Cells[x + Side * y];
        if (!cell) return false;
        if (!(cell->Flags & 1)) {
            bool found = false;
            if (Child) {
                for (unsigned long j = 0; j < 2; ++j)
                    for (unsigned long i = 0; i < 2; ++i) {
                        CAABox cb; float cd;
                        if (Child->Get(cb, cd, i + x * 2, j + y * 2)) {
                            if (found) {
                                if (cell->Box.Min.X > cb.Min.X) cell->Box.Min.X = cb.Min.X;
                                if (cell->Box.Min.Y > cb.Min.Y) cell->Box.Min.Y = cb.Min.Y;
                                if (cell->Box.Min.Z > cb.Min.Z) cell->Box.Min.Z = cb.Min.Z;
                                if (cell->Box.Max.X < cb.Max.X) cell->Box.Max.X = cb.Max.X;
                                if (cell->Box.Max.Y < cb.Max.Y) cell->Box.Max.Y = cb.Max.Y;
                                if (cell->Box.Max.Z < cb.Max.Z) cell->Box.Max.Z = cb.Max.Z;
                                if (cell->DrawDistance < cd) cell->DrawDistance = cd;
                            } else { cell->Box = cb; cell->DrawDistance = cd; found = true; }
                        }
                    }
            }
            for (Primitive* p = cell->Primitives; p; p = p->Next) {
                CAABox pb;
                if (p->GetBoundingBox(&pb)) {
                    if (found) {
                        if (cell->Box.Min.X > pb.Min.X) cell->Box.Min.X = pb.Min.X;
                        if (cell->Box.Min.Y > pb.Min.Y) cell->Box.Min.Y = pb.Min.Y;
                        if (cell->Box.Min.Z > pb.Min.Z) cell->Box.Min.Z = pb.Min.Z;
                        if (cell->Box.Max.X < pb.Max.X) cell->Box.Max.X = pb.Max.X;
                        if (cell->Box.Max.Y < pb.Max.Y) cell->Box.Max.Y = pb.Max.Y;
                        if (cell->Box.Max.Z < pb.Max.Z) cell->Box.Max.Z = pb.Max.Z;
                        if (cell->DrawDistance < p->GetDrawRange().Far) cell->DrawDistance = p->GetDrawRange().Far;
                    } else { cell->Box = pb; cell->DrawDistance = p->GetDrawRange().Far; found = true; }
                }
            }
            if (found) cell->Flags |= 1; else return false;
        }
        box = cell->Box; drawDistance = cell->DrawDistance;
        return true;
    }
};
static CAABox MakeBox(float a, float b) { CAABox r; r.Min.X = r.Min.Y = r.Min.Z = a; r.Max.X = r.Max.Y = r.Max.Z = b; return r; }
int main()
{
    // child grid 2x2 with two filled cells, parent 1x1 with one primitive
    Primitive c0; c0.Box = MakeBox(0, 1); c0.Far = 10; c0.HasBox = true; c0.Next = 0;
    Primitive c3; c3.Box = MakeBox(-2, 0.5f); c3.Far = 30; c3.HasBox = true; c3.Next = 0;
    Primitive pp; pp.Box = MakeBox(0, 5); pp.Far = 20; pp.HasBox = true; pp.Next = 0;
    Cell k0 = { &c0, MakeBox(0, 0), 0, 0 }; Cell k3 = { &c3, MakeBox(0, 0), 0, 0 };
    Cell k1 = { 0, MakeBox(0, 0), 0, 0 };
    Cell* childCells[4] = { &k0, &k1, 0, &k3 };
    Grid child = { 0, 2, childCells };
    Cell top = { &pp, MakeBox(0, 0), 0, 0 };
    Cell* topCells[1] = { &top };
    Grid parent = { &child, 1, topCells };
    CAABox box; float dist = 0;
    if (!parent.Get(box, dist, 0, 0) || box.Min.X != -2 || box.Max.Y != 5 || dist != 30 || !(top.Flags & 1) || (k1.Flags & 1)) {
        std::printf("00b919c0_TEST FAIL\n"); return 1;
    }
    if (child.Get(box, dist, 1, 0)) { std::printf("00b919c0_TEST FAIL\n"); return 1; }
    std::printf("00b919c0_TEST PASS\n");
    return 0;
}