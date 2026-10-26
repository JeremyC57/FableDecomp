// CSubtitleRenderer::CEmbeddedGraphic::CEmbeddedGraphic(const CEmbeddedGraphic& other) -- copy the graphic, the
// character index and the 2D offset
// Retail: Fable.exe (TLC, VC7.1) @ 0x004935F8, 31 bytes, `ret 4` (__thiscall: ecx = this, returned in eax).
// Identity: Ego_r.pdb CSubtitleRenderer::CEmbeddedGraphic is 16 bytes {long Graphic +0, long CharIndex +4, C2DVector
// Offset +8}; the PDB name set has the vector<CSubtitleRenderer::CEmbeddedGraphic> machinery (_Construct, _Cons_val,
// _Destroy_range, ...). Callers are that vector's insert / uninitialised-copy / fill helpers (0x0049417D, called from
// CSubtitleRenderer::ProcessEmbeddedGraphicTags at 0x0049243A; 0x00493959, 0x00493984, 0x00493911). The old
// NPlayerGui::GFToScreen label was propagated from an unrelated body.
// Shape: the members are copied as four dword moves (the floats of Offset as integers: a plain struct copy).
class C2DVector { public:
    float X;                                            // +0x00
    float Y;                                            // +0x04
};

class CSubtitleRenderer {
public:
    class CEmbeddedGraphic {
    public:
        long      Graphic;                              // +0x00
        long      CharIndex;                            // +0x04
        C2DVector Offset;                               // +0x08
        CEmbeddedGraphic(const CEmbeddedGraphic& other);
    };
};

CSubtitleRenderer::CEmbeddedGraphic::CEmbeddedGraphic(const CEmbeddedGraphic& other)
    : Graphic(other.Graphic), CharIndex(other.CharIndex), Offset(other.Offset)
{
}
