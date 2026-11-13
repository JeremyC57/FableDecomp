#ifndef FABLE_ENGINE_CTCLIGHTNINGNODE_H
#define FABLE_ENGINE_CTCLIGHTNINGNODE_H
// New shared header (w22 agent2022, row 0x007F26B0 CTCLightningOrb::Initialise): the lightning-node component the orb attaches to its
// thing. RTTI .?AVCTCLightningNode@@ : CTCBase. Members: Ego_r.pdb names and order at the retail offsets (no container before the vector):
// PParent CIntelligentPointer<CThing> +0x0C, PSourceOfFaction CIntelligentPointer<const CThing> +0x14, PSourceOfHit CIntelligentPointer<CThing>
// +0x1C, PDef CDefPointer<const CLightningDef> +0x24, Timer +0x28, LightningBranches vector<CCountedPointer<CLightningBranch> > +0x2C (12-byte
// STLport vector), sizeof 0x38 (the landed SetPDef 0x007F3A90's audit). Setters: Ego_r.pdb / FableWin.pdb ?SetPParent@CTCLightningNode@@QAEXAAVCThing@@@Z,
// ?SetPSourceOfFaction@..QAEXPBVCThing@@@Z, ?SetPSourceOfHit@..QAEXPAVCThing@@@Z, ?SetPDef@..QAEXABV?$CDefPointer@$$CBVCLightningDef@@@@@Z, inline
// (class body): retail keeps out-of-line copies at 0x007F39A0 (writes +0x0C through the CThing pointer vtable 0x01238C6C) / 0x007F39F0 (+0x14,
// the const-CThing vtable 0x01237F1C) / 0x007F3A40 (+0x1C) / 0x007F3A90 (SetPDef), the bodies `P.. = pointer` (a temporary pointer, then
// GetPItem / SetPItem: FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN). Needs FABLE_RETAIL_CTCBASE, FABLE_RETAIL_CINTELLIGENTPOINTER(_ASSIGN),
// FABLE_RETAIL_DEF_CLASS_BASE.
#include <stddef.h>
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"
class CThing;
class CLightningDef;

class CTCLightningNode : public CTCBase {
public:
    class CLightningBranch;
    CTCLightningNode(CThing&);                                           // ??0CTCLightningNode@@QAE@AAVCThing@@@Z
    virtual ~CTCLightningNode();
    void SetPParent(CThing& parent) { PParent = &parent; }               // ?SetPParent@CTCLightningNode@@QAEXAAVCThing@@@Z (0x007F39A0)
    void SetPSourceOfFaction(const CThing* source) { PSourceOfFaction = source; }   // ?SetPSourceOfFaction@..QAEXPBVCThing@@@Z (0x007F39F0)
    void SetPSourceOfHit(CThing* source) { PSourceOfHit = source; }      // ?SetPSourceOfHit@..QAEXPAVCThing@@@Z (0x007F3A40)
    void SetPDef(const CDefPointer<const CLightningDef>& def) { PDef = def; }   // ?SetPDef@..QAEXABV?$CDefPointer@$$CBVCLightningDef@@@@@Z (0x007F3A90)
    CIntelligentPointer<CThing>                          PParent;              // +0x0c
    CIntelligentPointer<const CThing>                    PSourceOfFaction;     // +0x14
    CIntelligentPointer<CThing>                          PSourceOfHit;         // +0x1c
    CDefPointer<const CLightningDef>                     PDef;                 // +0x24
    long                                                 Timer;                // +0x28
    std::vector<CCountedPointer<CLightningBranch> >      LightningBranches;    // +0x2c
};
FABLE_STATIC_ASSERT(offsetof(CTCLightningNode, PSourceOfFaction) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CTCLightningNode, PDef) == 0x24);
FABLE_STATIC_ASSERT(sizeof(CTCLightningNode) == 0x38);
#endif
