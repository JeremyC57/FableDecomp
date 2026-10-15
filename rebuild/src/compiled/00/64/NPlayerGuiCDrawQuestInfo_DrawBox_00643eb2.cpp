#pragma optimize("y", off)
#pragma optimize("s", on)
// NPlayerGui::CDrawQuestInfo::DrawBox(CEnginePrimitivePrepareInfo*, const C2DBoxF&) -- mark the drawer as drawing and draw the quest box's background, grown by a fixed border, on layer 0x4b
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00643EB2, length 97 bytes.
// Callees: the global DrawBox(CEnginePrimitivePrepareInfo*, vector<CEnginePrimitiveHandle>*, const C2DBoxF&, EEngine2DLayer)
// @ 0x0048E0A2 (`__fastcall`: info in ecx, the drawer's PrimitiveHandles (+0x1c) in edx, the box and the layer on the stack).
// Layout: CDrawBase Drawing +0x18, PrimitiveHandles +0x1c; C2DBoxF = TopLeft + BottomRight C2DVectors, 16 bytes; the
// inline user copy constructors make the box copy memberwise integer moves (a POD box copies with movsd x4 under /Os,
// a plain inline copy constructor stays out of line in this size TU, hence `__forceinline`).
// Constants: 8.0f @ 0x0125916C, 2.0f @ 0x0122DCDC (float pool).
// Shape: frame-pointer size TU; the box is copied into a frame local and grown statement by statement (left -= 8,
// top -= 8, right += 2, bottom += 8): VC forwards the copied left/bottom words straight into the x87 and copies the other
// two through edx, keeping the 8.0f on the FPU stack for the three uses.
struct CEnginePrimitivePrepareInfo;
struct CEnginePrimitiveHandle;

struct C2DVector {
    float X;                                       // +0x00
    float Y;                                       // +0x04
    __forceinline C2DVector(const C2DVector& other) : X(other.X), Y(other.Y) { }
};

struct C2DBoxF {
    C2DVector TopLeft;                             // +0x00
    C2DVector BottomRight;                         // +0x08
    __forceinline C2DBoxF(const C2DBoxF& other) : TopLeft(other.TopLeft), BottomRight(other.BottomRight) { }
};

struct CRetailHandleVector {                       // VC6 Dinkumware vector<CEnginePrimitiveHandle>: three words
    CEnginePrimitiveHandle* m_pFirst;
    CEnginePrimitiveHandle* m_pLast;
    CEnginePrimitiveHandle* m_pEnd;
};

enum EEngine2DLayer { ENGINE_2D_LAYER_QUEST_BOX = 0x4b };

void __fastcall DrawBox(CEnginePrimitivePrepareInfo* info, CRetailHandleVector* handles, const C2DBoxF& box,
                        EEngine2DLayer layer);   // 0x0048E0A2

namespace NPlayerGui {

struct CDrawQuestInfo {
    unsigned char       _base[0x18];               // CDrawBase vptr, GUI pointers
    bool                Drawing;                   // +0x18
    CRetailHandleVector PrimitiveHandles;          // +0x1c
    void DrawBox(CEnginePrimitivePrepareInfo* info, const C2DBoxF& box);
};

void CDrawQuestInfo::DrawBox(CEnginePrimitivePrepareInfo* info, const C2DBoxF& box)
{
    Drawing = true;
    C2DBoxF border = box;
    border.TopLeft.X -= 8.0f;
    border.TopLeft.Y -= 8.0f;
    border.BottomRight.X += 2.0f;
    border.BottomRight.Y += 8.0f;
    ::DrawBox(info, &PrimitiveHandles, border, ENGINE_2D_LAYER_QUEST_BOX);
}

}