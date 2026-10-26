// NScript::CV_SickChildScript::CWitch::OnPersist(CPersistContext& context) -- saves/loads the script's IntroDone and ThingsForPotionGot by name (default false / 0L)
// Leaf: ?OnPersist@CWitch@CV_SickChildScript@NScript@@EAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ECD890, 68 bytes (80 with int3 padding), thiscall, `ret 4`.
// Identity (proven): NScript::CV_SickChildScript::CWitch::OnPersist(CPersistContext&); evidence: it is slot 4 (OnPersist) of the RTTI-less
//   script vtable 0x012F1A78 (stored by 0x00EC7FF0); the two literals name exactly the Ego_r.pdb members IntroDone / ThingsForPotionGot of
//   CV_SickChildScript::CWitch (the only Ego_r class with both; Ego_r +0x1C / +0x20, retail +0x1C / +0x20: the entity-script prefix (vptr, Game, CScriptThing Me, ParentClass, PMasterData) is 0x1C bytes in both), and the
//   Ego_r/FableWin.pdb name sets hold ?OnPersist@CWitch@CV_SickChildScript@NScript@@EAEXAAVCPersistContext@@@Z.
// Label: the manifest's "sub_00ecd890" is a gap-scan placeholder (family H036: seven two-member OnPersist bodies).
// Callees: CPersistContext::Transfer<bool> 0x004045C0, Transfer<long> 0x00410BE0, Transfer<float> 0x00410620 (FableWin.pdb
//   ??$Transfer@<T>@CPersistContext@@QAEXPBDAA<T>AB<T>@Z), here bool then long.
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
class CV_SickChildScript {
public:
    class CWitch;
};
class CV_SickChildScript::CWitch : public CEntityScriptBase {
private:
    virtual void OnPersist(CPersistContext& context);
public:
    bool   IntroDone;                         // +0x1C
    unsigned char _pad1d[0x3];
    long   ThingsForPotionGot;                // +0x20
};

void CV_SickChildScript::CWitch::OnPersist(CPersistContext& context)
{
    context.Transfer("IntroDone", IntroDone, false);
    context.Transfer("ThingsForPotionGot", ThingsForPotionGot, 0L);
}
}