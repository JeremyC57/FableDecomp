// CQ_TraderConflictGoodScript::WatchForHittingEnemies() -- thread: until a trader follows the hero (then until three reach the teleporter), any creature the hero hits (plainly, or with the special ability 14 after any special-ability hit) turns every creature within 20 units of it, or aware of the hero, onto the hero as its best enemy target
// Leaf: WatchForHittingEnemies
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
enum EHeroAbility { HERO_ABILITY_NONE };

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

class CScriptThing : public CBaseClass { public:                      // 12 bytes, vtable 0x01238C8C
    void* PImpData;                                     // +0x04 CCountedPointer Data
    void* PImpInfo;                                     // +0x08 CCountedPointer Info
    virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005(); virtual void t006(); virtual void t007(); virtual void t008();
    virtual void t009(); virtual void t010(); virtual void t011(); virtual void t012(); virtual void t013(); virtual void t014(); virtual void t015(); virtual void t016();
    virtual void t017(); virtual void t018(); virtual void t019(); virtual void t020();
    virtual bool MsgIsHitBy(const CCharString& attacker) const;   // slot 21 (+0x54)
    virtual void t022(); virtual void t023(); virtual void t024(); virtual void t025(); virtual void t026(); virtual void t027(); virtual void t028(); virtual void t029();
    virtual void t030(); virtual void t031(); virtual void t032(); virtual void t033(); virtual void t034(); virtual void t035(); virtual void t036(); virtual void t037();
    virtual void t038(); virtual void t039(); virtual void t040();
    virtual bool MsgIsHitBySpecialAbilityFrom(EHeroAbility ability, const CCharString& attacker) const;   // slot 41 (+0xa4)
    virtual bool MsgIsHitByAnySpecialAbilityFrom(const CCharString& attacker) const;   // slot 42 (+0xa8)
};

bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

struct CScriptThingVector {                             // std::vector<CScriptThing> (STLport, 12 bytes)
    CScriptThing* _M_start;
    CScriptThing* _M_finish;
    CScriptThing* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    CScriptThing* begin() { return _M_start; }
    CScriptThing& operator[](unsigned int n) { return *(begin() + n); }
};

namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {

class CQ_TraderConflictGoodScript : public CScriptBase {
public:
    CGameScriptInterface* GSI;                          // +0x40
    void*                 PMasterData;                  // +0x44
    CScriptThingVector    AllCreatures;                 // +0x48
    long                  TradersFollowing;             // +0x54
    long                  TradersReachedTeleporter;     // +0x58
    void WatchForHittingEnemies();
    void UpdateTraders();                               // 0x00DFC320 (name unproven: thiscall helper polled once per frame)
};

void CQ_TraderConflictGoodScript::WatchForHittingEnemies()
{
    while (TradersFollowing < 1) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        for (unsigned int i = 0; i < AllCreatures.size(); ++i) {
            if (IsActiveThreadTerminating())
                return;
            if (AllCreatures[i].MsgIsHitBy("")
                || (AllCreatures[i].MsgIsHitByAnySpecialAbilityFrom("")
                    && !AllCreatures[i].MsgIsHitBySpecialAbilityFrom((EHeroAbility)14, "SCRIPT_NAME_HERO"))) {
                if (IsActiveThreadTerminating())
                    return;
                for (unsigned int j = 0; j < AllCreatures.size(); ++j) {
                    if (IsActiveThreadTerminating())
                        return;
                    if (IsDistanceBetweenThingsUnder(AllCreatures[i], AllCreatures[j], 20.0f)
                        || GSI->IsThingAwareOfOtherThingInAnyWay(GSI->GetHero(), AllCreatures[j])) {
                        if (IsActiveThreadTerminating())
                            return;
                        GSI->GiveThingBestEnemyTarget(AllCreatures[j], GSI->GetHero());
                    }
                }
                if (IsActiveThreadTerminating())
                    return;
            }
        }
        if (IsActiveThreadTerminating())
            return;
        UpdateTraders();
    }
    if (IsActiveThreadTerminating())
        return;
    while (TradersReachedTeleporter < 3) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        UpdateTraders();
    }
    if (IsActiveThreadTerminating())
        return;
}
}