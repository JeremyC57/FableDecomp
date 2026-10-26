// CQ_HangingTreeGoodScript::CQ_HangingTreeGoodScript(void* owner, CGameScriptInterface* game) -- quest-script constructor: script base, owner/interface pair, vtable 0x012D260C, four empty CScriptThing handles, two empty strings and five empty vectors
// Leaf: ??0CQ_HangingTreeGoodScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D6EF20, length 246 bytes, `ret 8`.
// Callees: CScriptBase::CScriptBase @ 0x00CB8110, CBase::CBase @ 0x0099A2D0 (the CScriptThing root, x4; CScriptThing
// vtable 0x01238C8C), CCharString::CCharString() @ 0x0099E4B0 (x2).
// Layout: Ego_r.pdb CQ_HangingTreeGoodScript - 0x14 (retail STLport vectors are 12 bytes): bools +0x48..+0x59,
// HTG_State +0x5c, CurrentWayPoint +0x60, CurrentGuards +0x64, Leader +0x68, Prisoner +0x74, PartySpawnPoint +0x80,
// ActiveWayPoint +0x8c, CurrentLevel +0x98, ReasonForFailing +0x9c, CurrentEnemyVector +0xa0, CurrentGuardVector
// +0xac, RegionsWhereSpawned +0xb8, AreasSpawnedIn +0xc4, GuardsHealthVector +0xd0 (0xdc bytes).
// Shape: the landed CV_BanditCampPathScript ctor model (0x00E46BD0): novtable script-object base storing the
// arguments before the vtable, CScriptThing members whose PImp member object zeroes Data/Info after the vtable store,
// the real STLport _Vector_base/_STLP_alloc_proxy vector; every zero word shares edi.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
struct CGameScriptInterface;

struct CScriptBase {
    virtual void v00();
    unsigned char _body[0x3c];                       // +0x04..+0x40
    CScriptBase();                                   // 0x00CB8110
};

struct __declspec(novtable) CScriptObject : CScriptBase {
    CGameScriptInterface* m_pGame;                   // +0x40
    void* m_pOwner;                                  // +0x44
    CScriptObject(void* owner, CGameScriptInterface* game) : CScriptBase(), m_pGame(game), m_pOwner(owner) { }
};

namespace N_0099a2d0 { struct CBase {
    CBase();                                         // 0x0099A2D0
    virtual ~CBase();                                // 0x0099A2E0
}; } using N_0099a2d0::CBase;

struct CScriptThing;
struct CCPPointerInfo;

struct CCountedPointer_CScriptThing {                // CScriptThing::PImp
    CScriptThing* Data;                              // +0
    CCPPointerInfo* Info;                            // +4
    CCountedPointer_CScriptThing() : Data(0), Info(0) { }
};

struct CScriptThing : CBase {                        // vtable 0x01238C8C
    CCountedPointer_CScriptThing PImp;               // +0x04
    CScriptThing() { }
    virtual ~CScriptThing();
};

struct CCharString {
    void* PStringData;
    CCharString();                                   // 0x0099E4B0
};

template <class T>
struct allocator {
    allocator() { }
};

template <class T, class A>
struct _STLP_alloc_proxy : A {                       // STLport: the end-of-storage pointer over an empty allocator
    T _M_data;
    _STLP_alloc_proxy(const A& a, T p) : A(a), _M_data(p) { }
};

template <class T>
struct _Vector_base {
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, allocator<T> > _M_end_of_storage;
    _Vector_base(const allocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0) { }
};

template <class T>
struct CRetailVector : _Vector_base<T> {             // STLport vector<T>: three pointers (12 bytes in retail)
    CRetailVector() : _Vector_base<T>(allocator<T>()) { }
};

namespace NScript {
class CQ_HangingTreeGoodScript : public CScriptObject {
public:    // vtable 0x012D260C
    bool LeaderSpokenTo;                             // +0x48
    bool PartySuccessfullySetUp;                     // +0x49
    bool MissionSucceeded;                           // +0x4a
    bool MissionFailed;                              // +0x4b
    bool SwitchingRegions;                           // +0x4c
    bool ReassignLeaderNow;                          // +0x4d
    bool PartyDefeated;                              // +0x4e
    bool SpawnHere;                                  // +0x4f
    bool AtWayPoint;                                 // +0x50
    bool HeroKilledPrisoner;                         // +0x51
    bool QuestStartScreenShown;                      // +0x52
    bool PartyReadyToTransition;                     // +0x53
    bool BanditYelledOut;                            // +0x54
    bool ExecutionerBeenAttacked;                    // +0x55
    bool ExecutionerBeenAttackedByHero;              // +0x56
    bool BanditsFlee;                                // +0x57
    bool PrisonerEscaping;                           // +0x58
    bool ExitCutsceneStart;                          // +0x59
    long HTG_State;                                  // +0x5c
    long CurrentWayPoint;                            // +0x60
    long CurrentGuards;                              // +0x64
    CScriptThing Leader;                             // +0x68
    CScriptThing Prisoner;                           // +0x74
    CScriptThing PartySpawnPoint;                    // +0x80
    CScriptThing ActiveWayPoint;                     // +0x8c
    CCharString  CurrentLevel;                       // +0x98
    CCharString  ReasonForFailing;                   // +0x9c
    CRetailVector<CScriptThing> CurrentEnemyVector;  // +0xa0
    CRetailVector<CScriptThing> CurrentGuardVector;  // +0xac
    CRetailVector<CCharString>  RegionsWhereSpawned; // +0xb8
    CRetailVector<CCharString>  AreasSpawnedIn;      // +0xc4
    CRetailVector<float>        GuardsHealthVector;  // +0xd0
    virtual void v00();
    CQ_HangingTreeGoodScript(void* owner, CGameScriptInterface* game);
};
} // namespace NScript
using NScript::CQ_HangingTreeGoodScript;

NScript::CQ_HangingTreeGoodScript::CQ_HangingTreeGoodScript(void* owner, CGameScriptInterface* game)
    : CScriptObject(owner, game)
{
}