// CLevelScriptBase::CLevelScriptBase(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- (CLevelScriptBase<NScript::CDanyalTestScriptsData>) the CScriptBase base, Game = &game (+0x40), the template's vtable, MasterScriptData = data (+0x44)
// Leaf: ??0?$CLevelScriptBase@VCDanyalTestScriptsData@NScript@@@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CDD220, 34 bytes (+ int3 padding to 48), thiscall, `ret 8`, returns this.
// Identity (probable): CLevelScriptBase<NScript::CDanyalTestScriptsData>::CLevelScriptBase(CScriptDataBase*, CGameScriptInterfaceBase&); evidence: the stored vtable 0x012C2F3C (no RTTI, as every script vtable) holds an own deleting destructor and the four CScriptBase hooks 0x00CBD4B0..0x00CBD4E0 unoverridden, i.e. a bare CLevelScriptBase<TData>; both PDBs hold exactly five CLevelScriptBase<TData> constructors and retail has exactly these five rows (0x00CDD160..0x00CDD220, before the Sunnyvale master script code); the TData is assigned by order: FableWin.pdb lays the five constructors, destructors and deleting destructors out as SunnyvaleMaster, alextest, Empty, joss_test_scripts, DanyalTestScripts, and retail's constructors and deleting destructors 0x00CDD250.. follow their vtables in one order (the agent950 F003 script constructors inline this same base constructor)
// Label: the manifest's "CVertexShaderData" is propagated (family J024: six two-argument constructors that call one base
//   constructor and store the arguments around the own vtable store; retail has no identical-code folding).
// Callees: CScriptBase::CScriptBase() 0x00CB8110 (??0CScriptBase@@QAE@XZ, Ego_r.pdb; the catalog also lists stand-ins).
// Shape: the out-of-line template constructor (no caller: each script's Alloc inlines its own copy); the second argument is loaded first and Game stored before the vtable, MasterScriptData after it (agent950 F003).
class CScriptDataBase;
class CGameScriptInterfaceBase;
class CPersistContext;
class CScriptBase {                                             // retail 0x40 bytes
public:
    CScriptBase();                                              // 0x00CB8110
    virtual ~CScriptBase();
protected:
    virtual void RegisterMain();
    virtual void Main();
    virtual void Init();
public:
    virtual void OnPersist(CPersistContext& context);
    unsigned char _body[0x3c];                                  // +0x04 .. +0x40
};
template <class TData> class CLevelScriptBase : public CScriptBase {   // vtable: own deleting destructor + the four CScriptBase hooks
public:
    CLevelScriptBase(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    CGameScriptInterfaceBase* Game;                             // +0x40
    CScriptDataBase*          MasterScriptData;                 // +0x44
};
template <class TData>
CLevelScriptBase<TData>::CLevelScriptBase(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : Game(&game), MasterScriptData(data)
{
}
namespace NScript { class CDanyalTestScriptsData; }
template CLevelScriptBase<NScript::CDanyalTestScriptsData >::CLevelScriptBase(CScriptDataBase*, CGameScriptInterfaceBase&);