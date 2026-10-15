// CInputProcessQuickAccessItems::CancelQuickAccessCharge() -- if the controlled creature is alive, queue game event 0x29 (cancel quick-access charge) for its player carrying the charging quick-access slot and a zero byte
// Leaf: CancelQuickAccessCharge
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0068E3C0, length 141 bytes.
// Callees: CPlayer::GetControlledCreature @ 0x00487DD0 (MyPlayer +0x18), CMemoryBuffer::Copy(unsigned long, const void*)
// @ 0x009F1760 (landed), CMainGameComponent::AddGameEvent(const CGameEvent&) @ 0x0041649C on the global component
// @ 0x013B86A0, __security_check_cookie @ 0x00BFE9F9. Global 0x0137EE78 = the quick-access slot being charged.
// Model: CGameEvent as the landed CTCInventoryBase::AddCloseInventoryEvent 0x005BC527 (Ego_r.pdb, 40 bytes, over an empty
// CMemoryBuffer); AddLong / AddUBYTE are inline `Copy(sizeof v, &v)` with the by-value argument homed in the frame. The
// creature's player number is the byte at +0x90; its dead flag bit 0 of +0x91.
struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data);   // 0x009F1760
};
struct CGameEvent : CMemoryBuffer {
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddLong(long value) { Copy(sizeof(value), &value); }
    void AddUBYTE(unsigned char value) { Copy(1, &value); }
};
struct CMainGameComponent {
    void AddGameEvent(const CGameEvent& event);    // 0x0041649C
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
extern long g_QuickAccessChargeSlot;               // 0x0137EE78
struct CThingCreatureBase {
    unsigned char _pad00[0x90];
    signed char   m_playerNumber;                  // +0x90
    unsigned char m_dead : 1;                      // +0x91 bit 0
    unsigned char m_91_1 : 7;
};
class CPlayer {
public:
    CThingCreatureBase* GetControlledCreature();   // 0x00487DD0
};
struct CWorld; struct CGameDefinitionManager; struct CGamePlayerInterface;
class CInputProcessGameBase {                      // engine/CInputProcessGameBase.h layout
public:
    virtual void v00();
    unsigned char           _pad04[0x10];          // +0x04 CAInputProcess
    CWorld*                 World;                 // +0x14
    CPlayer*                MyPlayer;              // +0x18
    CGameDefinitionManager* DefinitionManager;     // +0x1c
    CMainGameComponent*     Component;             // +0x20
    CGamePlayerInterface*   PGamePlayerInterface;  // +0x24
};
class CInputProcessQuickAccessItems : public CInputProcessGameBase {
public:
    void CancelQuickAccessCharge();
};

void CInputProcessQuickAccessItems::CancelQuickAccessCharge()
{
    CThingCreatureBase* creature = MyPlayer->GetControlledCreature();
    if (creature && !creature->m_dead) {
        CGameEvent gameEvent(0x29);
        gameEvent.Player = creature->m_playerNumber;
        gameEvent.AddLong(g_QuickAccessChargeSlot);
        gameEvent.AddUBYTE(0);
        g_pMainGameComponent->AddGameEvent(gameEvent);
    }
}