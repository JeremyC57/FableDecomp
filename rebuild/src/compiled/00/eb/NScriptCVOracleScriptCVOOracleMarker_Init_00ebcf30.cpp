// NScript::CV_OracleScript::CVO_OracleMarker::Init() -- reset the script's state: TimesPlayedYCat, TimesPlayedMCat, TimesPlayedCCat, TimesPlayedACat, HaveBeenToldHowToInteract = 0/false
// Leaf: ?Init@CVO_OracleMarker@CV_OracleScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EBCF30, 18 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 of the entity-script vtable 0x012F0CD8 (stored by the landed CVO_OracleMarker::Alloc); FableWin.pdb
//   ?Init@CVO_OracleMarker@CV_OracleScript@NScript@@EAEXXZ; the members it clears are Ego_r.pdb's CVO_OracleMarker fields at their PDB offsets.
// Shape (family K066, representative 0x00CF06D0): one zero register (al, or eax when a long is cleared) and one store per
//   member in source order; every state member is cleared.
class CScriptDataBase;
class CGameScriptInterfaceBase;
class CScriptThing {                                    // 12 bytes (Ego_r): vptr, PImp (a CCountedPointer)
public:
    virtual ~CScriptThing();
    void* PImpData;                                     // +0x04
    void* PImpInfo;                                     // +0x08
};
class CScriptBase;
class CEntityScriptBase {                               // 20 bytes (Ego_r)
public:
    virtual ~CEntityScriptBase();
    virtual CScriptBase* GetParentScript() const;       // slot 1
private:
    virtual void Init();                                // slot 2
public:
    CGameScriptInterfaceBase* Game;                     // +0x04
    CScriptThing Me;                                    // +0x08
};
class CScriptBase {
public:
    virtual ~CScriptBase();
    unsigned char _body[0x3c];
};
class CLevelScriptBase : public CScriptBase {
public:
    CGameScriptInterfaceBase* Game;                     // +0x40
    CScriptDataBase* MasterScriptData;                  // +0x44
};
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_OracleScript : public CLevelScriptBase {
public:
    class CVO_OracleMarker : public CEntityScriptBase {
    private:
        virtual void Init();
    public:
        CV_OracleScript* ParentClass;                             // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        long  TimesPlayedYCat;                         // +0x1C
        long  TimesPlayedMCat;                         // +0x20
        long  TimesPlayedCCat;                         // +0x24
        long  TimesPlayedACat;                         // +0x28
        bool  HaveBeenToldHowToInteract;               // +0x2C
    };
};

void CV_OracleScript::CVO_OracleMarker::Init()
{
    TimesPlayedYCat = 0;
    TimesPlayedMCat = 0;
    TimesPlayedCCat = 0;
    TimesPlayedACat = 0;
    HaveBeenToldHowToInteract = false;
}
}