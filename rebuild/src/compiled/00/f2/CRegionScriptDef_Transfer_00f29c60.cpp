// CRegionScriptDef::Transfer(CPersistContext& context) -- saves/loads the 4 members RandomVillagerMax / RandomGuardMax / RandomBanditMax / RegionDangerLevel by name with zero defaults
// Leaf: ?Transfer@CRegionScriptDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F29C60, 119 bytes, thiscall, `ret 4`.
// Identity (proven): CRegionScriptDef::Transfer(CPersistContext&); evidence: the def registration CScriptInfoManager::RegisterDefs passes "CRegionScriptDef" with the Alloc that stores vtable 0x012FB630, whose slot 18 is Transfer (as for the landed CCombatSequenceDef::Transfer 0x0046C6F0); the 4 literals name exactly the Ego_r.pdb
//   members of CRegionScriptDef (the only Ego_r class with all of them; retail def members sit 4 below Ego_r, offsets asserted), and the Ego_r/FableWin.pdb name sets
//   hold ?Transfer@CRegionScriptDef@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "sub_00f29c60" is propagated / a placeholder (family H038: seven four-member persist bodies).
// Callees: CPersistContext::Transfer<T>(const char*, T&, const T&) (FableWin.pdb ??$Transfer@<T>@CPersistContext@@QAEXPBDAA<T>AB<T>@Z):
//   Transfer<long> 0x00410BE0.
// Model: the landed CQ_BowerstoneTownLifeIntroScript::OnPersist 0x00D24660 (family G050); local classes -- Review at landing.
// Shape: context in edi, this in esi, the zero in ebx; each default is a temporary in a dead stack slot (the first in the
//   `push ecx` slot, the rest in the context argument slot); the last member's address is `add esi, N` in place.
class CPersistContext {
public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);
};
class CRegionScriptDef {                                          // def: retail fields from +0x3C
public:
    virtual void Transfer(CPersistContext& context);   // slot 18
    unsigned char _base[0x3C - 4];                     // def base body
    long           RandomVillagerMax;                 // +0x3C (Ego_r +0x40)
    long           RandomGuardMax;                    // +0x40 (Ego_r +0x44)
    long           RandomBanditMax;                   // +0x44 (Ego_r +0x48)
    long           RegionDangerLevel;                 // +0x48 (Ego_r +0x4C)
};

void CRegionScriptDef::Transfer(CPersistContext& context)
{
    context.Transfer("RandomVillagerMax", RandomVillagerMax, 0L);
    context.Transfer("RandomGuardMax", RandomGuardMax, 0L);
    context.Transfer("RandomBanditMax", RandomBanditMax, 0L);
    context.Transfer("RegionDangerLevel", RegionDangerLevel, 0L);
}
