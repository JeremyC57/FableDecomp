// NScript::CQ_GuildTrainingScript::OnPersist(CPersistContext& context) -- saves/loads the 2 members GameState / CurrentBirdsKilled by name (defaults 0L / 0L)
// Leaf: ?OnPersist@CQ_GuildTrainingScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D3BC10, 73 bytes (+ int3 padding to 80), thiscall, `ret 4`.
// Identity (proven): NScript::CQ_GuildTrainingScript::OnPersist(CPersistContext&); evidence: it is slot 4 (OnPersist) of the RTTI-less script vtable 0x012CD458 (stored by 0x00D3B390); the literals GameState / CurrentBirdsKilled name exactly the Ego_r.pdb members of CQ_GuildTrainingScript (the only Ego_r class with both; retail quest-script members sit 0x14 below Ego_r, offsets asserted), and the Ego_r/FableWin.pdb name sets hold ?OnPersist@CQ_GuildTrainingScript@NScript@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "sub_00d3bc10" is propagated / a placeholder (family J014: six two-member persist bodies).
// Callees: CPersistContext::Transfer<T>(const char*, T&, const T&) (FableWin.pdb ??$Transfer@<T>@CPersistContext@@QAEXPBDAA<T>AB<T>@Z):
//   Transfer<long> 0x00410BE0, Transfer<long> 0x00410BE0.
// Shape: context in edi, this in esi; each default is a temporary in a dead stack slot (the first in the `push ecx` slot,
//   the second in the context argument slot); the second member's address is `add esi, N` in place. Model: agent952 H036
//   (the landed CQ_BowerstoneTownLifeIntroScript::OnPersist 0x00D24660 shape).
class CPersistContext {
public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);
};
namespace NScript {
class CScriptBase {                                 // the quest-script base chain up to the first own member (retail 0x48 bytes)
public:
    virtual ~CScriptBase();                          // slot 0
    virtual void v01(); virtual void v02(); virtual void v03();   // RegisterMain, Main, Init
    virtual void OnPersist(CPersistContext& context);   // slot 4
    unsigned char _base[0x48 - 4];
};
class CQ_GuildTrainingScript : public CScriptBase {
public:
    virtual void OnPersist(CPersistContext& context);
    long           GameState;                         // +0x48 (Ego_r +0x5C)
    unsigned char _pad4c[0x8];
    long           CurrentBirdsKilled;                // +0x54 (Ego_r +0x68)
};

void CQ_GuildTrainingScript::OnPersist(CPersistContext& context)
{
    context.Transfer("GameState", GameState, 0L);
    context.Transfer("CurrentBirdsKilled", CurrentBirdsKilled, 0L);
}
}
