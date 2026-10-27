// NScript::CV_FishermanScript::Init() -- reset the script's state: HeroHasSavedFishermanFromWasps, IntroPlayed, WaspsAlive, QuestCardGiven, FishermanTasks = 0/false
// Leaf: ?Init@CV_FishermanScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E7B420, 18 bytes, thiscall, plain `ret`.
// Identity (proven): slot 3 of the script vtable 0x012EA024 (stored by the landed script constructor / Alloc); FableWin.pdb
//   ?Init@CV_FishermanScript@NScript@@UAEXXZ; the members it clears are Ego_r.pdb's CV_FishermanScript fields at their PDB - 0x14 offsets.
// Shape (family K066, representative 0x00CF06D0): one zero register (al, or eax when a long is cleared) and one store per
//   member in source order; FishermanMustRunAway is left alone.
class CScriptDataBase;
class CGameScriptInterfaceBase;
class CPersistContext;
class CScriptBase {                                     // retail 0x40 bytes
public:
    virtual ~CScriptBase();
protected:
    virtual void RegisterMain();
    virtual void Main();
    virtual void Init();                                // slot 3
public:
    virtual void OnPersist(CPersistContext& context);
    unsigned char _body[0x3c];                          // +0x04 .. +0x40
};
class CLevelScriptBase : public CScriptBase {           // PDB CLevelScriptBase<TData> (TData enters no symbol here)
public:
    CGameScriptInterfaceBase* Game;                     // +0x40
    CScriptDataBase* MasterScriptData;                  // +0x44
};
namespace NScript {
class CV_FishermanScript : public CLevelScriptBase {
public:
    virtual void Init();
    bool  IntroPlayed;                             // +0x48
    bool  HeroHasSavedFishermanFromWasps;          // +0x49
    long  WaspsAlive;                              // +0x4C
    bool  FishermanMustRunAway;                    // +0x50
    bool  QuestCardGiven;                          // +0x51
    long  FishermanTasks;                          // +0x54
};

void CV_FishermanScript::Init()
{
    HeroHasSavedFishermanFromWasps = false;
    IntroPlayed = false;
    WaspsAlive = 0;
    QuestCardGiven = false;
    FishermanTasks = 0;
}
}