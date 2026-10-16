// Grade: DIFFER(186v186) -- store scheduling only: in the loop copies retail loads all three words before the three stores (and the first copy stores Y later); ours interleaves load/store | tried: user copy ctor without operator= (best), implicit copy, float-local operator= | next: lane gap: scheduling (maybe an operator= on a 3-dword struct base)
// NShape::CClosedShapeBase::EditorDrawPreparePrimitivesForRendering(CEnginePrimitivePrepareInfo*, CWorldMap*, C3DVector*, CRGBColour*) -- editor outline: one landscape-hugging line per edge of the closed point loop (last point back to the first)
// Leaf: EditorDrawPreparePrimitivesForRendering
// Retail: Fable.exe (TLC, VC7.1) @ 0x00761560, 192 bytes incl. padding (ret 10h). Graphics/editor row (owner note):
//          class NShape::CClosedShapeBase, vtable 0x0126A224 (rtti-vtables.tsv; base NShape::CShape 0x0126A208).
// Callees: DrawPrimitiveLineOverLandscape(CEnginePrimitivePrepareInfo*, CWorldMap*, C3DVector* from, C3DVector* to,
//          C3DVector* offset, CRGBColour* colour) @ 0x00761280 (__fastcall: info in ecx, map in edx).
// Layout (landed NShape::CShape model 0x00762440/0x00765080): m_id +0x04, point vector {first +0x08, last +0x0C, end +0x10}
//          of 12-byte C3DVectors, edge vector +0x14.
// Shape: C3DVector with a user copy ctor and no operator=, so `current = *it` copies through a pointer register; the
//          loop runs until the iterator wraps back to begin(); begin()/end() re-read every pass.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CRGBColour;
struct CWorldMap;
struct CEnginePrimitivePrepareInfo;

void __fastcall DrawPrimitiveLineOverLandscape(CEnginePrimitivePrepareInfo* info, CWorldMap* map, C3DVector* from,
                                               C3DVector* to, C3DVector* offset, CRGBColour* colour);   // 0x00761280

struct CPointVector {
    C3DVector* m_pFirst;                        // +0
    C3DVector* m_pLast;                         // +4
    C3DVector* m_pEnd;                          // +8
    C3DVector* begin() const { return m_pFirst; }
    C3DVector* end() const { return m_pLast; }
};

namespace NShape {
struct CShape {                                 // vtable 0x0126A208
    virtual ~CShape();
    long         m_id;                          // +0x04
    CPointVector m_points;                      // +0x08
};

struct CClosedShapeBase : CShape {              // vtable 0x0126A224
    void EditorDrawPreparePrimitivesForRendering(CEnginePrimitivePrepareInfo* info, CWorldMap* map, C3DVector* offset,
                                                 CRGBColour* colour);
};
}

void NShape::CClosedShapeBase::EditorDrawPreparePrimitivesForRendering(CEnginePrimitivePrepareInfo* info, CWorldMap* map,
                                                                       C3DVector* offset, CRGBColour* colour)
{
    if (m_points.begin() == m_points.end())
        return;
    C3DVector* it = m_points.begin();
    C3DVector previous = *it;
    ++it;
    while (it != m_points.begin()) {
        C3DVector current;
        if (it == m_points.end()) {
            it = m_points.begin();
            current = *it;
        } else {
            current = *it;
            ++it;
        }
        DrawPrimitiveLineOverLandscape(info, map, &previous, &current, offset, colour);
        previous = current;
    }
}
