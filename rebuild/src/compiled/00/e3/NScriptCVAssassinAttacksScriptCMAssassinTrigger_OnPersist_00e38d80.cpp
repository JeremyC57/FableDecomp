// NScript::CV_AssassinAttacksScript::CM_AssassinTrigger::OnPersist(CPersistContext& context) -- persist the trigger's regionName and regionId with their default values
// Leaf: ?OnPersist@CM_AssassinTrigger@CV_AssassinAttacksScript@NScript@@EAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E38D80, 79 bytes, thiscall, `ret 4` (private virtual, entity vtable slot 4).
// Identity (proven): slot 4 (OnPersist) of the RTTI-less entity vtable 0x012E36B8, which the landed
//   NScript::CV_AssassinAttacksScript::CM_AssassinTrigger::Alloc 0x00E37D50 stores (site 0x00E37D85); Ego_r.pdb has ?OnPersist@CM_AssassinTrigger@CV_AssassinAttacksScript@NScript@@EAEXAAVCPersistContext@@@Z.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Layout: Ego_r.pdb CM_AssassinTrigger: ParentClass +0x14, PMasterData +0x18, CCharString regionName +0x1c,
//   AA_LocationIdentifier regionId +0x20.
// Callees: the two-argument CPersistContext::Transfer<T>(name, value) inline (Ego_r.pdb has both
//   ??$Transfer@VCCharString@@@CPersistContext@@QAEXPBDAAVCCharString@@@Z and the AA_LocationIdentifier one), passing
//   GetDefaultVal<T>(): CPersistContext::GetDefaultVal<CCharString> 0x0040E1E0 (protected static __fastcall, Ego_r.pdb
//   ??$GetDefaultVal@VCCharString@@@CPersistContext@@KI?AVCCharString@@XZ; the result in the pushed-ecx slot) and the
//   inlined enum default 0; Transfer<CCharString>(name, value, default) 0x004109A0, ~CCharString 0x0099EAE0,
//   Transfer<AA_LocationIdentifier>(name, value, default) 0x00E38DD0 (this TU's instance); keys 0x012E37A8, 0x012E379C.
class CCharString {
public:
    void* PData;
    ~CCharString();                                     // 0x0099EAE0
};
class CPersistContext {
protected:
    template <class T> static T __fastcall GetDefaultVal();   // the CCharString instance is 0x0040E1E0
public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);
    template <class T> void Transfer(const char* name, T& value) { Transfer(name, value, GetDefaultVal<T>()); }
};
class CBaseClass {                                      // vtable 0x01231710
public:
    CBaseClass() { }
    virtual ~CBaseClass();
};
class CCPPointerInfo {
public:
    unsigned long RefCount;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};
class CScriptThing : public CBaseClass {                // 12 bytes, vtable 0x01238C8C
public:
    virtual ~CScriptThing();                            // slot 0
    CCountedPointer<CScriptThing> PImp;                 // +0x04 Data, +0x08 Info
};
class CGameScriptInterface;
class CScriptBase;
class CPersistContext;
class CEntityScriptBase {                               // Ego_r.pdb: vptr, Game +0x04, Me +0x08 (20 bytes)
public:
    virtual ~CEntityScriptBase();                       // slot 0
    virtual void Main() = 0;                            // slot 1
    virtual void Init();                                // slot 2
    virtual CScriptBase* GetParentScript() const = 0;   // slot 3
    virtual void OnPersist(CPersistContext& context);   // slot 4
    virtual void OnPredicateFail();                     // slot 5
    virtual void OnInterrupted();                       // slot 6
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_AssassinAttacksScript {
public:
    enum AA_LocationIdentifier { };                  // Ego_r.pdb nested enum
    class CM_AssassinTrigger;
};
}
// The enum default is inline (retail stores 0 into the dead `context` slot); a generic `return T();` body plus an explicit
// specialization for CCharString makes VC7.1 fail with C1001, so the inline one is the enum's specialization.
template <> inline NScript::CV_AssassinAttacksScript::AA_LocationIdentifier __fastcall
CPersistContext::GetDefaultVal<NScript::CV_AssassinAttacksScript::AA_LocationIdentifier>()
{
    return (NScript::CV_AssassinAttacksScript::AA_LocationIdentifier)0;
}
namespace NScript {
class CV_AssassinAttacksScript::CM_AssassinTrigger : public CEntityScriptBase {
public:
    CV_AssassinAttacksScript* ParentClass;              // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;              // +0x18
    CCharString               regionName;               // +0x1c
    AA_LocationIdentifier     regionId;                 // +0x20
private:
    virtual void OnPersist(CPersistContext& context);
};

void CV_AssassinAttacksScript::CM_AssassinTrigger::OnPersist(CPersistContext& context)
{
    context.Transfer("regionName", regionName);
    context.Transfer("regionId", regionId);
}
}