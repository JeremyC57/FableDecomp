// CCachedFile::~CCachedFile() -- close the file, then destroy PDiskFile, Pathname and DataBuffer and run the base destructor (tail jump to ~CBaseClassNonCopyable)
// Leaf: ??1CCachedFile@@UAE@XZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00826380, 95 bytes (int3 padding 0x008263DF): push esi ; mov esi,ecx ; the CCachedFile vtable
//   store 0x012724D4 ; call Close 0x00A5AFA0 (direct: a virtual called by name inside a destructor binds statically) ; the inline
//   ~CCountedPointer on PDiskFile +0x1C (Info +0x20) ; `lea ecx,[esi+18h]` ; Data = 0, Info = 0 ; call ~CWideString 0x0099B510
//   (Pathname) ; the inline STLport ~vector<unsigned char> on DataBuffer +0x0C (`if (_M_start) free(_M_start)`) ; mov ecx,esi ;
//   pop esi ; jmp ~CBaseClassNonCopyable 0x0099A300 (CAFile's own destructor is compiler-generated and stores nothing).
// Identity (proven, two sources): (1) the vtable stored first, 0x012724D4, is .?AVCCachedFile@@ (rtti-vtables.tsv; RTTI CCachedFile :
//   CAFile : CBaseClassNonCopyable : CBaseClass); its slot 0 is the deleting dtor 0x00826420 (one of the two callers; the other is
//   0x00A7AFF3); slot 2 0x00A5AFA0 is the Close this calls; (2) Ego_r.pdb ??1CCachedFile@@UAE@XZ (0x0067FA30, beside the inline IsOpen /
//   GetPathname / ??_G -- retail's slot 10 / 11 / 0 bodies 0x008263F0 / 0x00826400 / 0x00826420 follow this row) and the Ego_r member
//   list. The catalog label "CChunkedFile::~CChunkedFile" is wrong (CChunkedFile is a different class: RTTI CChunkedFile :
//   CBaseClassNonCopyable, no CAFile base) -- NAME_FIXES.tsv.
// Callees: CCachedFile::Close 0x00A5AFA0 (?Close@CCachedFile@@UAEXXZ, Ego_r), the count block's DeleteFunc, operator delete 0x00BFE9BC
//   (??3@YAXPAX@Z), CWideString::~CWideString 0x0099B510 (??1CWideString@@QAE@XZ), free 0x00BFEA14 (allocator<unsigned char>::deallocate),
//   CBaseClassNonCopyable::~CBaseClassNonCopyable 0x0099A300 (??1CBaseClassNonCopyable@@MAE@XZ, tail jump).
// Layout: Ego_r CCachedFile members at retail offsets (12-byte STLport vector): Position +0x04, BufferStartPosition +0x08, DataBuffer
//   (CArray<unsigned char>) +0x0C, Pathname (CWideString) +0x18, PDiskFile (CCountedPointer<CAFile>) +0x1C, Opened +0x24 (slot 10
//   IsOpen 0x008263F0 reads it), BufferValid +0x25; sizeof 0x28. Position +0x04 is what slot 7 (GetPosition 0x008263E0) returns.
//   CAFile's vtable 0x0122ECCC: slot 0 ??_G 0x004192B0 (calls ~CBaseClassNonCopyable directly), slots 1-11 and 14 _purecall, slot 12
//   IsIOFinished 0x00401650 (`mov al,1`), 13 GetAlignment 0x00401660 (`mov eax,1`), 15 GetFileInformation 0x00412090 (Ego_r/FableWin
//   CAFile U names); the pure slots are named from CCachedFile's overrides (Ego_r names, donor order = retail order: Close 0x00A5AFA0,
//   Write 4, SetPosition 5, MovePosition 6, SetLength 8, GetLength 9, Open 1, Read 3, Clone 14). No engine header exists for CAFile /
//   CCachedFile; CBaseClassNonCopyable is engine/CBaseClassNonCopyable.h, CWideString fable_string.h, CCountedPointer
//   engine/CCountedPointer.h, the vector STLport 4.6 (_Vector_base + _STLP_alloc_proxy, as the landed 0x00B18780).
// Shape: the user destructor body is `Close();`; the member and base destructors follow implicitly in reverse declaration order.
extern "C" void __cdecl free(void* block);                      // 0x00BFEA14
#include "engine/CArray.h"

#include "fable_string.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CCountedPointer.h"
class CASuspendableProcess;
class CFileInformation;
enum FileAccessType { };

class CAFile : public CBaseClassNonCopyable {           // vtable 0x0122ECCC
public:
    virtual void Open(const CWideString& pathname, FileAccessType access, unsigned long flags) = 0;   // slot 1
    virtual void Close() = 0;                                                                         // slot 2
    virtual void Read(void* buffer, long size, CASuspendableProcess* process) = 0;                    // slot 3
    virtual void Write(const void* buffer, long size, CASuspendableProcess* process) = 0;             // slot 4
    virtual void SetPosition(long position) = 0;                                                      // slot 5
    virtual void MovePosition(long offset) = 0;                                                       // slot 6
    virtual long GetPosition() const = 0;                                                             // slot 7
    virtual void SetLength(long length) = 0;                                                          // slot 8
    virtual long GetLength() const = 0;                                                               // slot 9
    virtual bool IsOpen() const = 0;                                                                  // slot 10
    virtual CWideString GetPathname() const = 0;                                                      // slot 11
    virtual bool IsIOFinished() const;                                                                // slot 12 0x00401650
    virtual long GetAlignment() const;                                                                // slot 13 0x00401660
    virtual CAFile* Clone() const = 0;                                                                // slot 14
    virtual CFileInformation GetFileInformation();                                                    // slot 15 0x00412090
};

class CCachedFile : public CAFile {                     // vtable 0x012724D4; retail sizeof 0x28
public:
    virtual ~CCachedFile();
    virtual void Open(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x00A5B260
    virtual void Close();                                                                         // 0x00A5AFA0
    virtual void Read(void* buffer, long size, CASuspendableProcess* process);                    // 0x00A5B370
    virtual void Write(const void* buffer, long size, CASuspendableProcess* process);             // 0x00A5AFB0
    virtual void SetPosition(long position);                                                      // 0x00A5B020
    virtual void MovePosition(long offset);                                                       // 0x00A5B0A0
    virtual long GetPosition() const;                                                             // 0x008263E0
    virtual void SetLength(long length);                                                          // 0x00A5B190
    virtual long GetLength() const;                                                               // 0x00A5B200
    virtual bool IsOpen() const;                                                                  // 0x008263F0
    virtual CWideString GetPathname() const;                                                      // 0x00826400
    virtual CAFile* Clone() const;                                                                // 0x00A5B710
    long                    Position;                   // +0x04
    long                    BufferStartPosition;        // +0x08
    CArray<unsigned char>   DataBuffer;                 // +0x0C
    CWideString             Pathname;                   // +0x18
    CCountedPointer<CAFile> PDiskFile;                  // +0x1C
    bool                    Opened;                     // +0x24
    bool                    BufferValid;                // +0x25
};
FABLE_STATIC_ASSERT(sizeof(CCachedFile) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CCachedFile, Pathname) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CCachedFile, PDiskFile) == 0x1C);

CCachedFile::~CCachedFile()
{
    Close();
}