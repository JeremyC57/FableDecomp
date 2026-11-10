#ifndef FABLE_ENGINE_CTHREADEDFILEOBJECTDESC_H
#define FABLE_ENGINE_CTHREADEDFILEOBJECTDESC_H
#include "rebuild_abi.h"
#include "engine/EThreadedFilePriority.h"
class CThreadedFileCallback;
// Retail opt-in (Codex pass 17): both PDBs give the six members; 0x009D5D7A..8A initialises this 0x18-byte descriptor.
class CThreadedFileObjectDesc {
public:
    void* PDst;
    unsigned long FilePos;
    unsigned long Size;
    CThreadedFileCallback* PCallback;
    bool QueueWhenFinished;
    EThreadedFilePriority Priority;
    CThreadedFileObjectDesc() : PDst(0), FilePos(0), Size(0), PCallback(0), QueueWhenFinished(true), Priority(THREADED_FILE_PRIORITY_NORMAL) { }
};
FABLE_STATIC_ASSERT(sizeof(CThreadedFileObjectDesc)==0x18);
#endif
