// NScript::CV_LostTraderScript::CLostTraderBrother::OnPersist(CPersistContext& context) -- saves/loads the script's stateBrain and HasToldHeroWhereBrotherIs by name (default 0L / false)
// Leaf: ?OnPersist@CLostTraderBrother@CV_LostTraderScript@NScript@@EAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EA5990, 68 bytes (80 with int3 padding), thiscall, `ret 4`.
// Identity (proven): NScript::CV_LostTraderScript::CLostTraderBrother::OnPersist(CPersistContext&); evidence: it is slot 4 (OnPersist) of the RTTI-less
//   script vtable 0x012EE034 (stored by 0x00EA4DE0); the two literals name exactly the Ego_r.pdb members stateBrain / HasToldHeroWhereBrotherIs of
//   CV_LostTraderScript::CLostTraderBrother (the only Ego_r class with both; Ego_r +0x1C / +0x20, retail +0x1C / +0x20: the entity-script prefix (vptr, Game, CScriptThing Me, ParentClass, PMasterData) is 0x1C bytes in both), and the
//   Ego_r/FableWin.pdb name sets hold ?OnPersist@CLostTraderBrother@CV_LostTraderScript@NScript@@EAEXAAVCPersistContext@@@Z.
// Label: the manifest's "sub_00ea5990" is a gap-scan placeholder (family H036: seven two-member OnPersist bodies).
// Callees: CPersistContext::Transfer<bool> 0x004045C0, Transfer<long> 0x00410BE0, Transfer<float> 0x00410620 (FableWin.pdb
//   ??$Transfer@<T>@CPersistContext@@QAEXPBDAA<T>AB<T>@Z), here long then bool.
// Model: the landed CQ_BowerstoneTownLifeIntroScript::OnPersist 0x00D24660 (family G050); local classes -- Review at landing.
// Shape: each default is a temporary in a dead stack slot (the first in the `push ecx` slot, the second in the context
//   argument slot after the context moves to edi).
class CPersistContext {
public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // bool 0x004045C0, long 0x00410BE0, float 0x00410620
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
class CV_LostTraderScript {
public:
    class CLostTraderBrother;
};
class CV_LostTraderScript::CLostTraderBrother : public CEntityScriptBase {
private:
    virtual void OnPersist(CPersistContext& context);
public:
    long   stateBrain;                        // +0x1C
    bool   HasToldHeroWhereBrotherIs;         // +0x20
};

void CV_LostTraderScript::CLostTraderBrother::OnPersist(CPersistContext& context)
{
    context.Transfer("stateBrain", stateBrain, 0L);
    context.Transfer("HasToldHeroWhereBrotherIs", HasToldHeroWhereBrotherIs, false);
}
}