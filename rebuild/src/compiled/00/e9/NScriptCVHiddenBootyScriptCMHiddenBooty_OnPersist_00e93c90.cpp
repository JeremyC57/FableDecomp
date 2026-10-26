// CV_HiddenBootyScript::CM_HiddenBooty::OnPersist(CPersistContext& context) -- saves/loads the script flags HasHeroGotAllClues, HasHeroGotAnyClues and PlayedIntroduction by name (default false)
// Leaf: ?OnPersist@CM_HiddenBooty@CV_HiddenBootyScript@NScript@@EAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E93C90, 93 bytes (+ int3 padding), thiscall, `ret 4`.
// Identity: NScript::CV_HiddenBootyScript::CM_HiddenBooty::OnPersist(CPersistContext&) -- proven; evidence: it is slot 4 of the RTTI-less script vtable
//   0x012ECAE8 (an entity-script vtable); the three literals name exactly the Ego_r.pdb members HasHeroGotAllClues / HasHeroGotAnyClues / PlayedIntroduction of NScript::CV_HiddenBootyScript::CM_HiddenBooty (Ego_r
//   +28 / +29 / +30, retail +0x1C / +0x1D / +0x1E: same offsets); FableWin.pdb name set holds
//   ?OnPersist@CM_HiddenBooty@CV_HiddenBootyScript@NScript@@EAEXAAVCPersistContext@@@Z. Family G050 (8 three-flag OnPersist bodies, catalogued only as
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
class CEntityScriptBase {                           // retail 0x14 bytes (vptr, Game, CScriptThing Me)
public:
    virtual ~CEntityScriptBase();                    // slot 0
    virtual void v01(); virtual void v02(); virtual void v03();
private:
    virtual void OnPersist(CPersistContext& context);   // slot 4
protected:
    unsigned char _base[0x14 - 4];
};
class CV_HiddenBootyScript;
class CQ_SunnyvaleMasterData;
class CV_HiddenBootyScript {
public:
    class CM_HiddenBooty;
};
class CV_HiddenBootyScript::CM_HiddenBooty : public CEntityScriptBase {
private:
    virtual void OnPersist(CPersistContext& context);
public:
    CV_HiddenBootyScript* ParentClass;                              // +0x14
    CQ_SunnyvaleMasterData* PMasterData;             // +0x18
    bool HasHeroGotAllClues;                           // +0x1C
    bool HasHeroGotAnyClues;                           // +0x1D
    bool PlayedIntroduction;                           // +0x1E
};

void CV_HiddenBootyScript::CM_HiddenBooty::OnPersist(CPersistContext& context)
{
    context.Transfer("HasHeroGotAllClues", HasHeroGotAllClues, false);
    context.Transfer("HasHeroGotAnyClues", HasHeroGotAnyClues, false);
    context.Transfer("PlayedIntroduction", PlayedIntroduction, false);
}
}