// CGridCircle::CGridCircle(float radius, CCellVisitor* visitor) -- visit every integer grid cell within a radius: walk the square of half-width ceil(radius * 1.1) around the origin and hand each cell whose length is at most the radius to the visitor (slot 0)
// Leaf: ??0CGridCircle@@QAE@MPAVCCellVisitor@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C46900, 157 bytes (+ int3 padding), __fastcall `ret 8`, returns this; an
//   8-byte aligned frame (`and esp,0FFFFFFF8h`).
// Library: libpk (no PDB/RTTI): descriptive names. Only caller the region map's constructor 0x00C467D0, which builds
//   this empty walker as a temporary in its dead first-argument slot with radius / cell size and a stack collector
//   (vtable 0x012B7D64: slot 0 0x00C466F0 interns the four corners of each cell). The manifest's
//   "CGuiVarTransfer_IterateRange" is propagated.
// Callees: MSVCR71 `ceil` through the import table (0x014400F4: declared dllimport so the call is `ff 15`; the
//   <math.h> declaration gives a direct `call`), _ftol2 0x00BFEA70, C2DVectorI::GetLength 0x00C62090
//   (sqrt of the integer squared length).
// Shape: both loops rotated over signed ints with the negated extent kept in a stack slot; `this` is spilled to the
//   frame and returned from it after the loops (`mov eax,esi` on the empty path); the cell is a stack local whose
//   address goes to GetLength and the visitor.
extern "C" __declspec(dllimport) double __cdecl ceil(double x);

struct C2DVectorI {
    long X;                                 // +0x00
    long Y;                                 // +0x04
    C2DVectorI(long x, long y) : X(x), Y(y) { }
    float GetLength() const;                // 0x00C62090
};

class CCellVisitor {
public:
    virtual void Visit(const C2DVectorI& cell) = 0;     // slot 0
};

class CGridCircle {
public:
    CGridCircle(float radius, CCellVisitor* visitor);
};

CGridCircle::CGridCircle(float radius, CCellVisitor* visitor)
{
    long extent = (long)ceil(radius * 1.1f);
    for (long x = -extent; x <= extent; ++x)
        for (long y = -extent; y <= extent; ++y) {
            C2DVectorI cell(x, y);
            if (cell.GetLength() <= radius)
                visitor->Visit(cell);
        }
}