// CTCTavernGame::SetStateAfterInstructions(unsigned long, CWideString, ETavernGameStates) -- show the game's instruction subtitles (one per entry of the text group), then move to the requested state (or skip straight on when instructions are being skipped)
// Leaf: SetStateAfterInstructions
// Grade: DIFFER(536v536) -- one register: the state-0x2f FinishInstructions call loads the vtable through eax where retail uses edx (retail keeps the group pointer live in eax across the state tests); the if/else-if chain gets edx but then folds the constant 5 into the state-5 SetState copy
// Retail: Fable.exe (TLC, VC7.1) @ 0x005EB180, 536 bytes, `ret 0Ch` (the CWideString title is passed by value and destroyed here).
// Callees: the tavern-game vtable slot 30 (+0x78), CTCBetting::PlaySound(const CCharString&) 0x005EA950 (catalog "PlaySoundW"),
//   CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0, CWideString(const wchar_t*) 0x0099B6B0 / ~ 0x0099B510,
//   _atexit 0x004012BC, CDataBank::GetPGroupEntry(unsigned long, bool) 0x009C8890, NGameText::CDataBank::GetPGroupEntry
//   0x009C9070, the text entry's CWideString getter 0x009C7A40, CSubtitleRenderer::SetText 0x00492F69.
// Globals: the skip-instructions flag 0x013BAE1C, the world 0x013B86A0 (+0x14 data bank, +0x1614C font bank), the player GUI
//   0x013B8790 (its subtitle renderer at +0x18); the function-local static wide string 0x013B8C50 (guard 0x013B8C54).
// Layout: retail CTCTavernGame (12-byte vectors, see differ/005ea650.cpp): TavernGameState +0xc0, previous state +0xc4,
//   the bool at +0xd8.
#include <stddef.h>
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
struct CWideString {
    void* m_pData;
    CWideString(const wchar_t* text);                       // 0x0099B6B0
    ~CWideString();                                         // 0x0099B510
};

struct CCounted {
    virtual void v00();
    virtual void Destroy();
    long RefCount;
    void DecRef() { --RefCount; }
    long GetRefCount() const { return RefCount; }
    void Release()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CULongVector {                                       // retail std::vector<unsigned long> (12 bytes)
    unsigned long* first;
    unsigned long* last;
    unsigned long* end;
};
struct CGroupEntry : CCounted {
    unsigned char _pad08[0x1c - 0x08];
    CULongVector Entries;                                   // +0x1c
};
struct CTextEntry : CCounted {
    CWideString GetText() const;                            // 0x009C7A40
};
template <class T> struct CIVCountedPointer {
    T* Data;
    __forceinline ~CIVCountedPointer()
    {
        if (Data) {
            Data->Release();
            Data = 0;
        }
    }
    T* operator->() const { return Data; }
};
struct CDataBank {
    CIVCountedPointer<CGroupEntry> GetPGroupEntry(unsigned long id, bool flag);   // 0x009C8890
    CIVCountedPointer<CTextEntry> GetPTextEntry(unsigned long id, bool flag) const;   // 0x009C9070
};
struct CFontBank;
struct CWorld {
    unsigned char _pad00[0x14];
    CDataBank* PDataBank;                                   // +0x14
    unsigned char _pad18[0x1614c - 0x18];
    CFontBank* PFontBank;                                   // +0x1614c
};
extern CWorld* g_pWorld;                                    // 0x013B86A0
struct CSubtitleRenderer {
    void SetText(CFontBank* font, const CWideString& text, const CWideString& speaker, const CWideString& title,
                 const CWideString& extra, unsigned long priority, float duration, bool flag1, bool flag2);   // 0x00492F69
};
struct CPlayerGui {
    unsigned char _pad00[0x18];
    CSubtitleRenderer Subtitles;                            // +0x18
};
extern CPlayerGui* g_pPlayerGui;                            // 0x013B8790
extern bool g_bSkipTavernInstructions;                      // 0x013BAE1C

typedef long ETavernGameStates;
struct CTCTavernGame {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void FinishInstructions();                      // slot 30 (+0x78)
    unsigned char _pad04[0xc0 - 0x04];
    ETavernGameStates TavernGameState;                      // +0xc0
    ETavernGameStates PreviousTavernGameState;              // +0xc4
    unsigned char _padc8[0xd8 - 0xc8];
    bool          ShowingResult;                            // +0xd8
    unsigned long PlaySound(const CCharString& name);       // 0x005EA950
    void SetState(ETavernGameStates state)
    {
        PreviousTavernGameState = TavernGameState;
        TavernGameState = state;
    }
    void SetStateAfterInstructions(unsigned long key, CWideString title, ETavernGameStates state);
};

void CTCTavernGame::SetStateAfterInstructions(unsigned long key, CWideString title, ETavernGameStates state)
{
    if (g_bSkipTavernInstructions) {
        g_bSkipTavernInstructions = false;
        FinishInstructions();
        return;
    }
    PlaySound("TAVERN_GAME_OBJECT_SELECTION_CHANGE");
    static CWideString s_missingText(L"Missing text entry");
    CDataBank* bank = g_pWorld->PDataBank;
    CIVCountedPointer<CGroupEntry> group = bank->GetPGroupEntry(key, true);
    if (group.Data) {
        const CULongVector& ids = group->Entries;
        for (const unsigned long* it = ids.first; it != ids.last; ++it) {
            CIVCountedPointer<CTextEntry> entry = bank->GetPTextEntry(*it, true);
            CWideString extra(L"");
            CWideString speaker(L"");
            CFontBank* font = g_pWorld->PFontBank;
            CPlayerGui* gui = g_pPlayerGui;
            gui->Subtitles.SetText(font, entry->GetText(), speaker, title, extra, 10, 0.0f, false, true);
        }
    }
    if (g_bSkipTavernInstructions) {
        g_bSkipTavernInstructions = false;
        FinishInstructions();
    } else {
        if (state == 5)
            ShowingResult = true;
        if (state == 0x2f)
            FinishInstructions();
        else
            SetState(state);
    }
}
