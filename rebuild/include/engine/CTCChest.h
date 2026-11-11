#ifndef FABLE_ENGINE_CTCCHEST_H
#define FABLE_ENGINE_CTCCHEST_H
// Chunk 1010 (row 0x00896390 CGameScriptInterface::OpenChest): the chest component (interface TCI_CONTAINER 0x3C, the
// `mov [esp+10h],3Ch ; call 0x0040F020` lookup) as script code reaches it. Methods only, no layout (the row holds a pointer):
//   ?GetOpenAnimNameForCreature@CTCChest@@QBE?AVCCharString@@XZ  0x00750240 (FableWin.pdb; the landed row spells it GetOpenAnimationForCreature)
//   ?Open@CTCChest@@QAEXXZ                                        0x00750540 (FableWin.pdb; landed row CTCChest_Open_00750540)
//   ?Close@CTCChest@@QAEXXZ                                       0x007506C0 (FableWin.pdb; landed row CTCChest_Close_007506C0)
// Needs the polymorphic CTCBase (FABLE_RETAIL_CTCBASE); defines it when the includer has not.
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "engine/CTCBase.h"
class CCharString;
class CTCChest : public CTCBase {
public:
    CCharString GetOpenAnimNameForCreature() const;                      // 0x00750240
    void Open();                                                         // 0x00750540
    void Close();                                                        // 0x007506C0
#ifdef FABLE_RETAIL_CTCCHEST_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
public:
    virtual bool HasFrameUpdate() const;  // slot 8, 0x00750950
public:
    virtual bool IsComponentSerialisable() const;  // slot 23, 0x00750970
public:
#endif
};
#endif
