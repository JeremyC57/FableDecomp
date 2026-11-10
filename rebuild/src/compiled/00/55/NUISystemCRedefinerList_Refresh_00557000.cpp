// NUISystem::CRedefinerList::Refresh() -- rebuild the control-redefinition list from the profile's assigned inputs, re-lay the visible rows and the undefined warning, then refresh three sound-system categories
// Leaf: ?Refresh@CRedefinerList@NUISystem@@QAEXXZ
// Leaf: Refresh@CRedefinerList
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00557000, length 123 bytes.
// Sound manager (agentSND2, 2026-11-10): PSoundManager is the shared engine/CASoundManager.h one; the local CSoundSystem model is gone and its slots carry the header names (RefreshCategory -> SetGainFader slot 46).
// Callees: CRedefinerList::ClearList @ 0x005567B0, CTBaseSingleton<CUserProfileManager>::Get @ 0x0040D2A0 (static),
// CUserProfileManager::GetAssignedInputsPassive @ 0x004088E0, CRedefinerList::RefreshScriptThings(const vector&)
// @ 0x00556A40 (catalog name; it rebuilds the rows from the inputs), CScrollableList::UpdateVisibleList(float, float)
// @ 0x00555A90 (the base list, `this` at offset 0), CRedefinerList::UpdateUndefinedWarning(float) @ 0x00556810;
// the sound-system global @ 0x013B8394 (as in the landed CThingSoundEmitter rows), its vtable slot 46 (+0xb8) called
// with category flags 0x10, 0x80, 0x40.
// Shape: straight-line; `this` is dead before the global test (esi popped there); each virtual call re-reads the
// global (the call may change it).
class CActionInputControl;
struct CActionInputVector;                         // vector<CActionInputControl>

class CUserProfileManager { public:
    static CUserProfileManager& __fastcall Get();             // 0x0040D2A0
    const CActionInputVector& GetAssignedInputsPassive();   // 0x004088E0
};

#include "engine/CASoundManager.h"

namespace NUISystem {

class CScrollableList { public:
    void UpdateVisibleList(float offset, float time);   // 0x00555A90
};

class CRedefinerList : public CScrollableList { protected:
    void ClearList();                              // 0x005567B0 ?ClearList@CRedefinerList@NUISystem@@IAEXXZ (protected, both PDBs; respelled at the chunk 708 landing)
public:
    void RefreshScriptThings(const CActionInputVector& inputs);   // 0x00556A40
    void UpdateUndefinedWarning(float time);       // 0x00556810
    void Refresh();
};

void CRedefinerList::Refresh()
{
    ClearList();
    RefreshScriptThings(CUserProfileManager::Get().GetAssignedInputsPassive());
    UpdateVisibleList(0.0f, 0.2f);
    UpdateUndefinedWarning(0.2f);
    if (PSoundManager) {
        PSoundManager->SetGainFader(0x10, 0, 0);
        PSoundManager->SetGainFader(0x80, 0, 0);
        PSoundManager->SetGainFader(0x40, 0, 0);
    }
}

} // namespace NUISystem