// NParticleEngine::CParticleEmitterDatabase::GenerateHeaderFile() -- write the C++ header that enumerates the emitter templates: "enum EParticleEmitter {", one upper-cased "\tNAME = n," line per emitter numbered from 1, then "};"
// Leaf: GenerateHeaderFile
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AE8680, length 386 bytes (padded to 400), `ret`.
// Ego_r.pdb: `?GenerateHeaderFile@CParticleEmitterDatabase@NParticleEngine@@QAEXXZ`.
// Callees: CCharString::ToWideString @ 0x0099E480, CDiskFileWin32 ctor @ 0x0099AD80 and its inlined dtor (vtable 0x0122D06C, Close
//   0x0099A920, ~CWideString 0x0099B510, ~CBase 0x0099A300), CCharString(const char*, long) 0x0099EBF0 / copy 0x0099EC30 / dtor 0x0099EAE0 /
//   operator= 0x0099EFB0 / ToUpper 0x0099EE20 / SaveToFile(CAFile&) 0x0099E5F0, the `__fastcall` string builders operator+(const char*,
//   const CCharString&) 0x0099F690, operator+(const CCharString&, const char*) 0x0099F600, operator+(const CCharString&, const
//   CCharString&) 0x0099F570 and GFIntToCharString(long) 0x0099F830 (hidden results in ecx).
// Strings: "enum EParticleEmitter\r\n{\r\n" @ 0x0129F0F4, ",\r\n" @ 0x0129F0F0, " = " @ 0x0129B6A8, "\t" @ 0x0129B6A4, "};\r\n" @ 0x0129F0E8.
// Layout: PDB CParticleEmitterDatabase (0x58) with smaller retail containers: Emitters (CVectorMap<long, CCountedPointer<CParticleEmitter>>,
//   12-byte entries {key, Data, Info}) begin/end at +0x28/+0x2c, HeaderFilename +0x48; the emitter's name CCharString at +0x140.
// Shape: every write goes through an inline `CAFile& operator<<(CAFile&, const CCharString&)` (the string temporary is built
//   before the file reference is pushed; a direct `CCharString("..").SaveToFile(file)` pushes the file first); the line is one `"\t" + name + " = " + GFIntToCharString(index) + ",\r\n"` expression (VC builds the number first, then the
//   left chain); the end iterator is re-read every pass; the counter is bumped before the line and name die.
class CWideString { public:
    void* PStringData;
    ~CWideString();                                     // 0x0099B510
};
class CAFile;
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    CCharString ToUpper() const;                        // 0x0099EE20
    CWideString ToWideString() const;                   // 0x0099E480
    void SaveToFile(CAFile& file) const;                // 0x0099E5F0
};
CCharString __fastcall operator+(const char* left, const CCharString& right);          // 0x0099F690
CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
CCharString __fastcall GFIntToCharString(long value);                                  // 0x0099F830

namespace N_0099a300 { struct CBase {
    virtual void v00();
    ~CBase();                                           // 0x0099A300
}; } using N_0099a300::CBase;
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };
class CAFile : public CBase { public:
};
class CDiskFileWin32 : public CAFile { public:                        // vtable 0x0122D06C, 0x1c bytes
    virtual void v00();
    virtual void Open(const CWideString& pathname, FileAccessType access, unsigned long flags);   // slot 1
    virtual void Close();                               // 0x0099A920 (slot 2)
    long        AccessType;                             // +0x04
    long        Position;                               // +0x08
    long        Length;                                 // +0x0c
    void*       WinFileHandle;                          // +0x10
    bool        Opened;                                 // +0x14
    CWideString Pathname;                               // +0x18
    CDiskFileWin32(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099AD80
    ~CDiskFileWin32() { Close(); }
};

inline CAFile& operator<<(CAFile& file, const CCharString& text)
{
    text.SaveToFile(file);
    return file;
}

namespace NParticleEngine {

class CParticleEmitter { public:
    unsigned char _pad00[0x140];
    CCharString   Name;                                 // +0x140
};

struct CEmitterEntry {                                  // CVectorMap element: key + CCountedPointer<CParticleEmitter>
    long              Handle;
    CParticleEmitter* Data;
    void*             Info;
};

class CParticleEmitterDatabase {
public:
    unsigned char  _pad00[0x28];
    CEmitterEntry* m_entriesBegin;                     // +0x28
    CEmitterEntry* m_entriesEnd;                       // +0x2c
    unsigned char  _pad30[0x18];
    CCharString    HeaderFilename;                      // +0x48
    void GenerateHeaderFile();
};

void CParticleEmitterDatabase::GenerateHeaderFile()
{
    CDiskFileWin32 file(HeaderFilename.ToWideString(), FILE_ACCESS_WRITE, 2);
    file << CCharString("enum EParticleEmitter\r\n{\r\n");
    long index = 1;
    for (CEmitterEntry* it = m_entriesBegin; it != m_entriesEnd; ++it) {
        CCharString name(it->Data->Name);
        name = name.ToUpper();
        CCharString line = "\t" + name + " = " + GFIntToCharString(index) + ",\r\n";
        file << line;
        ++index;
    }
    file << CCharString("};\r\n");
}

}