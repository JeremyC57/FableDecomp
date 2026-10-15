// NUISystem::CMenuEntry::CMenuEntry(long) -- menu entry ctor: CClickable base, own three vtables, available, no action lists yet, then set up the rumble actions
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0053DFE0, 103 bytes.
// Shape: CClickable(long) (0x0055B460); CMenuEntry's vtables for the three polymorphic subobjects
// (+0 0x0124662C, +4 0x01246604, +0x18 0x012465FC, rtti NUISystem::CMenuEntry); IsAvailable = true and
// the eight action-list pointers zeroed from one register (initialisers); InitializeRumbleActions()
// (0x0053DA30) runs in the body. Field names from the PDB CMenuEntry (retail offsets sit 0x30 below
// the PDB's: IsAvailable +0x198 vs PDB +0x1c8).
struct CActionList;

namespace NUISystem {

struct CClickableBase0 { virtual void v00(); };
struct CClickableBase1 { virtual void v10(); unsigned char _body[0x10]; };
struct CClickableBase2 { virtual void v20(); };

struct CClickable : CClickableBase0, CClickableBase1, CClickableBase2 {
    unsigned char _body[0x198 - 0x1c];
    CClickable(long id);                            // 0x0055B460
};

struct CMenuEntry : CClickable {
    bool         IsAvailable;                       // +0x198
    CActionList* PUIActionParams;                   // +0x19c
    CActionList* PThingActionParams;                // +0x1a0
    CActionList* PActions;                          // +0x1a4
    CActionList* PActionsOnSelected;                // +0x1a8
    CActionList* PActionsOnBack;                    // +0x1ac
    CActionList* PPreActions;                       // +0x1b0
    CActionList* PActionsOnUnselected;              // +0x1b4
    CActionList* PActionsOnVisible;                 // +0x1b8
    virtual void v00();
    virtual void v10();
    virtual void v20();
    CMenuEntry(long id);
    void InitializeRumbleActions();                 // 0x0053DA30
};

CMenuEntry::CMenuEntry(long id)
    : CClickable(id)
    , IsAvailable(true)
    , PUIActionParams(0)
    , PThingActionParams(0)
    , PActions(0)
    , PActionsOnSelected(0)
    , PActionsOnBack(0)
    , PPreActions(0)
    , PActionsOnUnselected(0)
    , PActionsOnVisible(0)
{
    InitializeRumbleActions();
}

}