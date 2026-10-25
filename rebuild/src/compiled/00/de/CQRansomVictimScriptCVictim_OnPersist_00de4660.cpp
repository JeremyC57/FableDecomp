// CQ_RansomVictimScript::CVictim::OnPersist(CPersistContext& context) -- saves/loads the script flags WaitingForThingsToDie, ToldHeroOfThingsAttackingHim and GotOutOfCave by name (default false)
// Leaf: ?OnPersist@CVictim@CQ_RansomVictimScript@NScript@@EAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DE4660, 93 bytes (+ int3 padding), thiscall, `ret 4`.
// Identity: NScript::CQ_RansomVictimScript::CVictim::OnPersist(CPersistContext&) -- proven; evidence: it is slot 4 of the RTTI-less script vtable
//   0x012DD1AC (an entity-script vtable); the three literals name exactly the Ego_r.pdb members WaitingForThingsToDie / ToldHeroOfThingsAttackingHim / GotOutOfCave of NScript::CQ_RansomVictimScript::CVictim (Ego_r
//   +28 / +29 / +30, retail +0x1C / +0x1D / +0x1E: same offsets); FableWin.pdb name set holds
//   ?OnPersist@CVictim@CQ_RansomVictimScript@NScript@@EAEXAAVCPersistContext@@@Z. Family G050 (8 three-flag OnPersist bodies, catalogued only as
//   "_gapscan::sub_<addr>").
// Callees: CPersistContext::Transfer<bool>(const char*, bool&, const bool&) 0x004045C0 (FableWin.pdb
//   ??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z, already bound at that address).
// Shape: each `false` default is a bool temporary in a dead stack slot (the first in the `push ecx` slot, the next two in the
//   context argument slot after the context moves to edi).
class CPersistContext {
public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // <bool>: 0x004045C0
};
namespace NScript {
class CEntityScriptBase {                           // retail 0x14 bytes (vptr, Game, CScriptThing Me)
public:
    virtual ~CEntityScriptBase();                    // slot 0
    virtual void v01(); virtual void v02(); virtual void v03();
private:
    virtual void OnPersist(CPersistContext& context);   // slot 4
protected:
    unsigned char _base[0x14 - 4];
};
class CQ_RansomVictimScript;
class CQ_SunnyvaleMasterData;
class CQ_RansomVictimScript {
public:
    class CVictim;
};
class CQ_RansomVictimScript::CVictim : public CEntityScriptBase {
private:
    virtual void OnPersist(CPersistContext& context);
public:
    CQ_RansomVictimScript* ParentClass;                              // +0x14
    CQ_SunnyvaleMasterData* PMasterData;             // +0x18
    bool WaitingForThingsToDie;                        // +0x1C
    bool ToldHeroOfThingsAttackingHim;                 // +0x1D
    bool GotOutOfCave;                                 // +0x1E
};

void CQ_RansomVictimScript::CVictim::OnPersist(CPersistContext& context)
{
    context.Transfer("WaitingForThingsToDie", WaitingForThingsToDie, false);
    context.Transfer("ToldHeroOfThingsAttackingHim", ToldHeroOfThingsAttackingHim, false);
    context.Transfer("GotOutOfCave", GotOutOfCave, false);
}
}