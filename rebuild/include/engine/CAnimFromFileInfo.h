#ifndef FABLE_ENGINE_CANIMFROMFILEINFO_H
#define FABLE_ENGINE_CANIMFROMFILEINFO_H
#include "fable_string.h"
class CAnimFromFileInfo{
#ifdef FABLE_RETAIL_CANIMFROMFILEINFO_CTOR_PASS26
public:
 // Both PDB complete types and original donor01e91cb0: no KeyFrameCount store.
 CAnimFromFileInfo():Active(false),PathName(L""),SegmentStartTime(0),SegmentEndTime(0),Duration(0){}
#endif
friend class CAnimateThing;private:bool Active;CWideString PathName;float SegmentStartTime,SegmentEndTime,Duration;long KeyFrameCount;};
#endif
