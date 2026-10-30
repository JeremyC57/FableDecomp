// CDefClassBase::GetSubDef<CEntitySoundDef>(CDefPointer<const CEntitySoundDef>* out) const -- the def's CEntitySoundDef sub-def: look the sub-def
// index up by class name, fetch the def from the definition manager into *out; false when the def has no such sub-def
// Leaf: ??$GetSubDef@VCEntitySoundDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCEntitySoundDef@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065F604, 162 bytes, thiscall `ret 4`, frame-pointer size TU.
// Identity (FINDINGS chunk 384; manifest GetSubDef<CAbilityDef>): Ego_r.pdb / FableWin.pdb name set
//   `??$GetSubDef@VCEntitySoundDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCEntitySoundDef@@@@@Z`
//   and the class-name literal 'CEntitySoundDef' the body builds for the lookup (one retail body per T: no ICF, so each
//   instantiation is its own address); the model of the sweep3 GetSubDef family.
//   The manager call @ 0x009ADA10 is Ego_r `?GetPDefFromGlobalIndex@CDefinitionManager@@IAE?AV?$CDefPointer@VCDefClassBase@@@@J@Z`,
//   the protected non-const overload: PDefManager (+0x1C) is a plain CDefinitionManager*, and Ego_r / FableWin list
//   the public const overload (QBE, CDefPointer<const CDefClassBase>) first, 0x30 bytes before it, as retail 0x009AD9E0
//   (called by the const CDefinitionManager::GetDef<T>) precedes 0x009ADA10. The index lookup is the def's virtual
//   slot 14 (+0x38), the const GetPSubDefInfo.
// Body/shape (wave 2 class-model migration, 2026-10-30): the shared retail models (FABLE_RETAIL_DEF_CLASS_BASE) and the
//   FableWin.pdb member set of the template: T::StatGetTypeName() (inline; the class-name literal) feeds the const slot-14
//   GetPSubDefInfo; the manager's CDefPointer<CDefClassBase> temporary converts (template ctor, addref) into a local
//   CDefPointer<const CDefClassBase> and is released (the `mov edi,[eax+4] ; dec edi` shape); SetDynamicCast<const
//   CDefClassBase> assigns it into *out (release old, store, addref); the local's destructor releases (`dec [esi+4] ; jne`).
#pragma optimize("y", off)
#pragma optimize("s", on)

#include "fable_string.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CSubDefClassBase.h"
#include "engine/CDefPointer.h"

class CEntitySoundDef : public CSubDefClassBase {                    // RTTI .?AVCEntitySoundDef@@ : CSubDefClassBase
public:
    static CCharString __fastcall StatGetTypeName() { return CCharString("CEntitySoundDef", -1); }   // ?StatGetTypeName@CEntitySoundDef@@SI?AVCCharString@@XZ (inline)
    virtual void Transfer(CPersistContext&);                      // ?Transfer@CEntitySoundDef@@UAEXAAVCPersistContext@@@Z
    virtual void Copy(const CDefClassBase*);                      // ?Copy@CEntitySoundDef@@UAEXPBVCDefClassBase@@@Z
    virtual unsigned long GetSizeofClass();                       // ?GetSizeofClass@CEntitySoundDef@@UAEKXZ
};

class CDefinitionManager {
    friend class CDefClassBase;
protected:
    CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long index);   // 0x009ADA10 (protected non-const overload; hidden result pointer)
};

template <>
bool CDefClassBase::GetSubDef<CEntitySoundDef>(CDefPointer<const CEntitySoundDef>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(CEntitySoundDef::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
