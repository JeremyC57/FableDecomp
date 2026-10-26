// CBankFile::ReadRecoveryFile(unsigned long validation) -- reload the entry list from the bank's recovery file: false when the file
//   is missing, its "BREC" signature does not match or its validation word differs from `validation`; otherwise read the two
//   entry-list words and hand the stream to ReadEntryList
// Leaf: ?ReadRecoveryFile@CBankFile@@
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x009D0380, 679 bytes, ret 4; PDB ?ReadRecoveryFile@CBankFile@@AAE_NK@Z. Flags: /GS (cookie
//   0x0139C8A8, check 0x00BFE9F9; char signature[5] from "BREC" 0x0129B59C, as the landed WriteRecoveryFile 0x009CDF50 writes it).
// Callees: CBankFile::GetRecoveryFilePath 0x009CBFE0 (PDB AAE?AVCWideString@@XZ), CAFile::PathExists 0x00999230 (static
//   __fastcall, PDB ?PathExists@CAFile@@SI_NABVCWideString@@@Z), CBaseClassNonCopyable ctor 0x0099A2F0 / dtor 0x0099A300,
//   CDiskFileWin32 (vtable 0x0122D06C) Open 0x0099A6A0 / Close 0x0099A920, CWideString default ctor 0x0099AED0 / dtor 0x0099B510,
//   CFileDataInputStream ctor 0x00994700 / dtor 0x00994780, CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (inline Read,
//   as the landed 0x00B16F60), CBankFile::ReadEntryList 0x009CFBC0 (PDB AAE_NAAVCFileDataInputStream@@KK@Z, result dropped).
// Layout: CDiskFileWin32 / CFileDataInputStream as Ego_r.pdb (engine/CDiskFileWin32.h, engine/CFileDataInputStream.h: 0x24
//   bytes) restated locally because the inline Read and the inline file ctor/dtor need bodies; CDataInputStream member names
//   from engine/CDataInputStream.h. No CBankFile member is touched.
// Shape: the body is one `if (PathExists)` with the failure return after it (retail places that block last); the signature and
//   validation checks sit in their own blocks so the two trailing words reuse the signature's frame slot and the dead
//   validation argument slot, as retail.
extern "C" int __cdecl memcmp(const void*, const void*, unsigned int);
extern "C" void* __cdecl memcpy(void*, const void*, unsigned int);
#pragma intrinsic(memcmp, memcpy)
class CWideString { public:
    void* PStringData;
    CWideString();                                     // 0x0099AED0
    ~CWideString();                                    // 0x0099B510
};
class CAFile { public:
    static bool __fastcall PathExists(const CWideString& path);   // 0x00999230
};
class CASuspendableProcess;
struct CBaseClassNonCopyable {
    CBaseClassNonCopyable();                           // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();                  // 0x0099A300
};
enum FileAccessType { FILE_ACCESS_READ = 1 };
class CDiskFileWin32 : public CBaseClassNonCopyable { public:       // vtable 0x0122D06C
    FileAccessType AccessType;                         // +0x04
    long Position;                                     // +0x08
    long Length;                                       // +0x0c
    void* WinFileHandle;                               // +0x10
    bool Opened;                                       // +0x14
    CWideString Pathname;                              // +0x18
    CDiskFileWin32() : CBaseClassNonCopyable(), Pathname() { Opened = false; }
    virtual ~CDiskFileWin32() { Close(); }
    virtual void Open(const CWideString&, FileAccessType, unsigned long);   // slot 1, 0x0099A6A0
    virtual void Close();                                                   // slot 2, 0x0099A920
};
class CDataInputStream { public:
    void* __vftable;
    unsigned long StreamPos;                            // +0x04
    long StreamSize;                                    // +0x08
    const unsigned char* CurrentSourcePtr;              // +0x0c
    unsigned long SourceChunkStreamPos;                 // +0x10
    long SourceChunkBytesRemaining;                     // +0x14
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 p) { return p >= 0 && p <= 0x7fffffff; }
    __forceinline void Read(void* dst, long size) {
        __int64 np = (__int64)(StreamPos + size);
        if (!IsValidPosition(np)) return;
        if (SourceChunkBytesRemaining >= size) {
            memcpy(dst, CurrentSourcePtr, size); CurrentSourcePtr += size; SourceChunkBytesRemaining -= size; StreamPos += size;
        } else ReadWithSrcChunkOverflow((unsigned char*)dst, size);
    }
};
class CFileDataInputStream : public CDataInputStream { public:
    CAFile* File;                                       // +0x18
    void* Buffer;                                       // +0x1c
    long BufferSize;                                    // +0x20
    CFileDataInputStream(CAFile* file, long bufferSize);   // 0x00994700
    ~CFileDataInputStream();                              // 0x00994780
};
struct CBankFile_Retail {
    virtual void s0();
    CWideString GetRecoveryFilePath();                 // 0x009CBFE0
    bool ReadEntryList(CFileDataInputStream& stream, unsigned long a, unsigned long b);   // 0x009CFBC0
    bool ReadRecoveryFile(unsigned long validation);
};

bool CBankFile_Retail::ReadRecoveryFile(unsigned long validation)
{
    CWideString path = GetRecoveryFilePath();
    if (CAFile::PathExists(path)) {
        CDiskFileWin32 file;
        file.Open(path, FILE_ACCESS_READ, 4);
        CFileDataInputStream stream((CAFile*)&file, 0x4000);
        unsigned long header;
        {
            char signature[] = "BREC";
            stream.Read(&header, 4);
            if (memcmp(&header, signature, 4) != 0)
                return false;
        }
        {
            unsigned long value;
            stream.Read(&value, 4);
            if (value != validation)
                return false;
        }
        {
            unsigned long first;
            stream.Read(&first, 4);
            unsigned long second;
            stream.Read(&second, 4);
            ReadEntryList(stream, first, second);
        }
        return true;
    }
    return false;
}