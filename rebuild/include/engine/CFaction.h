#ifndef FABLE_ENGINE_CFACTION_H
#define FABLE_ENGINE_CFACTION_H
#include "fable_string.h"
#include "stlport/fable_map.h"
// Both original PDBs: class key V, private Name/AlliedFactions/EnemyFactions.
// Layout: native006B9510/006B9530 sets are12bytes, so offsets00/04/10 and size1C.
class CFaction {
public:
    CFaction() {}
    ~CFaction(); // original006BA6D0; FableWin02444350.
    CCharString GetName() const { return Name; }
    void InitialiseFromFaction(const CFaction& faction) {
        Name=faction.Name;
        EnemyFactions=faction.EnemyFactions;
        AlliedFactions=faction.AlliedFactions;
    }
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
FABLE_STATIC_ASSERT(sizeof(CFaction)==0x1c);
#endif
