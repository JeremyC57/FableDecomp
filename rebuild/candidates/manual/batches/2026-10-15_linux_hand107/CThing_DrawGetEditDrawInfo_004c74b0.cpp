// CThing::DrawGetEditDrawInfo(CEditContext* context, SEditDrawInfo* info) -- fill the editor draw info: the thing's player number, a mid-grey opaque colour, an empty label and no highlight; always true
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C74B0, 53 bytes. CThing vtable 0x01239ABC slot 25.
// Callees: CCharString::operator=(const char*) @ 0x0099EFE0 with the empty literal @ 0x0122D70E.
// Shape: PlayerNumber is the signed char at retail +0x90 (PDB +0xa0), widened with `movsx`; the colour is a
// CRGBColour stored R, G, B then A (`[+16h] [+15h] [+14h]` then `[+17h]`), i.e. an inline setter taking and
// storing (r, g, b, a) over the B, G, R, A byte layout (a constructed temporary is built in a frame slot first). The first argument is unused.
struct CEditContext;

struct CCharString {
    void* m_pData;
    CCharString& operator=(const char* text);        // 0x0099EFE0
};

struct CRGBColour {
    unsigned char B;                                 // +0
    unsigned char G;                                 // +1
    unsigned char R;                                 // +2
    unsigned char A;                                 // +3
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    {
        R = r;
        G = g;
        B = b;
        A = a;
    }
};

struct SEditDrawInfo {
    unsigned char _pad00[0x10];
    CCharString   Label;                             // +0x10
    CRGBColour    Colour;                            // +0x14
    long          PlayerNumber;                      // +0x18
    unsigned char _pad1c[0x20 - 0x1c];
    bool          Highlighted;                       // +0x20
};

struct CThing {
    void*         __vftable;                         // +0x00
    unsigned char _pad04[0x90 - 0x04];
    signed char   PlayerNumber;                      // +0x90 (PDB +0xa0)
    bool DrawGetEditDrawInfo(CEditContext* context, SEditDrawInfo* info);
};

bool CThing::DrawGetEditDrawInfo(CEditContext* context, SEditDrawInfo* info)
{
    info->PlayerNumber = PlayerNumber;
    info->Colour.Set(0x80, 0x80, 0x80, 0xff);
    info->Label = "";
    info->Highlighted = false;
    return true;
}
