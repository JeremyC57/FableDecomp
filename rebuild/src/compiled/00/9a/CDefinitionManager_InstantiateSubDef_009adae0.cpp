// CDefinitionManager::InstantiateSubDef(const CCharString& class_name, unsigned long parent_index) -- create one sub-def of a registered def class under an existing parent def: look the class record up by name; on a hit allocate the def through the class's AllocFunc, mark it as not a template, bind it to this manager under the next global index, record it (with no instantiation name) in InstantiatedDefs and in the class's GlobalIndices, tell the parent def about it (AddSubDef) and return a counted handle to it; on a miss only make sure the parent is loaded and return a null handle
// Leaf: ?InstantiateSubDef@CDefinitionManager@@IAE?AV?$CDefPointer@VCDefClassBase@@@@ABVCCharString@@K@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x009ADAE0, 417 bytes (`ret 0Ch`: hidden result, class name, parent index). Ego_r.pdb /
//   FableWin.pdb ?InstantiateSubDef@CDefinitionManager@@IAE?AV?$CDefPointer@VCDefClassBase@@@@ABVCCharString@@K@Z (protected).
//   Callers: CDefinitionManager::CompileSubDefinition 0x009AE150 (twice).
// Callees: CDefinitionManager::GetPDefClassInfo(const CCharString&) 0x009AD580; the class record's AllocFunc (+4, __fastcall, no
//   arguments); operator new 0x00BFEA1A (16-byte CInstantiatedDefInfo); CDefString::SetString 0x009D5060 (inside the inline
//   CDefString(const CCharString&) constructor); STLport vector<CInstantiatedDefInfo*>::_M_insert_overflow 0x009B4BD0 and
//   vector<unsigned long>::_M_insert_overflow 0x00464ACE (the grow paths of the two inline push_backs);
//   CDefinitionManager::LoadDef(unsigned long) const 0x009AD6E0 (twice, inside the inline GetPDefFromGlobalIndex);
//   CDefClassBase::AddSubDef(const CCharString&, long, long) (virtual slot 12, +0x30) on the parent def;
//   CIVCountedPointeeBase::Delete (slot 1, +4) on a last reference (inside the inline ~CDefPointer, three sites).
//   Inline: CDefClassBase::SetAsTemplate(bool), CDefPointer<CDefClassBase>(CDefClassBase*), its copy constructor and destructor,
//   CInstantiatedDefInfo's constructor, CArray::size / push_back / operator[], CDefString() and CDefString(const CCharString&),
//   CDefinitionManager::GetPDefFromGlobalIndex(long) (the protected non-const overload).
// Layout: CDefinitionManager InstantiatedDefs +0xA4; CDefClassInfo Name +0, AllocFunc +4, GlobalIndices +8; CInstantiatedDefInfo
//   ClassName +0, InstantiationName +4, ClassIndex +8, PDef +0xC; CDefClassBase IVRefCount +4, PDefManager +0x1C,
//   GlobalIndex +0x20, Template = bit 1 of +0x24.
// Shape: the same skeleton as its neighbour InstantiateDef 0x009ADC90: `if (info) { ... return pdef; } ... return null;` gives the
//   two epilogues (the miss one last); `info` is homed in the dead class_name argument slot; the two indices are signed longs
//   taken before the allocation (ebx / edi); the record is built in its own statement and then pushed, so the class-name
//   CDefString temporary and the two push_back value temporaries share the one 4-byte frame slot (`sub esp,8` = that slot plus
//   the spilled `this`). The record's instantiation name is a default CDefString (`mov dword ptr [ebp+4],0FFFFFFFFh`).
//   The template flag is cleared through the inline CDefClassBase::SetAsTemplate(false) (FableWin.pdb
//   ?SetAsTemplate@CDefClassBase@@QAEX_N@Z): the inlined bool parameter is why retail has `mov al,[esi+24h] ; and al,0FDh ;
//   mov [esi+24h],al` (0x009ADB24) where a direct `Template = false` compiles to `and byte ptr [esi+24h],0FDh`.
//   The parent handle is a named local built from the inline GetPDefFromGlobalIndex (LoadDef, then a counted copy of
//   InstantiatedDefs[parent_index]->PDef: `test edi,edi ; je ; inc [edi+4]` at 0x009ADBFB); returning `pdef` copies it into the
//   result (`mov [ebx],esi ; inc [esi+4]`), then the parent handle and the creation handle are released in that order.
//   On the miss path the discarded GetPDefFromGlobalIndex(parent_index) temporary is copied and released at once (the
//   `inc edx ; mov eax,edx ; dec eax` pair at 0x009ADC5C) before the null handle is returned.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_CTOR_SETSTRING
#define FABLE_RETAIL_CSYMBOLMAP
#define FABLE_RETAIL_CSYMBOLMAP_ADDSYMBOLDIRTY
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFCLASSBASE_SETASTEMPLATE
#define FABLE_RETAIL_CDEFPOINTER_POINTER_CTOR_ADOPT
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_INSTANTIATEDEF
#define FABLE_RETAIL_CDEFINITIONMANAGER_INSTANTIATESUBDEF
#include "fable_string.h"
#include "engine/CDefString.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefinitionManager.h"

CDefPointer<CDefClassBase> CDefinitionManager::InstantiateSubDef(const CCharString& class_name, unsigned long parent_index)
{
    CDefClassInfo* info = GetPDefClassInfo(class_name);
    if (info) {
        long global_index = InstantiatedDefs.size();
        long class_index = info->GlobalIndices.size();
        CDefPointer<CDefClassBase> pdef(info->AllocFunc());
        pdef->SetAsTemplate(false);
        pdef->PDefManager = this;
        pdef->GlobalIndex = global_index;
        CInstantiatedDefInfo* def_info = new CInstantiatedDefInfo(info->Name, CDefString(), class_index, pdef);
        InstantiatedDefs.push_back(def_info);
        info->GlobalIndices.push_back(global_index);
        CDefPointer<CDefClassBase> parent = GetPDefFromGlobalIndex(parent_index);
        parent->AddSubDef(info->Name, global_index, parent_index);
        return pdef;
    }
    GetPDefFromGlobalIndex(parent_index);
    return CDefPointer<CDefClassBase>();
}