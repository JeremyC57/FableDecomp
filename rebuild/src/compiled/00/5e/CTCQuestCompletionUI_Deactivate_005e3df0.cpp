// CTCQuestCompletionUI::Deactivate() -- when active: start the quitting fade-out, tell the black overlay component (virtual slot 48, true) and queue game event 9 carrying a zero byte
// Leaf: Deactivate
// Flags: /GS
// Grade: DIFFER(123v119) -- ours copies `this` to eax after the /GS cookie load (mov eax,ecx ; mov cl,[eax+0Ch]) where retail keeps ecx and loads al; AddGameEvent arg via lea ecx after the push instead of mov ecx,[global] ; lea edx
// Retail: Fable.exe 0x005E3DF0, 119 bytes (plain `ret`, /GS cookie 0x0139C8A8 / __security_check_cookie 0x00BFE9F9).
// Callees: NUISystem::CComponent virtual slot 48 (+0xC0) with `true`, CMemoryBuffer::Copy(unsigned long, const void*)
// @ 0x009F1760 (inline CGameEvent::AddUBYTE), CMainGameComponent::AddGameEvent(const CGameEvent&) @ 0x0041649C on the
// global component @ 0x013B86A0. CGameEvent model as landed 0x0068E3C0 / 0x005BC527 (Ego_r.pdb, 40 bytes).
// Layout: retail sits 20 bytes below the PDB from the containers on (Components vector 12 bytes, Boasts list 8,
// three maps 12 each, IsOverridingScreenFade 0x005E3E70 tests +0x1C Quitting / +0xC0): Active +0x0C,
// PBlack +0xBC (PDB 0xD0), QuittingFadeOut +0xC0 (PDB 0xD4).
struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data);   // 0x009F1760
};
class CGameEvent : public CMemoryBuffer { public:
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddUBYTE(unsigned char value) { Copy(1, &value); }
};
class CMainGameComponent { public:
    void AddGameEvent(const CGameEvent& event);    // 0x0041649C
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
namespace NUISystem {
class CComponent { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void SetFading(bool on);               // slot 48 (+0xC0)
};
}
class CTCQuestCompletionUI { public:
    virtual void v00();
    unsigned char _pad04[0x08];
    bool          Active;                          // +0x0c
    bool          ReallyActive;                    // +0x0d
    unsigned char _pad0e[0xbc - 0x0e];
    NUISystem::CComponent* PBlack;                 // +0xbc
    bool          QuittingFadeOut;                 // +0xc0
    void Deactivate();
};

void CTCQuestCompletionUI::Deactivate()
{
    if (Active) {
        QuittingFadeOut = true;
        PBlack->SetFading(true);
        CGameEvent gameEvent(9);
        gameEvent.AddUBYTE(0);
        g_pMainGameComponent->AddGameEvent(gameEvent);
    }
}