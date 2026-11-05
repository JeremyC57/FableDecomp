// Retail 0x008fd56a: _Dest_val<pair<long,CRegisteredMusicEntry>> style destroy helper.
// Leaf: @_Dest_val_pair_long_CRegisteredMusicEntry@4
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// push esi; mov esi,ecx; lea ecx,[esi+0x34]; call 0xa01c10; mov ecx,esi; pop esi; jmp 0x8fd544
// VOID member: destroy the sub-object at this+0x34, then tail-call the base/other destroy on this.

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
extern "C" void __fastcall tail_8fd544(void* self);      // jmp  0x8fd544 (ecx = this)

extern "C" void __fastcall _Dest_val_pair_long_CRegisteredMusicEntry(void* self)
{
    ((CBaseIntelligentPointer*)((char*)self + 0x34))->CBaseIntelligentPointer::~CBaseIntelligentPointer();
    tail_8fd544(self);
}