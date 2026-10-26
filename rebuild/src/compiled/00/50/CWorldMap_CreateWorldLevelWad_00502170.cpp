// CWorldMap::CreateWorldLevelWad(const CWideString& worldFile, const CWideString& wadFile) -- development build step:
// when both WAD-building switches are on, read the level list from the world file, open the WAD bank for writing and
// add every real level (from index 1) and its thing file to it, then close the bank
// Leaf: CreateWorldLevelWad
// Retail: Fable.exe (TLC, VC7.1) @ 0x00502170, 383 bytes, `ret 8`. PDB ?CreateWorldLevelWad@CWorldMap@@QAEXABVCW...
// Callees: CWorldMap::GetLevelFilesFromWorldFile(const CWideString&, vector<CWideString>&) 0x00501D30,
//   CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0, the __fastcall DisplayProgress(const CCharString&, float,
//   bool, bool) 0x009E9F40, CBankFileAsync ctor 0x009D5F80 (CWADBank vtable 0x012442DC stored inline after it, as
//   the landed CWorldMap::SetAsUsingWAD 0x004FDAB0), the default bank block size 0x00997B30 (`mov eax,800h`),
//   CBankFileAsync::OpenFileReadWrite 0x009D5200, CBankFile::BeginUpdateEntries 0x009CE8D0 / EndUpdateEntries
//   0x009CEA70, CWideString copy ctor 0x0099B720 / ~ 0x0099B510, the bank's add-file update 0x00A3A410, the
//   __fastcall GetThingFilenameFromLevelFilename 0x004FAFF0, CBankFileAsync::Close 0x009D58A0, ~CBankFileAsync
//   0x009D6490, free 0x00BFEA14 (vector storage).
// Layout: no CWorldMap members are touched (`this` only forwarded to GetLevelFilesFromWorldFile); the two switches
//   are bytes 0x01375459 / 0x0137545A; the CWADBank local is 0x164 bytes.
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CWideString { public:
    void* PStringData;
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
};
void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b);   // 0x009E9F40
CWideString __fastcall GetThingFilenameFromLevelFilename(const CWideString& levelFile);    // 0x004FAFF0
unsigned long GetDefaultBankBlockSize();                // 0x00997B30
extern bool g_WADBuildEnabled;                          // 0x01375459
extern bool g_WADBuildLevels;                           // 0x0137545A
template <class T> struct vector {                      // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector()
    {
        for (T* p = _M_start; p != _M_finish; ++p)
            p->~T();
        if (_M_start)
            free(_M_start);
    }
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long n) { return _M_start[n]; }
};
class CBankFileAsync {
public:
    CBankFileAsync();                                   // 0x009D5F80
    virtual ~CBankFileAsync();                          // 0x009D6490
    void OpenFileReadWrite(const CWideString& name, unsigned long flags, unsigned long blockSize);   // 0x009D5200
    void BeginUpdateEntries();                          // 0x009CE8D0
    void EndUpdateEntries();                            // 0x009CEA70
    void Close();                                       // 0x009D58A0
    void UpdateEntryFromFile(const CWideString& fileName);   // 0x00A3A410
    unsigned char _body[0x164 - 4];
};
class CWADBank : public CBankFileAsync {                // vtable 0x012442DC
public:
    CWADBank() { }
};
class CWorldMap { public:
    void GetLevelFilesFromWorldFile(const CWideString& worldFile, vector<CWideString>& levelFiles);   // 0x00501D30
    void CreateWorldLevelWad(const CWideString& worldFile, const CWideString& wadFile);
};
CWideString* ReadLevelFileList(const CWideString& worldFile, CWideString* first, CWideString* last);
// Same-TU stand-in for GetLevelFilesFromWorldFile 0x00501D30 (retail's LTCG knowledge): with its body visible VC7.1
// knows the level-file vector's address does not escape, so begin/end stay in registers across the later calls.
#pragma auto_inline(off)
void CWorldMap::GetLevelFilesFromWorldFile(const CWideString& worldFile, vector<CWideString>& levelFiles)
{
    levelFiles._M_finish = ReadLevelFileList(worldFile, levelFiles._M_start, levelFiles._M_end_of_storage);
}
#pragma auto_inline(on)
void CWorldMap::CreateWorldLevelWad(const CWideString& worldFile, const CWideString& wadFile)
{
    if (!g_WADBuildEnabled || !g_WADBuildLevels)
        return;
    vector<CWideString> levelFiles;
    GetLevelFilesFromWorldFile(worldFile, levelFiles);
    DisplayProgress("Opening WAD bank", -1.0f, false, false);
    CWADBank bank;
    bank.OpenFileReadWrite(wadFile, 0, GetDefaultBankBlockSize());
    bank.BeginUpdateEntries();
    unsigned long count = levelFiles.size();
    for (unsigned long i = 1; i < count; ++i) {
        DisplayProgress("Updating WAD bank entries", -1.0f, false, false);
        CWideString levelFile(levelFiles[i]);
        bank.UpdateEntryFromFile(levelFile);
        CWideString thingFile = GetThingFilenameFromLevelFilename(levelFiles[i]);
        bank.UpdateEntryFromFile(thingFile);
    }
    bank.EndUpdateEntries();
    bank.Close();
}