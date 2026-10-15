// CTCInGameMenu::SetPDef(long globalDefIndex) -- remembers the definition's global index and resolves the inventory definition pointer through the game definition manager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0047D5F2, length 27 bytes.
// Callees: CTCBase::GetDefinitionManager() const 0x006869D0, CDefinitionManager::GetDef<CInventoryDef>(long,
//          CDefPointer<const CInventoryDef>*) const 0x0048591C (manifest label names the template
//          instance after CSpecialAbilitiesDrunkennessDef: one body serves every 4-byte CDefPointer).
// Ego_r PDB: ?SetPDef@CTCInGameMenu@@QAEXJ@Z. engine/CTCInGameMenu.h: PDef (CDefPointer<const
// CInventoryDef>, its single Object word) +0x10, GlobalDefIndex +0x14.
// Shape: the GetDef arguments (`lea edx,[ecx+10h]` and the index) are pushed before the
// GetDefinitionManager call that yields GetDef's `this`; the index store is scheduled between the
// pushes and the call; GetDef's bool result is dropped (`call ; ret 4`, no tail jump because the
// argument counts differ).
#include "engine/CTCInGameMenu.h"

struct CGameDefinitionManager {
    bool GetDef(long globalDefIndex, CDefPointeeBase** pDefObject) const;   // 0x0048591C (CDefPointer<T>* = its Object word)
};

struct CTCInGameMenu_Methods : CTCInGameMenu {
    const CGameDefinitionManager& GetDefinitionManager() const;              // CTCBase, 0x006869D0
    void SetPDef(long globalDefIndex);
};

void CTCInGameMenu_Methods::SetPDef(long globalDefIndex)
{
    GlobalDefIndex = globalDefIndex;
    GetDefinitionManager().GetDef(globalDefIndex, &PDef_Object);
}
