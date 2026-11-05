// _global::FUN_006d4d00() -- CIntelligentPointer<CThing> raw-pointer accessor: forwards to CBaseIntelligentPointer::GetPItem
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) @ 0x006D4D00, 5 bytes: a lone `jmp 0x00A01B50` (tail call, `this` passed through).
//   Unreferenced out-of-line copy of an inline typed accessor (the next function, 0x006D4D10, is the
//   CIntelligentPointer<const CThing> constructor).
class CThing;
class CBaseObject;

#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

struct CIntelligentPointer_CThing : CBaseIntelligentPointer {
    CThing* FUN_006d4d00() const;
};

CThing* CIntelligentPointer_CThing::FUN_006d4d00() const
{
    return (CThing*)GetPItem();
}