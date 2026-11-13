#ifndef FABLE_ENGINE_CFILEINSTALLER_H
#define FABLE_ENGINE_CFILEINSTALLER_H
// Layout: both original PDB full types; retail containers yield owner0xa0, activefile0xb0.
// Retail-volatile: original Ego_r/FableWin modifier cv_flags2 and pointer attributes820a.
// Completion callback009c42f0 writes PCompletedRead; worker009c5748/4d tests then rereads it.
// Thread/decompression completion, ExitThread, Running, Paused and Completed are original cross-thread members.
#include "fable_string.h"
#include "engine/Win32Types.h"
#include "engine/CArray.h"
#include "engine/CTBaseSingleton.h"
#include "engine/CCountedPointer.h"
#include "engine/CCompressorZlib.h"
#include "stlport/fable_map.h"
// Platform SDK winbase.h original asynchronous I/O record.
#ifndef FABLE_RETAIL_OVERLAPPED_DECLARED
#define FABLE_RETAIL_OVERLAPPED_DECLARED
struct _OVERLAPPED {
 unsigned long Internal,InternalHigh;
 union {struct {unsigned long Offset,OffsetHigh;};void* Pointer;};
 void* hEvent;
};
#endif
class CFileInstaller:public CTBaseSingleton<CFileInstaller> {
public: class CFileInfo;class CBufferChunk;class CPendingWrite;class CActiveRead;class CActiveFile;class CActiveWrite;
class CFileInfo {
public:
 CWideString SrcPath;
 CWideString DstPath;
 long InputSize;
 bool Started;
 bool Decompress;
 bool CreateMarkerFile;
 CFileInfo(const CFileInfo&);
#ifdef FABLE_RETAIL_CFILEINSTALLER_CFILEINFO_CTOR
 // Opt-in (chunk 2032, row 0x009C47A0 CFileInstaller::AddFile): FableWin.pdb ??0CFileInfo@CFileInstaller@@QAE@ABVCWideString@@0J_N1@Z,
 // inline in retail: AddFile expands it after operator new(0x10) inside the wildcard scan loop (0x009C4A86..0x009C4AB5: both copy
 // constructors 0x0099B720, then InputSize +8, Started = false +0xC, Decompress +0xD, CreateMarkerFile +0xE) and calls its COMDAT
 // 0x009C3130 (landed, the same stores, `ret 14h`) at the two single-file sites (0x009C4C90, 0x009C4DA1).
 CFileInfo(const CWideString& src_path,const CWideString& dst_path,long input_size,bool decompress,bool create_marker_file)
  : SrcPath(src_path),DstPath(dst_path),InputSize(input_size),Started(false),Decompress(decompress),CreateMarkerFile(create_marker_file) { }
#else
 CFileInfo(const CWideString&,const CWideString&,long,bool,bool);
#endif
 CFileInfo();
 ~CFileInfo();
 CFileInfo& operator=(const CFileInfo&);
};

class CBufferChunk {
public:
 unsigned char * Mem;
 bool Used;
 bool IsInput;
 CBufferChunk();
 ~CBufferChunk();
};

class CPendingWrite {
public:
 CBufferChunk* PBufferChunk;
 unsigned long FilePos;
 unsigned long WriteLength;
 CPendingWrite(unsigned long,unsigned long,CBufferChunk*);
 CPendingWrite();
};

class CActiveRead {
public:
 CActiveFile* PActiveFile;
 long FilePos;
 long TransferSize;
 CBufferChunk* PBufferChunk;
 _OVERLAPPED OverlappedInfo;
 CFileInstaller* PFileInstaller;
 CActiveRead(CFileInstaller*,CActiveFile&,CBufferChunk*,long);
};

class CActiveFile {
public:
 void * SrcFileHandle;
 void * DstFileHandle;
 unsigned long CurrentSrcOffset;
 unsigned long NoBytesRead;
 unsigned long NoBytesWritten;
 unsigned long InputSize;
 unsigned long OutputSize;
 CCountedPointer<CFileInfo const > PFileInfo;
 unsigned long MinWriteSize;
 unsigned long MinReadSize;
 double StartTime;
 CCompressorZlib Decompressor;
 std::list<CPendingWrite,std::allocator<CPendingWrite> > PendingWrites;
 std::list<CActiveRead *,std::allocator<CActiveRead *> > PendingDecompression;
 unsigned long DecompressSrcPos;
 unsigned long DecompressDstPos;
 CBufferChunk* PDecompressSrcBuffer;
 CBufferChunk* PDecompressDstBuffer;
 unsigned long DecompressDstUsed;
 bool DecompressFinish;
 CActiveFile(const CActiveFile&);
 CActiveFile();
 CActiveFile(CCountedPointer<CFileInfo const >&);
 bool IsReadFinished()const;
 bool IsWriteFinished()const;
 void OnReadFinished();
 void OnWriteFinished();
 ~CActiveFile();
 CActiveFile& operator=(const CActiveFile&);
};

class CActiveWrite {
public:
 CActiveFile* PActiveFile;
 CBufferChunk* PBufferChunk;
 _OVERLAPPED OverlappedInfo;
 CFileInstaller* PFileInstaller;
 CActiveWrite(CFileInstaller*,CActiveFile&,CBufferChunk*);
};

public:
 CFileInstaller(const CFileInstaller&);
 CFileInstaller();
 virtual ~CFileInstaller();
 void AddFile(const CCharString&,const CCharString&,const CArray<CCharString>*,bool);
 void SetBuffers(unsigned long,unsigned long);
 void ClearBuffers();
 static void FABLE_FASTCALL ProcessDirtyDiscError();
 void Pause();
 void Unpause();
 bool IsPaused()const;
 bool IsComplete()const;
 bool IsFileCompleted(const CCharString&)const;
 float GetPercentComplete()const;
 void SetIgnoreTimeStampWhenCopying(bool);
 void ResetTotalNumberOfBytesWritten();
protected:
 static void FABLE_CDECL BeginThread(void *);
 void ThreadMain();
 static void FABLE_CDECL BeginDecompressionThread(void *);
 void DecompressionThreadMain();
 void InitBuffers(unsigned long,unsigned long);
 CBufferChunk* AllocateBuffer(bool);
 void FreeBuffer(CBufferChunk*);
 void StartReadingFile(CActiveFile&);
 void StartWritingFile(CActiveFile&,unsigned long);
 long GetMinTransferSize(const CWideString&)const;
 bool IsFileCopyNecessary(CCountedPointer<CFileInfo const >&)const;
 bool IsCompressedFilename(const CWideString&,CWideString&)const;
 static void FABLE_FASTCALL SetDirtyDiscError(bool);
 static void FABLE_STDCALL OnFileReadCompletion(unsigned long,unsigned long,_OVERLAPPED*);
 static void FABLE_STDCALL OnFileWriteCompletion(unsigned long,unsigned long,_OVERLAPPED*);
 void ProcessFileReadCompletion();
 void PostFileReadCompletion(CActiveRead*);
 void ProcessActiveFileForDecompression(CActiveFile&);
 void ProcessActiveFileForWrite(CActiveFile&);
 void ProcessActiveFileForRead(CActiveFile&);
 void ProcessWrites();
 void ProcessReads();
 void ProcessAlways();
 _RTL_CRITICAL_SECTION CriticalSection;
 CArray<CBufferChunk> BufferChunks;
 unsigned char * PReadBuffer;
 unsigned char * PWriteBuffer;
 long AvailableBuffers;
 long AllocatedInputBuffers;
 std::list<CCountedPointer<CFileInfo const >,std::allocator<CCountedPointer<CFileInfo const > > > FilesToCopy;
 std::list<CCountedPointer<CFileInfo const >,std::allocator<CCountedPointer<CFileInfo const > > > FilesToFinish;
 std::list<CActiveFile,std::allocator<CActiveFile> > ActiveFiles;
 std::set<CCharString,std::less<CCharString>,std::allocator<CCharString> > CompletedFiles;
 CActiveFile* PCurrentReadFile;
 CActiveFile* PCurrentWriteFile;
 long NoActiveReads;
 long NoActiveWrites;
 bool WriteMode;
 unsigned long BufferChunkSize;
 unsigned long NoBufferChunks;
 long NoTotalBytesWritten;
 long TotalSizeOfAllFiles;
 CActiveRead* volatile PCompletedRead;
 volatile bool ThreadFinished;
 volatile bool DecompressionThreadFinished;
 volatile bool ExitThread;
 volatile bool Running;
 volatile unsigned long Paused;
 volatile bool Completed;
 bool IgnoreTimeStampWhenCopying;
 void * Event;
 void * DecompressionEvent;
 void * PauseStartEvent;
 void * PauseFinishEvent;
 std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString const ,CCharString> > > CompressedFileTypes;
 static bool DirtyDisc;
public:
 CFileInstaller& operator=(const CFileInstaller&);
};
FABLE_STATIC_ASSERT(sizeof(CFileInstaller)==0xa0);
FABLE_STATIC_ASSERT(sizeof(CFileInstaller::CActiveFile)==0xb0);
#endif
