// NScript::CPersonalScript_EmptyScript::CTestPos::OnPersist(CPersistContext& context) -- saves/loads the 2 members CurrentActiveScript / QuestCardNumber by name (defaults 0L / 0L)
// Leaf: ?OnPersist@CTestPos@CPersonalScript_EmptyScript@NScript@@EAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EF5450, 73 bytes (+ int3 padding to 80), thiscall, `ret 4`.
// Identity (proven): NScript::CPersonalScript_EmptyScript::CTestPos::OnPersist(CPersistContext&); evidence: it is slot 4 (OnPersist) of the RTTI-less entity-script vtable 0x012F63D0 (stored by 0x00EF3740); the literals CurrentActiveScript / QuestCardNumber name exactly the Ego_r.pdb members of CTestPos (the only Ego_r class with both; entity-script members sit at their Ego_r offsets, offsets asserted), and the Ego_r/FableWin.pdb name sets hold ?OnPersist@CTestPos@CPersonalScript_EmptyScript@NScript@@EAEXAAVCPersistContext@@@Z.
// Label: the manifest's "sub_00ef5450" is propagated / a placeholder (family J014: six two-member persist bodies).
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
class CPersonalScript_EmptyScript {
public:
    class CTestPos;
};
class CPersonalScript_EmptyScript::CTestPos : public CEntityScriptBase {
private:
    virtual void OnPersist(CPersistContext& context);
public:
    long           CurrentActiveScript;               // +0x1C (Ego_r +0x1C)
    long           QuestCardNumber;                   // +0x20 (Ego_r +0x20)
};

void CPersonalScript_EmptyScript::CTestPos::OnPersist(CPersistContext& context)
{
    context.Transfer("CurrentActiveScript", CurrentActiveScript, 0L);
    context.Transfer("QuestCardNumber", QuestCardNumber, 0L);
}
}
