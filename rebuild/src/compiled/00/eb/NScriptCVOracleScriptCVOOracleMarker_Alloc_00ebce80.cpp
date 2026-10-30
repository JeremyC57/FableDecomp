// NScript::CV_OracleScript::CVO_OracleMarker::Alloc(CV_OracleScript& parent, CScriptDataBase* data, const CScriptThing& me) -- entity-script factory of the "VO_OracleMarker" binding: new CVO_OracleMarker(parent, data, me) (CEntityScriptBase(*parent.Game, me), ParentClass = &parent, PMasterData = data) handed to a fresh CCountedPointer<CEntityScriptBase>
// Leaf: ?Alloc@CVO_OracleMarker@CV_OracleScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EBCE80, 145 bytes (+ int3 padding), static `__fastcall` (hidden result
//   pointer ecx, parent edx, data and me on the stack), `ret 8`, returns the result pointer.
// Identity: Ego_r.pdb ?Alloc@CVO_OracleMarker@CV_OracleScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z is exactly the symbol this source emits.
//   CV_OracleScript::Main 0x00EBCD70 (slot 2 of the script vtable 0x012F0C84, which the "V_Oracle" script's Alloc stores at its CScriptInfoManager::RegisterScripts registration) stores this address as the factory of the CEntityScriptBinding<CV_OracleScript> record named "VO_OracleMarker" (site 0x00EBCDBA).
//   Was landed as _global::Create_CVtable_012f0cd8 (named by the entity vtable address); same shape as the family-pass F014 Alloc rows.
// Callees: scalar operator new 0x00BFEA1A (0x30 = sizeof(CVO_OracleMarker), then 0x0C for the count block),
//   CScriptThing::CScriptThing(const CScriptThing&) 0x004ABE90 (Me), and the count block's deleter
//   0x00CDEE00 (stored, not called: `if (p) p->~CEntityScriptBase()` deleting, `__fastcall`).
// Data: vtables CEntityScriptBase 0x012C3224 (stored before the Me copy) and CVO_OracleMarker 0x012F0CD8.
// Layout: Ego_r.pdb CV_OracleScript::CVO_OracleMarker (48 bytes = the retail new size): ParentClass +0x14, PMasterData +0x18, TimesPlayedYCat +0x1C, TimesPlayedMCat +0x20, TimesPlayedCCat +0x24, TimesPlayedACat +0x28, HaveBeenToldHowToInteract +0x2C.
//   CEntityScriptBase: vptr, Game +0x04, Me +0x08 (Ego_r 20 bytes). The parent's Game is CLevelScriptBase's +0x40.
// Shape: the PDB ctor (parent, data, me) inlined: base and ParentClass in the init list, PMasterData assigned in the body;
//   STLport-free counted pointer ctor: Data first, the null test on the argument, the 12-byte {1, deleter, Data} block
//   read back from Data, three exits (block, failed new, null object).
class CGameScriptInterfaceBase;
class CScriptDataBase;

// 0x00CDEE00 is CCountedPointer<T>::DeleteData (engine/CCountedPointer.h; reloc_bindings binds both spellings)

#include "engine/CCountedPointer.h"


// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_OracleScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CVO_OracleMarker : public CEntityScriptBase {
    public:
        CV_OracleScript* ParentClass;                   // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        long TimesPlayedYCat;                           // +0x1C
        long TimesPlayedMCat;                           // +0x20
        long TimesPlayedCCat;                           // +0x24
        long TimesPlayedACat;                           // +0x28
        bool HaveBeenToldHowToInteract;                 // +0x2C
        CVO_OracleMarker(CV_OracleScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        virtual ~CVO_OracleMarker();
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_OracleScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

CCountedPointer<CEntityScriptBase> __fastcall CV_OracleScript::CVO_OracleMarker::Alloc(CV_OracleScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CVO_OracleMarker(parent, data, me));
}
}
