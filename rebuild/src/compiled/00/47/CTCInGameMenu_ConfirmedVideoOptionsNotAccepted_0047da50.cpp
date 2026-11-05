// CTCInGameMenu::ConfirmedVideoOptionsNotAccepted() -- the "video options not accepted" dialog was confirmed: drop its OK button, the dialog and the mouse pointer from the UI manager, hand the pointer back to the manager, hide the dialog, revert the hero inventory's video changes and re-apply/save the profile without the options-changed dialog
// Leaf: ConfirmedVideoOptionsNotAccepted
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad21[0x44 - 0x21] removed.
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
//
// Retail 0x0047DA50, 291 bytes (plain `ret`; frame pointer, `leave`). Size TU with a frame pointer:
// `#pragma optimize("y", off)` + `("s", on)` (the sibling ConfirmedVideoOptionsChanged 0x0047D012 has no frame local).
// Callees: CCharString(const char*, long) @ 0x0099EBF0 ("PC_OK_GAME_OKBUTTON" @ 0x01236708, built in its by-value argument
// slot), the dialog's FindChild (virtual slot 3), NUISystem::CManager::GetInstance @ 0x0041E5F2 (four times), the manager's
// ReleaseObserver (slot 10) / AddObserver (slot 2), the pointer's ObserveEvent (observer slot 3), the dialog's SetActive (slot
// 48), std::bitset<>::test @ 0x00410DE0 and CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined
// GetInterface for CTCInventory, type 0x11), CTCInventory::RevertVideoChanges @ 0x00582754,
// CTBaseSingleton<CUserProfileManager>::Get @ 0x0040D2A0 (four times), CUserProfileManager::Apply @ 0x0040BC80 and ::Save
// @ 0x0040A740.
// Layout: retail CTCInGameMenu (12-byte containers): PVideoOptionsNotAcceptedDialog +0xb0, PMousePointer +0xec; CUserProfileManager
// DisplayDialogOnOptionsChanged +0x34; retail CThing bitset +0x20, interface map +0x44.
// Shape: the landed ConfirmedVideoOptionsChanged body with the not-accepted dialog, then the inventory revert through the
// out-parameter GetInterface (null-tested component) and the profile manager fetched afresh for every statement.
#pragma optimize("y", off)
#pragma optimize("s", on)

class CCharStringData;
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"

namespace NUISystem {
class CObserver { public:                                   // the observer interface at +4 of every component
    virtual void o00(); virtual void o01(); virtual void o02();
    virtual void ObserveEvent(long eventType);       // slot 3 (+0x0C)
};
struct CComponentBase {                              // the primary vtable
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual class CComponent* FindChild(CCharString name);   // slot 3 (+0x0C)
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33();
    virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38();
    virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43();
    virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void SetActive_slot48(bool active);      // slot 48 (+0xC0)
};
class CComponent : public CComponentBase, public CObserver { public:
};
class CYesNo : public CComponent { public:
};
class CManager { public:                                    // NUISystem::CManager (CObservable vtable part)
    virtual void m00(); virtual void m01();
    virtual void AddObserver(CObserver* observer);   // slot 2 (+0x08)
    virtual void m03(); virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07(); virtual void m08();
    virtual void m09();
    virtual void ReleaseObserver_0055ca00(CObserver* observer);   // slot 10 (+0x28)
    static CManager* __fastcall GetInstance();       // 0x0041E5F2
};
}

class CTCBase;
#include "engine/CVectorMap.h"

typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCInventory { public:
    void RevertVideoChanges();                              // 0x00582754
};
class CUserProfileManager { public:
    unsigned char _pad00[0x34];
    bool          DisplayDialogOnOptionsChanged;            // +0x34
    void Apply();                                           // 0x0040BC80
    void Save();                                            // 0x0040A740
    static CUserProfileManager& __fastcall Get();           // 0x0040D2A0 CTBaseSingleton<CUserProfileManager>::Get
};

class CTCInGameMenu { public:
    void*                 __vftable;                        // +0x00
    CThing*               ParentThing;                      // +0x04 (CTCBase)
    unsigned char         _pad_0x08[0xA8];
    NUISystem::CYesNo*    PVideoOptionsNotAcceptedDialog;   // +0xB0 (PDB +0xD0)
    unsigned char         _pad_0xb4[0x38];
    NUISystem::CComponent* PMousePointer;                   // +0xEC (PDB +0x110)
    void ConfirmedVideoOptionsNotAccepted();
};

void CTCInGameMenu::ConfirmedVideoOptionsNotAccepted()
{
    NUISystem::CComponent* okButton = PVideoOptionsNotAcceptedDialog->FindChild(CCharString("PC_OK_GAME_OKBUTTON", -1));
    if (okButton)
        NUISystem::CManager::GetInstance()->ReleaseObserver_0055ca00(okButton);
    NUISystem::CManager::GetInstance()->ReleaseObserver_0055ca00(PVideoOptionsNotAcceptedDialog);
    NUISystem::CManager::GetInstance()->ReleaseObserver_0055ca00(PMousePointer);
    NUISystem::CManager::GetInstance()->AddObserver(PMousePointer);
    PMousePointer->ObserveEvent(0x19);
    PVideoOptionsNotAcceptedDialog->SetActive_slot48(false);
    CThing* parent = ParentThing;
    CTCInventory* inventory;
    if (parent->GetTC(TCI_INVENTORY, &inventory) && inventory)
        inventory->RevertVideoChanges();
    CUserProfileManager::Get().DisplayDialogOnOptionsChanged = false;
    CUserProfileManager::Get().Apply();
    CUserProfileManager::Get().Save();
    CUserProfileManager::Get().DisplayDialogOnOptionsChanged = true;
}