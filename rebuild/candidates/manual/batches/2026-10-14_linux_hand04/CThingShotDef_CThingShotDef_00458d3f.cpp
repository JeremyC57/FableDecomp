#pragma optimize("s", on)
// CThingShotDef::CThingShotDef() -- CParentDefClassBase(), vtable, a def index at -1 and four zero floats
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00458D3F, oracle length 42 bytes. Size-optimised TU
// (`#pragma optimize("s", on)`: the `or dword ptr [mem],-1` idiom and the fldz/fstp float zeroes are the
// /Os forms; the /Ot build stores immediates and DIFFERs).
// Callees: CParentDefClassBase::CParentDefClassBase @ 0x00430370 (landed). Vtable 0x01234E2C = CThingShotDef
// (rtti-vtables.tsv; the next function, 0x00458D69, is CThingShotDef::GetSizeofClass). The manifest label
// "C3DSkeletalMorph::C3DSkeletalMorph" is propagated and rejected.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CParentDefClassBase::CParentDefClassBase ; fldz ; mov [esi],offset vftable ;
//                or dword ptr [esi+60h],-1 ; fstp [esi+64h] ; fldz ; mov eax,esi ; fstp [esi+68h] ; fldz ; fstp [esi+6Ch] ;
//                fldz ; fstp [esi+70h] ; pop esi ; ret
// Shape: CParentDefClassBase is 0x60 bytes in retail; the def's own members start at +0x60. The vtable
// store precedes the `-1` store, which VC7.1 only does when that member is a sub-object with an inline
// constructor (a plain `long` in the init list sinks the vtable store below it, tried: DIFFER), so the
// index is modelled as a small def-index wrapper that defaults to -1 (= no def); a C3DVector with an
// inline zeroing constructor and a float at 0.0f follow. The PDB CThingShotDef layout (176 bytes, Speed at
// +64) is from a different build and does not match the retail offsets, so member names are descriptive.
struct CParentDefClassBase {
    virtual void v00();
    unsigned char m_state[0x60 - 4];       // +0x4 .. +0x60: base state, untouched here
    CParentDefClassBase();                 // 0x00430370
};

struct CDefIndex {                         // a def reference by global index, -1 = none
    long m_index;
    CDefIndex() : m_index(-1) { }
};

struct C3DVector {
    float X, Y, Z;
    C3DVector() : X(0.0f), Y(0.0f), Z(0.0f) { }
};

struct CThingShotDef : CParentDefClassBase {
    virtual void v00();                    // own vtable (0x01234E2C)
    CDefIndex m_objectDef;                 // +0x60
    C3DVector m_offset;                    // +0x64
    float     m_speed;                     // +0x70
    CThingShotDef();
};

CThingShotDef::CThingShotDef()
    : CParentDefClassBase(), m_objectDef(), m_offset(), m_speed(0.0f)
{
}
