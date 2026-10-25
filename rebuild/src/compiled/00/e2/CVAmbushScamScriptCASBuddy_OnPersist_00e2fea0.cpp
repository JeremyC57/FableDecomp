// CV_AmbushScamScript::CAS_Buddy::OnPersist(CPersistContext& context) -- saves/loads the script flags BuddyHeroApproachComplete, BuddyHeroApproachSpeechComplete and BuddyFollowSpeechComplete by name (default false)
// Leaf: ?OnPersist@CAS_Buddy@CV_AmbushScamScript@NScript@@EAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E2FEA0, 93 bytes (+ int3 padding), thiscall, `ret 4`.
// Identity: NScript::CV_AmbushScamScript::CAS_Buddy::OnPersist(CPersistContext&) -- proven; evidence: it is slot 4 of the RTTI-less script vtable
//   0x012E286C (an entity-script vtable); the three literals name exactly the Ego_r.pdb members BuddyHeroApproachComplete / BuddyHeroApproachSpeechComplete / BuddyFollowSpeechComplete of NScript::CV_AmbushScamScript::CAS_Buddy (Ego_r
//   +28 / +29 / +30, retail +0x1C / +0x1D / +0x1E: same offsets); FableWin.pdb name set holds
//   ?OnPersist@CAS_Buddy@CV_AmbushScamScript@NScript@@EAEXAAVCPersistContext@@@Z. Family G050 (8 three-flag OnPersist bodies, catalogued only as
//   "_gapscan::sub_<addr>").
// Callees: CPersistContext::Transfer<bool>(const char*, bool&, const bool&) 0x004045C0 (FableWin.pdb
//   ??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z, already bound at that address).
// Shape: each `false` default is a bool temporary in a dead stack slot (the first in the `push ecx` slot, the next two in the
//   context argument slot after the context moves to edi).
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
class CV_AmbushScamScript;
class CQ_SunnyvaleMasterData;
class CV_AmbushScamScript {
public:
    class CAS_Buddy;
};
class CV_AmbushScamScript::CAS_Buddy : public CEntityScriptBase {
private:
    virtual void OnPersist(CPersistContext& context);
public:
    CV_AmbushScamScript* ParentClass;                              // +0x14
    CQ_SunnyvaleMasterData* PMasterData;             // +0x18
    bool BuddyHeroApproachComplete;                    // +0x1C
    bool BuddyHeroApproachSpeechComplete;              // +0x1D
    bool BuddyFollowSpeechComplete;                    // +0x1E
};

void CV_AmbushScamScript::CAS_Buddy::OnPersist(CPersistContext& context)
{
    context.Transfer("BuddyHeroApproachComplete", BuddyHeroApproachComplete, false);
    context.Transfer("BuddyHeroApproachSpeechComplete", BuddyHeroApproachSpeechComplete, false);
    context.Transfer("BuddyFollowSpeechComplete", BuddyFollowSpeechComplete, false);
}
}