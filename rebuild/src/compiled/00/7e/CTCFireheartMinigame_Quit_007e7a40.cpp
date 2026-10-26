// CTCFireheartMinigame::Quit() -- end the session: drop the hero out of the fireheart player mode, flag the player GUI, clear the subtitle's second-button override text and release the board primitives
// Leaf: Quit
//
// Retail 0x007E7A40, 128 bytes (plain `ret`). Ego_r.pdb `?Quit@CTCFireheartMinigame@@QAEXXZ`. Callees:
// CTCBase::GetPlayerManager() @ 0x00686D40 (manifest "CViewBase::GetDisplayEngine", propagated),
// CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetCurrentMode @ 0x00633BE0, CPlayer::RemoveMode
// (EPlayerMode) @ 0x00634190, CCharString::CCharString(const char*, long) @ 0x0099EBF0 ("" @ 0x0122D70E),
// CSubtitleRenderer::SetButton2OverrideText(const CCharString&) @ 0x0048F881, ~CCharString @ 0x0099EAE0,
// vector<CEnginePrimitiveHandle>::erase(first, last) @ 0x00440D70 (manifest "Vector_EraseRange").
// Global g_pPlayerGui @ 0x013B8790 (its subtitle renderer at +0x18, a flag byte at +0x330).
// Layout = Ego_r.pdb CTCFireheartMinigame: InSession +0x4c, Handles +0x8c (12-byte retail vector).
// Shape: the player is a named local (edi); RemoveMode's constant argument is the compared mode value
// still in eax; the empty text is a named CCharString in its own block (destroyed before the erase) in the `push ecx`
// frame word; the handle vector is
// cleared through the out-of-line range erase (begin/end read as the two pushes).
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);             // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

enum EPlayerMode { PLAYER_MODE_FIREHEART_MINIGAME = 0x2a };

class CPlayer { public:
    EPlayerMode GetCurrentMode();                           // 0x00633BE0
    void RemoveMode(EPlayerMode mode);                      // 0x00634190
};

class CPlayerManager { public:
    CPlayer* GetMainPlayer();                               // 0x00449970
};

class CSubtitleRenderer { public:
    void SetButton2OverrideText(const CCharString& text);   // 0x0048F881
};

class CPlayerGui { public:
    unsigned char     _pad00[0x18];
    CSubtitleRenderer SubtitleRenderer;                     // +0x18
    unsigned char     _pad19[0x330 - 0x19];
    bool              FireheartQuitPending;                 // +0x330
};
extern CPlayerGui* g_pPlayerGui;                            // 0x013B8790

class CEnginePrimitiveHandle;
template <class T> struct CRetailVector {                   // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    T* erase(T* first, T* last);                            // 0x00440D70
    void clear() { erase(begin(), end()); }
};

class CThing;

class CTCFireheartMinigame { public:
    virtual void v00();
    CThing*       ParentThing;                              // +0x04 (CTCBase)
    unsigned char _pad08[0x4c - 0x08];
    bool          InSession;                                // +0x4c
    bool          ForceQuit;                                // +0x4d
    unsigned char _pad4e[0x8c - 0x4e];
    CRetailVector<CEnginePrimitiveHandle> Handles;          // +0x8c
    CPlayerManager* GetPlayerManager();                     // 0x00686D40 (CTCBase)
    void Quit();
};

void CTCFireheartMinigame::Quit()
{
    InSession = false;
    CPlayer* player = GetPlayerManager()->GetMainPlayer();
    if (player->GetCurrentMode() == PLAYER_MODE_FIREHEART_MINIGAME)
        player->RemoveMode(PLAYER_MODE_FIREHEART_MINIGAME);
    g_pPlayerGui->FireheartQuitPending = true;
    {
        CCharString noText("", -1);
        g_pPlayerGui->SubtitleRenderer.SetButton2OverrideText(noText);
    }
    Handles.clear();
}