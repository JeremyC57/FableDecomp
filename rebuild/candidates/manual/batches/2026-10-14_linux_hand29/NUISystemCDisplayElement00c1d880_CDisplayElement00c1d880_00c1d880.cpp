// NUISystem::CDisplayElement_00c1d880::CDisplayElement_00c1d880(long, long, long) -- the display-element base with the first and THIRD arguments, own vtable, the second argument stored at +0x20
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1D880, length 37 bytes.
// Callees: the display-element base constructor @ 0x00C1EAA0 (two arguments; manifest "NUISystem_CDisplayElement_Constructor"). Vtable 0x012B32A0 is not in
// rtti-vtables.tsv; the chunk label "CDrawAutoPickups::CDrawAutoPickups" is propagated (CDrawAutoPickups takes two arguments and lives in the 0x0064xxxx
// module), so the class is named by address. Shape: `ret 0Ch`; the third argument is loaded before `push esi` (`[esp+0Ch]`) and the first after it, both pushed for the base; the second
// argument is read after the base call and stored after the vtable.
struct CDisplayElementBase_00c1eaa0 {
    virtual void v00();
    unsigned char _body[0x20 - 4];                 // +0x04 .. +0x20
    CDisplayElementBase_00c1eaa0(long a, long b);  // 0x00C1EAA0
};

struct CDisplayElement_00c1d880 : CDisplayElementBase_00c1eaa0 {   // vtable 0x012B32A0
    virtual void v00();
    long m_style;                                  // +0x20
    CDisplayElement_00c1d880(long a, long style, long b);
};

CDisplayElement_00c1d880::CDisplayElement_00c1d880(long a, long style, long b)
    : CDisplayElementBase_00c1eaa0(a, b), m_style(style)
{
}
