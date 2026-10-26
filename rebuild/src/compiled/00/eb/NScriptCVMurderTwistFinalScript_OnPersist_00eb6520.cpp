// NScript::CV_MurderTwistFinalScript::OnPersist(CPersistContext& context) -- saves/loads the 4 members SheriffState / AssassinState / TaskDeclined / PreemptiveAttackInProgress by name with zero defaults
// Leaf: ?OnPersist@CV_MurderTwistFinalScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EB6520, 119 bytes, thiscall, `ret 4`.
// Identity (proven): NScript::CV_MurderTwistFinalScript::OnPersist(CPersistContext&); evidence: it is slot 4 (OnPersist) of the RTTI-less script vtable 0x012EF978 (stored by 0x00EAE910); the 4 literals name exactly the Ego_r.pdb
//   members of CV_MurderTwistFinalScript (the only Ego_r class with all of them; retail quest-script members sit 0x14 below Ego_r (the retail CScriptBase is 0x40 bytes, Ego_r 0x54), offsets asserted), and the Ego_r/FableWin.pdb name sets
//   hold ?OnPersist@CV_MurderTwistFinalScript@NScript@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "sub_00eb6520" is propagated / a placeholder (family H038: seven four-member persist bodies).
// Callees: CPersistContext::Transfer<T>(const char*, T&, const T&) (FableWin.pdb ??$Transfer@<T>@CPersistContext@@QAEXPBDAA<T>AB<T>@Z):
//   Transfer<bool> 0x004045C0, Transfer<ESheriffState> 0x00EB65A0, Transfer<EAssassinState> 0x00EB6750.
// Model: the landed CQ_BowerstoneTownLifeIntroScript::OnPersist 0x00D24660 (family G050); local classes -- Review at landing.
// Shape: context in edi, this in esi, the zero in ebx; each default is a temporary in a dead stack slot (the first in the
//   `push ecx` slot, the rest in the context argument slot); the last member's address is `add esi, N` in place.
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
class CV_MurderTwistFinalScript : public CScriptBase {
public:
    enum ESheriffState { ESheriffState_Force32 = 0x7fffffff };   // enumerators not recovered
    enum EAssassinState { EAssassinState_Force32 = 0x7fffffff };   // enumerators not recovered
    virtual void OnPersist(CPersistContext& context);
    ESheriffState  SheriffState;                      // +0x48 (Ego_r +0x5C)
    EAssassinState AssassinState;                     // +0x4C (Ego_r +0x60)
    bool           TaskDeclined;                      // +0x50 (Ego_r +0x64)
    bool           PreemptiveAttackInProgress;        // +0x51 (Ego_r +0x65)
};

void CV_MurderTwistFinalScript::OnPersist(CPersistContext& context)
{
    context.Transfer("SheriffState", SheriffState, (CV_MurderTwistFinalScript::ESheriffState)0);
    context.Transfer("AssassinState", AssassinState, (CV_MurderTwistFinalScript::EAssassinState)0);
    context.Transfer("TaskDeclined", TaskDeclined, false);
    context.Transfer("PreemptiveAttackInProgress", PreemptiveAttackInProgress, false);
}
}
