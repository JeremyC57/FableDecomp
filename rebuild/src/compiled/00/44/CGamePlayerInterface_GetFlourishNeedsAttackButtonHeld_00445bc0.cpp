// CGamePlayerInterface::GetFlourishNeedsAttackButtonHeld() const -- returns the controls definition's FlourishNeedsAttackButtonHeld flag (CControlsDef +0x4d through PControlsDef +0x7a0)
// Retail: Fable.exe @ 0x00445BC0, 10 bytes. Catalog label NThingShot::CTCShotDefinitionBase::CanBeHitByMeleeStrike is wrong
// (three byte-near siblings 0x00445BA0/BB0/BC0 share it). Evidence: callers CInputProcessCombat::ProcessCombatInput 0x0068F6FD load `this` from their
// +0x24 player interface; Ego_r.pdb CGamePlayerInterface::PControlsDef (CDefPointer<CControlsDef const>) is at +0x7a0
// (engine/CGamePlayerInterface.h); CControlsDef::FlourishNeedsAttackButtonHeld is PDB +0x55, retail +0x4d (def layouts sit 4 below the
// PDB and the Controls vector is a 12-byte STLport vector: ToggleZTarget +0x48 .. FlourishNeedsAttackButtonHeld +0x4d).
// Shape: reading the def through the counted def pointer's inline operator->.
#include "engine/CGamePlayerInterface.h"

struct CControlsDef {                                    // retail layout
    unsigned char _base[0x3c];                           // CDefClassBase
    void*         Controls[3];                           // +0x3c vector<CActionInputControl>
    bool          ToggleZTarget;                         // +0x48
    bool          ToggleSpells;                          // +0x49
    bool          ToggleSneak;                           // +0x4a
    bool          ToggleExpressionMenu;                  // +0x4b
    bool          ToggleExpressionShift;                 // +0x4c
    bool          FlourishNeedsAttackButtonHeld;         // +0x4d
};
struct CGamePlayerInterface_Methods : CGamePlayerInterface {
    const CControlsDef* GetControlsDef() const { return (const CControlsDef*)PControlsDef_Object; }
    bool GetFlourishNeedsAttackButtonHeld() const;
};

bool CGamePlayerInterface_Methods::GetFlourishNeedsAttackButtonHeld() const
{
    return GetControlsDef()->FlourishNeedsAttackButtonHeld;
}