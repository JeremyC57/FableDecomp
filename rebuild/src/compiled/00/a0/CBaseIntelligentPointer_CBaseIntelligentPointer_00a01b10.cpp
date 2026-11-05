// CBaseIntelligentPointer::CBaseIntelligentPointer() -- sets the vtable (0x0129C95C) and nulls the record pointer at +4
// Leaf: ??0CBaseIntelligentPointer@@QAE@XZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A01B10, oracle length 16 bytes.
// Identity: Ego_r.pdb + FableWin.pdb ??0CBaseIntelligentPointer@@QAE@XZ; the vtable stored is 0x0129C95C
//   (RTTI .?AVCBaseIntelligentPointer@@). Ego_r layout: vptr + CBaseObjectPointer* PData (+0x04).
class CBaseObject;
class CBaseObjectPointer {                             // Ego_r.pdb, 8 bytes
public:
    CBaseObject*  object;                              // +0x00 the observed object, 0 once it died
    unsigned long refcount;                            // +0x04 intelligent pointers sharing this record
};
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

CBaseIntelligentPointer::CBaseIntelligentPointer()
    : PData(0)
{
}
