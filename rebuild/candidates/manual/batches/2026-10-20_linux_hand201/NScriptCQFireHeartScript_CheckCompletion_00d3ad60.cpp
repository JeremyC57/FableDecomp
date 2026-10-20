// NScript::CQ_FireHeartScript::CheckCompletion() -- after a round: when every prophet has been freed or killed play the Fireheart outro and mark the mission succeeded, otherwise spawn the StartRound thread for the next round
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D3AD60, length 212 bytes, plain `ret`.
// Identity: Ego_r.pdb `?CheckCompletion@CQ_FireHeartScript@NScript@@QAEXXZ`.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; CQ_FireHeartScript::PlayCutscene(CCharString) 0x00D39330
// (Ego_r.pdb `?PlayCutscene@CQ_FireHeartScript@NScript@@QAEXVCCharString@@@Z`; catalog label "Game_InitializeRound" is
// propagated); operator new 0x00BFEA1A; CSpawnedFuncBase::CSpawnedFuncBase(const CCharString&, unsigned long)
// 0x00CDD450; CScriptBase::AddSpawnedFunction(CSpawnedFuncBase*, const CCharString&) 0x00CB7E50 (Ego_r.pdb
// `?AddSpawnedFunction@CScriptBase@@QAEXPAVCSpawnedFuncBase@@ABVCCharString@@@Z`; catalog label
// "CGuiVarTransferStruct::Add" is propagated); CQ_FireHeartScript::StartRound 0x00D39960 (stored as the thread's
// member-function pointer); CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0.
// Strings: "CS_FIREHEART_OUTRO" @ 0x012CD418, "StartRound" @ 0x012CD228, "" @ 0x0122D70E.
// Layout: retail quest base 0x48 bytes; MissionSucceeded +0x49, FreedProphets +0x64, KilledProphets +0x68,
// NumberOfProphets +0x78 (Ego_r.pdb less 0x14 and 4 per map); CSpawnedFunc0<CQ_FireHeartScript> is 0x3C bytes (the
// 0x34-byte CSpawnedFuncBase, Func +0x34, PClass +0x38; vtable 0x012CCBA4).
// Shape: both arms open with a termination return (VC hoists only `mov ecx,edi`); the cutscene name is built straight
// in PlayCutscene's argument slot (`push ecx ; mov ecx,esp`: a by-value CCharString with a user copy constructor);
// the thread's name is an implicit conversion temporary that exists only when operator new succeeded, so VC guards
// its destructor with a flag bit in ebx; the empty region name is an explicit temporary (the ctor's eax is pushed).
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                     // 0x0099EAE0
};

void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A

struct CSpawnedFuncBase {                               // retail 0x34 bytes (CScriptProcessBase + thread state)
    virtual ~CSpawnedFuncBase();
    virtual void RunSuspendableProcess();
    virtual void SuspendSuspendableProcess(bool suspend);
    virtual void Restart();
    virtual void SuspendableProcess() = 0;
    unsigned char _body[0x30];                          // +0x04 .. +0x34
    CSpawnedFuncBase(const CCharString& name, unsigned long id);   // 0x00CDD450
};

template <class T> struct CSpawnedFunc0 : CSpawnedFuncBase {
    void (T::*Func)();                                  // +0x34
    T* PClass;                                          // +0x38
    CSpawnedFunc0(const CCharString& name, unsigned long id, T* pclass, void (T::*func)())
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass) {}
    virtual void SuspendableProcess() { (PClass->*Func)(); }
};

namespace NScript {
struct CScriptBase {                                    // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                          // +0x04 .. +0x40 (PActiveThread at +0x2c)
    bool IsActiveThreadTerminating();                   // 0x00CB7940
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName);   // 0x00CB7E50
};

struct CQ_FireHeartScript : CScriptBase {
    void* GSI;                                          // +0x40
    void* PMasterData;                                  // +0x44
    bool  QuestStartScreenShown;                        // +0x48
    bool  MissionSucceeded;                             // +0x49
    bool  MissionFailed;                                // +0x4a
    unsigned char _maps04c[0x18];                       // +0x4c .. +0x64 (CutsceneFlags, csargs: 12-byte maps)
    long  FreedProphets;                                // +0x64
    long  KilledProphets;                               // +0x68
    long  StartNumberOfSuns;                            // +0x6c
    long  NumberOfRounds;                               // +0x70
    long  NumberOfPatterns;                             // +0x74
    long  NumberOfProphets;                             // +0x78
    void CheckCompletion();
    void PlayCutscene(CCharString name);                // 0x00D39330
    void StartRound();                                  // 0x00D39960
};

void CQ_FireHeartScript::CheckCompletion()
{
    if (FreedProphets + KilledProphets == NumberOfProphets) {
        if (IsActiveThreadTerminating())
            return;
        PlayCutscene(CCharString("CS_FIREHEART_OUTRO", -1));
        MissionSucceeded = true;
        return;
    }
    if (IsActiveThreadTerminating())
        return;
    AddSpawnedFunction(new CSpawnedFunc0<CQ_FireHeartScript>("StartRound", 0, this, &CQ_FireHeartScript::StartRound),
                       CCharString("", -1));
}
}
