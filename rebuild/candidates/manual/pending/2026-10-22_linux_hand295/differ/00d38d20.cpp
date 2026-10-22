// CQ_FireHeartScript::RunVillagerComments() -- until the mission succeeds, every 25 seconds pick at random one villager within 25 of the hero and flag him to speak
// Grade: DIFFER(529v526) -- block layout: retail places the loop-exit path (inline ~vector free + DeregisterTimer) before the terminating-return copies, ours after; plus `lea ebp,[ebx+eax]` vs `mov ebp,[esi+0x110] ; add ebp,ebx` for &Villagers[i]
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D38D20, length 526 bytes.
// Callees: vector<long>(size_type) 0x00D38F30 (out of line), free 0x00BFEA14 (inline vector destructor copies), rand
//   0x00BFEB16, GetDistanceBetweenThings 0x00CBE583 (__fastcall, float), CScriptBase::IsActiveThreadTerminating
//   0x00CB7940; through the interface (+0x40): NewScriptFrame (7), GetHero (70); the timer service (global 0x0143E8F8):
//   RegisterTimer (87), DeregisterTimer (88), SetTimer (89), GetTimer (90) through a local CTimer.
// Layout: the landed CQ_FireHeartScript ctor 0x00D34470: MissionSucceeded +0x49, NumberOfVillagers +0x84,
//   ShouldVillagerSpeak +0xec (retail vector<bool> is a plain byte vector), Villagers +0x110.
// Shape: a local CTimer (registered in its constructor, deregistered in its destructor) and a local vector of candidate
//   indices; VC7.1 expands both destructors at the three exits.
extern "C" void __cdecl free(void* p);             // 0x00BFEA14
int __cdecl rand();                                // 0x00BFEB16
struct CScriptThing { unsigned char _body[12]; };
template <class T> struct vector {                 // STLport vector (retail 12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    explicit vector(unsigned int n);               // 0x00D38F30 for long
    ~vector() { if (_M_start) free(_M_start); }
    T& operator[](unsigned int n) { return _M_start[n]; }
};
float __fastcall GetDistanceBetweenThings(const CScriptThing& a, const CScriptThing& b);   // 0x00CBE583
struct CGameScriptInterface {                       // the quest's game script interface (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c) NewScriptFrame
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual CScriptThing& GetHero();   // slot 70 (+0x118) GetHero
    virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078();
    virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086();
    virtual long RegisterTimer();   // slot 87 (+0x15c) RegisterTimer
    virtual void DeregisterTimer(long timer);   // slot 88 (+0x160) DeregisterTimer
    virtual void SetTimer(long timer, long time);   // slot 89 (+0x164) SetTimer
    virtual long GetTimer(long timer) const;   // slot 90 (+0x168) GetTimer
};
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
struct CTimer {                                         // a script timer: the interface's timer index
    long Index;
    CTimer() { Index = g_pGameScriptInterface->RegisterTimer(); }
    ~CTimer() { g_pGameScriptInterface->DeregisterTimer(Index); }
    void SetTime(long time) { g_pGameScriptInterface->SetTimer(Index, time); }
    long GetTime() const { return g_pGameScriptInterface->GetTimer(Index); }
};
namespace NScript {
struct CScriptBase {                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};
struct CQ_FireHeartScript : CScriptBase {
    CGameScriptInterface* Game;                         // +0x40
    void*                 Data;                         // +0x44
    bool                  QuestStartScreenShown;        // +0x48
    bool                  MissionSucceeded;             // +0x49
    unsigned char         _pad4a[0x84 - 0x4a];
    long                  NumberOfVillagers;            // +0x84
    unsigned char         _pad88[0xec - 0x88];
    vector<bool>          ShouldVillagerSpeak;          // +0xec
    unsigned char         _padf8[0x110 - 0xf8];
    vector<CScriptThing>  Villagers;                    // +0x110
    void RunVillagerComments();
};
void CQ_FireHeartScript::RunVillagerComments()
{
    CTimer commentTimer;
    vector<long> candidates(NumberOfVillagers);
    while (!MissionSucceeded) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        commentTimer.SetTime(25);
        while (commentTimer.GetTime() != 0) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        long count;
        do {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            count = 0;
            for (long i = 0; i < NumberOfVillagers; ++i) {
                if (IsActiveThreadTerminating())
                    return;
                if (GetDistanceBetweenThings(Game->GetHero(), Villagers[i]) <= 25.0f) {
                    if (IsActiveThreadTerminating())
                        return;
                    candidates[count] = i;
                    count++;
                }
            }
            if (IsActiveThreadTerminating())
                return;
        } while (count <= 0);
        if (IsActiveThreadTerminating())
            return;
        ShouldVillagerSpeak[candidates[rand() % count]] = true;
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
}
}
