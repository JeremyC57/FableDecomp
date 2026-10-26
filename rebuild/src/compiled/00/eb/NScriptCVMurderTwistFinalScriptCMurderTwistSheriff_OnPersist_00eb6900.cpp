// NScript::CV_MurderTwistFinalScript::CMurderTwistSheriff::OnPersist(CPersistContext& context) -- saves/loads the 4 members HasGivenReminderToHero / HasBeingRumbledCutscenePlayed / HasExpressedSuspicionToHero / HasSaidWellDoneToHero by name with zero defaults
// Leaf: ?OnPersist@CMurderTwistSheriff@CV_MurderTwistFinalScript@NScript@@EAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EB6900, 119 bytes, thiscall, `ret 4`.
// Identity (proven): NScript::CV_MurderTwistFinalScript::CMurderTwistSheriff::OnPersist(CPersistContext&); evidence: it is slot 4 (OnPersist) of the RTTI-less script vtable 0x012EFA0C (stored by 0x00EAED90); the 4 literals name exactly the Ego_r.pdb
//   members of CMurderTwistSheriff (the only Ego_r class with all of them; entity-script members sit at their Ego_r offsets (the vptr/Game/Me/ParentClass/PMasterData prefix is 0x1C bytes in both), offsets asserted), and the Ego_r/FableWin.pdb name sets
//   hold ?OnPersist@CMurderTwistSheriff@CV_MurderTwistFinalScript@NScript@@EAEXAAVCPersistContext@@@Z.
// Label: the manifest's "sub_00eb6900" is propagated / a placeholder (family H038: seven four-member persist bodies).
// Callees: CPersistContext::Transfer<T>(const char*, T&, const T&) (FableWin.pdb ??$Transfer@<T>@CPersistContext@@QAEXPBDAA<T>AB<T>@Z):
//   Transfer<bool> 0x004045C0.
// Model: the landed CQ_BowerstoneTownLifeIntroScript::OnPersist 0x00D24660 (family G050); local classes -- Review at landing.
// Shape: context in edi, this in esi, the zero in ebx; each default is a temporary in a dead stack slot (the first in the
//   `push ecx` slot, the rest in the context argument slot); the last member's address is `add esi, N` in place.
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
class CV_MurderTwistFinalScript {
public:
    class CMurderTwistSheriff;
};
class CV_MurderTwistFinalScript::CMurderTwistSheriff : public CEntityScriptBase {
private:
    virtual void OnPersist(CPersistContext& context);
public:
    unsigned char _pad1c[0x18];
    bool           HasGivenReminderToHero;            // +0x34 (Ego_r +0x34)
    bool           HasBeingRumbledCutscenePlayed;     // +0x35 (Ego_r +0x35)
    bool           HasExpressedSuspicionToHero;       // +0x36 (Ego_r +0x36)
    bool           HasSaidWellDoneToHero;             // +0x37 (Ego_r +0x37)
};

void CV_MurderTwistFinalScript::CMurderTwistSheriff::OnPersist(CPersistContext& context)
{
    context.Transfer("HasGivenReminderToHero", HasGivenReminderToHero, false);
    context.Transfer("HasBeingRumbledCutscenePlayed", HasBeingRumbledCutscenePlayed, false);
    context.Transfer("HasExpressedSuspicionToHero", HasExpressedSuspicionToHero, false);
    context.Transfer("HasSaidWellDoneToHero", HasSaidWellDoneToHero, false);
}
}
