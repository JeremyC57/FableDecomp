// CQ_BanditCampScript::OnPersist(CPersistContext& context) -- saves/loads the script flags Gate1Open, BoastBegun and ActivatedBanditRaid by name (default false)
// Leaf: ?OnPersist@CQ_BanditCampScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D00A90, 93 bytes (+ int3 padding), thiscall, `ret 4`.
// Identity: NScript::CQ_BanditCampScript::OnPersist(CPersistContext&) -- proven; evidence: it is slot 4 of the RTTI-less script vtable
//   0x012C8300 (slots 2/3 are the catalogue's CQ_BanditCampScript::Main 0x00D00AF0 / ::Init 0x00D006D0); the three literals name exactly the Ego_r.pdb members Gate1Open / BoastBegun / ActivatedBanditRaid of NScript::CQ_BanditCampScript (Ego_r
//   +93 / +94 / +97, retail +0x49 / +0x4A / +0x4D: the retail quest-script base is 0x14 bytes smaller); FableWin.pdb name set holds
//   ?OnPersist@CQ_BanditCampScript@NScript@@UAEXAAVCPersistContext@@@Z. Family G050 (8 three-flag OnPersist bodies, catalogued only as
//   "_gapscan::sub_<addr>").
// Callees: CPersistContext::Transfer<bool>(const char*, bool&, const bool&) 0x004045C0 (FableWin.pdb
//   ??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z, already bound at that address).
// Shape: each `false` default is a bool temporary in a dead stack slot (the first in the `push ecx` slot, the next two in the
//   context argument slot after the context moves to edi).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (already NScript). Same bytes.
class CPersistContext {
public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // <bool>: 0x004045C0
};
namespace NScript {
class CScriptBase {                                 // the quest-script base chain up to the first own member (retail 0x48 bytes)
public:
    virtual ~CScriptBase();                          // slot 0
    virtual void v01(); virtual void v02(); virtual void v03();   // RegisterMain, Main, Init
    virtual void OnPersist(CPersistContext& context);   // slot 4
    unsigned char _base[0x48 - 4];
};
class CQ_BanditCampScript : public CScriptBase {
public:
    virtual void OnPersist(CPersistContext& context);
    bool AttackedOuterGateGuards;                      // +0x48
    bool Gate1Open;                                    // +0x49
    bool BoastBegun;                                   // +0x4A
    bool MissionSucceeded;                             // +0x4B
    bool MissionFailed;                                // +0x4C
    bool ActivatedBanditRaid;                          // +0x4D
};

void CQ_BanditCampScript::OnPersist(CPersistContext& context)
{
    context.Transfer("Gate1Open", Gate1Open, false);
    context.Transfer("BoastBegun", BoastBegun, false);
    context.Transfer("ActivatedBanditRaid", ActivatedBanditRaid, false);
}
}