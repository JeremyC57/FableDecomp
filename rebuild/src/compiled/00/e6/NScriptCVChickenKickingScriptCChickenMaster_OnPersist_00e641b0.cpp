// CV_ChickenKickingScript::CChickenMaster::OnPersist(CPersistContext& context) -- saves/loads the script flags HeroHasPlayed, HaveTalked and GhostChat by name (default false)
// Leaf: ?OnPersist@CChickenMaster@CV_ChickenKickingScript@NScript@@EAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E641B0, 93 bytes (+ int3 padding), thiscall, `ret 4`.
// Identity: NScript::CV_ChickenKickingScript::CChickenMaster::OnPersist(CPersistContext&) -- proven; evidence: it is slot 4 of the RTTI-less script vtable
//   0x012E747C (an entity-script vtable); the three literals name exactly the Ego_r.pdb members HeroHasPlayed / HaveTalked / GhostChat of NScript::CV_ChickenKickingScript::CChickenMaster (Ego_r
//   +29 / +28 / +31, retail +0x1D / +0x1C / +0x1F: same offsets); FableWin.pdb name set holds
//   ?OnPersist@CChickenMaster@CV_ChickenKickingScript@NScript@@EAEXAAVCPersistContext@@@Z. Family G050 (8 three-flag OnPersist bodies, catalogued only as
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
class CV_ChickenKickingScript;
class CQ_SunnyvaleMasterData;
class CV_ChickenKickingScript {
public:
    class CChickenMaster;
};
class CV_ChickenKickingScript::CChickenMaster : public CEntityScriptBase {
private:
    virtual void OnPersist(CPersistContext& context);
public:
    CV_ChickenKickingScript* ParentClass;                              // +0x14
    CQ_SunnyvaleMasterData* PMasterData;             // +0x18
    bool HaveTalked;                                   // +0x1C
    bool HeroHasPlayed;                                // +0x1D
    bool KickingChickens;                              // +0x1E
    bool GhostChat;                                    // +0x1F
};

void CV_ChickenKickingScript::CChickenMaster::OnPersist(CPersistContext& context)
{
    context.Transfer("HeroHasPlayed", HeroHasPlayed, false);
    context.Transfer("HaveTalked", HaveTalked, false);
    context.Transfer("GhostChat", GhostChat, false);
}
}