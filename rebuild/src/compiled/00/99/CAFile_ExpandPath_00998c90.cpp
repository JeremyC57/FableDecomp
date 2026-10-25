// CAFile::ExpandPath(const CWideString& path, const CWideString& base) -- make a path absolute against a base directory: drive-rooted ("X:\") and UNC paths are kept, a root-relative path gets the base's drive letter, ".." paths are resolved, "." prefixes and plain relative paths are appended to the base
// Leaf: ?ExpandPath@CAFile@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00998C90, 474 bytes (+ int3 padding). FableWin.pdb `?ExpandPath@CAFile@@SI?AVCWideString@@ABV2@0@Z`:
//   a static `__fastcall` returning CWideString through the hidden pointer in ecx (path in edx, base on the stack), `ret 4`.
//   Callers: CAFile::IsRoomForFile 0x00999AB0 (this chunk), CAFile::CreateDirectoryForFile 0x00999620 (landed), others.
// Callees: CWideString() 0x0099AED0, CWideString(const CWideString&) 0x0099B720, ~CWideString 0x0099B510, operator=
//   (const CWideString&) 0x0099B7D0, operator+=(const wchar_t*) 0x0099B940, operator+=(wchar_t) 0x0099B9A0, GetLength()
//   0x0099B220, GetAsCharArray() 0x0099AF10, operator const wchar_t*() 0x0099AF00 (the CAFile siblings' binding of the
//   two byte-identical accessors), GetSubString(unsigned long, unsigned long) 0x0099BDB0, the global __fastcall
//   operator+(const CWideString&, const wchar_t*) 0x0099BF30 and operator+(const CWideString&, const CWideString&)
//   0x0099BE70, CAFile::GetAbsoluteFilePath(const CWideString&, const CWideString&) 0x00998C30 (landed). Data: L"\\"
//   0x0122DE78, L":" 0x0122DE70.
// Shape: the base is copied and given a trailing backslash (the last index a NAMED local inside the length test: it
//   takes ebx with a local push/pop, freeing ebp for `base`; the one-condition form spills it); the path's text pointer is read once; the five cases
//   assign a named result that is copied into the return slot (no NRVO in VC7.1); the drive case builds the drive
//   string by appending the base's first character to an empty string.
typedef unsigned short wchar;

class CWideString
{
public:
    void* PStringData;                                              // +0x00
    CWideString();                                                  // 0x0099AED0
    CWideString(const CWideString& other);                          // 0x0099B720
    ~CWideString();                                                 // 0x0099B510
    const CWideString& operator=(const CWideString& other);         // 0x0099B7D0
    const CWideString& operator+=(const wchar* text);               // 0x0099B940
    const CWideString& operator+=(wchar c);                         // 0x0099B9A0
    long GetLength() const;                                         // 0x0099B220
    const wchar* GetAsCharArray() const;                            // 0x0099AF10
    operator const wchar*() const;                                  // 0x0099AF00
    CWideString GetSubString(unsigned long start, unsigned long end) const;   // 0x0099BDB0
};
CWideString __fastcall operator+(const CWideString& left, const wchar* right);        // 0x0099BF30
CWideString __fastcall operator+(const CWideString& left, const CWideString& right);  // 0x0099BE70

class CAFile
{
public:
    static CWideString __fastcall GetAbsoluteFilePath(const CWideString& directory, const CWideString& path);   // 0x00998C30
    static CWideString __fastcall ExpandPath(const CWideString& path, const CWideString& base);
};

CWideString __fastcall CAFile::ExpandPath(const CWideString& path, const CWideString& base)
{
    CWideString result;
    const wchar* text = path.GetAsCharArray();
    CWideString directory(base);
    if (directory.GetLength() > 0) {
        long last = directory.GetLength() - 1;
        if (directory.GetAsCharArray()[last] != L'\\')
            directory += L"\\";
    }
    if ((path.GetLength() >= 3 && text[1] == L':' && text[2] == L'\\') ||
        (path.GetLength() >= 2 && text[0] == L'\\' && text[1] == L'\\')) {
        result = path;
    } else if (path.GetLength() > 0 && text[0] == L'\\') {
        CWideString drive;
        drive += base[0];
        result = drive + L":" + path;
    } else if (path.GetLength() > 2 && text[0] == L'.' && text[1] == L'.') {
        result = GetAbsoluteFilePath(directory, path);
    } else if (path.GetLength() > 1 && text[0] == L'.') {
        result = directory + path.GetSubString(2, path.GetLength());
    } else {
        result = directory + path;
    }
    return result;
}