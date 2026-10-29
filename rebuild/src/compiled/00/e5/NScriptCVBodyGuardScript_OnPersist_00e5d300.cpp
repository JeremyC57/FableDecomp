// CV_BodyGuardScript::OnPersist(CPersistContext&) -- save/load the four henchmen's global status and the four respawn timers (each timer's remaining time goes through the context as a float and is written back)
// Leaf: ?OnPersist@CV_BodyGuardScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E5D300, 483 bytes (496 with padding), `ret 4`.
// Callees: CPersistContext::Transfer(const char*, long&, const long&) 0x00410BE0 and (const char*, float&, const float&)
//   0x00410620, __ftol2 0x00BFEA70; the global script interface 0x0143E8F8 slots SetTimer (89) and GetTimer (90) behind
//   the inline CTimer accessors.
// Strings: "HenchmanGlobalStatus[HT_EVIL1]" 0x012E6F2C .. "[HT_GOOD2]" 0x012E6ECC, "Evil1RespawnTimer" 0x012E6EB8,
//   "Evil2RespawnTimer" 0x012E6EA4, "Good1RespawnTimer" 0x012E6E90, "Good2RespawnTimer" 0x012E6E7C.
// Layout (retail = Ego_r.pdb - 0x14, less 4 more after the 12-byte STLport HenchmanList at +0x64):
//   HenchmanDeadRespawnTimer[4] +0x74, HenchmanGlobalStatus[4] +0x84.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CPersistContext { public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // CPersistContext::Transfer<T> (member template, PDB)
    //   instances here: long 0x00410BE0; float 0x00410620
};
class CGameScriptInterface { public:                   // retail vtable 0x01260F0C (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086();
    virtual long RegisterTimer();                                                  // slot 87 (+0x15c)
    virtual void DeregisterTimer(long index);                                      // slot 88 (+0x160)
    virtual void SetTimer(long index, long time);                                  // slot 89 (+0x164)
    virtual long GetTimer(long index) const;                                       // slot 90 (+0x168)
};

extern CGameScriptInterface* g_pGameScriptInterface;        // 0x0143E8F8
class CTimer { public:                                             // 4 bytes
    long TimerIndex;
    long GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
    void SetTime(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); }
};
struct CScriptThingVector {                                 // std::vector<CScriptThing> (STLport, 12 bytes)
    void* _M_start;
    void* _M_finish;
    void* _M_end_of_storage;
};
class CScriptBase { public:                                        // retail 0x40 bytes
    virtual void v00();
    unsigned char _body[0x3c];
};
struct CQuestScriptBase : CScriptBase {
    void* Game;                                             // +0x40
    void* Data;                                             // +0x44
};
namespace NScript {
class CV_BodyGuardScript : public CQuestScriptBase {
public:
    long               TotalWages;                          // +0x48
    bool               HeroCannotAffordHelp;                // +0x4c
    bool               HeroWieldingWeapon;                  // +0x4d
    long               HeroStoodStillCounter;               // +0x50 CCounter
    CTimer             HenchmanLastIdleChatTimer;           // +0x54
    CTimer             HenchmanLastBoredChatTimer;          // +0x58
    CTimer             CommentaryTimer;                     // +0x5c
    CTimer             HenchmanFreakedOutThePlayerTimer;    // +0x60
    CScriptThingVector HenchmanList;                        // +0x64
    long               ConversationIndex;                   // +0x70
    CTimer             HenchmanDeadRespawnTimer[4];         // +0x74
    long               HenchmanGlobalStatus[4];             // +0x84
    void OnPersist(CPersistContext& context);
};
} // namespace NScript
using NScript::CV_BodyGuardScript;

void NScript::CV_BodyGuardScript::OnPersist(CPersistContext& context)
{
    context.Transfer<long>("HenchmanGlobalStatus[HT_EVIL1]", HenchmanGlobalStatus[0], 0);
    context.Transfer<long>("HenchmanGlobalStatus[HT_EVIL2]", HenchmanGlobalStatus[1], 0);
    context.Transfer<long>("HenchmanGlobalStatus[HT_GOOD1]", HenchmanGlobalStatus[2], 0);
    context.Transfer<long>("HenchmanGlobalStatus[HT_GOOD2]", HenchmanGlobalStatus[3], 0);
    float evil1 = (float)HenchmanDeadRespawnTimer[0].GetTime();
    float evil2 = (float)HenchmanDeadRespawnTimer[1].GetTime();
    float good1 = (float)HenchmanDeadRespawnTimer[2].GetTime();
    float good2 = (float)HenchmanDeadRespawnTimer[3].GetTime();
    context.Transfer("Evil1RespawnTimer", evil1, 0.0f);
    context.Transfer("Evil2RespawnTimer", evil2, 0.0f);
    context.Transfer("Good1RespawnTimer", good1, 0.0f);
    context.Transfer("Good2RespawnTimer", good2, 0.0f);
    HenchmanDeadRespawnTimer[0].SetTime((long)evil1);
    HenchmanDeadRespawnTimer[1].SetTime((long)evil2);
    HenchmanDeadRespawnTimer[2].SetTime((long)good1);
    HenchmanDeadRespawnTimer[3].SetTime((long)good2);
}