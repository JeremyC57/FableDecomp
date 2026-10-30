// CQ_HangingTreeGoodScript::SpawnEnemies(const C3DVector&, CCharString, long) -- unless the party is gone or the leader is dead: during an ambush stage give it up to 10 seconds, then create the given number of bandits near the point (each targeting the leader and allied to the prisoner), collect them, have the first one shout the charge, and move to stage 5
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D736B0, length 692 bytes (`ret 0xc`); called from 0x00D7367A,
//   0x00D73A99, 0x00D73C4F and 0x00D73EC2.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, ~CTimer 0x00CD4470, CCharString(const char*, long)
//   0x0099EBF0 / ~CCharString 0x0099EAE0, ~CScriptThing 0x004AA840, vector<CScriptThing>::erase(iterator, iterator)
//   0x008AC9D0; CScriptThing::IsDead (slot 76); the timer service g_pGameScriptInterface 0x0143E8F8: RegisterTimer (87),
//   DeregisterTimer (88, the inline ~CTimer body), SetTimer (89), GetTimer (90); through the interface (+0x40): NewScriptFrame (7), GetHero (70),
//   GetAllThingsWithScriptName (75), CreateCreatureNearby (92), SetCreatureCreationDelayFrames (95),
//   ResetCreatureCreationDelayFrames (96), AddNewConversation (364), AddPersonToConversation (365),
//   AddLineToConversation (366), GiveThingBestEnemyTarget (451), EntitySetThingAsAllyOfThing (599).
// Strings: "HTG_FreedomFighter" 0x012D2668, "TEXT_QST_029_BANDIT_CHARGE" 0x012D220C.
// Layout (retail = Ego_r.pdb - 0x14, 12-byte vectors): HTG_State +0x5c, Leader +0x68, Prisoner +0x74, CurrentEnemyVector
//   +0xa0.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CScriptThing { public:                                   // vtable 0x01238C8C, 12 bytes
    virtual ~CScriptThing();                            // slot 0; complete destructor 0x004AA840
    virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005(); virtual void t006(); virtual void t007(); virtual void t008();
    virtual void t009(); virtual void t010(); virtual void t011(); virtual void t012(); virtual void t013(); virtual void t014(); virtual void t015(); virtual void t016();
    virtual void t017(); virtual void t018(); virtual void t019(); virtual void t020(); virtual void t021(); virtual void t022(); virtual void t023(); virtual void t024();
    virtual void t025(); virtual void t026(); virtual void t027(); virtual void t028(); virtual void t029(); virtual void t030(); virtual void t031(); virtual void t032();
    virtual void t033(); virtual void t034(); virtual void t035(); virtual void t036(); virtual void t037(); virtual void t038(); virtual void t039(); virtual void t040();
    virtual void t041(); virtual void t042(); virtual void t043(); virtual void t044(); virtual void t045(); virtual void t046(); virtual void t047(); virtual void t048();
    virtual void t049(); virtual void t050(); virtual void t051(); virtual void t052(); virtual void t053(); virtual void t054(); virtual void t055(); virtual void t056();
    virtual void t057(); virtual void t058(); virtual void t059(); virtual void t060(); virtual void t061(); virtual void t062(); virtual void t063(); virtual void t064();
    virtual void t065(); virtual void t066(); virtual void t067(); virtual void t068(); virtual void t069(); virtual void t070(); virtual void t071(); virtual void t072();
    virtual void t073(); virtual void t074(); virtual void t075();
    virtual bool IsDead() const;                        // slot 76 (+0x130) 0x004AA890
    void* PImp_Data;                                    // +0x04
    void* PImp_Info;                                    // +0x08
};
namespace std {
template<class T> class allocator { };
template<class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return _M_start[n]; }
    T& front() { return *begin(); }
    T* erase(T* first, T* last);                        // 0x008AC9D0 for CScriptThing
    void clear() { erase(begin(), end()); }
};
}
class C3DVector { public: float X, Y, Z; };
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
class CTimer { public:                                         // a script timer: the interface's timer index
    long Index;
    CTimer() { Index = g_pGameScriptInterface->RegisterTimer(); }
    ~CTimer() { g_pGameScriptInterface->DeregisterTimer(Index); }   // out-of-line copy 0x00CD4470
    __forceinline void SetTime(long time) { g_pGameScriptInterface->SetTimer(Index, time); }
    __forceinline long GetTime() const { return g_pGameScriptInterface->GetTimer(Index); }
};
namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_HangingTreeGoodScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void*                 Data;                         // +0x44
    unsigned char         _pad48[0x5c - 0x48];
    long                  HTG_State;                    // +0x5c
    long                  CurrentWayPoint;              // +0x60
    long                  CurrentGuards;                // +0x64
    CScriptThing          Leader;                       // +0x68
    CScriptThing          Prisoner;                     // +0x74
    unsigned char         _pad80[0xa0 - 0x80];
    std::vector<CScriptThing> CurrentEnemyVector;       // +0xa0
    void SpawnEnemies(const C3DVector& pos, CCharString defName, long count);
};
void CQ_HangingTreeGoodScript::SpawnEnemies(const C3DVector& pos, CCharString defName, long count)
{
    if (HTG_State == 3 || Leader.IsDead()) {
        if (IsActiveThreadTerminating())
            return;
        return;
    }
    if (HTG_State == 4 || HTG_State == 3) {
        if (IsActiveThreadTerminating())
            return;
        CTimer timer;
        timer.SetTime(10);
        while (timer.GetTime() > 0 && (HTG_State == 4 || HTG_State == 3)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        if (timer.GetTime() <= 0)
            return;
    }
    Game->SetCreatureCreationDelayFrames(1);
    for (long i = 0; i < count; ++i) {
        if (IsActiveThreadTerminating())
            return;
        CScriptThing enemy = Game->CreateCreatureNearby(defName, pos, 1.0f, "HTG_FreedomFighter", false);
        Game->GiveThingBestEnemyTarget(enemy, Leader);
        Game->EntitySetThingAsAllyOfThing(enemy, Prisoner);
    }
    if (IsActiveThreadTerminating())
        return;
    Game->ResetCreatureCreationDelayFrames();
    CurrentEnemyVector.clear();
    Game->GetAllThingsWithScriptName("HTG_FreedomFighter", CurrentEnemyVector);
    if (CurrentEnemyVector.size() != 0) {
        if (IsActiveThreadTerminating())
            return;
        long conversation = Game->AddNewConversation(CurrentEnemyVector.front(), false, false);
        Game->AddPersonToConversation(conversation, Game->GetHero());
        Game->AddLineToConversation(conversation, "TEXT_QST_029_BANDIT_CHARGE", false, CurrentEnemyVector.front(), Game->GetHero());
    }
    HTG_State = 5;
}
}