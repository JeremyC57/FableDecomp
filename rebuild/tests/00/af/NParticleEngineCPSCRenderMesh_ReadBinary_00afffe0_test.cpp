// Behaviour test for NParticleEngine::CPSCRenderMesh::ReadBinary (0x00AFFFE0): a standalone model -- every member is read from the
// stream in WriteBinary's order (bank indices as SLONG, the six colours, the packed integers as ULONG, the flags as EBOOL,
// RenderSizeParam last), so a stream written member by member reads back into the same fields.
#include <cstdio>
struct Stream { unsigned long v[64]; int pos; unsigned long Next() { return v[pos++]; } };
struct Mesh {
    long BankIndex, TrailBankIndex; unsigned long Colours[6]; unsigned long BlendMode, TrailBlendMode; bool CentredOnPos, AlphaFadeEnable;
    unsigned long TrailWidth; bool UseEndColour; unsigned long RenderSizeParam;
    void ReadBinary(Stream& in) {
        BankIndex = (long)in.Next(); TrailBankIndex = (long)in.Next();
        for (int i = 0; i < 6; ++i) Colours[i] = in.Next();
        BlendMode = in.Next(); TrailBlendMode = in.Next();
        CentredOnPos = in.Next() != 0; AlphaFadeEnable = in.Next() != 0;
        TrailWidth = in.Next(); UseEndColour = in.Next() != 0; RenderSizeParam = in.Next();
    }
};
int main()
{
    Stream s = { { (unsigned long)-1, 5, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 2, 3, 1, 0, 1023, 1, 0xC0FFEE }, 0 };
    Mesh m; m.ReadBinary(s);
    bool ok = m.BankIndex == -1 && m.TrailBankIndex == 5 && m.Colours[0] == 0x11 && m.Colours[5] == 0x66 && m.BlendMode == 2
        && m.TrailBlendMode == 3 && m.CentredOnPos && !m.AlphaFadeEnable && m.TrailWidth == 1023 && m.UseEndColour
        && m.RenderSizeParam == 0xC0FFEE && s.pos == 15;
    printf(ok ? "00afffe0_TEST PASS\n" : "00afffe0_TEST FAIL\n");
    return ok ? 0 : 1;
}