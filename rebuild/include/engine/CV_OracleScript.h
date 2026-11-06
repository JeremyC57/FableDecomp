#ifndef FABLE_ENGINE_CV_ORACLESCRIPT_H
#define FABLE_ENGINE_CV_ORACLESCRIPT_H
// Retail opt-in (chunk 806): NScript::CV_OracleScript, new header (the landed same-TU rows 0x00EBCD70 / 0x00EBCE80 / 0x00EBCF20 /
// 0x00EBCF30 / 0x00EBEDA0 keep their local models).
// Layout: Ego_r.pdb CV_OracleScript (struct_layouts_egor.tsv, 92 bytes) = retail CLevelScriptBase (0x48), no members of its own.
// Nested CVO_OracleMarker (Ego_r.pdb): CEntityScriptBase (0x14), ParentClass +0x14, PMasterData +0x18, TimesPlayedYCat +0x1c,
// TimesPlayedMCat +0x20, TimesPlayedCCat +0x24, TimesPlayedACat +0x28, HaveBeenToldHowToInteract +0x2c, sizeof 0x30 (Alloc
// 0x00EBCE80 allocates 0x30; the landed Init 0x00EBCF30 clears the five members at these offsets).
// Entity vtable 0x012F0CD8: 0 sdd 0x00EBCF50, 1 Main 0x00EBCF70, 2 Init 0x00EBCF30, 3 GetParentScript 0x00EBCF20, 4 OnPersist
// 0x00EBEDA0. Methods: Ego_r.pdb / FableWin.pdb ?<Name>@CVO_OracleMarker@CV_OracleScript@NScript@@ (Main / Init / OnPersist EAE).
#ifdef FABLE_RETAIL_NSCRIPT   // the retail NScript model only; no #error branch (VC7.1 under Wine spins on #error)
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
class CPersistContext;
class CScriptDataBase;
class CGameScriptInterfaceBase;
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_OracleScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CV_OracleScript(CScriptDataBase*, CGameScriptInterfaceBase&);       // 0x00EBCCA0
    static CScriptBase* FABLE_FASTCALL Alloc(CScriptDataBase*, CGameScriptInterfaceBase&);   // 0x00EBEE30
    virtual void RegisterMain();                                         // 0x00EBCCD0
    virtual void Main();                                                 // 0x00EBCD70
    class CVO_Woman;                                                     // Alloc 0x00EBE540
    class CVO_OracleMarker : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_OracleScript&, CScriptDataBase*, const CScriptThing&); // 0x00EBCE80
        virtual CScriptBase* GetParentScript() const;                    // 0x00EBCF20
    private:
        virtual void Main();                                             // 0x00EBCF70
        virtual void Init();                                             // 0x00EBCF30
        virtual void OnPersist(CPersistContext&);                        // 0x00EBEDA0
    protected:
        CV_OracleScript& ParentClass;                                    // +0x14
        CQ_SunnyvaleMasterData* PMasterData;                             // +0x18
    private:
        long TimesPlayedYCat;                                            // +0x1c
        long TimesPlayedMCat;                                            // +0x20
        long TimesPlayedCCat;                                            // +0x24
        long TimesPlayedACat;                                            // +0x28
        bool HaveBeenToldHowToInteract;                                  // +0x2c
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_OracleScript) == 0x48);
FABLE_STATIC_ASSERT(sizeof(NScript::CV_OracleScript::CVO_OracleMarker) == 0x30);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CV_ORACLESCRIPT_H
