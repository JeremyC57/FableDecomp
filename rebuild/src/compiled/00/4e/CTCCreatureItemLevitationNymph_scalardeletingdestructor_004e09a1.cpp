// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
#pragma optimize("s", on)
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
extern void __fastcall CTCCreatureItemLevitationNymph_Destruct(void* nymph);
void* __fastcall CTCCreatureItemLevitationNymph_CompositeScalarDeletingDestructor(void* nymph, int, unsigned int flags)
{
    ((CBaseIntelligentPointer*)(static_cast<unsigned char*>(nymph) + 40))->CBaseIntelligentPointer::~CBaseIntelligentPointer();
    CTCCreatureItemLevitationNymph_Destruct(nymph);
    if (flags & 1)
        operator delete(nymph);
    return nymph;
}