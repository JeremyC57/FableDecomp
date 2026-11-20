// CFrameData_004aecf0::CFrameData_004aecf0() -- a non-polymorphic 0x20-byte record: a CCharString at +0xc, two other string objects at +0x10 and +0x14 (constructor 0x0099AED0), two flag bytes at +0x19/+0x1a and the word at +0x1c cleared
// Leaf: ??0CFrameData_004aecf0@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AECF0, length 42 bytes.
// Callees: CCharString::CCharString @ 0x0099E4B0 (+0xc), the string constructor @ 0x0099AED0 twice (+0x10, +0x14; labelled CCharString::CCharString, another
// overload). Shape: no vtable store, the three clears share eax, `this` is returned. The manifest label "CFrameData::ResetTexture" is propagated.
#pragma optimize("s", on)
// NF193 (q4546 / q6309): the two string members at +0x10 / +0x14 are CWideString (their constructor 0x0099AED0 is the PDB
//   ??0CWideString@@QAE@XZ, Ego_r.pdb / FableWin.pdb name sets) and the +0xc member is CCharString (??0CCharString@@QAE@XZ 0x0099E4B0):
//   both from the shared fable_string.h instead of the local CCharString / CNameString_0099aed0 stand-ins.
#include "fable_string.h"

struct CFrameData_004aecf0 {
    unsigned char        _pad[0xc];               // +0x00 .. +0x0c (not initialised)
    CCharString          m_name;                   // +0x0c
    CWideString          m_textA;                  // +0x10
    CWideString          m_textB;                  // +0x14
    unsigned char        _pad18;                   // +0x18 (not initialised)
    bool                 m_flagA;                  // +0x19
    bool                 m_flagB;                  // +0x1a
    unsigned char        _pad1b;
    long                 m_value;                  // +0x1c
    CFrameData_004aecf0();
};

CFrameData_004aecf0::CFrameData_004aecf0()
    : m_flagA(false), m_flagB(false), m_value(0)
{
}