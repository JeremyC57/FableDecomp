// CTCQuestCard::SetRegionName(const CCharString& name) -- when the name differs, store it and refresh the player GUI
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00702560, length 53 bytes.
// Callees: CCharString::operator!=(const CCharString&) const @ 0x0099E900 (on the argument; the manifest's
// "less<CCharString>::operator()" is propagated, the landed 0x0099E900 is CCharString_NotEqual), CCharString::operator= @
// 0x0099EFB0, the player-GUI refresh @ 0x004378C0 on the global 0x013B8790 (as the landed
// CGameScriptInterface::SetRegionExitAsActive 0x0089EEC0 names them; the label "_Cons_val<...>" is propagated).
// Shape: `name != RegionName` with the ARGUMENT as `this` (`mov ecx,edi ; push esi`); the refresh is the last statement, a void
// thiscall, so VC7.1 tail-jumps it and rewrites the dead argument slot with the 0 (`mov [esp+4],0 ; jmp`).
// Layout: engine/CTCQuestCard.h (Ego_r.pdb): RegionName (CCharString, one data pointer) at +0x20; the header flattens the
// string to its data pointer, so an inline accessor views that member as the CCharString it is.
#include "engine/CTCQuestCard.h"

struct CCharString {
    CCharStringData* PStringData;
    bool operator!=(const CCharString& other) const;        // 0x0099E900
    CCharString& operator=(const CCharString& other);       // 0x0099EFB0
};

struct CPlayerGui {
    void Refresh(long flag);                                // 0x004378C0
};
extern CPlayerGui* g_pPlayerGui;                            // 0x013B8790

struct CTCQuestCard_Methods : CTCQuestCard {
    CCharString& RegionName() { return *reinterpret_cast<CCharString*>(&RegionName_PStringData); }
    void SetRegionName(const CCharString& name);
};

void CTCQuestCard_Methods::SetRegionName(const CCharString& name)
{
    if (name != RegionName()) {
        RegionName() = name;
        g_pPlayerGui->Refresh(0);
    }
}
