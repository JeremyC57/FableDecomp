// CWorldMap::GetThingFilenameFromWorldFilename(const CWideString& worldFilename) -- swap a world path's extension for L"gtng": split the path, rebuild it from the drive, directory and name with the new extension
// Leaf: ?GetThingFilenameFromWorldFilename@CWorldMap@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004FB0A0, 166 bytes (padded to 176), plain `ret` (static `__fastcall`: hidden result
//   in ecx, the path in edx). PDB name ?GetThingFilenameFromWorldFilename@CWorldMap@@SI?AVCWideString@@ABV2@@Z (Ego_r.pdb name set,
//   the second of the pair GetThingFilenameFromLevelFilename / GetThingFilenameFromWorldFilename, both in this order).
// Callers: none in retail (no call or absolute reference). The catalog label "GetThingFilenameFromLevelFilename" is
//   propagated from its twin 0x004FAFF0 (same code, other literal); the PDB name set holds exactly this pair, listed
//   level-then-world like the retail twins, so the second (uncalled, L"gtng") body is the world-file variant.
// Callees: CWideString() 0x0099AED0 (manifest "CCharString" is propagated), CWideString(const wchar_t*) 0x0099B6B0,
//   CAFile::SplitPath 0x00997620 (`__fastcall`: path ecx, drive edx, directory/name/extension on the stack),
//   CAFile::MakePath 0x00997780 (`__fastcall`: hidden result ecx, drive edx, directory/name/extension on the stack),
//   copy ctor 0x0099B720, ~CWideString 0x0099B510. String L"gtng" @ 0x012442CC.
// Shape: three empty strings constructed drive, name, directory (the stack slots +0x10/+0x0C/+0x08 and the argument
//   order of both path calls pin that declaration order), the extension, the split with no extension output, the
//   rebuilt path in a named local, then `return path;` copies it into the hidden result (no NRVO) and the five locals
//   die in reverse order.
typedef unsigned short wchar16;                    // wchar_t under the era defaults

class CWideString {
public:
    void* PStringData;
    CWideString();                                 // 0x0099AED0
    CWideString(const wchar16* text);              // 0x0099B6B0
    CWideString(const CWideString& other);         // 0x0099B720
    ~CWideString();                                // 0x0099B510
};

class CAFile {
public:
    static void __fastcall SplitPath(const CWideString& path, CWideString* drive, CWideString* directory, CWideString* name,
                                     CWideString* extension);   // 0x00997620
    static CWideString __fastcall MakePath(const CWideString& drive, const CWideString& directory, const CWideString& name,
                                           const CWideString& extension);   // 0x00997780
};

class CWorldMap {
public:
    static CWideString __fastcall GetThingFilenameFromWorldFilename(const CWideString& worldFilename);
};

CWideString __fastcall CWorldMap::GetThingFilenameFromWorldFilename(const CWideString& worldFilename)
{
    CWideString drive;
    CWideString name;
    CWideString directory;
    CWideString extension((const wchar16*)L"gtng");
    CAFile::SplitPath(worldFilename, &drive, &directory, &name, 0);
    CWideString path = CAFile::MakePath(drive, directory, name, extension);
    return path;
}