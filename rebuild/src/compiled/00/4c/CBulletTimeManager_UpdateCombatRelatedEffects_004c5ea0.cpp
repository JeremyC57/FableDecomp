// CBulletTimeManager::UpdateCombatRelatedEffects() -- while the main player's creature is performing a combat action, keep the default bullet-time multiplier on and restart the strike-action countdown; otherwise count it down
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C5EA0, length 107 bytes.
// Callees: CPlayerManager::GetMainPlayer @ 0x00449970 (landed name), CPlayer::IsInitialised @ 0x0099A330 (the shared
// CInitBaseClass `mov al,[ecx+4]` query), CPlayer::GetPlayerCreature @ 0x00487DC0 (landed name),
// CThingCreatureBase::IsPerformingAnyAction @ 0x006622A0; virtual slot 15 (+0x3c) of the creature's object at +0x100
// (a bool "doing a strike" query). Globals 0x01375550 and 0x01378D08 are multiplied for the countdown length.
// Layout: rebuild/include/engine/CBulletTimeManager.h puts a 0x10-byte map first; retail reads PlayerManager at
// +0x18 (the header says +0x1c) and the flag/tick at +0x0d/+0x10, i.e. every member sits 4 bytes lower (the retail
// map is 12 bytes, as other retail STL containers). A local model with the header's names is used.
// Shape: the main player is fetched twice (no CSE across the out-of-line calls); the creature pointer lives in edi;
// the countdown decrement is `if (tick > 0) tick = tick - 1` read once into eax.
struct CStrikeQuery {                          // the creature's object at +0x100
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual bool IsStriking();                // slot 15 (+0x3c)
};
struct CThingPlayerCreature {
    unsigned char _pad[0x100];
    CStrikeQuery* m_pStrikeQuery;             // +0x100
    bool IsPerformingAnyAction();             // 0x006622A0
};
struct CPlayer {
    bool IsInitialised();                     // 0x0099A330
    CThingPlayerCreature* GetPlayerCreature();// 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                 // 0x00449970
};
extern long g_BulletTimeStrikeSeconds;        // 0x01375550
extern long g_BulletTimeFramesPerSecond;      // 0x01378D08

class CBulletTimeManager {
public:
    unsigned char   BulletTimeInfoMap[0x0c];  // +0x00 map<void*, CBulletTimeInfo> (12 bytes in retail)
    bool            Override;                 // +0x0c
    bool            UseDefaultMultiplier;     // +0x0d
    long            LastStrikeActionTick;     // +0x10
    void*           World;                    // +0x14 CWorld*
    CPlayerManager* PlayerManager;            // +0x18
    void UpdateCombatRelatedEffects();
};

void CBulletTimeManager::UpdateCombatRelatedEffects()
{
    if (!PlayerManager->GetMainPlayer()->IsInitialised())
        return;
    CThingPlayerCreature* creature = PlayerManager->GetMainPlayer()->GetPlayerCreature();
    if (creature == 0 || !creature->IsPerformingAnyAction())
        return;
    if (creature->m_pStrikeQuery->IsStriking()) {
        UseDefaultMultiplier = true;
        LastStrikeActionTick = g_BulletTimeStrikeSeconds * g_BulletTimeFramesPerSecond;
    } else if (LastStrikeActionTick > 0) {
        LastStrikeActionTick = LastStrikeActionTick - 1;
    }
}