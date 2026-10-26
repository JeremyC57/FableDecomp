// CAFile::GetAbsoluteFilePath(const CWideString& path, const CWideString& basePath) -- wide overload: narrow both paths, resolve with the CCharString overload, widen the result
// Retail: Fable.exe (TLC, VC7.1) @ 0x00998C30, 92 bytes. FableWin.pdb ?GetAbsoluteFilePath@CAFile@@SI?AVCWideString@@ABV2@0@Z:
// a static `__fastcall` returning CWideString by value (hidden result pointer in ecx, path in edx, basePath on the
// stack, `ret 4`).
// Callees: CWideString::ToCharString() const @ 0x0099B2C0 (x2, hidden result pushed), CAFile::GetAbsoluteFilePath(const
// CCharString&, const CCharString&) @ 0x00998250 (static `__fastcall`, hidden result in ecx, first argument in edx),
// CCharString::ToWideString() const @ 0x0099E480 (its hidden result is our own result pointer, pushed first),
// ~CCharString @ 0x0099EAE0 (x3).
// Shape: one return expression; VC evaluates the arguments right to left (basePath narrowed first), the narrowed
// result lands in basePath's dead argument slot, and the three narrow temporaries die in reverse construction order.
// Leaf: ?GetAbsoluteFilePath@CAFile@@SI?AVCWideString
typedef unsigned short wchar16;          // wchar_t is not a built-in type under the retail flags

class CWideStringData;
class CCharStringData;
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
    static CCharString __fastcall GetAbsoluteFilePath(const CCharString& path, const CCharString& basePath);   // 0x00998250
    static CWideString __fastcall GetAbsoluteFilePath(const CWideString& path, const CWideString& basePath);
};

CWideString __fastcall CAFile::GetAbsoluteFilePath(const CWideString& path, const CWideString& basePath)
{
    return GetAbsoluteFilePath(path.ToCharString(), basePath.ToCharString()).ToWideString();
}