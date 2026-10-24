// CAFile::ConvertPathToFullPath(const CCharString& path) -- narrow overload: widen the path, expand it against the current path, narrow the result
// Retail: Fable.exe (TLC, VC7.1) @ 0x009990B0, 85 bytes. FableWin.pdb ?ConvertPathToFullPath@CAFile@@SI?AVCCharString@@ABV2@@Z:
// a static `__fastcall` returning CCharString by value (hidden result pointer in ecx, path in edx), plain `ret`.
// Callees: CCharString::ToWideString() const @ 0x0099E480, CAFile::GetCurrentPath() @ 0x00997F80, CAFile::ExpandPath
// @ 0x00998C90 (static `__fastcall`, hidden result in ecx, path in edx, base on the stack), CWideString::ToCharString()
// const @ 0x0099B2C0 (hidden result pushed), ~CWideString @ 0x0099B510 (x3).
// Shape: the wide overload (landed 0x00999110, `return ExpandPath(path, GetCurrentPath());`) is inlined, so its
// current-path temporary dies at the end of the inlined return (before ToCharString); the expanded result dies at the
// end of the return statement and the widened path (a NAMED local, so its address is re-taken with `lea` for the
// ExpandPath argument instead of reusing ToWideString's eax in a callee-saved register) at scope exit.
// Leaf: ?ConvertPathToFullPath@CAFile@@SI?AVCCharString
typedef unsigned short wchar16;          // wchar_t is not a built-in type under the retail flags

struct CWideStringData;
struct CCharStringData;
class CCharString;
class CWideString
{
public:
    CWideStringData* PStringData;                       // +0x00
    ~CWideString();                                     // 0x0099B510
    CCharString ToCharString() const;                   // 0x0099B2C0
};

class CCharString
{
public:
    CCharStringData* PStringData;                       // +0x00
    ~CCharString();                                     // 0x0099EAE0
    CWideString ToWideString() const;                   // 0x0099E480
};

class CAFile
{
public:
    static CWideString __fastcall GetCurrentPath();                                             // 0x00997F80
    static CWideString __fastcall ExpandPath(const CWideString& path, const CWideString& base);  // 0x00998C90
    static CWideString __fastcall ConvertPathToFullPath(const CWideString& path)                // 0x00999110 (inlined here)
    {
        return ExpandPath(path, GetCurrentPath());
    }
    static CCharString __fastcall ConvertPathToFullPath(const CCharString& path);
};

CCharString __fastcall CAFile::ConvertPathToFullPath(const CCharString& path)
{
    CWideString widePath = path.ToWideString();
    return ConvertPathToFullPath(widePath).ToCharString();
}