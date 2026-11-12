// CDefinitionManager::InstantiateDef(const CCharString& class_name, const CCharString& instantiation_name, bool is_template) -- create one def of a registered def class: look the class record up by name (fail when it is not registered), allocate the def through the class's AllocFunc, mark it as a template or not, name it when it is a parent def, bind it to this manager under the next global index, record it in InstantiatedDefs and in the class's GlobalIndices, publish the instance name in both symbol maps and drop the creation reference (the local def handle's destructor)
// Leaf: ?InstantiateDef@CDefinitionManager@@IAE_NABVCCharString@@0_N@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x009ADC90, 382 bytes (`ret 0Ch`). Ego_r.pdb / FableWin.pdb
//   ?InstantiateDef@CDefinitionManager@@IAE_NABVCCharString@@0_N@Z (protected, bool, two CCharString references and a bool).
//   Callers: ParseStringForSymbols 0x009ADE90 (class name, instance name, true / false), AddDefClass 0x009B0AC0 (init.Name, null name, false).
// Callees: CDefinitionManager::GetPDefClassInfo(const CCharString&) 0x009AD580; the class record's AllocFunc (+4, __fastcall, no
//   arguments); CDefClassBase::IsParentDef (virtual slot 16, +0x40) and SetInstantiationName(CDefString) (slot 11, +0x2C);
//   CDefString::SetString 0x009D5060 (three times, inside the inline CDefString(const CCharString&) constructor); operator new
//   0x00BFEA1A (16-byte CInstantiatedDefInfo); STLport vector<CInstantiatedDefInfo*>::_M_insert_overflow 0x009B4BD0 and
//   vector<unsigned long>::_M_insert_overflow 0x00464ACE (the grow paths of the two inline push_backs); CSymbolMap::AddSymbolDirty
//   (name, value) 0x009E5420 and (group, name, value) 0x009E54E0; CIVCountedPointeeBase::Delete (slot 1, +4) on the last reference
//   (inside the inline ~CDefPointer). Inline: CDefPointer<CDefClassBase>(CDefClassBase*), its copy constructor and destructor,
//   CInstantiatedDefInfo's constructor, CArray::size / push_back, CDefString(const CCharString&).
// Layout: CDefinitionManager PSymbolMap +0x58, PDefinitionSymbolMap +0x60, InstantiatedDefs +0xA4; CDefClassInfo Name +0,
//   AllocFunc +4, GlobalIndices +8; CInstantiatedDefInfo ClassName +0, InstantiationName +4, ClassIndex +8, PDef +0xC;
//   CDefClassBase IVRefCount +4, PDefManager +0x1C, GlobalIndex +0x20, Template = bit 1 of +0x24.
// Shape: `if (info) { ... return true; } return false;` gives the two epilogues (the miss one at the end). After the lookup the
//   class name is dead: everything uses info->Name (CDefClassInfo +0), so `info` is homed in the class_name argument slot. The
//   two indices are signed longs taken before the allocation (edi / ebx); pushing the long global index into the
//   vector<unsigned long> makes the conversion temporary retail stores in the dead is_template slot. The record is built in its
//   own statement (the two by-value CDefString arguments are built right to left, instance name first) and then pushed, which
//   lets the class-name temporary share that same slot.
// The def handle: `pdef` is a CDefPointer<CDefClassBase> local built from AllocFunc's result by the handle's pointer constructor
//   (FableWin.pdb ??0?$CDefPointer@VCDefClassBase@@@@QAE@PAVCDefClassBase@@@Z 0x030644A0), which adopts the creation reference
//   without counting it (no `inc` after `mov esi,eax` at 0x009ADCD6). The record's constructor has the FableWin.pdb signature
//   ??0CInstantiatedDefInfo@CDefinitionManager@@QAE@VCDefString@@0KABV?$CDefPointer@VCDefClassBase@@@@@Z (0x03061650): it takes the
//   handle by const reference and copies it into PDef, the single `mov [ebp+0Ch],esi ; inc dword ptr [esi+4]` at 0x009ADD49.
//   The handle's destructor at the end of the `if` block is the final `dec dword ptr [esi+4] ; jne ; call [edx+4]` (0x009ADDEA):
//   it drops the creation reference and deletes the def when no record kept it (the record allocation failed). VC7.1 emits
//   neither the copy constructor's nor the destructor's `if (Object)` test for this local (retail has none either).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_CTOR_SETSTRING
#define FABLE_RETAIL_CSYMBOLMAP
#define FABLE_RETAIL_CSYMBOLMAP_ADDSYMBOLDIRTY
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_POINTER_CTOR_ADOPT
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_INSTANTIATEDEF
#include "fable_string.h"
#include "engine/CDefString.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefinitionManager.h"

bool CDefinitionManager::InstantiateDef(const CCharString& class_name, const CCharString& instantiation_name, bool is_template)
{
    CDefClassInfo* info = GetPDefClassInfo(class_name);
    if (info) {
        long global_index = InstantiatedDefs.size();
        long class_index = info->GlobalIndices.size();
        CDefPointer<CDefClassBase> pdef(info->AllocFunc());
        pdef->Template = is_template;
        if (pdef->IsParentDef())
            pdef->SetInstantiationName(instantiation_name);
        pdef->PDefManager = this;
        pdef->GlobalIndex = global_index;
        CInstantiatedDefInfo* def_info = new CInstantiatedDefInfo(info->Name, instantiation_name, class_index, pdef);
        InstantiatedDefs.push_back(def_info);
        info->GlobalIndices.push_back(global_index);
        PSymbolMap->AddSymbolDirty(instantiation_name, global_index);
        PDefinitionSymbolMap->AddSymbolDirty(info->Name, instantiation_name, global_index);
        return true;
    }
    return false;
}