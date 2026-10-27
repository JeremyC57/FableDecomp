// NScript::CQ_GuildTrainingMeleeScript::Init() -- reset the melee-training state: tutorial counter 0, tutorial state 1, the Whisper flags cleared, melee repeating on
// Leaf: ?Init@CQ_GuildTrainingMeleeScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D55DA0, 35 bytes, thiscall, plain `ret` (virtual, level-script vtable slot 3).
// Identity (proven): slot 3 of the RTTI-less level-script vtable 0x012D0274, which the landed NScript::CQ_GuildTrainingMeleeScript::CQ_GuildTrainingMeleeScript 0x00D55D70 (and the Alloc 0x00D5A9F0) stores;
//   the level-script slots are 0 dtor, 1 RegisterMain, 2 Main, 3 Init, 4 OnPersist (FableWin.pdb CScriptBase order; slot 1
//   and 2 of these vtables are the landed RegisterMain / Main rows, slot 4 of 0x012CBA58 the landed CQ_EndGameScript::OnPersist
//   0x00D29830). Ego_r.pdb has ?Init@CQ_GuildTrainingMeleeScript@NScript@@UAEXXZ.
// Layout: Ego_r.pdb CQ_GuildTrainingMeleeScript members, each at Ego_r offset - 0x14.
// Shape: retail stores +0x50, +0x52, +0x51, +0x53: the source order.
class CGameScriptInterfaceBase;
class CPersistContext;
class CScriptBase {                                     // retail 0x40 bytes (Ego_r 0x54: debug containers)
public:
    virtual ~CScriptBase();                             // slot 0
protected:
    virtual void RegisterMain();                        // slot 1
    virtual void Main();                                // slot 2
    virtual void Init();                                // slot 3
public:
    virtual void OnPersist(CPersistContext& context);   // slot 4
    unsigned char _body[0x3c];
};
template <class TData> class CLevelScriptBase : public CScriptBase {
public:
    CGameScriptInterfaceBase* Game;                     // +0x40
    TData*                    MasterScriptData;         // +0x44
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_GuildTrainingMeleeScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    long GenericTutorialCounter;                        // +0x48
    long TutorialState;                                 // +0x4c
    bool TalkedToWhisper;                               // +0x50
    bool WhisperArrived;                                // +0x51
    bool EarlyHitWhisper;                               // +0x52
    bool WhisperStopWalking;                            // +0x53
    bool MeleeRepeating;                                // +0x54
    bool MeleeRepeatKnown;                              // +0x55
    bool MeleeOpponentReset;                            // +0x56
    virtual void Init();
};

void CQ_GuildTrainingMeleeScript::Init()
{
    GenericTutorialCounter = 0;
    TutorialState = 1;
    TalkedToWhisper = false;
    EarlyHitWhisper = false;
    WhisperArrived = false;
    WhisperStopWalking = false;
    MeleeRepeating = true;
    MeleeRepeatKnown = false;
    MeleeOpponentReset = false;
}
}