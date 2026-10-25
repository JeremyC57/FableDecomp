// TransferIMEColour(CPersistContext& context, const CCharString& name, unsigned long colour) -- persist one IME colour as four <name>R/G/B/A byte settings (defaulting to the given colour) and return it as an opaque X8R8G8B8 colour
// Grade: DIFFER(290v290) -- only the return packing: retail loads the colour dword once (`mov ecx,[esp+14h] ; movzx edx,ch ; movzx ecx,cl`), this form loads G and B as bytes; every constructor form that gives retail's dword-load tail (initialiser list, B-first or A-first body) misses the head's `shr eax,8 ; mov edx,eax ; shr edx,8` register split (19 forms swept)
// Leaf: ?TransferIMEColour@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00405220, 290 bytes (304 with padding), `ret 4`; FableWin.pdb
//   `?TransferIMEColour@@YIKAAVCPersistContext@@ABVCCharString@@K@Z` (__fastcall: context in ecx, name in edx, the colour on
//   the stack). Callers: the twelve colour settings of NUISystem::CIME::LoadIMESettings 0x00405350.
// Inlined: `?IMEColour2RGB@@YI?AVCRGBColour@@K@Z` (B/G/R from the low three bytes, A = the IME composition alpha
//   0x013750B0) and `?RGB2IMEColour@@YIKAAVCRGBColour@@@Z` (0xFF000000 | R<<16 | G<<8 | B).
// Callees: operator+(const CCharString&, const char*) 0x0099F600 (__fastcall, hidden result in ecx), CCharString::operator
//   const char* 0x0099E4C0, CPersistContext::Transfer<unsigned char>(const char*, unsigned char&, const unsigned char&)
//   0x00405A60 (`??$Transfer@E@CPersistContext@@QAEXPBDAAEABE@Z`), ~CCharString 0x0099EAE0.
// Layout: Ego_r.pdb CRGBColour = union { struct { uchar B, G, R, A; } } (B +0, G +1, R +2, A +3).
// Shape: IMEColour2RGB peels the bytes off with two `>>= 8` steps (retail `shr eax,8 ; mov edx,eax ; shr edx,8`; a
//   `colour >> 16` folds to one `shr 10h`) into the CRGBColour(r, g, b, a) constructor with BODY assignments (the
//   initialiser-list form stores green before computing red). The colour is built in the dead argument slot and copied whole into a frame word
//   as the defaults; each Transfer pushes &default, &value, then the name temporary (operator+ into the frame word,
//   converted to const char*), destroyed after the call. The return packs the transferred bytes with `or 0FFFFFF00h`.
class CCharString {
public:
    ~CCharString();                                      // 0x0099EAE0
    operator const char*() const;                        // 0x0099E4C0
private:
    void* m_pData;
};
CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600

class CPersistContext {
public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // 0x00405A60 (T = unsigned char)
};

struct CRGBColour {
    unsigned char B;                                     // +0
    unsigned char G;                                     // +1
    unsigned char R;                                     // +2
    unsigned char A;                                     // +3
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};

extern unsigned char g_imeCompositionAlpha;              // 0x013750B0

static inline CRGBColour IMEColour2RGB(unsigned long colour)
{
    unsigned char blue = (unsigned char)colour;
    colour >>= 8;
    unsigned char green = (unsigned char)colour;
    colour >>= 8;
    unsigned char red = (unsigned char)colour;
    return CRGBColour(red, green, blue, g_imeCompositionAlpha);
}

static inline unsigned long RGB2IMEColour(CRGBColour& colour)
{
    return (0xffUL << 24) | ((unsigned long)colour.R << 16) | ((unsigned long)colour.G << 8) | colour.B;
}

unsigned long __fastcall TransferIMEColour(CPersistContext& context, const CCharString& name, unsigned long colour)
{
    CRGBColour rgb = IMEColour2RGB(colour);
    CRGBColour defaultRgb = rgb;
    context.Transfer(name + "R", rgb.R, defaultRgb.R);
    context.Transfer(name + "G", rgb.G, defaultRgb.G);
    context.Transfer(name + "B", rgb.B, defaultRgb.B);
    context.Transfer(name + "A", rgb.A, defaultRgb.A);
    return RGB2IMEColour(rgb);
}
