// SIcicleRecord_008e2e00::SIcicleRecord_008e2e00() -- build a 0x18-byte record: two default intelligent pointers at +0 and +8 (vtable 0x01238C6C), the word at +0x10 cleared and the word at +0x14 set to -1
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E2E00, length 64 bytes.
// The chunk label "CIcicleData::CIcicleData" is propagated; named by address. Shape: the second pointer's address in edi before the first vtable store, `mov eax,esi`.
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CThing;
struct SIcicleRecord_008e2e00 {
    CIntelligentPointer<CThing> m_pointer0;       // +0x00
    CIntelligentPointer<CThing> m_pointer8;       // +0x08
    long                         m_10;             // +0x10
    long                         m_14;             // +0x14
    SIcicleRecord_008e2e00();
};

SIcicleRecord_008e2e00::SIcicleRecord_008e2e00()
    : m_pointer0(), m_pointer8(), m_10(0), m_14(-1)
{
}