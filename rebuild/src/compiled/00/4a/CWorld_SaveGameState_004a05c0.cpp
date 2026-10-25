// CWorld::SaveGameState(const CWideString& name, bool questScreens, bool flag) -- save the world to the profile path for name (the hero state as well while between quest screens, else the stale .hs companion is deleted) with the memory pool shut down around it
// Leaf: ?SaveGameState@CWorld@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004A05C0, 448 bytes, `ret 0Ch` (FableWin.pdb ?SaveGameState@CWorld@@QAEXABVCWideString@@_N1@Z).
// Callees: GFGetTime 0x009E1BC0 (__fastcall double, discarded), CWideString(const CWideString&) 0x0099B720, CWideString()
//   0x0099AED0, SplitPath 0x00997620 (__fastcall), wcscmp 0x00BFE94C, MemCmp_Unsigned16 0x00403C60, CUserProfileManager::Get
//   0x0040D2A0 + GetPathNameFromFileName 0x00408120, operator!=(const wchar16*) 0x0099B150, operator= 0x0099B7D0, the engine
//   memory-pool getter (virtual slot 72, +0x120), CHandleBasedMemoryPool::BeginShutdown(float) 0x00A02220 / EndShutdown
//   0x00A02370, CWorld::SaveGameStateInternal 0x0049F4C0, SaveHeroStateInternal 0x004A00E0, operator+(const CWideString&,
//   const wchar16*) 0x0099BF30 (L".hs" 0x01236A74), PathExists 0x00999230, CDriveManager::DeleteFile 0x009E4BC0
//   (g_pDriveManager 0x013CAA28), ~CWideString 0x0099B510. L"" 0x0122D70C.
// Globals: two file-static save flags 0x013B89D0 / 0x013B89D1 (read by SaveGameStateInternal; descriptive names).
typedef unsigned short wchar16;
extern "C" int __cdecl wcscmp(const unsigned short* a, const unsigned short* b);   // 0x00BFE94C
int __cdecl MemCmp_Unsigned16(const unsigned short* lhs, const unsigned short* rhs, unsigned long count);   // 0x00403C60
double __fastcall GFGetTime();                          // 0x009E1BC0

static const unsigned short s_Empty[] = { 0 };          // L"" 0x0122D70C
static const unsigned short s_HeroStateExt[] = { '.', 'h', 's', 0 };   // L".hs" 0x01236A74

struct CWideStringData {
    unsigned short* _M_start;
    unsigned short* _M_finish;
    unsigned short* _M_end_of_storage;
    long            NoRefs;
};
class CWideString {
public:
    CWideStringData* PStringData;
    CWideString();                                      // 0x0099AED0
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    bool operator!=(const wchar16* text) const;         // 0x0099B150
};
CWideString __fastcall operator+(const CWideString& left, const wchar16* right);   // 0x0099BF30
void __fastcall SplitPath(const CWideString& path, CWideString* drive, CWideString* directory, CWideString* name,
                          CWideString* extension);  // 0x00997620
bool __fastcall PathExists(const CWideString& path);   // 0x00999230

class CUserProfileManager {
public:
    static CUserProfileManager& __fastcall Get();       // 0x0040D2A0
    CWideString GetPathNameFromFileName(const CWideString& file, bool b) const;   // 0x00408120
};
class CDriveManager {
public:
    long DeleteFile(const CWideString& path);           // 0x009E4BC0
};
struct CDriveManagerPointer {
    CDriveManager* Object;
    void*          Info;
    CDriveManager* operator->() const { return Object; }
};
extern CDriveManagerPointer g_pDriveManager;            // 0x013CAA28

class CHandleBasedMemoryPool {
public:
    bool BeginShutdown(float timeout);                  // 0x00A02220
    void EndShutdown();                                 // 0x00A02370
};
class CPoolOwner {                                      // descriptive: the object at component->+0x28->+0x2C
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual CHandleBasedMemoryPool* GetMemoryPool();    // slot 72 (+0x120)
};
struct CPoolOwnerHolder { unsigned char _pad[0x2c]; CPoolOwner* Owner; };          // +0x2C
class CMainGameComponent { public: unsigned char _pad[0x28]; CPoolOwnerHolder* Holder; };  // +0x28

extern bool g_SavingQuestScreenState;                   // 0x013B89D0
extern bool g_SavingHeroState;                          // 0x013B89D1

class CWorld {
public:
    void*               __vftable;
    unsigned char       _pad04[4];
    CMainGameComponent* Component;                      // +0x08
    bool SaveGameStateInternal(const CWideString& path, bool flag);   // 0x0049F4C0
    bool SaveHeroStateInternal(const CWideString& name, bool flag);   // 0x004A00E0
    void SaveGameState(const CWideString& name, bool questScreens, bool flag);
};

void CWorld::SaveGameState(const CWideString& name, bool questScreens, bool flag)
{
    GFGetTime();
    CWideString path(name);
    CWideString drive;
    CWideString directory;
    CWideString fileName;
    SplitPath(name, &drive, &directory, &fileName, 0);
    if (directory.PStringData == 0) {
        if (wcscmp(s_Empty, s_Empty) == 0)
            goto empty;
        goto resolved;
    }
    if ((unsigned long)(directory.PStringData->_M_finish - directory.PStringData->_M_start) != 0
        || MemCmp_Unsigned16(directory.PStringData->_M_start, s_Empty, 0) != 0)
        goto resolved;
empty:
    {
        CWideString profilePath = CUserProfileManager::Get().GetPathNameFromFileName(name, true);
        if (profilePath != s_Empty)
            path = profilePath;
    }
resolved:
    CHandleBasedMemoryPool* pool = Component->Holder->Owner->GetMemoryPool();
    bool shutDown = false;
    if (pool)
        shutDown = pool->BeginShutdown(2.0f);
    if (questScreens)
        g_SavingQuestScreenState = true;
    SaveGameStateInternal(path, flag);
    if (questScreens) {
        g_SavingQuestScreenState = false;
        g_SavingHeroState = true;
        SaveHeroStateInternal(name, flag);
    }
    else {
        CWideString heroState = path + s_HeroStateExt;
        if (PathExists(heroState))
            g_pDriveManager->DeleteFile(heroState);
    }
    g_SavingHeroState = false;
    GFGetTime();
    if (shutDown)
        pool->EndShutdown();
}