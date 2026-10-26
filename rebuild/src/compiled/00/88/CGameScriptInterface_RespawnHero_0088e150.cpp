// CGameScriptInterface::RespawnHero(bool flag) const -- queues a "respawn hero" game event (type 8) for the main player, carrying the flag byte
// Grade: DIFFER(111v111) -- the by-value byte handed to CMemoryBuffer::Copy gets its own frame byte (`sub esp,30h`, `lea ecx,[esp+4]`) where retail homes it in the dead flag argument slot (`sub esp,2Ch`, `lea ecx,[esp+34h] ; ... mov [esp+3Ch],al`), shifting every frame offset by 4; tried an inline AddUBYTE(unsigned char), __forceinline AddUBYTE, an inline AddBOOL(bool), __forceinline AddBOOL(bool)
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088E150, length 111 bytes.
// Callees: CMemoryBuffer::Copy(unsigned long, const void*) @ 0x009F1760 (the inlined CGameEvent::AddBOOL), CPlayerManager::GetMainPlayerNumber @ 0x00449700
// (`mov eax,[ecx+1Ch]`) on PlayerManager (+0x14), CMainGameComponent::AddGameEvent(const CGameEvent&) @ 0x0041649C on the global component @ 0x013B86A0,
// __security_check_cookie @ 0x00BFE9F9 (cookie @ 0x0139C8A8).
// Layout: CGameEvent from Ego_r.pdb (40 bytes, as the landed 0x005BC527): Type +0, Player +4, Data[32] +5, EndPos +0x25, Valid +0x26, Replacement +0x27; it derives from CMemoryBuffer.
// Shape: /GS cookie frame (the 32-byte data array); the inline constructor stores Type 8, Player -1, EndPos 0, Valid 1, Replacement 0;
// AddBOOL's by-value bool is homed in the dead flag slot (an `unsigned char` parameter takes a new frame byte); the player number overwrites Player after the Copy.
// Leaf: RespawnHero
struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data);   // 0x009F1760
};

class CGameEvent : public CMemoryBuffer { public:
    long          Type;                           // +0x00
    signed char   Player;                         // +0x04
    unsigned char Data[32];                       // +0x05
    unsigned char EndPos;                         // +0x25
    bool          Valid;                          // +0x26
    bool          Replacement;                    // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    __forceinline void AddBOOL(bool value) { Copy(1, &value); }   // __forceinline + bool: the by-value copy is homed in the caller's dead bool slot
};

class CMainGameComponent { public:
    void AddGameEvent(const CGameEvent& event);   // 0x0041649C
};
extern CMainGameComponent* g_pMainGameComponent;  // 0x013B86A0

struct CPlayerManager_Retail {
    long GetMainPlayerNumber();                   // 0x00449700
};

enum { GAME_EVENT_RESPAWN_HERO = 8 };

class CWorld;
class CGameScriptInterface { public:
    virtual void v000();
    CWorld*                World;                 // +0x04 (engine/CGameScriptInterface.h)
    void*                  Component;             // +0x08
    void*                  DisplayEngine;         // +0x0c
    void*                  DefinitionManager;     // +0x10
    CPlayerManager_Retail* PlayerManager;         // +0x14
    void RespawnHero(bool flag) const;
};

void CGameScriptInterface::RespawnHero(bool flag) const
{
    CGameEvent event(GAME_EVENT_RESPAWN_HERO);
    event.AddBOOL(flag);
    event.Player = (signed char)PlayerManager->GetMainPlayerNumber();
    g_pMainGameComponent->AddGameEvent(event);
}