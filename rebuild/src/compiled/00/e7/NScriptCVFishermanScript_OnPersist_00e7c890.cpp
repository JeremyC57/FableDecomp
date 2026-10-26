// NScript::CV_FishermanScript::OnPersist(CPersistContext& context) -- saves/loads the script's FishermanTasks and HeroHasSavedFishermanFromWasps by name (default 0L / false)
// Leaf: ?OnPersist@CV_FishermanScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E7C890, 68 bytes (80 with int3 padding), thiscall, `ret 4`.
// Identity (proven): NScript::CV_FishermanScript::OnPersist(CPersistContext&); evidence: it is slot 4 (OnPersist) of the RTTI-less
//   script vtable 0x012EA024 (stored by 0x00E7B3F0); the two literals name exactly the Ego_r.pdb members FishermanTasks / HeroHasSavedFishermanFromWasps of
//   CV_FishermanScript (the only Ego_r class with both; Ego_r +0x68 / +0x5D, retail +0x54 / +0x49: the retail quest-script base is 0x14 bytes smaller), and the
//   Ego_r/FableWin.pdb name sets hold ?OnPersist@CV_FishermanScript@NScript@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "sub_00e7c890" is a gap-scan placeholder (family H036: seven two-member OnPersist bodies).
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
class CScriptBase {                                 // the quest-script base chain up to the first own member (retail 0x48 bytes)
public:
    virtual ~CScriptBase();                          // slot 0
    virtual void v01(); virtual void v02(); virtual void v03();   // RegisterMain, Main, Init
    virtual void OnPersist(CPersistContext& context);   // slot 4
    unsigned char _base[0x48 - 4];
};
class CV_FishermanScript : public CScriptBase {
public:
    virtual void OnPersist(CPersistContext& context);
    unsigned char _pad48[0x1];
    bool   HeroHasSavedFishermanFromWasps;    // +0x49
    unsigned char _pad4a[0xA];
    long   FishermanTasks;                    // +0x54
};

void CV_FishermanScript::OnPersist(CPersistContext& context)
{
    context.Transfer("FishermanTasks", FishermanTasks, 0L);
    context.Transfer("HeroHasSavedFishermanFromWasps", HeroHasSavedFishermanFromWasps, false);
}
}