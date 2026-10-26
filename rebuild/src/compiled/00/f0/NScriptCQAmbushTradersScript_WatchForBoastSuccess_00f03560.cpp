// CQ_AmbushTradersScript::WatchForBoastSuccess() -- script thread: once all guards (or all traders) are dead, record the matching boast as achieved on the owning quest record (unless the thread is terminating); waits a script frame per pass
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F03560, length 118 bytes.
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (landed), NewScriptFrame (slot 7, +0x1c) through the
// quest's game script interface (+0x40). Referenced only as data (0x00F02A20, the thread table).
// Layout: AllTradersDead +0x53, AllGuardsDead +0x54 (Ego_r.pdb +0x67/+0x68 - 0x14); the owner pointer (+0x44) is the
// quest's persistent record, whose boast flags sit at +0xc8 (guards) and +0xc9 (traders).
// Shape: `while (!terminating)` rotated; the `true` stored into both flags is hoisted into bl before the loop.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CGameScriptInterface { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
};

class CScriptBase { public:                                        // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                              // +0x04 .. +0x40
    bool IsActiveThreadTerminating();                       // 0x00CB7940
};

struct SAmbushTradersRecord {                               // the owner record (descriptive)
    unsigned char _pad00[0xc8];
    bool BoastGuardsDeadAchieved;                           // +0xc8
    bool BoastTradersDeadAchieved;                          // +0xc9
};

struct CQuestScriptBase : CScriptBase {                     // retail 0x48 bytes
    CGameScriptInterface* GSI;                              // +0x40
    SAmbushTradersRecord* Owner;                            // +0x44
};

namespace NScript {
class CQ_AmbushTradersScript : public CQuestScriptBase {
public:
    bool QuestFailed;                                       // +0x48
    bool QuestFinished;                                     // +0x49
    bool QuestSucceeded;                                    // +0x4a
    bool QuestOver;                                         // +0x4b
    bool RegionInvalid;                                     // +0x4c
    bool BanditIntroDone;                                   // +0x4d
    bool TraderEscaped;                                     // +0x4e
    bool NoEnemiesLeft;                                     // +0x4f
    bool PhaseComplete;                                     // +0x50
    bool ConvoyTrigger2;                                    // +0x51
    bool AllBanditsDead;                                    // +0x52
    bool AllTradersDead;                                    // +0x53
    bool AllGuardsDead;                                     // +0x54
    void WatchForBoastSuccess();
};
} // namespace NScript
using NScript::CQ_AmbushTradersScript;

void NScript::CQ_AmbushTradersScript::WatchForBoastSuccess()
{
    while (!IsActiveThreadTerminating()) {
        if (AllGuardsDead && !Owner->BoastGuardsDeadAchieved) {
            if (IsActiveThreadTerminating())
                return;
            Owner->BoastGuardsDeadAchieved = true;
        }
        if (AllTradersDead && !Owner->BoastTradersDeadAchieved) {
            if (IsActiveThreadTerminating())
                return;
            Owner->BoastTradersDeadAchieved = true;
        }
        GSI->NewScriptFrame();
    }
}