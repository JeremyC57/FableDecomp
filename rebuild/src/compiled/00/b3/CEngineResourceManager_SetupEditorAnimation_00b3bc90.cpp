// CEngineResourceManager::SetupEditorAnimation(const CWideString&) -- resolves or adds an animation in the editor mesh bank.
// Leaf: ?SetupEditorAnimation@CEngineResourceManager@@QAEKABVCWideString@@@Z
// Flags: default
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_PEEKMESHBANK
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_GETMESHBANK
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_SETUP_EDITOR_ANIMATION
#define FABLE_RETAIL_EMESHBANKNO_ENUMERATORS
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CMESHBANKSTATEBLOCK
#define FABLE_RETAIL_CBANKFILE_ENTRIES
#define FABLE_RETAIL_CBANKFILE_EDITOR_ENTRY_UPDATE
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CAFILE_EXPANDPATH
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_GETASLOWERCASE
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR
#include "engine/CAFile.h"
#include "engine/CMeshDataBank.h"
#include "engine/CMeshBankStateBlock.h"
#include "engine/CEngineResourceManager.h"

inline CWideString __fastcall CEngineResourceManager::ConvertArtResourcePathToFullPath(const CWideString& filename)
{
    return CAFile::ExpandPath(filename, L"c:\\dev\\wishworld\\");
}

unsigned long CEngineResourceManager::SetupEditorAnimation(const CWideString& filename)
{
    CMeshDataBank* editor_mesh_bank = GetMeshBank(EDITOR_MESH_BANK_NO);
    CCharString anim_path = ConvertArtResourcePathToFullPath(filename).ToCharString();
    CCharString lc_filename = filename.ToCharString().GetAsLowerCase();
    CCharString anim_symbol = CCharString("ANIMATION[") + lc_filename + "]";
    unsigned long anim_index = editor_mesh_bank->FindIndexBySymbol(anim_symbol);
    if (anim_index == 0) {
        CCountedPointer<CMeshBankStateBlock> state_block(new CMeshBankStateBlock);
        editor_mesh_bank->AddEntry(anim_symbol, 6, anim_path, state_block);
        anim_index = editor_mesh_bank->FindIndexBySymbol(anim_symbol);
        editor_mesh_bank->BeginUpdateEntries();
        editor_mesh_bank->UpdateEntry(anim_index);
        editor_mesh_bank->EndUpdateEntries();
    }
    return anim_index;
}