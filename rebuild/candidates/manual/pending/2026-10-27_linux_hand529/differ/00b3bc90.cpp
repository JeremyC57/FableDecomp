// CEngineResourceManager::SetupEditorAnimation(const CWideString& filename) -- returns the mesh-bank entry for an editor animation file, adding it first if the bank lacks it: the entry symbol is "ANIMATION[<lower-case file name>]", the stored path the file expanded against c:\dev\wishworld\; a new entry gets a fresh mesh-bank state block (data type 6) and is updated at once
// Grade: DIFFER(384v371) -- 2 grades; the call sequence and every callee now match retail's, but VC7.1 lays the six string temporaries out in other frame slots and keeps the entry index in edi (retail: ebx, with edi holding the counted pointer's Info through the new-entry block, so retail pushes ebx). Grade 1 (one full expression for the path) destroyed the base-directory temporary after the conversion; the inline helper fixed that order. Next idea: the counted pointer / state block as the first local of the new-entry block, or the symbol built before the path.
// Leaf: ?SetupEditorAnimation@CEngineResourceManager@@QAEKABVCWideString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B3BC90, 371 bytes, thiscall `ret 4`, returns the bank entry index.
// Identity (proven): Ego_r.pdb ?SetupEditorAnimation@CEngineResourceManager@@QAEKABVCWideString@@@Z (unsigned long from a
//   const CWideString&; donor 0x009DB270); its only caller is CEngine::GetEditorAnimation 0x00B23CB0 (Ego_r
//   ?GetEditorAnimation@CEngine@@UBEPBVC3DAnimationInfo@@ABVCWideString@@@Z, CEngine vtable 0x012A0FC8), which forwards
//   its argument to the resource manager global 0x01436E98. The catalog's "CEngine::GetEditorAnimation" for this row
//   names the caller -- label correction.
// Callees (Ego_r spellings): CWideString(const wchar_t*) 0x0099B6B0, CAFile::ExpandPath(const CWideString&, const
//   CWideString&) 0x00B00820.. at 0x00998C90 (static __fastcall), ~CWideString 0x0099B510, CWideString::ToCharString() const
//   0x0099B2C0, CCharString::GetAsLowerCase() const 0x0099ED10, CCharString(const char*, long) 0x0099EBF0,
//   operator+(const CCharString&, const CCharString&) 0x0099F570, operator+(const CCharString&, const char*) 0x0099F600,
//   ~CCharString 0x0099EAE0, CBankFile::FindIndexBySymbol 0x009CCDC0, AddEntry 0x009CD620, BeginUpdateEntries 0x009CE8D0,
//   UpdateEntry(unsigned long) 0x009CBA80, EndUpdateEntries 0x009CEA70, operator new 0x00BFEA1A,
//   CMeshBankStateBlock::CMeshBankStateBlock 0x00A2D650, CCountedPointer<CMeshBankStateBlock>(CMeshBankStateBlock*)
//   0x00B3E580, operator delete 0x00BFE9BC.
// Layout: CEngineResourceManager MeshBanks[3] at +0x10 (Ego_r; the bank used is MeshBanks[1], +0x14); local models.
void* __cdecl operator new(unsigned int size);             // 0x00BFEA1A
void __cdecl operator delete(void* p);                     // 0x00BFE9BC

class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                        // 0x0099EAE0
    CCharString GetAsLowerCase() const;                    // 0x0099ED10
};
CCharString __fastcall operator+(const CCharString& a, const CCharString& b);   // 0x0099F570
CCharString __fastcall operator+(const CCharString& a, const char* b);          // 0x0099F600

class CWideString {
public:
    void* PStringData;
    CWideString(const unsigned short* text);               // 0x0099B6B0
    ~CWideString();                                        // 0x0099B510
    CCharString ToCharString() const;                      // 0x0099B2C0
};
class CAFile {
public:
    static CWideString __fastcall ExpandPath(const CWideString& path, const CWideString& baseDirectory);   // 0x00998C90
};

class CCPPointerInfo {
public:
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer {
public:
    T*              Data;                                  // +0
    CCPPointerInfo* Info;                                  // +4
    CCountedPointer(T* data);                              // 0x00B3E580 (CMeshBankStateBlock)
    template <class U> CCountedPointer(const CCountedPointer<U>& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer()
    {
        if (Info) {
            if (--Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
};
class CBankStateBlock {
public:
    virtual ~CBankStateBlock();
};
class CMeshBankStateBlock : public CBankStateBlock {
public:
    CMeshBankStateBlock();                                 // 0x00A2D650
    unsigned char _body[0x30];
};
class CBankFile {
public:
    unsigned long FindIndexBySymbol(const CCharString& symbol) const;   // 0x009CCDC0
    void AddEntry(const CCharString& symbol, unsigned long dataType, const CCharString& filename,
                  CCountedPointer<CBankStateBlock> stateBlock);        // 0x009CD620
    void BeginUpdateEntries();                             // 0x009CE8D0
    void UpdateEntry(unsigned long index);                 // 0x009CBA80
    void EndUpdateEntries();                               // 0x009CEA70
};
class CMeshDataBank : public CBankFile {
};

// inline helper (descriptive name: inlined, so absent from the PDB): the named expanded path dies at its return, after
// the conversion, while the base-directory temporary dies with its own statement -- retail's destructor order
static inline CCharString GetEditorAnimationPathname(const CWideString& filename)
{
    CWideString pathname = CAFile::ExpandPath(filename, CWideString(L"c:\\dev\\wishworld\\"));
    return pathname.ToCharString();
}

class CEngineResourceManager {
public:
    virtual ~CEngineResourceManager();
    unsigned long SetupEditorAnimation(const CWideString& filename);
    unsigned char _pad_0x04[0xC];                          // +0x04 CEngineComponent members
    CMeshDataBank* MeshBanks[3];                           // +0x10
    CMeshDataBank* MainMeshBank;                           // +0x1C
};

unsigned long CEngineResourceManager::SetupEditorAnimation(const CWideString& filename)
{
    CMeshDataBank* bank = MeshBanks[1];
    CCharString path = GetEditorAnimationPathname(filename);
    CCharString name = filename.ToCharString().GetAsLowerCase();
    CCharString symbol = CCharString("ANIMATION[") + name + "]";
    unsigned long index = bank->FindIndexBySymbol(symbol);
    if (index == 0) {
        CCountedPointer<CMeshBankStateBlock> stateBlock(new CMeshBankStateBlock);
        bank->AddEntry(symbol, 6, path, stateBlock);
        index = bank->FindIndexBySymbol(symbol);
        bank->BeginUpdateEntries();
        bank->UpdateEntry(index);
        bank->EndUpdateEntries();
    }
    return index;
}
