// CDefClassBase::GetSubDef<CReplaceableMeshDef>(CDefPointer<const CReplaceableMeshDef>* out) const -- the def's CReplaceableMeshDef sub-def: look the sub-def
// index up by class name, fetch the def from the definition manager into *out; false when the def has no such sub-def
// Leaf: ??$GetSubDef@VCReplaceableMeshDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCReplaceableMeshDef@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007A6CA0, 182 bytes, thiscall `ret 4`, /O2 TU.
// Identity (sweep3 GetSubDef family; was the address-named CDefClassBase::GetSubDef_007a6ca0): Ego_r.pdb / FableWin.pdb name set
//   `??$GetSubDef@VCReplaceableMeshDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCReplaceableMeshDef@@@@@Z`
//   and the class-name literal 'CReplaceableMeshDef' the body builds for the lookup (one retail body per T: no ICF, so each
//   instantiation is its own address). Same template model as the landed 0x0065F604 GetSubDef<CEntitySoundDef>.
//   The manager call @ 0x009ADA10 is Ego_r `?GetPDefFromGlobalIndex@CDefinitionManager@@IAE?AV?$CDefPointer@VCDefClassBase@@@@J@Z`,
//   the protected non-const overload: PDefManager (+0x1C) is a plain CDefinitionManager*, and Ego_r / FableWin list
//   the public const overload (QBE, CDefPointer<const CDefClassBase>) first, 0x30 bytes before it, as retail 0x009AD9E0
//   (called by the const CDefinitionManager::GetDef<T>) precedes 0x009ADA10. The index lookup is the def's virtual
//   slot 14 (+0x38), the const GetPSubDefInfo.
// Retail bytes:  push ecx ; push esi ; push edi ; push -1 ; mov esi,ecx ; push offset 'CReplaceableMeshDef' ; lea ecx,[name] ; call CCharString::CCharString ; mov eax,[esi] ; lea ecx,[name] ; push ecx ; mov ecx,esi ; call [eax+38h] ; lea ecx,[name] ; mov edi,eax ; call CCharString::~CCharString ; test edi,edi ; je F ; mov edx,[edi] ; mov ecx,[esi+1Ch] ; push edx ; lea eax,[temp] ; push eax ; call GetPDefFromGlobalIndex ; mov esi,[eax] ; test esi,esi ; je C ; inc dword ptr [esi+4] ; C: mov eax,[temp] ; test eax,eax ; je A ; mov edi,[eax+4] ; mov ecx,eax ; add eax,4 ; dec edi ; mov edx,edi ; test edx,edx ; mov [eax],edi ; jne N ; mov edx,[ecx] ; call [edx+4] ; N: mov dword ptr [temp],0 ; A: mov edi,[out] ; mov ecx,[edi] ; cmp ecx,esi ; je K ; test ecx,ecx ; je B ; dec dword ptr [ecx+4] ; jne Z ; mov eax,[ecx] ; call [eax+4] ; Z: mov dword ptr [edi],0 ; B: test esi,esi ; mov [edi],esi ; je R ; inc dword ptr [esi+4] ; K: test esi,esi ; je R ; dec dword ptr [esi+4] ; jne R ; mov e?x,[esi] ; mov ecx,esi ; call [e?x+4] ; R: pop edi ; mov al,1 ; pop esi ; pop ecx ; ret 4 ; F: pop edi ; xor al,al ; pop esi ; pop ecx ; ret 4
// Body/shape (wave 2 class-model migration, 2026-10-30): the shared retail models (FABLE_RETAIL_DEF_CLASS_BASE) and the
//   FableWin.pdb member set of the template: T::StatGetTypeName() (inline; the class-name literal) feeds the const slot-14
//   GetPSubDefInfo; the manager's CDefPointer<CDefClassBase> temporary converts (template ctor, addref) into a local
//   CDefPointer<const CDefClassBase> and is released (the `mov edi,[eax+4] ; dec edi` shape); SetDynamicCast<const
//   CDefClassBase> assigns it into *out (release old, store, addref); the local's destructor releases (`dec [esi+4] ; jne`).

#include "fable_string.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CSubDefClassBase.h"
#include "engine/CDefPointer.h"

class CReplaceableMeshDef : public CSubDefClassBase {                    // RTTI .?AVCReplaceableMeshDef@@ : CSubDefClassBase
public:
    static CCharString __fastcall StatGetTypeName() { return CCharString("CReplaceableMeshDef", -1); }   // ?StatGetTypeName@CReplaceableMeshDef@@SI?AVCCharString@@XZ (inline)
    virtual void Transfer(CPersistContext&);                      // ?Transfer@CReplaceableMeshDef@@UAEXAAVCPersistContext@@@Z
    virtual void Copy(const CDefClassBase*);                      // ?Copy@CReplaceableMeshDef@@UAEXPBVCDefClassBase@@@Z
    virtual unsigned long GetSizeofClass();                       // ?GetSizeofClass@CReplaceableMeshDef@@UAEKXZ
};

class CDefinitionManager {
    friend class CDefClassBase;
protected:
    CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long index);   // 0x009ADA10 (protected non-const overload; hidden result pointer)
};

template <>
bool CDefClassBase::GetSubDef<CReplaceableMeshDef>(CDefPointer<const CReplaceableMeshDef>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(CReplaceableMeshDef::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
