// CTCShop::OpenShoppingInterface() -- tell the hero's shopping component which shop it is in, then queue game event 0x1a (open the shopping interface, payload 0xc) for the main player
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00773B20, 210 bytes (plain `ret`, /GS cookie 0x0139C8A8).
// Callees: CTCBase::GetGameSubsystem @ 0x00686D60 (its +0x1c member is the player manager here), CPlayerManager::GetMainPlayer
// const @ 0x00449960, CPlayer::GetHeroThing @ 0x00487DC0, the const-less interface LowerBound @ 0x0040F020, the interface 0x17
// component's shop setter @ 0x005BC4CB (`mov [ecx+0B8h],arg ; ret 4`, named SetCurrentShop -- the manifest's
// CObservable::AddExclusiveObserver is propagated), CMemoryBuffer::Copy @ 0x009F1760, CTCBase::GetPlayerManager const
// @ 0x00686D50, CPlayerManager::GetMainPlayerNumber @ 0x00449700, CMainGameComponent::AddGameEvent @ 0x0041649C on
// g_pMainGameComponent 0x013B86A0, __security_check_cookie @ 0x00BFE9F9.
// Layout: CThing dead flag bit 0 of +0x91, interface bitset +0x20 (0x17 = bit 23 of the first word), interface map +0x44;
// CGameEvent as the landed CTCWife::SendPauseEvent 0x006CEBF0 (40 bytes over an empty CMemoryBuffer).
// Shape: the out-parameter GetInterface lookup (leaves the component in eax before `mov ecx,eax`); the event payload is an inline by-value
// AddULONG (its value homed in the frame word below the event) written before the player number.
struct CThing;
struct CTCBase;
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                            // +0x91 bit 0: dead
    __forceinline bool GetInterface(long type, CTCBase** out)   // inline: the landed GetInterface family shape (0x007504E0)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = it->component;
            return true;
        }
        return false;
    }
};
struct CTCShoppingComponent {                           // the hero's interface 0x17 component (an inventory TC)
    void SetCurrentShop(CThing* shop);                  // 0x005BC4CB (+0xb8)
};
struct CPlayer {
    CThing* GetHeroThing();                             // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer() const;                     // 0x00449960
    long GetMainPlayerNumber();                         // 0x00449700
};
struct CGameSubsystem {
    unsigned char _pad00[0x1c];
    CPlayerManager* PlayerManager;                      // +0x1c
};
struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data);  // 0x009F1760
};
struct CGameEvent : CMemoryBuffer {
    long          Type;                                 // +0x00
    signed char   Player;                               // +0x04
    unsigned char Data[32];                             // +0x05
    unsigned char EndPos;                               // +0x25
    bool          Valid;                                // +0x26
    bool          Replacement;                          // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddULONG(unsigned long value) { Copy(4, &value); }
};
struct CMainGameComponent {
    void AddGameEvent(const CGameEvent& event);         // 0x0041649C
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0
struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    bool ToBeRemoved;                                   // +0x08
    bool IsInGlobalUpdateTCs;                           // +0x09
    unsigned char _pad0a[2];
    CPlayerManager* GetPlayerManager() const;           // 0x00686D50
    CGameSubsystem* GetGameSubsystem();                 // 0x00686D60
};
struct CTCShop : CTCBase {
    void OpenShoppingInterface();
};

void CTCShop::OpenShoppingInterface()
{
    CThing* hero = GetGameSubsystem()->PlayerManager->GetMainPlayer()->GetHeroThing();
    CTCShoppingComponent* shopping;
    if (hero && !(hero->m_flags91 & 1) && hero->GetInterface(0x17, (CTCBase**)&shopping))
        shopping->SetCurrentShop(ParentThing);
    CGameEvent event(0x1a);
    event.AddULONG(0xc);
    event.Player = (signed char)GetPlayerManager()->GetMainPlayerNumber();
    g_pMainGameComponent->AddGameEvent(event);
}