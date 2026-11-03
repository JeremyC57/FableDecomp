#ifndef FABLE_ENGINE_CTCOWNEDENTITY_H
#define FABLE_ENGINE_CTCOWNEDENTITY_H
// Retail model (hand-written 2026-11-03, row 0x005EB9E0 CTCTavernGame::FrameUpdate): the thing component CTCOwnedEntity
// (interface TCI_OWNED_ENTITY = 0x33). RTTI .?AVCTCOwnedEntity@@ : CTCBase (ghidra_out/rtti_bases.tsv). Members (Ego_r.pdb,
// struct_layouts_egor.tsv, sizeof 0x20, no containers so the retail offsets are the PDB's): SwitchableNavigationTCAdded +0x0C,
// POwner (CIntelligentPointer<CThing>, byte-packed) +0x0D, OwnerUID +0x18. Retail reads POwner at +0x0D (0x005EBACD
// `lea edi,[eax+0Dh]`, then GetPItem 0x00A01B50), so this header needs the packed pointer classes (FABLE_RETAIL_CINTELLIGENTPOINTER)
// and the polymorphic CTCBase (FABLE_RETAIL_CTCBASE); it defines both when the includer has not.
// Methods: the Ego_r.pdb / FableWin.pdb member set. GetPOwner is FableWin-only (inline in Ego_r and retail: two GetPItem calls on
// POwner at 0x005EBAD2 / 0x005EBADD); StatGetInterfaceType is inline (the constant 0x33 folds into the callers' lookups).
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#ifndef FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#endif
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
class CCharString;
class CPersistContext;

class CTCOwnedEntity : public CTCBase {
public:
    static CTCBase* __fastcall Alloc(CThing&);                           // ?Alloc@CTCOwnedEntity@@SIPAVCTCBase@@AAVCThing@@@Z
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_OWNED_ENTITY; }   // ?StatGetInterfaceType@CTCOwnedEntity@@SI?AW4ETCInterfaceType@@XZ
    static CCharString __fastcall StatGetClassTypeName();                // ?StatGetClassTypeName@CTCOwnedEntity@@SI?AVCCharString@@XZ
    virtual void InitialActivate();                                      // ?InitialActivate@CTCOwnedEntity@@UAEXXZ
    virtual void OnInitialActivate();                                    // ?OnInitialActivate@CTCOwnedEntity@@UAEXXZ
    virtual bool IsSerialiseCreatable() const;                           // ?IsSerialiseCreatable@CTCOwnedEntity@@UBE_NXZ
    virtual ETCInterfaceType GetInterfaceType() const;                   // ?GetInterfaceType@CTCOwnedEntity@@UBE?AW4ETCInterfaceType@@XZ
    virtual CCharString GetClassTypeName() const;                        // ?GetClassTypeName@CTCOwnedEntity@@UBE?AVCCharString@@XZ
    virtual bool IsComponentSerialisable() const;                        // ?IsComponentSerialisable@CTCOwnedEntity@@UBE_NXZ
    CThing* GetPOwner() const { return POwner.Get(); }                   // FableWin ?GetPOwner@CTCOwnedEntity@@QBEPAVCThing@@XZ (inline)
    void SetPOwner(CThing*);                                             // ?SetPOwner@CTCOwnedEntity@@QAEXPAVCThing@@@Z
    void OnForceRecreateUID();                                           // ?OnForceRecreateUID@CTCOwnedEntity@@QAEXXZ
protected:
    virtual void OnSerialise(CPersistContext&, bool);                    // ?OnSerialise@CTCOwnedEntity@@MAEXAAVCPersistContext@@_N@Z
private:
    bool                        SwitchableNavigationTCAdded;             // +0x0C
    CIntelligentPointer<CThing> POwner;                                  // +0x0D
    unsigned __int64            OwnerUID;                                // +0x18
public:
    static void CheckRetailLayout() {
        FABLE_STATIC_ASSERT(offsetof(CTCOwnedEntity, SwitchableNavigationTCAdded) == 0x0c);
        FABLE_STATIC_ASSERT(offsetof(CTCOwnedEntity, POwner) == 0x0d);
        FABLE_STATIC_ASSERT(offsetof(CTCOwnedEntity, OwnerUID) == 0x18);
    }
};
FABLE_STATIC_ASSERT(sizeof(CTCOwnedEntity) == 0x20);

#endif // FABLE_ENGINE_CTCOWNEDENTITY_H
