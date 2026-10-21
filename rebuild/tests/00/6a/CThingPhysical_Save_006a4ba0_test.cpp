#include <cstdio>
#include <cstring>
#include <cstdlib>
#if !defined(_MSC_VER)
#define __fastcall
#endif
// CThingPhysical::Save(CCharString&) -- append the physical thing's save text after CThing's: "Health <h>;\r\n", "ObjectScale <s>;\r\n" when the scale differs from 1, and "CanComeBetweenCameraAndHero <n>;\r\n" when that value is set
// Leaf: ?Save@CThingPhysical
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A4BA0, length 322 bytes (virtual, `ret 4`).
// Callees: CThing::Save(CCharString&) @ 0x004C9120, CCharString::FormatDouble(long digits, double) @ 0x009A08B0 and
//   GFIntToCharString(long) @ 0x0099F830 (both LTCG `__fastcall` with the hidden result in ecx),
//   operator+(const char*, const CCharString&) @ 0x0099F690, operator+(const CCharString&, const char*) @ 0x0099F600,
//   CCharString::operator+=(const CCharString&) @ 0x0099F0A0, ~CCharString @ 0x0099EAE0.
//   Pool constants: 1.0f @ 0x0122DED8, 1e-4f @ 0x0129BA3C; literals "Health ", "ObjectScale ",
//   "CanComeBetweenCameraAndHero ", ";\r\n".
// Layout: retail CThingPhysical sits 0x18 below Ego_r.pdb for these members: Scale +0x9c (PDB +0xb4; landed SetScale
//   0x006A4B80 stores it there), Health +0xb4 (PDB +0xcc), CanComeBetweenCameraAndObservedThing +0xb8 (PDB +0xd0).
//   engine/CThingPhysical.h carries the PDB offsets for these three members, so a local model is used.
// Shape: each line is one `out += prefix + value + ";\r\n"` expression; the right-hand literal is pushed first, the
//   three string temporaries take the two frame words and the dead `out` argument slot.
#include <math.h>
#pragma intrinsic(fabs)

struct CCharString {
    void* m_pStringData;
    ~CCharString();                                                             // 0x0099EAE0
    CCharString& operator+=(const CCharString& other);                          // 0x0099F0A0
    static CCharString __fastcall FormatDouble(long digits, double value);      // 0x009A08B0
};
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600
CCharString __fastcall GFIntToCharString(long value);                           // 0x0099F830

struct CThing {
    virtual void Save(CCharString& out);                                        // 0x004C9120
};

struct CThingPhysical : CThing {
    unsigned char _pad04[0x9c - 4];
    float         Scale;                                                        // +0x9c
    unsigned char _padA0[0xb4 - 0xa0];
    float         Health;                                                       // +0xb4
    long          CanComeBetweenCameraAndObservedThing;                         // +0xb8
    virtual void Save(CCharString& out);
};

void CThingPhysical::Save(CCharString& out)
{
    CThing::Save(out);
    out += "Health " + CCharString::FormatDouble(6, Health) + ";\r\n";
    if (fabs(Scale - 1.0f) > 1e-4f)
        out += "ObjectScale " + CCharString::FormatDouble(6, Scale) + ";\r\n";
    if (CanComeBetweenCameraAndObservedThing != -1)
        out += "CanComeBetweenCameraAndHero " + GFIntToCharString(CanComeBetweenCameraAndObservedThing) + ";\r\n";
}


static char* Dup(const char* a, const char* b)
{ char* p = (char*)std::malloc(std::strlen(a) + std::strlen(b) + 1); std::strcpy(p, a); std::strcat(p, b); return p; }
static CCharString Make(char* text) { CCharString s; s.m_pStringData = text; return s; }
static int g_live = 0;
CCharString::~CCharString() { ++g_live; }   // leaks on purpose: VC7.1 has no NRVO, a freeing dtor would free returned copies
CCharString& CCharString::operator+=(const CCharString& other)
{ char* p = Dup((const char*)m_pStringData, (const char*)other.m_pStringData); std::free(m_pStringData); m_pStringData = p; return *this; }
CCharString __fastcall CCharString::FormatDouble(long digits, double value)
{ char buf[64]; std::sprintf(buf, "%.*f", (int)digits, value); return Make(Dup(buf, "")); }
CCharString __fastcall operator+(const char* left, const CCharString& right) { return Make(Dup(left, (const char*)right.m_pStringData)); }
CCharString __fastcall operator+(const CCharString& left, const char* right) { return Make(Dup((const char*)left.m_pStringData, right)); }
CCharString __fastcall GFIntToCharString(long value) { char buf[32]; std::sprintf(buf, "%ld", value); return Make(Dup(buf, "")); }
void CThing::Save(CCharString& out) { out += Make(Dup("Thing;", "")); }

int main()
{
    CThingPhysical t;
    CCharString out; out.m_pStringData = Dup("", "");
    t.Scale = 1.0f; t.Health = 12.5f; t.CanComeBetweenCameraAndObservedThing = -1;
    t.Save(out);
    if (std::strcmp((const char*)out.m_pStringData, "Thing;Health 12.500000;\r\n") != 0) { std::printf("006a4ba0_TEST FAIL\n"); return 1; }
    CCharString out2; out2.m_pStringData = Dup("", "");
    t.Scale = 2.0f; t.Health = 1.0f; t.CanComeBetweenCameraAndObservedThing = 3;
    t.Save(out2);
    if (std::strcmp((const char*)out2.m_pStringData, "Thing;Health 1.000000;\r\nObjectScale 2.000000;\r\nCanComeBetweenCameraAndHero 3;\r\n") != 0)
    { std::printf("006a4ba0_TEST FAIL\n"); return 1; }
    std::printf("006a4ba0_TEST PASS\n");
    return 0;
}