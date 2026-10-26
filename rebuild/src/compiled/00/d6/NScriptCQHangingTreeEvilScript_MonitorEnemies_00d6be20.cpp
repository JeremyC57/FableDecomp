// CQ_HangingTreeEvilScript::MonitorEnemies() -- script thread: wait until every current enemy is dead (then flag AllEnemiesDead); if the hero switches regions first, wait for the switch to finish, re-collect the HTE_AssaultGuard enemies and, if any still live, restart this monitor as a fresh thread
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D6BE20, length 369 bytes, plain `ret`.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, AreAllThingsInVectorDead (__fastcall, 0x00CBED00),
// vector<CScriptThing>::erase(iterator, iterator) 0x008AC9D0, CCharString(const char*, long) 0x0099EBF0 /
// ~CCharString 0x0099EAE0, operator new 0x00BFEA1A, CSpawnedFuncBase::CSpawnedFuncBase 0x00CDD450 (CSpawnedFunc vtable
// 0x012D1FE8), CScriptBase::AddSpawnedFunction 0x00CB7E50; through the quest's interface (+0x40): NewScriptFrame
// (slot 7), GetAllThingsWithScriptName (75).
// Strings: "HTE_AssaultGuard" 0x012D2064, "MonitorEnemies" 0x012D22F0, "" 0x0122D70E.
// Layout: retail = Ego_r.pdb - 0x14 (as the landed BanditYells 0x00D69CD0): SwitchingRegions +0x51, AllEnemiesDead
// +0x53, CurrentEnemyVector (STLport, 12 bytes) +0xAC.
// Shape: the region-switch arm sits inside the wait loop and ends the function (laid out after the main return);
// the respawn uses the Kraken DoMission (0x00DF4E00) inline `const char*` spawn wrapper (thread name guarded by an
// entry-zeroed flag). The thread spawns THIS function, and VC7.1 then refuses to inline the CSpawnedFunc ctor and the
// wrapper (both emitted out of line, the frame shrinks to `push ecx`) unless they are `__forceinline`.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CScriptThing { unsigned char _body[12]; };
template <class T> struct vector {                      // STLport vector (retail 12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    T* erase(T* first, T* last);                        // 0x008AC9D0 for CScriptThing
    void clear() { erase(begin(), end()); }
};
bool __fastcall AreAllThingsInVectorDead(vector<CScriptThing>* things);   // 0x00CBED00

void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A
struct CSpawnedFuncBase {                               // retail 0x34 bytes
    virtual ~CSpawnedFuncBase();
    virtual void RunSuspendableProcess();
    virtual void SuspendSuspendableProcess(bool suspend);
    virtual void Restart();
    virtual void SuspendableProcess() = 0;
    unsigned char _body[0x30];                          // +0x04 .. +0x34
    CSpawnedFuncBase(const CCharString& name, unsigned long id);   // 0x00CDD450
};
template <class T> struct CSpawnedFunc : CSpawnedFuncBase {
    void (T::*Func)();                                  // +0x34
    T* PClass;                                          // +0x38
    __forceinline CSpawnedFunc(const CCharString& name, unsigned long id, T* pclass, void (T::*func)())
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass) {}
    virtual void SuspendableProcess() { (PClass->*Func)(); }
};

struct CGameScriptInterface {                          // vtable 0x01260F0C
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
    virtual void v072(); virtual void v073(); virtual void v074();
    virtual long GetAllThingsWithScriptName(const CCharString& name, vector<CScriptThing>& things) const;   // slot 75 (+0x12c)
};

namespace NScript {
struct CScriptBase {                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                   // 0x00CB7940
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName);   // 0x00CB7E50
    __forceinline void SpawnFunction(CSpawnedFuncBase* func, const char* regionName = "") { AddSpawnedFunction(func, CCharString(regionName)); }
};
class CQ_HangingTreeEvilScript : public CScriptBase {
public:
    CGameScriptInterface* GSI;                          // +0x40
    void*                 Data;                         // +0x44
    unsigned char         _pad48[0x51 - 0x48];
    bool                  SwitchingRegions;             // +0x51 (PDB +0x65)
    bool                  RemoveBandits;                // +0x52
    bool                  AllEnemiesDead;               // +0x53
    unsigned char         _pad54[0xac - 0x54];
    vector<CScriptThing>  CurrentEnemyVector;           // +0xac (PDB +0xc0)
    void MonitorEnemies();
};
void CQ_HangingTreeEvilScript::MonitorEnemies()
{
    AllEnemiesDead = false;
    while (!AreAllThingsInVectorDead(&CurrentEnemyVector)) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (SwitchingRegions) {
            if (IsActiveThreadTerminating())
                return;
            while (SwitchingRegions) {
                GSI->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            CurrentEnemyVector.clear();
            AllEnemiesDead = true;
            GSI->GetAllThingsWithScriptName("HTE_AssaultGuard", CurrentEnemyVector);
            if (AreAllThingsInVectorDead(&CurrentEnemyVector))
                return;
            if (IsActiveThreadTerminating())
                return;
            SpawnFunction(new CSpawnedFunc<CQ_HangingTreeEvilScript>("MonitorEnemies", 0, this, &CQ_HangingTreeEvilScript::MonitorEnemies));
            return;
        }
    }
    if (IsActiveThreadTerminating())
        return;
    AllEnemiesDead = true;
}
}