// NScript::CQ_WhiteBalverineWWScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- quest-script factory: new CQ_WhiteBalverineWWScript(data, game) with the constructor inlined (CScriptBase, Game / MasterScriptData, the own vtable, the empty WhiteBalverine script thing)
// Leaf: ?Alloc@CQ_WhiteBalverineWWScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E1A770, 84 bytes (+ int3 padding to 96), static `__fastcall` (data in ecx, game in edx), plain `ret`.
// Identity (proven): NScript::CQ_WhiteBalverineWWScript::Alloc; evidence: the inlined constructor stores the RTTI-less vtable 0x012E0CE4 (also stored by this class's J036 constructor 0x00E18440), whose Main (slot 2, 0x00E18630) registers the entity scripts KG_Chief, WBWW_WhiteBalverine, WBWW_SoldierBalverine, WBWW_WhiteBalverine, all nested in Ego_r.pdb NScript::CQ_WhiteBalverineWWScript; the empty script thing lands at +0x50 = Ego_r CQ_WhiteBalverineWWScript::WhiteBalverine - 0x14 and `new` asks 0x5C (sizeof Ego_r 0x70 - 0x14); this factory is stored at 0x00CD7A4D (CScriptInfoManager registration 0x00CD52D0); the Ego_r/FableWin.pdb name sets hold ?Alloc@CQ_WhiteBalverineWWScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Label: the manifest's "Alloc" is generic (family J037: six one-CScriptThing quest-script factories).
// Callees: operator new 0x00BFEA1A (0x5C bytes), CScriptBase::CScriptBase 0x00CB8110 (??0CScriptBase@@QAE@XZ), CBaseClass::CBaseClass
//   0x0099A2D0 (??0CBaseClass@@QAE@XZ; the inline CScriptThing() then stores its vtable 0x01238C8C and zeroes PImp).
// Layout: Ego_r.pdb CQ_WhiteBalverineWWScript (0x70; retail 0x5C = Ego_r - 0x14): CLevelScriptBase<T> {Game, MasterScriptData} at
//   retail +0x40 / +0x44, WhiteBalverine (CScriptThing) Ego_r +0x64 -> retail +0x50.
// Model: the J036 constructor of the same class made inline (agent952 H037 shape); local classes -- Review at landing.
class CScriptDataBase;
class CGameScriptInterfaceBase;
class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;                               // +0x0
    CCPPointerInfo* Info;                               // +0x4
    CCountedPointer() : Data(0), Info(0) { }
};
class CBaseClass {
public:
    CBaseClass();                                       // 0x0099A2D0
    virtual ~CBaseClass();                              // 0x0099A2E0
};
class CScriptThing : public CBaseClass {               // vtable 0x01238C8C, 0x0C bytes
public:
    CCountedPointer<CScriptThing> PImp;                 // +0x04
    CScriptThing() { }
};
class CScriptBase {
public:
    CScriptBase();                                      // 0x00CB8110
    virtual ~CScriptBase();
    unsigned char _base[0x40 - 4];                      // Threads, EntityScripts, EntityScriptBindings, LastWorldUpdateFrame,
                                                        // PActiveThread, RuntimeScriptedEntityPersist
};
template <class TMasterData> class CLevelScriptBase : public CScriptBase {
public:
    CGameScriptInterfaceBase* Game;                     // +0x40
    TMasterData*              MasterScriptData;         // +0x44
    CLevelScriptBase(CScriptDataBase* data, CGameScriptInterfaceBase& game)
        : Game(&game), MasterScriptData((TMasterData*)data) { }
};
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_WhiteBalverineWWScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQ_WhiteBalverineWWScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
        : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    virtual ~CQ_WhiteBalverineWWScript();
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    unsigned char _members[0x50 - 0x48];          // POD members up to the script thing
    CScriptThing  WhiteBalverine;                            // +0x50
};
CScriptBase* __fastcall CQ_WhiteBalverineWWScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CQ_WhiteBalverineWWScript(data, game);
}
}