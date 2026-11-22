#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_STRING_THROW_INLINE
#define private public
#define protected public
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

#include "engine/CEngineInit.h"
#undef protected
#undef private
#include <ctype.h>
#include <stdio.h>
#include <wchar.h>

CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}

CEngineComponent::CEngineComponent() {}
CEngineComponent::~CEngineComponent() {}
void CEngineComponent::FrameStart(const CEngineComponentParams&) {}
void CEngineComponent::FrameEnd(const CEngineComponentParams&) {}
bool CEngineComponent::LoadNewScene(const CEngineComponentLoadNewSceneParams&) { return true; }
void CEngineComponent::Render(const CEngineComponentRenderParams&) {}
void CEngineComponent::PreLayerRender(const CEngineComponentRenderParams&) {}
void CEngineComponent::PostLayerRender(const CEngineComponentRenderParams&) {}
void CEngineComponent::WorldFrameUpdate(bool) {}
void CEngineComponent::PausedWorldFrameUpdate() {}
CCharString CEngineComponent::GetConsoleEnableFunctionName() { return CCharString(); }
void CEngineComponent::OnReleaseDefaultPoolResources() {}
bool CEngineComponent::OnRestoreDefaultPoolResources() { return true; }

CEngineInit::CEngineInit() {}
CEngineInit::~CEngineInit() {}
CEngineMemoryInit::CEngineMemoryInit() {}
CEngineResourceManager::CEngineResourceManager(const CEngineInit&, const CEngineMemoryInit&)
{
    MeshBanks[0] = MeshBanks[1] = MeshBanks[2] = 0;
}
CEngineResourceManager::~CEngineResourceManager() {}
void CEngineResourceManager::ProcessLoading() {}
unsigned long CEngineResourceManager::PeekSceneFilterFlags() const { return 0; }

CCharString::CCharString() : PStringData(0) {}
CCharString::CCharString(const char* text, long length) : PStringData(new CCharStringData)
{
    if (length < 0)
        length = strlen(text);
    PStringData->NoRefs = 1;
    PStringData->Data.PData = new char[length + 1];
    PStringData->Data.StringLength = length;
    PStringData->Data.DataLength = length + 1;
    PStringData->Data.UseFastExtend = false;
    memcpy(PStringData->Data.PData, text, length);
    PStringData->Data.PData[length] = 0;
}
CCharString::CCharString(const CCharString& other) : PStringData(other.PStringData)
{
    if (PStringData)
        ++PStringData->NoRefs;
}
CCharString::~CCharString()
{
    if (PStringData && --PStringData->NoRefs == 0) {
        delete[] PStringData->Data.PData;
        delete PStringData;
    }
}
static const char* NarrowText(const CCharString& text)
{
    return text.PStringData ? text.PStringData->Data.PeekData() : "";
}
CCharString CCharString::GetAsLowerCase() const
{
    const char* source = NarrowText(*this);
    char lower[128];
    const size_t n = strlen(source);
    if (n >= sizeof(lower))
        abort();
    for (size_t i = 0; i < n; ++i)
        lower[i] = (char)tolower((unsigned char)source[i]);
    lower[n] = 0;
    return CCharString(lower);
}
CCharString __fastcall operator+(const CCharString& left, const char* right)
{
    char result[256];
    if (strlen(NarrowText(left)) + strlen(right) >= sizeof(result))
        abort();
    strcpy(result, NarrowText(left));
    strcat(result, right);
    return CCharString(result);
}
CCharString __fastcall operator+(const CCharString& left, const CCharString& right)
{
    return left + NarrowText(right);
}

CWideString::CWideString() : PStringData(0) {}
CWideString::CWideString(const wchar_t* text) : PStringData(new CWideStringData)
{
    const size_t n = wcslen(text);
    PStringData->NoRefs = 1;
    PStringData->Data._M_deallocate_block();
    PStringData->Data._M_allocate_block(n + 1);
    memcpy(PStringData->Data._M_start, text, (n + 1) * sizeof(wchar_t));
    PStringData->Data._M_finish += n;
}
CWideString::CWideString(const CWideString& other) : PStringData(other.PStringData)
{
    if (PStringData)
        ++PStringData->NoRefs;
}
CWideString::~CWideString()
{
    if (PStringData && --PStringData->NoRefs == 0)
        delete PStringData;
}
static const wchar_t* WideText(const CWideString& text)
{
    return text.PStringData ? text.PStringData->Data.c_str() : L"";
}
CCharString CWideString::ToCharString() const
{
    const wchar_t* source = WideText(*this);
    char text[128];
    const size_t n = wcslen(source);
    if (n >= sizeof(text))
        abort();
    for (size_t i = 0; i <= n; ++i)
        text[i] = (char)source[i];
    return CCharString(text);
}

static const CBankFile* EditorBank;
static bool CallsValid;
static bool AlreadyPresent;
static long Phase;
static unsigned int Created;
static unsigned int Destroyed;
static unsigned int Paths;

CWideString __fastcall CAFile::ExpandPath(const CWideString& filename, const CWideString& root)
{
    ++Paths;
    CallsValid = CallsValid && wcscmp(WideText(filename), L"Walk.X") == 0
        && wcscmp(WideText(root), L"c:\\dev\\wishworld\\") == 0;
    return CWideString(L"FullPath/Walk.X");
}
CBankStateBlock::~CBankStateBlock() {}
CMeshBankStateBlock::CMeshBankStateBlock() { ++Created; }
CMeshBankStateBlock::~CMeshBankStateBlock() { ++Destroyed; }
void CMeshBankStateBlock::GetDefault(CBankStateBlock&) {}
unsigned long CMeshBankStateBlock::CreateCRC() { return 0; }
void CMeshBankStateBlock::Set(const CBankStateBlock*) {}

unsigned long CBankFile::FindIndexBySymbol(const CCharString& symbol) const
{
    CallsValid = CallsValid && this == EditorBank && strcmp(NarrowText(symbol), "ANIMATION[walk.x]") == 0;
    if (Phase == 0) {
        Phase = 1;
        return AlreadyPresent ? 19 : 0;
    }
    CallsValid = CallsValid && Phase == 2;
    Phase = 3;
    return 42;
}
void CBankFile::AddEntry(const CCharString& symbol, unsigned long type, const CCharString& path,
                         CCountedPointer<CBankStateBlock> state)
{
    CallsValid = CallsValid && this == EditorBank && Phase == 1 && state.GetReferenceCount() == 2
        && state.Get() != 0 && type == 6 && strcmp(NarrowText(symbol), "ANIMATION[walk.x]") == 0
        && strcmp(NarrowText(path), "FullPath/Walk.X") == 0;
    Phase = 2;
}
void CBankFile::BeginUpdateEntries()
{
    CallsValid = CallsValid && this == EditorBank && Phase == 3;
    Phase = 4;
}
void CBankFile::UpdateEntry(unsigned long entry)
{
    CallsValid = CallsValid && this == EditorBank && Phase == 4 && entry == 42;
    Phase = 5;
}
void CBankFile::EndUpdateEntries()
{
    CallsValid = CallsValid && this == EditorBank && Phase == 5;
    Phase = 6;
}

int main()
{
    CEngineInit init;
    CEngineMemoryInit memory;
    CEngineResourceManager manager(init, memory);
    CMeshDataBank bank;
    manager.MeshBanks[EDITOR_MESH_BANK_NO] = &bank;
    EditorBank = &bank;
    const CWideString filename(L"Walk.X");
    CallsValid = true;
    AlreadyPresent = true;
    if (manager.SetupEditorAnimation(filename) != 19 || !CallsValid || Phase != 1 || Paths != 1 || Created != 0)
        return 1;
    AlreadyPresent = false;
    Phase = 0;
    if (manager.SetupEditorAnimation(filename) != 42 || !CallsValid || Phase != 6 || Paths != 2 || Created != 1 || Destroyed != 1)
        return 2;
    puts("00b3bc90_TEST PASS");
    return 0;
}