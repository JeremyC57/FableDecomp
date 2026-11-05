// SIcicleRecord_0078e4a0::SIcicleRecord_0078e4a0(const SIcicleRecord_0078e4a0& other) -- copy a 0x18-byte record: the intelligent pointer at +0 (vtable 0x0123E7D8), the word at +8, the intelligent pointer at +0xc (vtable 0x01238C6C), the word at +0x14
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0078E4A0, length 87 bytes.
// The chunk label "CIcicleData::CIcicleData" is propagated over several unrelated records; named by address. Shape: the intelligent-pointer copy = base constructor, vtable, `SetPItem(other.GetPItem())` (0x00A01B10 / 0x00A01B50 / 0x00A01B90); `this` in esi, `other` in edi, `mov eax,esi`, `ret 4`.
class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct CIntelligentPointerA_0078e4a0 : CBaseIntelligentPointer {   // vtable 0x0123E7D8
    virtual CBaseObject v00();
    CIntelligentPointerA_0078e4a0(const CIntelligentPointerA_0078e4a0& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct CIntelligentPointerB_0078e4a0 : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual CBaseObject v00();
    CIntelligentPointerB_0078e4a0(const CIntelligentPointerB_0078e4a0& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct SIcicleRecord_0078e4a0 {
    CIntelligentPointerA_0078e4a0 m_pointer0;           // +0x00
    long                      m_8;                 // +0x08
    CIntelligentPointerB_0078e4a0 m_pointerc;           // +0x0c
    long                      m_14;                // +0x14
    SIcicleRecord_0078e4a0(const SIcicleRecord_0078e4a0& other);
};

SIcicleRecord_0078e4a0::SIcicleRecord_0078e4a0(const SIcicleRecord_0078e4a0& other)
    : m_pointer0(other.m_pointer0), m_8(other.m_8), m_pointerc(other.m_pointerc), m_14(other.m_14)
{
}