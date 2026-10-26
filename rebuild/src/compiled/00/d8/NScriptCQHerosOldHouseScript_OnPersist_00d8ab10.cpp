// CQ_HerosOldHouseScript::OnPersist(CPersistContext& context) -- saves/loads the script flags Helping, Helped and BootyDugUp by name (default false)
// Leaf: ?OnPersist@CQ_HerosOldHouseScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D8AB10, 93 bytes (+ int3 padding), thiscall, `ret 4`.
// Identity: NScript::CQ_HerosOldHouseScript::OnPersist(CPersistContext&) -- proven; evidence: it is slot 4 of the RTTI-less script vtable
//   0x012D5018 (a quest-script vtable: dtor, RegisterMain, Main, Init, OnPersist); the three literals name exactly the Ego_r.pdb members Helping / Helped / BootyDugUp of NScript::CQ_HerosOldHouseScript (Ego_r
//   +92 / +93 / +97, retail +0x48 / +0x49 / +0x4D: the retail quest-script base is 0x14 bytes smaller); FableWin.pdb name set holds
//   ?OnPersist@CQ_HerosOldHouseScript@NScript@@UAEXAAVCPersistContext@@@Z. Family G050 (8 three-flag OnPersist bodies, catalogued only as
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
class CQ_HerosOldHouseScript : public CScriptBase {
public:
    virtual void OnPersist(CPersistContext& context);
    bool Helping;                                      // +0x48
    bool Helped;                                       // +0x49
    bool MissionAborted;                               // +0x4A
    bool WifeAttacked;                                 // +0x4B
    bool WifeAttackedAndLeft;                          // +0x4C
    bool BootyDugUp;                                   // +0x4D
};

void CQ_HerosOldHouseScript::OnPersist(CPersistContext& context)
{
    context.Transfer("Helping", Helping, false);
    context.Transfer("Helped", Helped, false);
    context.Transfer("BootyDugUp", BootyDugUp, false);
}
}