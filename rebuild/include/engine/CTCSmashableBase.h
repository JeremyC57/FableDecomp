#ifndef FABLE_ENGINE_CTCSMASHABLEBASE_H
#define FABLE_ENGINE_CTCSMASHABLEBASE_H
// New header (chunk 892, row 0x008A8010 CGameScriptInterface::SmashAllWindowsWithinRadiusOfPoint). CTCSmashableBase : CTCBase
// (RTTI .?AVCTCSmashableBase@@ : CTCBase, vtable 0x0127185C) and CTCSmashableWindow : CTCSmashableBase (vtable 0x01240BA4).
// Layout: Ego_r.pdb CTCSmashableBase, 0x18 bytes: LastWorldFrameHit +0x0C, Smashed +0x10, Definition (CDefPointer<const
// CSmashableDef>) +0x14; CTCSmashableWindow adds no members. Interface type TCI_SMASHABLE = 78 (FableWin.pdb
// ?StatGetInterfaceType@CTCSmashableBase@@SI?AW4ETCInterfaceType@@XZ, public static __fastcall, inline in release: 0x008A80A7 keys
// the component map with the immediate 0x4E). Smash: the first virtual after CTCBase's 28 slots (slot 28, +0x70); protected in
// CTCSmashableBase (?Smash@CTCSmashableBase@@MAEXXZ, 0x007EED20), public in CTCSmashableWindow (?Smash@CTCSmashableWindow@@UAEXXZ,
// 0x007F4E10, landed); FableWin.pdb ??$GetTC@VCTCSmashableWindow@@@CThing@@QAE_NPAPAVCTCSmashableWindow@@@Z sits among the
// script-interface instantiations (0x02ACD110). Declarations only, no bodies: the default path of every other TU is unchanged.
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "fable_compiler.h"
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CDefPointer.h"
class CSmashableDef;
class CTCSmashableBase : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_SMASHABLE; }
    virtual void OnDie();                                                // 13 ?OnDie@CTCSmashableBase@@UAEXXZ
    virtual ETCInterfaceType GetInterfaceType() const;                   // 21 ?GetInterfaceType@CTCSmashableBase@@UBE?AW4ETCInterfaceType@@XZ
    bool IsSmashed() const;                                              // ?IsSmashed@CTCSmashableBase@@QBE_NXZ
    bool IsSmashable() const;                                            // ?IsSmashable@CTCSmashableBase@@QBE_NXZ
protected:
    CTCSmashableBase(CThing& thing);                                     // 0x007EEBE0 ??0CTCSmashableBase@@IAE@AAVCThing@@@Z
    virtual void OnCreate();                                             //  4 0x007EEC90
    virtual void OnInitialActivate();                                    //  6
    virtual void Smash();                                                // 28 0x007EED20
private:
    long                             LastWorldFrameHit;                  // +0x0C
    bool                             Smashed;                            // +0x10
    CDefPointer<const CSmashableDef> Definition;                         // +0x14
};
FABLE_STATIC_ASSERT(sizeof(CTCSmashableBase) == 0x18);

class CTCSmashableWindow : public CTCSmashableBase {
public:
    CTCSmashableWindow(CThing& thing);                                   // 0x004DC6BA ??0CTCSmashableWindow@@QAE@AAVCThing@@@Z
    virtual void Smash();                                                // 28 0x007F4E10 ?Smash@CTCSmashableWindow@@UAEXXZ
};
#endif
