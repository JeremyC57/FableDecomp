#ifndef FABLE_ENGINE_CTCENEMY_H
#define FABLE_ENGINE_CTCENEMY_H
// Retail opt-in (Codex pass18): original CTCBase hierarchy and every PDB field; native STLport list layout.
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "stlport/fable_containers.h"
#ifdef FABLE_RETAIL_CTCENEMY_QUERIES
#include "rebuild_abi.h"
#include "engine/ETCInterfaceType.h"
#endif
class CFaction;
class CTCEnemy : public CTCBase {
public:
    CTCEnemy(CThing& thing);
    virtual ~CTCEnemy();
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
    virtual void OnInitialActivate();
    virtual bool IsComponentSerialisable() const;
#ifdef FABLE_RETAIL_CTCENEMY_FACTION
    // Retail opt-in (Codex pass19): FableWin GetPFaction and original+24 field.
    const CFaction* GetPFaction() const { return PFaction; }
#endif
#ifdef FABLE_RETAIL_CTCTARGETINGAI_ATTACK
    static bool FABLE_FASTCALL AreThingsEnemies(const CThing&,const CThing&);
#endif
    void SetFaction(const CCharString& faction);
protected:
    virtual void OnSerialise(CPersistContext&,bool);
private:
    bool FriendsWithEverythingFlag;
    bool EnableFollowersEnemyProxy;
    std::list<CIntelligentPointer<const CThing> > EnemyThings;
    std::list<CIntelligentPointer<const CThing> > AllyThings;
    const CFaction* PFaction;
#ifdef FABLE_RETAIL_CTCENEMY_QUERIES
    // Retail opt-in (chunk 1021, row 0x0076D2F0 CTCEnemy::AreThingsEnemies): the CTCEnemy queries, spelled as the PDB name sets
    // (Ego_r.pdb donor order IsEnemyOfFaction 0x0077AA80 / IsAlliedWithFaction 0x0077AAA0 / IsEnemyOf 0x0077AD60 / CalculateEnemyTC
    // 0x0077B080 / AreThingsEnemies 0x0077B430 / IsAllyOf 0x0077B5B0 / AreThingsAllies 0x0077B740 = retail 0x0076C7D0 / 0x0076C7F0 /
    // 0x0076CCE0 / 0x0076CFE0 / 0x0076D2F0 / 0x0076D460 / 0x0076D4F0). StatGetInterfaceType, GetPFaction and IsFriendsWithEverything are
    // the FableWin.pdb inline accessors (?StatGetInterfaceType@CTCEnemy@@SI?AW4ETCInterfaceType@@XZ, ?GetPFaction@CTCEnemy@@QBEPBVCFaction@@XZ,
    // ?IsFriendsWithEverything@CTCEnemy@@ABE_NXZ; no out-of-line copy in Ego_r.pdb or Fable.exe). Declarations and inline accessors
    // only: no layout or vtable change.
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_ENEMY; }
    const CFaction* GetPFaction() const { return PFaction; }
    bool IsEnemyOfFaction(const CFaction* faction) const;                     // 0x0076C7D0
    bool IsAlliedWithFaction(const CFaction* faction) const;                  // 0x0076C7F0
    static bool FABLE_FASTCALL AreThingsEnemies(const CThing& thing1, const CThing& thing2);   // 0x0076D2F0
    static bool FABLE_FASTCALL AreThingsAllies(const CThing& thing1, const CThing& thing2);    // 0x0076D4F0
private:
    bool IsFriendsWithEverything() const { return FriendsWithEverythingFlag; }
    bool IsEnemyOf(const CThing& thing) const;                                // 0x0076CCE0
    bool IsAllyOf(const CThing& thing) const;                                 // 0x0076D460
    static bool FABLE_FASTCALL CalculateEnemyTC(const CThing& thing, const CTCEnemy** enemy_tc, const CThing*& enemy_thing,
                                            bool ignore_followers);           // 0x0076CFE0
#endif
};
FABLE_STATIC_ASSERT(sizeof(CTCEnemy)==28);
#endif
