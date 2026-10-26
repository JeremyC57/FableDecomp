// NScript::CQ_EndGameScript::OnPersist(CPersistContext& context) -- saves/loads the script's UndeadBar and QuestStartScreened by name (default 0.0f / false)
// Leaf: ?OnPersist@CQ_EndGameScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D29830, 68 bytes (80 with int3 padding), thiscall, `ret 4`.
// Identity (proven): NScript::CQ_EndGameScript::OnPersist(CPersistContext&); evidence: it is slot 4 (OnPersist) of the RTTI-less
//   script vtable 0x012CBA58 (stored by 0x00D27370); the two literals name exactly the Ego_r.pdb members UndeadBar / QuestStartScreened of
//   CQ_EndGameScript (the only Ego_r class with both; Ego_r +0x5C / +0x65, retail +0x48 / +0x51: the retail quest-script base is 0x14 bytes smaller), and the
//   Ego_r/FableWin.pdb name sets hold ?OnPersist@CQ_EndGameScript@NScript@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "sub_00d29830" is a gap-scan placeholder (family H036: seven two-member OnPersist bodies).
// Callees: CPersistContext::Transfer<bool> 0x004045C0, Transfer<long> 0x00410BE0, Transfer<float> 0x00410620 (FableWin.pdb
//   ??$Transfer@<T>@CPersistContext@@QAEXPBDAA<T>AB<T>@Z), here float then bool.
// Model: the landed CQ_BowerstoneTownLifeIntroScript::OnPersist 0x00D24660 (family G050); local classes -- Review at landing.
// Shape: each default is a temporary in a dead stack slot (the first in the `push ecx` slot, the second in the context
//   argument slot after the context moves to edi).
class CPersistContext {
public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // bool 0x004045C0, long 0x00410BE0, float 0x00410620
};
namespace NScript {
class CScriptBase {                                 // the quest-script base chain up to the first own member (retail 0x48 bytes)
public:
    virtual ~CScriptBase();                          // slot 0
    virtual void v01(); virtual void v02(); virtual void v03();   // RegisterMain, Main, Init
    virtual void OnPersist(CPersistContext& context);   // slot 4
    unsigned char _base[0x48 - 4];
};
class CQ_EndGameScript : public CScriptBase {
public:
    virtual void OnPersist(CPersistContext& context);
    float  UndeadBar;                         // +0x48
    unsigned char _pad4c[0x5];
    bool   QuestStartScreened;                // +0x51
};

void CQ_EndGameScript::OnPersist(CPersistContext& context)
{
    context.Transfer("UndeadBar", UndeadBar, 0.0f);
    context.Transfer("QuestStartScreened", QuestStartScreened, false);
}
}