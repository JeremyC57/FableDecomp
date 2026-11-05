// Leaf: ?CPlayerDef_Copy@@YIXPAVCCardDef@@HPBVCDefClassBase@@@Z
#pragma optimize("s",on)
// CCardDef::Copy  0x004ec0f0
// __fastcall void CCardDef::Copy(CCardDef* this, CDefClassBase const* src)
// Modeled as __fastcall with an edx dummy so `src` lands on the stack (true __fastcall shape).

class CDefClassBase;

class CCardDef { public:
    char pad[0x3c];
    unsigned int f3c;   // +0x3c
    unsigned int CardName;   // +0x40
};

// base-class Copy (non-virtual direct call @ 0x431ed0), __fastcall(this, src)
extern "C" void __fastcall BaseCopy(CCardDef* self, int edx_, CDefClassBase const* src);

void __fastcall CPlayerDef_Copy(CCardDef* self, int edx_, CDefClassBase const* src)
{
    BaseCopy(self, edx_, src);
    self->f3c = ((CCardDef const*)src)->f3c;
    self->CardName = ((CCardDef const*)src)->CardName;
}
// Stand-in (owner rule 2026-11-05): CPlayerDef_Copy for CPlayerDef::Copy; RTTI vtable slot ?Copy@CPlayerDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (fablewin).
