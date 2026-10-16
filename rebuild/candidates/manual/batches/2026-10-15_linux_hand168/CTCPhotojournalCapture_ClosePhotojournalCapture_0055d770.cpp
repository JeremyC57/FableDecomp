// CTCPhotojournalCapture::ClosePhotojournalCapture() -- mark the capture screen closed, queue game event 0x43 for the main player, and tell the parent's inventory component (interface 0x11) to start game rendering again
// Leaf: ClosePhotojournalCapture
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055D770, 150 bytes.
// Callees: CPlayerManager::GetMainPlayerNumber @ 0x00449700 (catalog "CWorld::DrawGetEnvironment") on the main game component's
//   player manager (+0x1c), CMainGameComponent::AddGameEvent(const CGameEvent&) @ 0x0041649C on g_pMainGameComponent @ 0x013B86A0,
//   CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface, interface 0x11: bit 17 of CThing+0x20),
//   CTCInventory::StartGameRendering @ 0x00581C1E, __security_check_cookie @ 0x00BFE9F9 (cookie 0x0139C8A8).
// GS note: the 40-byte CGameEvent (32-byte data array) gets the /GS cookie; esp-based frame (no size pragma).
// Layout: CTCPhotojournalCapture ScreenOpen +0x19 (PDB); CGameEvent as the landed 0x005BC527 (Type +0, Player +4, Data +5,
//   EndPos +0x25, Valid +0x26, Replacement +0x27).
// Shape: the ScreenOpen store shares the event ctor's `xor al,al` zero; the interface lookup's key goes in the frame word below
//   the event.
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CTCInventory {
    void StartGameRendering();                  // 0x00581C1E
};
struct CMemoryBuffer {
};
struct CGameEvent : CMemoryBuffer {
    long          Type;                         // +0x00
    signed char   Player;                       // +0x04
    unsigned char Data[32];                     // +0x05
    unsigned char EndPos;                       // +0x25
    bool          Valid;                        // +0x26
    bool          Replacement;                  // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
};
struct CPlayerManager {
    long GetMainPlayerNumber();                 // 0x00449700
};
struct CMainGameComponent {
    unsigned char   _pad00[0x1c];
    CPlayerManager* PlayerManager;              // +0x1c
    void AddGameEvent(const CGameEvent& event); // 0x0041649C
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
struct CTCPhotojournalCapture {
    void*   __vftable;                          // +0x00 (CTCBase)
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    unsigned char _pad0a[0x0f];
    bool    ScreenOpen;                         // +0x19
    void ClosePhotojournalCapture();
};

void CTCPhotojournalCapture::ClosePhotojournalCapture()
{
    ScreenOpen = false;
    CGameEvent event(0x43);
    event.Player = (signed char)g_pMainGameComponent->PlayerManager->GetMainPlayerNumber();
    g_pMainGameComponent->AddGameEvent(event);
    CTCInventory* inventory;
    if (ParentThing->GetInterface(&inventory, 0x11))
        inventory->StartGameRendering();
}
