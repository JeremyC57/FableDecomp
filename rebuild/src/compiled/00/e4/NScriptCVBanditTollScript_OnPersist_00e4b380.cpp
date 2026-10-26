// NScript::CV_BanditTollScript::OnPersist(CPersistContext& context) -- saves/loads the 4 members TraderDead / SafeTraderAppeared / TollPaid / GivenQuestCard by name with zero defaults
// Leaf: ?OnPersist@CV_BanditTollScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E4B380, 119 bytes, thiscall, `ret 4`.
// Identity (proven): NScript::CV_BanditTollScript::OnPersist(CPersistContext&); evidence: it is slot 4 (OnPersist) of the RTTI-less script vtable 0x012E4B7C (stored by 0x00E478A0); the 4 literals name exactly the Ego_r.pdb
//   members of CV_BanditTollScript (the only Ego_r class with all of them; retail quest-script members sit 0x14 below Ego_r (the retail CScriptBase is 0x40 bytes, Ego_r 0x54), offsets asserted), and the Ego_r/FableWin.pdb name sets
//   hold ?OnPersist@CV_BanditTollScript@NScript@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "sub_00e4b380" is propagated / a placeholder (family H038: seven four-member persist bodies).
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
class CScriptBase {                                 // the quest-script base chain up to the first own member (retail 0x48 bytes)
public:
    virtual ~CScriptBase();                          // slot 0
    virtual void v01(); virtual void v02(); virtual void v03();   // RegisterMain, Main, Init
    virtual void OnPersist(CPersistContext& context);   // slot 4
    unsigned char _base[0x48 - 4];
};
class CV_BanditTollScript : public CScriptBase {
public:
    virtual void OnPersist(CPersistContext& context);
    unsigned char _pad48[0x1];
    bool           TollPaid;                          // +0x49 (Ego_r +0x5D)
    unsigned char _pad4a[0x2];
    bool           TraderDead;                        // +0x4C (Ego_r +0x60)
    bool           SafeTraderAppeared;                // +0x4D (Ego_r +0x61)
    unsigned char _pad4e[0xA];
    bool           GivenQuestCard;                    // +0x58 (Ego_r +0x6C)
};

void CV_BanditTollScript::OnPersist(CPersistContext& context)
{
    context.Transfer("TraderDead", TraderDead, false);
    context.Transfer("SafeTraderAppeared", SafeTraderAppeared, false);
    context.Transfer("TollPaid", TollPaid, false);
    context.Transfer("GivenQuestCard", GivenQuestCard, false);
}
}
