// GetMarkerFileName(const CWideString& path) -- the install-marker file name for a path: split it, turn the extension's '.' into '_', and return drive + directory + name + extension + ".mkf"
// Grade: DIFFER(314v315) -- lane gap: custom register convention (LTCG-style): retail takes the hidden CWideString result pointer in esi (caller 0x009C39BE `lea esi,[esp+14h] ; call`), never saves esi and returns it with `ret 4`; VC7.1 without /GL always passes it on the stack
// Leaf: ?GetMarkerFileName@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C33A0, 315 bytes, `ret 4` (path), result in esi. Caller: a CFileInstaller
//   method at 0x009C39C2 (path = CFileInfo +4, the destination path). No Ego_r.pdb name (a file-static helper).
// Callees: CWideString::CWideString() @ 0x0099AED0, CWideString(const wchar_t*) @ 0x0099B6B0, copy @ 0x0099B720,
//   operator= @ 0x0099B7D0, ~CWideString @ 0x0099B510, SplitPath @ 0x00997620 (__fastcall), ReplaceSubstring @ 0x0099C1E0,
//   operator+(const CWideString&, const CWideString&) @ 0x0099BE70, operator+(const CWideString&, const wchar_t*) @ 0x0099BF30.
// Strings: L"_" 0x0122DE7C, L"." 0x0124C4A8, L".mkf" 0x0129B404.
// Shape: five locals built in the order drive, dir, name, ext, result (destroyed in reverse); the L"." temporary reuses
//   the dead `path` argument slot.
typedef unsigned short wchar_t;                    // VC7.1 default /Zc:wchar_t-
struct CWideStringData;
class CWideString {
public:
    CWideStringData* m_pStringData;
    CWideString();                                  // 0x0099AED0
    CWideString(const wchar_t* text);               // 0x0099B6B0
    CWideString(const CWideString& other);          // 0x0099B720
    ~CWideString();                                 // 0x0099B510
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    void ReplaceSubstring(const CWideString& find, const CWideString& replace);   // 0x0099C1E0
};

CWideString __fastcall operator+(const CWideString& left, const CWideString& right);   // 0x0099BE70
CWideString __fastcall operator+(const CWideString& left, const wchar_t* right);       // 0x0099BF30
void __fastcall SplitPath(const CWideString& path, CWideString* drive, CWideString* directory, CWideString* name,
                          CWideString* extension);  // 0x00997620

CWideString GetMarkerFileName(const CWideString& path)
{
    CWideString drive;
    CWideString directory;
    CWideString name;
    CWideString extension;
    CWideString result;
    SplitPath(path, &drive, &directory, &name, &extension);
    extension.ReplaceSubstring(CWideString(L"."), CWideString(L"_"));
    result = drive + directory + name + extension + L".mkf";
    return result;
}
