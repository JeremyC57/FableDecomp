// CTCActionUseScriptedHook::SetEntranceAsLocked  @ 0x00729c30
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// __fastcall void(CTCActionUseScriptedHook*, bool)

class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct Entrance
{
    // method at 0x00a01b50 : __fastcall, returns pointer/bool in eax
    char data[8];   // occupies +0x20..+0x27
};

class CTCActionUseScriptedHook
{ public:
    char pad0[0x20];
    Entrance m_entrance;   // +0x20
    char m_locked;         // +0x28

    void SetEntranceAsLocked(bool locked);
};

void CTCActionUseScriptedHook::SetEntranceAsLocked(bool locked)
{
    if (((void*)((CBaseIntelligentPointer*)&(this->m_entrance))->GetPItem()))
    {
        this->m_locked = locked;
    }
}