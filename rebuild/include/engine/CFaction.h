#ifndef FABLE_ENGINE_CFACTION_H
#define FABLE_ENGINE_CFACTION_H
#include "fable_string.h"
#include "stlport/fable_map.h"
// Both original PDBs: class key V, private Name/AlliedFactions/EnemyFactions.
// Layout: native006B9510/006B9530 sets are12bytes, so offsets00/04/10 and size1C.
class CFaction {
public:
#ifdef FABLE_RETAIL_CFACTION_CTOR_OUTOFLINE
    // NF59 opt-in (row 0x006BAED0 CPersistContext::Transfer<CFaction>): Ego_r.pdb 0x006F64E0 ??0CFaction@@QAE@XZ is out of line, and
    // retail calls it as 0x006BA420 (`call 0x006BA420` building GetDefaultVal's traits local). Declaration only.
    CFaction();                                                      // 0x006BA420
#else
    CFaction() {}
#endif
    ~CFaction(); // original006BA6D0; FableWin02444350.
    CCharString GetName() const { return Name; }
    void InitialiseFromFaction(const CFaction& faction) {
        Name=faction.Name;
        EnemyFactions=faction.EnemyFactions;
        AlliedFactions=faction.AlliedFactions;
    }
#ifdef FABLE_RETAIL_CFACTION_SCRIPTRELATIONS
    // Opt-in (NF433, rows 0x00890870 / 0x008908C0 / 0x00890910 CGameScriptInterface::SetFactionAsAlliedToFaction / AsNeutralToFaction /
    // AsEnemyToFaction): Ego_r.pdb / FableWin.pdb ?SetAsAlliedWithFaction@ / ?SetAsNeutralToFaction@ / ?SetAsEnemyOfFaction@CFaction@@QAEXABV1@@Z,
    // out of line in retail at 0x006B94E0 / 0x006B9510 / 0x006B9530 (each row calls its one twice). Declarations only.
    void SetAsAlliedWithFaction(const CFaction& faction);               // 0x006B94E0
    void SetAsNeutralToFaction(const CFaction& faction);                // 0x006B9510
    void SetAsEnemyOfFaction(const CFaction& faction);                  // 0x006B9530
#endif
#ifdef FABLE_RETAIL_CTCENEMY_ISENEMYOF_PASS30
    bool IsEnemyOfFaction(const CFaction&) const;
#endif
#ifdef FABLE_RETAIL_CFACTION_ISALLIEDWITHFACTION
    // NF37 opt-in (row 0x0076C7F0 CTCEnemy::IsAlliedWithFaction): Ego_r.pdb ?IsAlliedWithFaction@CFaction@@QBE_NABV1@@Z (Ego_r 0x006F4EC0;
    // retail 0x006B9460, the catalog's CFaction::IsAlliedWithFaction). Declaration only.
    bool IsAlliedWithFaction(const CFaction& faction) const;   // 0x006B9460
#endif
private:
    CCharString Name;
    typedef std::set<CCharString> TStringSet;
    TStringSet AlliedFactions;
    TStringSet EnemyFactions;
};
FABLE_STATIC_ASSERT_AT(23, sizeof(CFaction)==0x1c);
#endif
