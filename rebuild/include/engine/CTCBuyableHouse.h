#ifndef FABLE_ENGINE_CTCBUYABLEHOUSE_H
#define FABLE_ENGINE_CTCBUYABLEHOUSE_H
#ifdef FABLE_RETAIL_CTCBUYABLEHOUSE
// Retail opt-in (chunk 794, row 0x007039A0 CThingObject::OnHit): the real base (Fable.exe RTTI .?AVCTCBuyableHouse@@ : CTCBase) and the
// Ego_r.pdb members up to Rented (PDef +0x0C, IsScripted +0x10, OwnedByPlayer +0x11, Rented +0x12 -- the same offsets as the generated
// branch below; OnHit reads +0x11 / +0x12 off the interface-0xD5 component) with FableWin.pdb's inline accessors
// ?GetOwnedByPlayer@CTCBuyableHouse@@QBE_NXZ / ?GetRented@CTCBuyableHouse@@QBE_NXZ. The rest stays opaque (sizeof 0x4C as below).
#include "engine/CTCBase.h"
#include "engine/CDefPointer.h"
#ifdef FABLE_RETAIL_CTCBUYABLEHOUSE_WIFE
#include "engine/CIntelligentPointer.h"
#endif
#ifdef FABLE_RETAIL_CTCBUYABLEHOUSE_FRAMEUPDATE
#include "engine/CIntelligentPointer.h"
class CThing;
#endif
class CBuyableHouseDef;
class CTCBuyableHouse : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_BUYABLE_HOUSE; }
#if defined(FABLE_RETAIL_CTCWIFE_MARRY) || defined(FABLE_RETAIL_CTCBUYABLEHOUSE_WIFE)
    void SetWifeLivingHere(CThing*);
#endif
    bool GetOwnedByPlayer() const { return OwnedByPlayer; }
    bool GetRented() const { return Rented; }
private:
    CDefPointer<const CBuyableHouseDef> PDef;                // +0x0c
    bool IsScripted;                                         // +0x10
    bool OwnedByPlayer;                                      // +0x11
    bool Rented;                                             // +0x12
#if defined(FABLE_RETAIL_CTCBUYABLEHOUSE_FRAMEUPDATE)
    // Retail opt-in (chunk Q23, row 0x006C1250 ?FrameUpdate@CTCBuyableHouse@@UAEXXZ, CTCBase vtable slot 9): every Ego_r.pdb member
    // after Rented with its real type (struct_layouts_egor.tsv; the same offsets as the generated branch below and the opaque pads of
    // the branches that follow: DayNextRentIsDue +0x18, CurrentDressLevel +0x1C, MainSignPost +0x24 and DressingSignPost +0x2C
    // (CIntelligentPointer<CThing>, read through GetPItem 0x00A01B50 at 0x006C1291 / 0x006C13D6), FrameToNextCheckIfWeNeedToCreateAnything
    // +0x34, InitiallySetupDressLevel +0x40, VirtualMoneyBags +0x44, HasPreviouslyBeenBuyable +0x48), and the Ego_r.pdb / FableWin.pdb
    // member functions FrameUpdate reaches (same spellings and access in both PDBs: public QBE IsHouseBeingUsed(bool) 0x006BFB10,
    // GetRentsDue 0x006BF810, GetPriceForNextDressLevel 0x006BF920 and QAE SetupDressingAtLevel(long) 0x006C0330; private AAE
    // NeedsMainSignPost 0x006BFB90, NeedsDressingSignPost (no retail copy: inlined everywhere), CreateMainSignPost 0x006C0F00,
    // CreateDressingSignPost 0x006C0F70, CreateRentMoney 0x006C0C90). Includes FABLE_RETAIL_CTCBUYABLEHOUSE_WIFE's WifeLivingHere
    // (+0x38, same declaration); needs FABLE_RETAIL_CINTELLIGENTPOINTER (byte-packed pointer). No layout change.
public:
    virtual void FrameUpdate();                                          // slot 9  0x006C1250
    void SetupDressingAtLevel(long level);                               // 0x006C0330
    bool IsHouseBeingUsed(bool as_workplace) const;                      // 0x006BFB10
    long GetRentsDue() const;                                            // 0x006BF810
    long GetPriceForNextDressLevel() const;                              // 0x006BF920
private:
    bool NeedsMainSignPost();                                            // 0x006BFB90
    bool NeedsDressingSignPost();                                        // (inline everywhere in retail)
    void CreateMainSignPost();                                           // 0x006C0F00
    void CreateDressingSignPost();                                       // 0x006C0F70
    void CreateRentMoney();                                              // 0x006C0C90
    long DaySomeoneWillMoveIn;                                           // +0x14
    long DayNextRentIsDue;                                               // +0x18
    long CurrentDressLevel;                                              // +0x1c
    long MaxDressLevel;                                                  // +0x20
    CIntelligentPointer<CThing> MainSignPost;                            // +0x24
    CIntelligentPointer<CThing> DressingSignPost;                        // +0x2c
    long FrameToNextCheckIfWeNeedToCreateAnything;                       // +0x34
    CIntelligentPointer<CThing> WifeLivingHere;                          // +0x38
    bool InitiallySetupDressLevel;                                       // +0x40
    long VirtualMoneyBags;                                               // +0x44
    bool HasPreviouslyBeenBuyable;                                       // +0x48
    bool IsResidential;                                                  // +0x49
#elif defined(FABLE_RETAIL_CTCBUYABLEHOUSE_WIFE)
    // Retail opt-in (naming agent NF21, row 0x006D3C40 SetWifeLivingHere): FableWin.pdb ?SetWifeLivingHere@CTCBuyableHouse@@QAEXPAVCThing@@@Z
    // and ?GetWifeLivingHere@CTCBuyableHouse@@QBEPAVCThing@@XZ; Ego_r.pdb member CIntelligentPointer<CThing> WifeLivingHere +0x38 (the
    // pointer the row assigns). Needs FABLE_RETAIL_CINTELLIGENTPOINTER (byte-packed pointer). No layout change.
    unsigned char _pad_0x13[0x38 - 0x13];                    // +0x13 DaySomeoneWillMoveIn .. FrameToNextCheckIfWeNeedToCreateAnything (opaque)
    CIntelligentPointer<CThing> WifeLivingHere;              // +0x38
    unsigned char _pad_0x40[0x4c - 0x40];                    // +0x40 InitiallySetupDressLevel .. IsResidential (opaque)
#else
    unsigned char _pad_0x13[0x4c - 0x13];                    // +0x13 DaySomeoneWillMoveIn .. IsResidential (opaque)
#endif
#ifdef FABLE_RETAIL_CTCBUYABLEHOUSE_RECEIVEDCONFIRMATION
    // Retail opt-in (workflow wf8, row 0x0072EB40 ?ReceivedConfirmation@CTCActionUseBuyHouse@@AAEXAAVCTCBuyableHouse@@@Z): the public
    // Ego_r.pdb / FableWin.pdb member functions that row calls out of line (?GetPrice@CTCBuyableHouse@@QBEJXZ 0x006C0120,
    // ?SetRented@CTCBuyableHouse@@QAEX_N@Z 0x006C1150, ?SetOwnedByPlayer@CTCBuyableHouse@@QAEX_N@Z 0x006C17B0,
    // ?IncrementDressLevel@CTCBuyableHouse@@QAEXXZ 0x006C04B0; ?GetPriceForNextDressLevel@CTCBuyableHouse@@QBEJXZ 0x006BF920 unless
    // FABLE_RETAIL_CTCBUYABLEHOUSE_FRAMEUPDATE already declares it). Declarations only: no layout or vtable change.
public:
    long GetPrice() const;                                               // 0x006C0120
    void SetRented(bool rented);                                         // 0x006C1150
    void SetOwnedByPlayer(bool owned);                                   // 0x006C17B0
    void IncrementDressLevel();                                          // 0x006C04B0
#ifndef FABLE_RETAIL_CTCBUYABLEHOUSE_FRAMEUPDATE
    long GetPriceForNextDressLevel() const;                              // 0x006BF920
#endif
#endif
#ifdef FABLE_RETAIL_CTCBUYABLEHOUSE_GETSIGNPOSTPOS
    // Retail opt-in (w22 wf19, row 0x006BFCD0): Ego_r.pdb / FableWin.pdb ?GetSignPostPos@CTCBuyableHouse@@AAE_N_NAAVC3DVector@@PAVCRightHandedSet@@@Z
    // (private). Declaration only: no layout or vtable change.
private:
    bool GetSignPostPos(bool main, class C3DVector& pos, class CRightHandedSet* orientation);   // 0x006BFCD0
public:
#endif
#ifdef FABLE_RETAIL_CTCBUYABLEHOUSE_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals the declarations above lack, declaration only (no layout change; the
    // vtable already exists). Slots from the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots;
    // spellings from the Ego_r / FableWin name sets.
public:
    virtual bool HasFrameUpdate() const;  // slot 8, 0x006BF7A0
public:
    virtual bool IsComponentSerialisable() const;  // slot 23, 0x006BF7C0
public:
#endif
};
FABLE_STATIC_ASSERT(sizeof(CTCBuyableHouse) == 0x4c);
#else
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CTCBuyableHouse = 0x4c.
// Retail evidence: 1 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

class CDefPointeeBase;

#pragma pack(push, 1)
class CTCBuyableHouse {
public:
    void*            __vftable;                              // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char    _pad_0x04[0x4];                         // +0x04
    unsigned char    _pad_0x08[0x4];                         // +0x08
    CDefPointeeBase* PDef_Object;                            // +0x0c CDefPointer<CBuyableHouseDef_const_>::Object
    bool             IsScripted;                             // +0x10
    bool             OwnedByPlayer;                          // +0x11
    bool             Rented;                                 // +0x12
    unsigned char    _pad_0x13[0x1];                         // +0x13
    long             DaySomeoneWillMoveIn;                   // +0x14
    long             DayNextRentIsDue;                       // +0x18
    long             CurrentDressLevel;                      // +0x1c
    long             MaxDressLevel;                          // +0x20
    unsigned char    MainSignPost[0x8];                      // +0x24 CIntelligentPointer<CThing>
    unsigned char    DressingSignPost[0x8];                  // +0x2c CIntelligentPointer<CThing>
    long             FrameToNextCheckIfWeNeedToCreateAnything; // +0x34
    unsigned char    WifeLivingHere[0x8];                    // +0x38 CIntelligentPointer<CThing>
    bool             InitiallySetupDressLevel;               // +0x40
    unsigned char    _pad_0x41[0x3];                         // +0x41
    long             VirtualMoneyBags;                       // +0x44
    bool             HasPreviouslyBeenBuyable;               // +0x48
    bool             IsResidential;                          // +0x49
    unsigned char    _pad_0x4a[0x2];                         // +0x4a
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CTCBuyableHouse) == 0x4c);
FABLE_STATIC_ASSERT(offsetof(CTCBuyableHouse, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CTCBuyableHouse, PDef_Object) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CTCBuyableHouse, IsScripted) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CTCBuyableHouse, OwnedByPlayer) == 0x11);
FABLE_STATIC_ASSERT(offsetof(CTCBuyableHouse, Rented) == 0x12);
FABLE_STATIC_ASSERT(offsetof(CTCBuyableHouse, DaySomeoneWillMoveIn) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CTCBuyableHouse, DayNextRentIsDue) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CTCBuyableHouse, CurrentDressLevel) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CTCBuyableHouse, MaxDressLevel) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CTCBuyableHouse, MainSignPost) == 0x24);
FABLE_STATIC_ASSERT(offsetof(CTCBuyableHouse, DressingSignPost) == 0x2c);
FABLE_STATIC_ASSERT(offsetof(CTCBuyableHouse, FrameToNextCheckIfWeNeedToCreateAnything) == 0x34);
FABLE_STATIC_ASSERT(offsetof(CTCBuyableHouse, WifeLivingHere) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CTCBuyableHouse, InitiallySetupDressLevel) == 0x40);
FABLE_STATIC_ASSERT(offsetof(CTCBuyableHouse, VirtualMoneyBags) == 0x44);
FABLE_STATIC_ASSERT(offsetof(CTCBuyableHouse, HasPreviouslyBeenBuyable) == 0x48);
FABLE_STATIC_ASSERT(offsetof(CTCBuyableHouse, IsResidential) == 0x49);

#endif // FABLE_RETAIL_CTCBUYABLEHOUSE
#endif // FABLE_ENGINE_CTCBUYABLEHOUSE_H
