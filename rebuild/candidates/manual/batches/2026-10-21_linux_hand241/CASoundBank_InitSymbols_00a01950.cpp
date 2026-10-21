// CASoundBank::InitSymbols(const CWideString&) -- build the symbol-file path (drive + directory + name + "."), show the "Sound Bank: Init Symbols" progress line, create the CRC symbol map and fill it: from the binary cache when enabled, else from the text file (optionally writing the binary cache back)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A01950, length 332 bytes (`ret 4`).
// Callees: CWideString() @ 0x0099AED0 (x3), SplitPath @ 0x00997620 (__fastcall: path in ecx, drive in edx, directory / name /
//          extension on the stack), operator+(const CWideString&, const CWideString&) @ 0x0099BE70 (x2) and
//          operator+(const CWideString&, const wchar_t*) @ 0x0099BF30 (both __fastcall, hidden result in ecx), ~CWideString
//          @ 0x0099B510, CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, DisplayProgress @ 0x009E9F40
//          (__fastcall: text in ecx, flag in dl, the float and the second flag on the stack), operator new @ 0x00BFEA1A
//          (0x24-byte CCRCSymbolMap), the CCRCSymbolMap ctor @ 0x00A38500 (manifest "CParticleUpdateParameters" is
//          propagated), CCountedPointer<CCRCSymbolMap>::Reset(T*) @ 0x00A01AA0, CCRCSymbolMap::LoadBinary @ 0x00A38C20,
//          LoadFromFile(const CWideString&, const CCharString&) @ 0x00A39010, SaveBinary @ 0x00A382E0; data: L"." @ 0x012649D4,
//          "Sound Bank: Init Symbols" @ 0x0129C93C, "" @ 0x0122D70E, the two symbol-cache flags @ 0x013BC9F0 (load binary) and
//          @ 0x0138DD3B (save binary).
// Shape: three default-constructed CWideString locals (declared drive, directory, name: the first sits highest), the split
//          with a null extension, the concatenation built directly into the path local (the two intermediate temporaries
//          take the top frame word and the dead `path` argument slot); the two CCharString temporaries are implicit
//          conversions from literals (both homed in the dead argument slot); the map is `new` with the null test around
//          the constructor, handed to the counted pointer, and the loads go through the counted pointer's Data.
// Layout: Ego_r.pdb CASoundBank (12 bytes): vptr, CCountedPointer<CCRCSymbolMap> PSymbolMap +0x04 {Data, Info}.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
struct CWideString {
    void* PStringData;
    CWideString();                                       // 0x0099AED0
    ~CWideString();                                      // 0x0099B510
};
CWideString __fastcall operator+(const CWideString& a, const CWideString& b);   // 0x0099BE70
CWideString __fastcall operator+(const CWideString& a, const unsigned short* b);   // 0x0099BF30 (the lane has no native wchar_t)
void __fastcall SplitPath(const CWideString& path, CWideString* drive, CWideString* directory, CWideString* name,
    CWideString* extension);                             // 0x00997620
void __fastcall DisplayProgress(const CCharString& text, bool flag, float fraction, bool flag2);   // 0x009E9F40
void* __cdecl operator new(unsigned int size);           // 0x00BFEA1A

struct CCRCSymbolMap {                                   // 0x24 bytes
    unsigned char _body[0x24];
    CCRCSymbolMap();                                     // 0x00A38500
    void LoadBinary(const CWideString& path);            // 0x00A38C20
    bool LoadFromFile(const CWideString& path, const CCharString& tag);   // 0x00A39010
    void SaveBinary(const CWideString& path);            // 0x00A382E0
};
struct CCPPointerInfo;
struct CCountedSymbolMap {                               // CCountedPointer<CCRCSymbolMap>
    CCRCSymbolMap*  Data;
    CCPPointerInfo* Info;
    void Reset(CCRCSymbolMap* object);                   // 0x00A01AA0
};

extern bool g_bLoadBinarySoundSymbols;                   // 0x013BC9F0
extern bool g_bSaveBinarySoundSymbols;                   // 0x0138DD3B

struct CASoundBank {
    virtual ~CASoundBank();
    CCountedSymbolMap PSymbolMap;                        // +0x04
    void InitSymbols(const CWideString& path);
};

void CASoundBank::InitSymbols(const CWideString& path)
{
    CWideString drive;
    CWideString directory;
    CWideString name;
    SplitPath(path, &drive, &directory, &name, 0);
    CWideString symbolPath = drive + directory + name + (const unsigned short*)L".";
    DisplayProgress("Sound Bank: Init Symbols", false, -1.0f, false);
    PSymbolMap.Reset(new CCRCSymbolMap);
    if (g_bLoadBinarySoundSymbols) {
        PSymbolMap.Data->LoadBinary(symbolPath);
    } else {
        PSymbolMap.Data->LoadFromFile(path, "");
        if (g_bSaveBinarySoundSymbols)
            PSymbolMap.Data->SaveBinary(symbolPath);
    }
}
