// CGamePlayerInterface::GetToggleExpressionShift() const -- returns the controls definition's ToggleExpressionShift flag (CControlsDef +0x4c through PControlsDef +0x7a0)
// Leaf: CanBeHitByMeleeStrike
// Retail: Fable.exe @ 0x00445BB0, 10 bytes. Catalog label NThingShot::CTCShotDefinitionBase::CanBeHitByMeleeStrike is wrong
// (three byte-near siblings 0x00445BA0/BB0/BC0 share it). Evidence: callers CInputProcessControlCreature::Update / ProcessInput (0x00688221, 0x00688857) load `this` from their
// +0x24 player interface; Ego_r.pdb CGamePlayerInterface::PControlsDef (CDefPointer<CControlsDef const>) is at +0x7a0
// (engine/CGamePlayerInterface.h); CControlsDef::ToggleExpressionShift is PDB +0x54, retail +0x4c (def layouts sit 4 below the
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
    bool GetToggleExpressionShift() const;
};

bool CGamePlayerInterface_Methods::GetToggleExpressionShift() const
{
    return GetControlsDef()->ToggleExpressionShift;
}