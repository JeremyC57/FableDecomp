// CBankFile::ConvertPathToFilename(const CCharString& path) const -- strip the directory from a path: split it (as a wide string) into name and extension and return name + extension as a narrow string
// Retail: Fable.exe (TLC, VC7.1) @ 0x009CBE10, 121 bytes (+7 int3), __thiscall (`this` unused), ret 8 (hidden result + path).
// Callees: CWideString::CWideString() 0x0099AED0, CCharString::ToWideString() const 0x0099E480 (hidden result),
// SplitPath 0x00997620 (__fastcall: path in ecx, drive in edx, then directory, name, extension on the stack),
// CWideString::~CWideString 0x0099B510, CWideString::operator+= 0x0099B8D0, CWideString::ConvertToCharString()
// const 0x0099B2C0 (hidden result).
// Shape: the two wide-string locals are built first; SplitPath's stack arguments are pushed before its first
// argument is evaluated, so the ToWideString temporary (homed in the dead `path` argument slot) is built
// between the pushes and the call and destroyed right after it; the result is converted straight into the
// caller's hidden return slot (the converted temporary IS the return value), then both locals die.
class CCharString;

class CWideString {
public:
    void* m_pData;
    CWideString();                                          // 0x0099AED0
    CWideString(const CWideString& other);
    ~CWideString();                                         // 0x0099B510
    const CWideString& operator+=(const CWideString& other); // 0x0099B8D0
    CCharString ConvertToCharString() const;                // 0x0099B2C0
};

class CCharString {
public:
    void* m_pData;
    CCharString(const CCharString& other);
    ~CCharString();
    CWideString ToWideString() const;                       // 0x0099E480
};

void __fastcall SplitPath(const CWideString& path, CWideString* drive, CWideString* directory, CWideString* name, CWideString* extension);   // 0x00997620

class CBankFile_Retail {             // CBankFile (named as the landed retail-layout rows; nothing of the layout is used)
public:
    CCharString ConvertPathToFilename(const CCharString& path) const;
};

CCharString CBankFile_Retail::ConvertPathToFilename(const CCharString& path) const
{
    CWideString name;
    CWideString extension;
    SplitPath(path.ToWideString(), 0, 0, &name, &extension);
    name += extension;
    return name.ConvertToCharString();
}
