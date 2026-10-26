// CQ_MinionCampScript::RunSpawnPointControl() -- script thread for mission stage 3: each frame, while there are fewer minions than the maximum and the spawn timer has run out, pick the spawn point (a random one on the side holding the furthest spawn point), restart the spawn timer with a random period between the minimum and maximum (unless this is the last minion) and flag a spawn
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DA0C40, length 322 bytes (+ int3 padding), plain `ret`.
// Identity: Ego_r.pdb ?RunSpawnPointControl@CQ_MinionCampScript@NScript@@QAEXXZ.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, FindFurthestSpawnPoint(const list<CScriptThing>&,
// const CScriptThing*&, float&) 0x00DA0D90 (manifest label "CScriptBase_RunActiveSpawnPoints" is propagated),
// GetRandomSpawnPoint(const list<CScriptThing>&, CScriptThing&) 0x00DA0E40, rand 0x00BFEB16, _ftol 0x00BFEA70; the
// CTimer helpers through the global interface 0x0143E8F8 (GetTimer slot 90, SetTimer slot 89); the quest's interface
// (+0x40) NewScriptFrame (slot 7).
// Data: 1/32767 0x0129C81C.
// Layout: retail = Ego_r.pdb - 0x14 (STLport lists are one node pointer): NumberOfMinions +0x50, MaxNumberOfMinions
// +0x54, MinMinionSpawnPeriod +0x58, MaxMinionSpawnPeriod +0x5C, MinionSpawnTimer +0x70, CurrentStage +0x74,
// LeftSpawnPoints +0x90, RightSpawnPoints +0x94, ShouldSpawn +0x98, SpawnPointToSpawn +0x9C.
// Shape: `while (CurrentStage == 3)` rotated; both FindFurthestSpawnPoint calls share the zeroed out-pointer and the
// -1 distance; the two GetRandomSpawnPoint arms tail-merge; the random period keeps the scaled rand() on the x87
// stack through the interpolation (`fmul st(1)` ... `fstp st(0)`); the right-side test is written `if (!Find..)` so
// the left arm falls through; CScriptThing is modelled 4-aligned (a char[12] body shifts SpawnPointToSpawn to +0x99).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
class CScriptThing { public: void* _body[3]; };                // 12 bytes, 4-aligned
template <class T> struct list { void* _M_node; };      // STLport list (retail 4 bytes)
extern "C" int __cdecl rand();                          // 0x00BFEB16

class CGameScriptInterface { public:                          // vtable 0x01260F0C
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
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
    virtual long RegisterTimer();   // slot 87 (+0x15c)
    virtual void DeregisterTimer(long timer);   // slot 88 (+0x160)
    virtual void SetTimer(long timer, long seconds) const;   // slot 89 (+0x164)
    virtual long GetTimer(long timer) const;   // slot 90 (+0x168)
};
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8

class CTimer { public:                                         // a script timer: the interface's timer index
    long Index;
    void SetTime(long seconds) { g_pGameScriptInterface->SetTimer(Index, seconds); }
    long GetTime() { return g_pGameScriptInterface->GetTimer(Index); }
};

namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
class CScriptBase { public:                                    // retail 0x40 bytes
    unsigned char _body[0x40];
public:
    bool IsActiveThreadTerminating();   // 0x00CB7940
};
namespace NScript {
class CQ_MinionCampScript : public CScriptBase {
public:
    CGameScriptInterface* GSI;                          // +0x40
    void*                 PMasterData;                  // +0x44
    unsigned char         _pad48[0x50 - 0x48];
    long                  NumberOfMinions;              // +0x50 (PDB +0x64)
    long                  MaxNumberOfMinions;           // +0x54
    float                 MinMinionSpawnPeriod;         // +0x58
    float                 MaxMinionSpawnPeriod;         // +0x5c
    unsigned char         _pad60[0x70 - 0x60];
    CTimer                MinionSpawnTimer;             // +0x70
    long                  CurrentStage;                 // +0x74
    unsigned char         _pad78[0x90 - 0x78];
    list<CScriptThing>    LeftSpawnPoints;              // +0x90
    list<CScriptThing>    RightSpawnPoints;             // +0x94
    bool                  ShouldSpawn;                  // +0x98
    CScriptThing          SpawnPointToSpawn;            // +0x9c
    bool FindFurthestSpawnPoint(const list<CScriptThing>& points, const CScriptThing*& furthest, float& distance);   // 0x00DA0D90
    void GetRandomSpawnPoint(const list<CScriptThing>& points, CScriptThing& point);                                  // 0x00DA0E40
    void RunSpawnPointControl();
};
void CQ_MinionCampScript::RunSpawnPointControl()
{
    while (CurrentStage == 3) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (NumberOfMinions < MaxNumberOfMinions && MinionSpawnTimer.GetTime() <= 0) {
            if (IsActiveThreadTerminating())
                return;
            const CScriptThing* furthest = 0;
            float distance = -1.0f;
            FindFurthestSpawnPoint(LeftSpawnPoints, furthest, distance);
            if (!FindFurthestSpawnPoint(RightSpawnPoints, furthest, distance)) {
                if (IsActiveThreadTerminating())
                    return;
                GetRandomSpawnPoint(LeftSpawnPoints, SpawnPointToSpawn);
            } else {
                if (IsActiveThreadTerminating())
                    return;
                GetRandomSpawnPoint(RightSpawnPoints, SpawnPointToSpawn);
            }
            if (NumberOfMinions < MaxNumberOfMinions - 1) {
                if (IsActiveThreadTerminating())
                    return;
                float t = rand() * (1.0f / 32767.0f);
                MinionSpawnTimer.SetTime((long)(MinMinionSpawnPeriod + (MaxMinionSpawnPeriod - MinMinionSpawnPeriod) * t));
            }
            ShouldSpawn = true;
        }
    }
    if (IsActiveThreadTerminating())
        return;
}
}