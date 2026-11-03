#ifndef FABLE_ENGINE_CTCVILLAGEMEMBER_H
#define FABLE_ENGINE_CTCVILLAGEMEMBER_H
// Retail model (hand-written 2026-11-03, row 0x005EB9E0 CTCTavernGame::FrameUpdate): the thing component CTCVillageMember
// (interface TCI_VILLAGE_MEMBER = 0x23). RTTI .?AVCTCVillageMember@@ : CTCBase. Members (Ego_r.pdb, struct_layouts_egor.tsv,
// sizeof 0x20, no containers so the retail offsets are the PDB's): PDef (CDefPointer<const CVillageMemberDef>) +0x0C, PVillage
// (CIntelligentPointer<CThing>) +0x10, VillageUID +0x18. Methods: the Ego_r.pdb / FableWin.pdb member set; GetPVillage is out of
// line in Ego_r and retail (0x007EE540, `?GetPVillage@CTCVillageMember@@QBEPAVCTCVillage@@XZ`, the only spelling bound to it);
// StatGetInterfaceType is inline (the constant 0x23 folds into the callers' lookups). Needs the polymorphic CTCBase
// (FABLE_RETAIL_CTCBASE) and the packed intelligent pointers (FABLE_RETAIL_CINTELLIGENTPOINTER); defines both when the includer
// has not.
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
class CTCVillage;
class CVillageMemberDef;

class CTCVillageMember : public CTCBase {
public:
    static CTCBase* __fastcall Alloc(CThing&);                           // ?Alloc@CTCVillageMember@@SIPAVCTCBase@@AAVCThing@@@Z
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_VILLAGE_MEMBER; }   // ?StatGetInterfaceType@CTCVillageMember@@SI?AW4ETCInterfaceType@@XZ
    static CCharString __fastcall StatGetClassTypeName();                // ?StatGetClassTypeName@CTCVillageMember@@SI?AVCCharString@@XZ
    virtual void OnCreate();                                             // ?OnCreate@CTCVillageMember@@UAEXXZ
    virtual void OnInitialActivate();                                    // ?OnInitialActivate@CTCVillageMember@@UAEXXZ
    virtual void OnKill();                                               // ?OnKill@CTCVillageMember@@UAEXXZ
    virtual void OnSerialise(CPersistContext&, bool);                    // ?OnSerialise@CTCVillageMember@@UAEXAAVCPersistContext@@_N@Z
    virtual ETCInterfaceType GetInterfaceType() const;                   // ?GetInterfaceType@CTCVillageMember@@UBE?AW4ETCInterfaceType@@XZ
    virtual CCharString GetClassTypeName() const;                        // ?GetClassTypeName@CTCVillageMember@@UBE?AVCCharString@@XZ
    virtual bool IsComponentSerialisable() const;                        // ?IsComponentSerialisable@CTCVillageMember@@UBE_NXZ
    CTCVillage* GetPVillage() const;                                     // 0x007EE540
    void SetPVillage(CTCVillage*);                                       // ?SetPVillage@CTCVillageMember@@QAEXPAVCTCVillage@@@Z
    bool IsAttachedToVillage() const;                                    // ?IsAttachedToVillage@CTCVillageMember@@QBE_NXZ
private:
    void RunAttachmentDebug();                                           // ?RunAttachmentDebug@CTCVillageMember@@AAEXXZ
    CDefPointer<const CVillageMemberDef> PDef;                           // +0x0C
    CIntelligentPointer<CThing>          PVillage;                       // +0x10
    unsigned __int64                     VillageUID;                     // +0x18
public:
    static void CheckRetailLayout() {
        FABLE_STATIC_ASSERT(offsetof(CTCVillageMember, PDef) == 0x0c);
        FABLE_STATIC_ASSERT(offsetof(CTCVillageMember, PVillage) == 0x10);
        FABLE_STATIC_ASSERT(offsetof(CTCVillageMember, VillageUID) == 0x18);
    }
};
FABLE_STATIC_ASSERT(sizeof(CTCVillageMember) == 0x20);

#endif // FABLE_ENGINE_CTCVILLAGEMEMBER_H
