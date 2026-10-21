// CSubtitleRenderer::DrawBox(long, const CBoxStyle&, const C2DBoxF&) -- draws a subtitle box through the shared box helper
// Leaf: DrawBox@CSubtitleRenderer
// Retail: Fable.exe @ 0x0048FAA0, 22 bytes, `ret 0Ch`. Catalog label CMainGameComponent::BeginInputLoading is wrong: the
// only caller is CSubtitleRenderer::DrawPreparePrimitivesForEngine (0x00490710, ecx = its this, args: its own arg,
// &this->+0x94, &a 16-byte local box). The body ignores this and forwards to the __fastcall helper 0x0048E0A2 (also called
// from 0x005ECA28 and 0x00643F0A) with a trailing constant 0x47. The method name and argument types are behavioural
// placeholders: Ego_r.pdb does not identify this wrapper.
// Shape: size TU (`push dword ptr [esp+10h]` for the stack argument).
#pragma optimize("s", on)
class C2DBoxF { public: float X0, Y0, X1, Y1; };
class CBoxStyle;
void __fastcall DrawBox(long layer, const CBoxStyle& style, const C2DBoxF& box, long flags);   // 0x0048E0A2
class CSubtitleRenderer {
public:
    void DrawBox(long layer, const CBoxStyle& style, const C2DBoxF& box);
};
void CSubtitleRenderer::DrawBox(long layer, const CBoxStyle& style, const C2DBoxF& box)
{
    ::DrawBox(layer, style, box, 0x47);
}