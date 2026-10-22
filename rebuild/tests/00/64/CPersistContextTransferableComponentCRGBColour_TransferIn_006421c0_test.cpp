#include <cstdio>
// Behaviour test for 006421c0: TransferableComponent<CRGBColour>::TransferIn hands an opaque-black default and the target to the reader.
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour() { A = 255; R = 0; G = 0; B = 0; }
};
struct CPersistContext { int empty; };
struct CStringParser { int present; };
struct Traits {
    CRGBColour DefaultVal;
    void TransferIn(CPersistContext&, CStringParser& parser, CRGBColour* value)
    {
        if (!parser.present) *value = DefaultVal;     // the reader falls back to the default when nothing is parsed
        else { value->R = 10; value->G = 20; value->B = 30; value->A = 40; }
    }
};
struct Component {
    CRGBColour* TransferableValue;
    void TransferIn(CPersistContext& context, CStringParser& parser) { Traits traits; traits.TransferIn(context, parser, TransferableValue); }
};
int main()
{
    CRGBColour col; col.R = 1; col.G = 2; col.B = 3; col.A = 4;
    Component c; c.TransferableValue = &col;
    CPersistContext ctx; ctx.empty = 0;
    CStringParser p; p.present = 0;
    c.TransferIn(ctx, p);
    int ok = (col.R == 0 && col.G == 0 && col.B == 0 && col.A == 255);
    p.present = 1; c.TransferIn(ctx, p);
    ok = ok && col.R == 10 && col.G == 20 && col.B == 30 && col.A == 40;
    if (!ok) { std::printf("006421c0_TEST FAIL\n"); return 1; }
    std::printf("006421c0_TEST PASS\n");
    return 0;
}