// CCombatManager::RegisterCombatant(CThingCreatureBase&, CThingCreatureBase&, bool) -- start a fight: build (and drop) the "<attacker> attacking <attacked> on frame <n>" trace, register the attacked and the attacking side, then refresh the attacked creature's list of attackers
// Leaf: RegisterCombatant
// Retail: Fable.exe (TLC, VC7.1) @ 0x006EDC50, 328 bytes. `ret 0Ch`.
// Callees: CThing::GetDefName 0x004C7CC0, CDefStringTable::GetString 0x009D49B0 on 0x013CA828, CCharString copy ctor
//          0x0099EC30 / dtor 0x0099EAE0, `__fastcall` operator+(const CCharString&, const char*) 0x0099F600 and
//          operator+(const CCharString&, const CCharString&) 0x0099F570 (catalogued CCharString::AppendData), the
//          `__fastcall` world-frame string 0x0049D880; RegisterAttackedCombatant 0x006ED270, RegisterAttackingCombatant
//          0x006EDA80 (catalogued RegisterCombatant), the combatant map's find 0x006F0250, UpdateAttackingCombatantsList
//          (CCombatantInfo&, bool) 0x006ED030 (Ego_r name; catalogued CCombatManager_UpdateDefenderState).
//          Literals " attacking " 0x01261F44, " on frame " 0x01261F50.
//
// Shape: both names come from an inline by-value GetDefNameString helper returning a NAMED local (VC7.1 builds the GetString temporary,
// copies it into the named local and destroys it: no NRVO through the inline); the trace is the whole operator+
// chain handed to an inline no-op. The attacked creature's record is the inline GetCombatantInfo (find, `->value`).
// Layout: the landed CCombatManager model (Combatants +0x10).
// Name fix (agentNF5 2026-11-10): CDefStringTable::GetString 0x009D49B0 is ?GetString@CDefStringTable@@QBE?AVCCharString@@J@Z (Ego_r.pdb / FableWin.pdb): const, takes the long table index (the CDefString's one word).
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_STATEACCESS
// Name fix (NF195, 2026-11-20, q4389; NF185's transformer): the invented CRetailRbMap_UID_CombatantInfo stand-in is replaced in place
//   by the STLport map<unsigned __int64, CCombatantInfo> (stlport/fable_map.h); its _M_find<unsigned __int64> 0x006F0250 is visible but
//   out of line (FABLE_RETAIL_STLP_TREE_MFIND_NOINLINE; the declaration-only form differs: retail keeps a register live across the call); FABLE_RETAIL_STLP_TREE_CONSTFIND gives the const find body.
#define FABLE_RETAIL_STLP_TREE_MFIND_NOINLINE
#define FABLE_RETAIL_STLP_TREE_CONSTFIND
#include "stlport/fable_map.h"
#include "engine/CThing.h"
CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
CCharString __fastcall GetWorldFrameString();      // 0x0049D880
class CDefString { public:
    long m_index;
};
class CDefStringTable { public:
    CCharString GetString(long name) const;        // 0x009D49B0
};
extern CDefStringTable GDefStringTable;   // 0x013CA828 ?GDefStringTable@@3VCDefStringTable@@A (ESTABLISHED; naming wave 21)           // 0x013CA828
inline void CombatTrace(const CCharString&) {}     // the trace output, compiled out

class CThingCreatureBase { public:
    unsigned char _pad[0x18];
    unsigned __int64 UniqueID;                     // +0x18
    unsigned __int64 GetUID() const { return UniqueID; }
};
inline CCharString GetDefNameString(const CThingCreatureBase& thing)
{
    CCharString name = GDefStringTable.GetString((((CThing*)&thing)->GetDefName()).m_index);
    return name;
}
struct CCombatantInfo {
    unsigned char _data[0x70];
};
class CCombatManager { public:
    virtual void v00();
    unsigned char _sequences[0xc];                 // +0x04 RegisteredCombatSequences
    std::map<unsigned __int64, CCombatantInfo> Combatants;     // +0x10
    CCombatantInfo& GetCombatantInfo(const CThingCreatureBase& creature) { return Combatants.find(creature.GetUID())->second; }
    void RegisterAttackedCombatant(CThingCreatureBase& attacker, CThingCreatureBase& attacked);   // 0x006ED270
    void RegisterAttackingCombatant(CThingCreatureBase& attacker, CThingCreatureBase& attacked, bool flag);   // 0x006EDA80
    void UpdateAttackingCombatantsList(CCombatantInfo& info, bool force);   // 0x006ED030
    void RegisterCombatant(CThingCreatureBase& attacker, CThingCreatureBase& attacked, bool flag);
};

void CCombatManager::RegisterCombatant(CThingCreatureBase& attacker, CThingCreatureBase& attacked, bool flag)
{
    CCharString attackerName = GetDefNameString(attacker);
    CCharString attackedName = GetDefNameString(attacked);
    CombatTrace(attackerName + " attacking " + attackedName + " on frame " + GetWorldFrameString());
    RegisterAttackedCombatant(attacker, attacked);
    RegisterAttackingCombatant(attacker, attacked, flag);
    UpdateAttackingCombatantsList(GetCombatantInfo(attacked), true);
}