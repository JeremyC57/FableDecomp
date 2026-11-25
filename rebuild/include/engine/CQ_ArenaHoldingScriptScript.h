#ifndef FABLE_ENGINE_CQ_ARENAHOLDINGSCRIPTSCRIPT_H
#define FABLE_ENGINE_CQ_ARENAHOLDINGSCRIPTSCRIPT_H
// Retail opt-in (chunk 804): NScript::CQ_ArenaHoldingScriptScript, new header (no includer before chunk 804; the 29 landed sibling
// rows keep their local models).
// Layout: Ego_r.pdb CQ_ArenaHoldingScriptScript (struct_layouts_egor.tsv, 100 bytes) at retail offsets = PDB - 0x14 (retail
// CLevelScriptBase is 0x48; no STLport container in the class): PlayerAttackedFans +0x48 .. InHitCutsceneAlready +0x4c, sizeof 0x50
// (the landed Alloc 0x00CF8860 news 0x50 bytes). Checked against retail: the fan Mains 0x00CF18D0 / 0x00CF2B70 / 0x00CF3D20 read
// PlayerAttackedFans +0x48, FansCanLeaveNow +0x49, ArenaOver +0x4b and InHitCutsceneAlready +0x4c, and write +0x48 / +0x4c.
// Nested fans (Ego_r.pdb, 28 bytes each): CEntityScriptBase (0x14), ParentClass +0x14, PMasterData +0x18; vtables 0x012C6260
// (COakvaleFan, stored by Alloc 0x00CF17F0), 0x012C6384 (CBowerstoneFan, Alloc 0x00CF2A90), 0x012C6480 (CHookCoastFan, Alloc
// 0x00CF3C40): slot 0 scalar deleting dtor, slot 1 Main, slot 2 Init, slot 3 GetParentScript.
// Retail methods: the Ego_r.pdb / FableWin.pdb name set ?<Name>@<Class>@CQ_ArenaHoldingScriptScript@NScript@@...
// Base: CLevelScriptBase<CQ_SunnyvaleMasterData> (the landed ctor 0x00CF06A0; the entity scripts' PMasterData is
// CQ_SunnyvaleMasterData*).
#ifdef FABLE_RETAIL_NSCRIPT
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_ArenaHoldingScriptScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();                                         // 0x00CF06F0 (slot 1)
    virtual void Main();                                                 // 0x00CF0790 (slot 2)
    virtual void Init();                                                 // 0x00CF06D0 (slot 3)
    bool PlayerAttackedFans;                                             // +0x48
    bool FansCanLeaveNow;                                                // +0x49
    bool OakvaleInsult;                                                  // +0x4a
    bool ArenaOver;                                                      // +0x4b
    bool InHitCutsceneAlready;                                           // +0x4c
    class CEntranceGuard;                                                // Alloc 0x00CF0B60
    class CEntranceGuard2;                                               // Alloc 0x00CF0C40
    class CFanMarker;                                                    // Alloc 0x00CF4DF0
    class COakvaleFan : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_ArenaHoldingScriptScript&, CScriptDataBase*, const CScriptThing&); // 0x00CF17F0
        virtual CScriptBase* GetParentScript() const;                    // 0x00CF1890
    protected:
        CQ_ArenaHoldingScriptScript& ParentClass;                        // +0x14
        CQ_SunnyvaleMasterData* PMasterData;                             // +0x18
    private:
        virtual void Init();                                             // 0x00CF18A0
        virtual void Main();                                             // 0x00CF18D0
    };
    class CBowerstoneFan : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_ArenaHoldingScriptScript&, CScriptDataBase*, const CScriptThing&); // 0x00CF2A90
        virtual CScriptBase* GetParentScript() const;                    // 0x00CF2B30
    protected:
        CQ_ArenaHoldingScriptScript& ParentClass;                        // +0x14
        CQ_SunnyvaleMasterData* PMasterData;                             // +0x18
    private:
        virtual void Init();                                             // 0x00CF2B40
        virtual void Main();                                             // 0x00CF2B70
    };
    class CHookCoastFan : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_ArenaHoldingScriptScript&, CScriptDataBase*, const CScriptThing&); // 0x00CF3C40
        virtual CScriptBase* GetParentScript() const;                    // 0x00CF3CE0
    protected:
        CQ_ArenaHoldingScriptScript& ParentClass;                        // +0x14
        CQ_SunnyvaleMasterData* PMasterData;                             // +0x18
    private:
        virtual void Init();                                             // 0x00CF3CF0
        virtual void Main();                                             // 0x00CF3D20
    };
#ifdef FABLE_RETAIL_CQ_ARENAHOLDINGSCRIPTSCRIPT_ALLOC
    // Retail opt-in (NF436, row 0x00CF8860 Alloc): FableWin.pdb ??_7CQ_ArenaHoldingScriptScript@NScript@@6B@ / LF_CLASS (all public: the user ctor, static
    //   Alloc, the virtual overrides) for what this header does not declare yet: CQ_ArenaHoldingScriptScript, Alloc. Declarations only, no layout
    //   change; existing includers do not define the macro.
public:
    CQ_ArenaHoldingScriptScript(CScriptDataBase* data, CGameScriptInterfaceBase& game); // FableWin.pdb ??0CQ_ArenaHoldingScriptScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
    static CScriptBase* FABLE_FASTCALL Alloc(CScriptDataBase*, CGameScriptInterfaceBase&);   // 0x00CF8860
#endif
};
#ifdef FABLE_RETAIL_CQ_ARENAHOLDINGSCRIPTSCRIPT_FANMARKER
// Retail opt-in (chunk 812): Ego_r.pdb CFanMarker (28 bytes: CEntityScriptBase 0x14, ParentClass +0x14, PMasterData +0x18); vtable
// 0x012C657C (stored by the Alloc 0x00CF4DF0): 0 scalar deleting dtor 0x00CF4EA0, 1 Main 0x00CF4EC0, 2 CEntityScriptBase::Init
// 0x00CDEBB0 (not overridden), 3 GetParentScript 0x00CF4E90, 4-6 the CEntityScriptBase defaults. Ego_r.pdb / FableWin.pdb
// ?Main@CFanMarker@CQ_ArenaHoldingScriptScript@NScript@@EAEXXZ (private). Declarations only.
class CQ_ArenaHoldingScriptScript::CFanMarker : public CEntityScriptBase {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_ArenaHoldingScriptScript&, CScriptDataBase*, const CScriptThing&); // 0x00CF4DF0
    virtual CScriptBase* GetParentScript() const;                        // 0x00CF4E90
protected:
    CQ_ArenaHoldingScriptScript& ParentClass;                            // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                                 // +0x18
private:
    virtual void Main();                                                 // 0x00CF4EC0
};
#endif
#ifdef FABLE_RETAIL_CQ_ARENAHOLDINGSCRIPTSCRIPT_ENTRANCEGUARD
// wfE1 opt-in (row 0x00CF68C0): Ego_r.pdb CEntranceGuard (32 bytes: CEntityScriptBase 0x14, ParentClass +0x14, PMasterData +0x18,
// IntroDone +0x1c, DoneArena +0x1d, the bools the landed Init 0x00CF0C10 clears); vtable 0x012C6158 stored by the landed Alloc
// 0x00CF0B60: 0 scalar deleting dtor 0x00CF0C20, 1 Main 0x00CF68C0, 2 Init 0x00CF0C10, 3 GetParentScript 0x00CF0C00, 4 OnPersist
// 0x00CF6890. Ego_r.pdb / FableWin.pdb ?Main@CEntranceGuard@CQ_ArenaHoldingScriptScript@NScript@@EAEXXZ (private). Declarations only.
class CQ_ArenaHoldingScriptScript::CEntranceGuard : public CEntityScriptBase {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_ArenaHoldingScriptScript&, CScriptDataBase*, const CScriptThing&); // 0x00CF0B60
    virtual CScriptBase* GetParentScript() const;                        // 0x00CF0C00
private:
    virtual void Main();                                                 // 0x00CF68C0
    virtual void Init();                                                 // 0x00CF0C10
    virtual void OnPersist(CPersistContext&);                            // 0x00CF6890
protected:
    CQ_ArenaHoldingScriptScript& ParentClass;                            // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                                 // +0x18
    bool IntroDone;                                                      // +0x1c
    bool DoneArena;                                                      // +0x1d
};
#endif
}
FABLE_STATIC_ASSERT_AT(88, sizeof(NScript::CQ_ArenaHoldingScriptScript) == 0x50);
FABLE_STATIC_ASSERT_AT(89, sizeof(NScript::CQ_ArenaHoldingScriptScript::COakvaleFan) == 0x1c);
#endif
#endif
