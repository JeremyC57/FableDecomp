// NScript::CV_RandomPopulationSimScript::CV_RandomPopulationSimScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- script constructor: CScriptBase base, then the CLevelScriptBase members Game = &game and MasterScriptData = data, then the script's vtable
// Leaf: ??0CV_RandomPopulationSimScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EC17E0, 48 bytes, `ret 8`. Stores the script vtable @ 0x012F12D8.
// Identity: that vtable is stored by the script's Alloc, which CScriptInfoManager::RegisterScripts (0x00CD52D0) registers as
//   "V_RandomPopulationSim" (site 0x00CD9391) -> NScript::CV_RandomPopulationSimScript; the PDB name sets hold this constructor's mangled name (above).
//   Family F003 (76 byte-identical bodies): the manifest label (CEngineSubPrimitiveAnimatedMesh ctor) is propagated. The
//   out-of-line constructor has no caller in retail (the Alloc inlines its own copy).
// Callees: CScriptBase::CScriptBase() @ 0x00CB8110.
// Layout: CScriptBase is retail 0x40 bytes (vptr + 0x3c); CLevelScriptBase<TData> (PDB) adds Game +0x40 and MasterScriptData
//   +0x44. TData (the master script data class) enters no symbol of this function, so the base is modelled with the data
//   pointer typed as its CScriptDataBase base.
// Shape: the intermediate CLevelScriptBase vtable store is dead and dropped by VC7.1; the inline base constructor's
//   initialisers load the second argument first (Game, `mov eax,[esp+0xc]`) and store +0x40 before +0x44.
class CScriptDataBase {
public:
    virtual ~CScriptDataBase();
};
class CGameScriptInterfaceBase;
class CPersistContext;
class CScriptBase {                                     // retail 0x40 bytes
public:
    CScriptBase();                                      // 0x00CB8110
    virtual ~CScriptBase();
protected:
    virtual void RegisterMain();
    virtual void Main();
    virtual void Init();
public:
    virtual void OnPersist(CPersistContext& context);
    unsigned char _body[0x3c];                          // +0x04 .. +0x40
};
class CLevelScriptBase : public CScriptBase {           // PDB CLevelScriptBase<TData>
public:
    CGameScriptInterfaceBase* Game;                     // +0x40
    CScriptDataBase* MasterScriptData;                  // +0x44
    CLevelScriptBase(CScriptDataBase* data, CGameScriptInterfaceBase& game) : Game(&game), MasterScriptData(data) {}
};
namespace NScript {
class CV_RandomPopulationSimScript : public CLevelScriptBase {
public:
    CV_RandomPopulationSimScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    virtual void RegisterMain();
    virtual void Main();
};
CV_RandomPopulationSimScript::CV_RandomPopulationSimScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase(data, game)
{
}
}