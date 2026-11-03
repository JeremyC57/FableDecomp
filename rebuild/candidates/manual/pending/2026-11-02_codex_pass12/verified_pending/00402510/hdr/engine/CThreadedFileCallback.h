#ifdef FABLE_RETAIL_GFMAIN
#ifndef FABLE_ENGINE_GFMAIN_CTHREADEDFILECALLBACK
#define FABLE_ENGINE_GFMAIN_CTHREADEDFILECALLBACK
// Retail opt-in (Codex pass12): original PDB declarations, native GFMain field offsets.
#include "engine/CBaseClassNonCopyable.h"
enum EThreadedFilePriority;
class CThreadedFileCallback:public CBaseClassNonCopyable{public:virtual~CThreadedFileCallback();virtual bool OnFileReadBegin(void**,EThreadedFilePriority);virtual void OnFileReadCompletion(bool);};
#endif

#else

#endif
