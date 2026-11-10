#ifndef FABLE_ENGINE_CTHREADEDFILECALLBACK_H
#define FABLE_ENGINE_CTHREADEDFILECALLBACK_H
#include "engine/CBaseClassNonCopyable.h"
#include "engine/EThreadedFilePriority.h"
// Retail opt-in (Codex pass 17): both PDBs give CBaseClassNonCopyable@0, no data, and these three virtual slots.
class CThreadedFileCallback : public CBaseClassNonCopyable {
public:
    CThreadedFileCallback() { }
    virtual ~CThreadedFileCallback() { }
    virtual bool OnFileReadBegin(void**, EThreadedFilePriority);
    virtual void OnFileReadCompletion(bool);
};
FABLE_STATIC_ASSERT(sizeof(CThreadedFileCallback)==4);
#endif
