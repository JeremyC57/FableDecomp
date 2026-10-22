// CInputProcessCombat::ProcessArmedMeleeInput(const CInputEvent& event, CProcessedInput& processed, const CThing& weapon) -- on a button press with a melee weapon: game action 8 queues the CREATURE_ABILITY_FLOURISH (when the hero's combat component allows a flourish) or CREATURE_ABILITY_BREAK_BLOCK ability event 0x16 with the button pressure; action 0xf queues event 0x20; action 0xc the CREATURE_ABILITY_ATTACK event 0x16; action 0xe event 0x22 with a true byte
// Leaf: ProcessArmedMeleeInput
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x0068ECA0, 684 bytes (+ int3 padding), `ret 0Ch` (/GS cookie 0x0139C8A8, __security_check_cookie 0x00BFE9F9).
// Callees: CWorld::GetMainPlayer 0x0049D9D0, CPlayer::GetHeroCreature 0x00487DC0, CVectorMap::LowerBound 0x004365B0 (const
//   overload, the inline GetInterface for interface 0x30), CTCCombat::IsMeleeFlourishValid 0x00733EE0 (landed) and 0x00733290
//   (labelled CTCCombat::Query_733290; descriptive IsBreakBlockValid), CInputProcessGameBase::IsEventButtonPressed 0x00687A90 /
//   IsEventGameAction 0x00687CF0 / GetButtonPressureForEvent 0x00687D10, CCharString 0x0099EBF0 / 0x0099EAE0,
//   CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410, CMemoryBuffer::Copy 0x009F1760,
//   CProcessedInput::AddGameEvent 0x00A0D340. Strings "CREATURE_ABILITY_FLOURISH" 0x0125BF48, "CREATURE_ABILITY_BREAK_BLOCK"
//   0x0125BF28, "CREATURE_ABILITY_ATTACK" 0x01259AFC.
// Layout: the landed CInputProcessCombat::ProcessUnarmedInput 0x0068E8F0 model (engine/CInputProcessGameBase.h fields,
//   CGameEvent as Ego_r.pdb); the thing's presence bitset +0x20 / interface map +0x44 as the landed 0x005BB999.
// Shape: the combat component pointer starts at 0 and the inline out-parameter GetInterface fills it (no hero test); the
//   unused weapon reference is the third argument (FableWin.pdb signature).
class CInputEvent;
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
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
    void AddUWORD(long value) { Copy(2, &value); }
    void AddFloat(float value) { Copy(sizeof(value), &value); }
    void AddBool(bool value) { Copy(1, &value); }
};
class CProcessedInput {
public:
    void AddGameEvent(const CGameEvent& event);    // 0x00A0D340
};
struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                // +0x20
    CInterfaceMap m_interfaces;                    // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CTCCombat {
    bool IsMeleeFlourishValid() const;             // 0x00733EE0
    bool IsBreakBlockValid() const;                // 0x00733290 (descriptive)
};
class CPlayer {
public:
    CThing* GetHeroCreature();                     // 0x00487DC0
};
struct CWorld {
    CPlayer* GetMainPlayer();                      // 0x0049D9D0
};
struct CMainGameComponent; struct CGamePlayerInterface;
class CInputProcessGameBase {                      // engine/CInputProcessGameBase.h layout
public:
    virtual void v00();
    unsigned char           _pad04[0x10];          // +0x04 CAInputProcess
    CWorld*                 World;                 // +0x14
    CPlayer*                MyPlayer;              // +0x18
    CDefinitionManager*     DefinitionManager;     // +0x1c
    CMainGameComponent*     Component;             // +0x20
    CGamePlayerInterface*   PGamePlayerInterface;  // +0x24
    bool IsEventButtonPressed(const CInputEvent& event);              // 0x00687A90
    bool IsEventGameAction(long action, const CInputEvent& event);    // 0x00687CF0
    float GetButtonPressureForEvent(const CInputEvent& event);        // 0x00687D10
};
class CInputProcessCombat : public CInputProcessGameBase {
public:
    void ProcessArmedMeleeInput(const CInputEvent& event, CProcessedInput& processed, const CThing& weapon);
};

void CInputProcessCombat::ProcessArmedMeleeInput(const CInputEvent& event, CProcessedInput& processed, const CThing& weapon)
{
    CThing* hero = World->GetMainPlayer()->GetHeroCreature();
    CTCCombat* combat = 0;
    hero->GetInterface(&combat, 0x30);
    if (!IsEventButtonPressed(event))
        return;
    if (IsEventGameAction(8, event) && combat->IsMeleeFlourishValid()) {
        CGameEvent gameEvent(0x16);
        gameEvent.AddUWORD(DefinitionManager->GetDefGlobalIndexFromName("CREATURE_ABILITY_FLOURISH"));
        gameEvent.AddFloat(GetButtonPressureForEvent(event));
        processed.AddGameEvent(gameEvent);
    } else if (IsEventGameAction(8, event) && combat->IsBreakBlockValid()) {
        CGameEvent gameEvent(0x16);
        gameEvent.AddUWORD(DefinitionManager->GetDefGlobalIndexFromName("CREATURE_ABILITY_BREAK_BLOCK"));
        gameEvent.AddFloat(GetButtonPressureForEvent(event));
        processed.AddGameEvent(gameEvent);
    } else if (IsEventGameAction(0xf, event)) {
        CGameEvent gameEvent(0x20);
        processed.AddGameEvent(gameEvent);
    } else if (IsEventGameAction(0xc, event)) {
        CGameEvent gameEvent(0x16);
        gameEvent.AddUWORD(DefinitionManager->GetDefGlobalIndexFromName("CREATURE_ABILITY_ATTACK"));
        gameEvent.AddFloat(GetButtonPressureForEvent(event));
        processed.AddGameEvent(gameEvent);
    } else if (IsEventGameAction(0xe, event)) {
        CGameEvent gameEvent(0x22);
        gameEvent.AddBool(true);
        processed.AddGameEvent(gameEvent);
    }
}
