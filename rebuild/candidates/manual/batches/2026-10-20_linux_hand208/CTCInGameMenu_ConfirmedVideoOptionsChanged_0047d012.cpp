// CTCInGameMenu::ConfirmedVideoOptionsChanged() -- the player kept the new video mode: dismiss the "options changed" dialog, hand input back to the mouse pointer and stop the revert timer
// Leaf: ConfirmedVideoOptionsChanged
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0047D012, 195 bytes; size TU without a frame pointer
// (`#pragma optimize("s", on)`, as the landed IsQuitMenuReceivingInput 0x0047D537 of the same TU).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (built in the by-value argument slot),
//          NUISystem::CManager::GetInstance() @ 0x0041E5F2 (__fastcall, x4), the dialog's CComponent slot 3 (+0x0C, find
//          child by name) and slot 48 (+0xC0), the manager's CObservable slots 10 (+0x28, 0x0055CA00: looks the
//          observer up in its list; name unproven) and 2 (+0x08, AddObserver), and the pointer's CObserver slot 3
//          (+0x0C, ObserveEvent; event 0x19).
// Shape: the dialog's vtable is read into edi before the by-value name is built; every component goes to the
// manager as its CObserver sub-object at +4 (`lea edi,[eax+4]` with the converting null test; the found OK button
// is already known non-null, `add edi,4`); ObserveEvent is called through the +4 base without a null test; the
// revert timer becomes -1.0 (a qword pool constant).
// Layout: retail CTCInGameMenu (12-byte containers; engine/CTCInGameMenu.h carries PDB offsets):
// PVideoOptionsChangedDialog +0xB4 (PDB +0xD4), VideoChangesTimer +0xC0 (PDB +0xE0), PMousePointer +0xEC (PDB +0x110).
#pragma optimize("s", on)

struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length);      // 0x0099EBF0
    CCharString(const CCharString& other);           // (declared: by-value arguments are built in place)
    ~CCharString();
};

namespace NUISystem {
struct CObserver {                                   // the observer interface at +4 of every component
    virtual void o00(); virtual void o01(); virtual void o02();
    virtual void ObserveEvent(long eventType);       // slot 3 (+0x0C)
};
struct CComponentBase {                              // the primary vtable
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual struct CComponent* FindChild(CCharString name);   // slot 3 (+0x0C)
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
struct CComponent : CComponentBase, CObserver {
};
struct CYesNo : CComponent {
};
struct CManager {                                    // NUISystem::CManager (CObservable vtable part)
    virtual void m00(); virtual void m01();
    virtual void AddObserver(CObserver* observer);   // slot 2 (+0x08)
    virtual void m03(); virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07(); virtual void m08();
    virtual void m09();
    virtual void ReleaseObserver_0055ca00(CObserver* observer);   // slot 10 (+0x28)
    static CManager* __fastcall GetInstance();       // 0x0041E5F2
};
}

struct CTCInGameMenu {
    void*                 __vftable;                 // +0x00
    unsigned char         _pad_0x04[0xB0];
    NUISystem::CYesNo*    PVideoOptionsChangedDialog;   // +0xB4 (PDB +0xD4)
    unsigned char         _pad_0xb8[0x08];
    double                VideoChangesTimer;         // +0xC0 (PDB +0xE0)
    unsigned char         _pad_0xc8[0x24];
    NUISystem::CComponent* PMousePointer;            // +0xEC (PDB +0x110)
    void ConfirmedVideoOptionsChanged();
};

void CTCInGameMenu::ConfirmedVideoOptionsChanged()
{
    NUISystem::CComponent* okButton = PVideoOptionsChangedDialog->FindChild(CCharString("PC_OK_GAME_OKBUTTON", -1));
    if (okButton)
        NUISystem::CManager::GetInstance()->ReleaseObserver_0055ca00(okButton);
    NUISystem::CManager::GetInstance()->ReleaseObserver_0055ca00(PVideoOptionsChangedDialog);
    NUISystem::CManager::GetInstance()->ReleaseObserver_0055ca00(PMousePointer);
    NUISystem::CManager::GetInstance()->AddObserver(PMousePointer);
    PMousePointer->ObserveEvent(0x19);
    PVideoOptionsChangedDialog->SetActive_slot48(false);
    VideoChangesTimer = -1.0;
}
