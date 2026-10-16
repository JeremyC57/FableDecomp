// ReplacePathExtension(const CWideString& path, const CWideString& extension) -- splits the path into drive, directory and name (dropping its extension) and rebuilds it with the given extension, returned by value
// Leaf: ReplacePathExtension
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00997800, length 122 bytes (padded to 128), `ret 4`; `__fastcall`
// (hidden result in ecx, path in edx, extension on the stack), returns the result pointer.
// Catalog label "CTrophyDef::CTrophyDef" is a wrong propagation: the body sits in the path-helper module between
// MakePath (0x00997780) and CDriveManager::GetItemFromPath (0x00997880) and only works on wide strings. The real
// name is not known; "ReplacePathExtension" is descriptive.
// Callees: CWideString::CWideString() @ 0x0099AED0 (x3), SplitPath @ 0x00997620 (__fastcall: path in ecx, drive in
// edx, then directory, name, extension; extension 0 = not wanted), MakePath @ 0x00997780 (__fastcall, hidden result
// in ecx, drive in edx, directory/name/extension on the stack), CWideString::~CWideString @ 0x0099B510 (x3).
// Shape: the three locals in declaration order (drive, directory, name), MakePath's result built straight into the
// hidden return slot.
class CWideString {
public:
    void* m_pData;
    CWideString();                                 // 0x0099AED0
    CWideString(const CWideString& other);
    ~CWideString();                                // 0x0099B510
};
void __fastcall SplitPath(const CWideString& path, CWideString* drive, CWideString* directory, CWideString* name, CWideString* extension);   // 0x00997620
CWideString __fastcall MakePath(const CWideString& drive, const CWideString& directory, const CWideString& name, const CWideString& extension);   // 0x00997780
CWideString __fastcall ReplacePathExtension(const CWideString& path, const CWideString& extension)
{
    CWideString drive;
    CWideString directory;
    CWideString name;
    SplitPath(path, &drive, &directory, &name, 0);
    return MakePath(drive, directory, name, extension);
}
