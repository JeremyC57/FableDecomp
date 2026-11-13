#ifndef FABLE_ENGINE_CCACHEDFILE_H
#define FABLE_ENGINE_CCACHEDFILE_H
// CCachedFile (chunk 2054 overlay; the repo copy is quarantined): the polymorphic retail class. RTTI .?AVCCachedFile@@ : CAFile :
// CBaseClassNonCopyable, vtable 0x012724D4, sizeof 0x28. Members: Ego_r.pdb names at retail offsets (12-byte STLport CArray):
// Position +0x04, BufferStartPosition +0x08, DataBuffer +0x0C, Pathname +0x18, PDiskFile +0x1C, Opened +0x24, BufferValid +0x25
// (the landed ~CCachedFile 0x00826380 and ctor 0x00A5B590 models). Methods: Ego_r.pdb / FableWin.pdb spellings (??0CCachedFile@@QAE@J@Z
// 0x00A5B590, ?OpenFromFile@CCachedFile@@QAEXAAVCAFile@@PAVCASuspendableProcess@@@Z 0x00A5B210, ?Read@CCachedFile@@UAEXPAXJPAV
// CASuspendableProcess@@@Z 0x00A5B370, ?Close@CCachedFile@@UAEXXZ 0x00A5AFA0). The destructor is inline (Ego_r ??1CCachedFile@@UAE@XZ is
// a COMDAT beside the inline IsOpen / GetPathname; retail's out-of-line copy 0x00826380 is `Close();` then the members): CNavQuadTree::
// LoadFromFile 0x00A7AF60 calls that copy on its early exit (0x00A7AFF3) and expands it at its final exit (0x00A7B8A9..0x00A7B917).
#ifndef FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CAFILE
#endif
#include "engine/CAFile.h"
#include "engine/CArray.h"
#include "engine/CCountedPointer.h"
class CCachedFile : public CAFile {
public:
    CCachedFile(long bufferSize);                                                                     // 0x00A5B590
    virtual ~CCachedFile() { Close(); }                                                               // slot 0 (out-of-line copy 0x00826380)
    void OpenFromFile(CAFile& file, CASuspendableProcess* psuspendable);                               // 0x00A5B210
    virtual void Open(const CWideString& pathname, FileAccessType access, unsigned long flags);       // 0x00A5B260
    virtual void Close();                                                                             // 0x00A5AFA0
    virtual void Read(void* buffer, long size, CASuspendableProcess* psuspendable);                   // 0x00A5B370
    virtual void Write(const void* buffer, long size, CASuspendableProcess* psuspendable);            // 0x00A5AFB0
    virtual void SetPosition(long position);                                                          // 0x00A5B020
    virtual void MovePosition(long offset);                                                           // 0x00A5B0A0
    virtual long GetPosition() const;                                                                 // 0x008263E0
    virtual void SetLength(long length);                                                              // 0x00A5B190
    virtual long GetLength() const;                                                                   // 0x00A5B200
    virtual bool IsOpen() const;                                                                      // 0x008263F0
    virtual CWideString GetPathname() const;                                                          // 0x00826400
    virtual CAFile* Clone() const;                                                                    // 0x00A5B710
    long                    Position;                   // +0x04
    long                    BufferStartPosition;        // +0x08
    CArray<unsigned char>   DataBuffer;                 // +0x0C
    CWideString             Pathname;                   // +0x18
    CCountedPointer<CAFile> PDiskFile;                  // +0x1C
    bool                    Opened;                     // +0x24
    bool                    BufferValid;                // +0x25
};
FABLE_STATIC_ASSERT(sizeof(CCachedFile) == 0x28);
#endif // FABLE_ENGINE_CCACHEDFILE_H
