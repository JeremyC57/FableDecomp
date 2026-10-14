#pragma optimize("s", on)
// NPlayerGui::CCrimeStatBar::CCrimeStatBar(CStatBarInfo*) -- base CStatBase(info), NormalisedTimeLeft zeroed, own vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00654392, length 29 bytes.
// Callees: NPlayerGui::CStatBase::CStatBase @ 0x0065431D. Vtable 0x0125A748 = NPlayerGui::CCrimeStatBar (rtti-vtables.tsv).
// Shape: the size-optimised TU (odd address in the /Os region) forwards the argument with `push dword ptr [esp+8]` and
// stores the float zero through the x87 (`fldz ; fstp`). PDB CCrimeStatBar (128 bytes) puts NormalisedTimeLeft at
// +0x7c; retail stores it at +0x6c (the base is 0x10 bytes smaller in the shipped build), so the layout is local.
struct CStatBarInfo;

namespace NPlayerGui {

struct CStatBase {
    virtual void v00();
    unsigned char _body[0x6c - 4];             // +0x04 .. +0x6c: the stat-bar base state
    CStatBase(CStatBarInfo* info);             // 0x0065431D
};

struct CCrimeStatBar : CStatBase {             // vtable 0x0125A748
    virtual void v00();
    float NormalisedTimeLeft;                  // +0x6c (PDB +0x7c)
    CCrimeStatBar(CStatBarInfo* info);
};

CCrimeStatBar::CCrimeStatBar(CStatBarInfo* info)
    : CStatBase(info), NormalisedTimeLeft(0.0f)
{
}

}
