// NScript::CQ_GuildTrainingScript::CBirdKiller::OnPersist(CPersistContext& context) -- saves/loads the 2 members BirdMode / CurrentBirds by name (defaults 0L / 0L)
// Leaf: ?OnPersist@CBirdKiller@CQ_GuildTrainingScript@NScript@@EAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D44C60, 73 bytes (+ int3 padding to 80), thiscall, `ret 4`.
// Identity (proven): NScript::CQ_GuildTrainingScript::CBirdKiller::OnPersist(CPersistContext&); evidence: it is slot 4 (OnPersist) of the RTTI-less entity-script vtable 0x012CE310 (stored by 0x00D42F40); the literals BirdMode / CurrentBirds name exactly the Ego_r.pdb members of CBirdKiller (the only Ego_r class with both; entity-script members sit at their Ego_r offsets, offsets asserted), and the Ego_r/FableWin.pdb name sets hold ?OnPersist@CBirdKiller@CQ_GuildTrainingScript@NScript@@EAEXAAVCPersistContext@@@Z.
// Label: the manifest's "sub_00d44c60" is propagated / a placeholder (family J014: six two-member persist bodies).
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
class CEntityScriptBase {                           // vptr, Game, CScriptThing Me, ParentClass, PMasterData: 0x1C bytes
public:
    virtual ~CEntityScriptBase();                    // slot 0
    virtual void v01(); virtual void v02(); virtual void v03();
private:
    virtual void OnPersist(CPersistContext& context);   // slot 4
public:
    unsigned char _base[0x1C - 4];
};
class CQ_GuildTrainingScript {
public:
    class CBirdKiller;
};
class CQ_GuildTrainingScript::CBirdKiller : public CEntityScriptBase {
private:
    virtual void OnPersist(CPersistContext& context);
public:
    long           BirdMode;                          // +0x1C (Ego_r +0x1C)
    unsigned char _pad20[0x4];
    long           CurrentBirds;                      // +0x24 (Ego_r +0x24)
};

void CQ_GuildTrainingScript::CBirdKiller::OnPersist(CPersistContext& context)
{
    context.Transfer("BirdMode", BirdMode, 0L);
    context.Transfer("CurrentBirds", CurrentBirds, 0L);
}
}
