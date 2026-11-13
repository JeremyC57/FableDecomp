#ifndef FABLE_ENGINE_CTCCONTAINER_H
#define FABLE_ENGINE_CTCCONTAINER_H
#include "engine/CTCBase.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
// Layout: native RTTI1269424 places CTCContainer between RewardHero and CTCBase.
// Ego_r ContainerItems+12; native STLport vector is12 bytes rather than donor16.
class CTCContainer : public CTCBase {
#ifdef FABLE_RETAIL_CTCCONTAINER_ITEMS
    // Retail opt-in (chunk 2039, row 0x006ACA30 CThingPlayerCreature::OnCollisionWithObject): FableWin.pdb
    // ?StatGetInterfaceType@CTCContainer@@SI?AW4ETCInterfaceType@@XZ (TCI_CONTAINER = 60, the row's PeekTC key 3Ch) and the inline
    // ?GetContainerItems@CTCContainer@@QBEABV?$vector@U?$pair@JVCCharString@@@std@@V?$allocator@U?$pair@JVCCharString@@@std@@@2@@std@@XZ
    // (0x01DF3910, first COMDAT copy after OnCollisionWithObject; no Ego_r.pdb or Fable.exe copy: the row reads _M_start / _M_finish
    // at +0x0C / +0x10 off the component in place). No layout change.
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_CONTAINER; }
    const std::vector<std::pair<long,CCharString> >& GetContainerItems() const { return ContainerItems; }
#endif
protected:
    std::vector<std::pair<long,CCharString> > ContainerItems;
#ifdef FABLE_RETAIL_CTCCONTAINER_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
public:
    virtual bool IsComponentSerialisable() const;  // slot 23, 0x0074FCA0
public:
#endif
};
#endif
