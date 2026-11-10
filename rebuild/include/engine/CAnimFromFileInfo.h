#ifndef FABLE_ENGINE_CANIMFROMFILEINFO_H
#define FABLE_ENGINE_CANIMFROMFILEINFO_H
#include "fable_string.h"
class CAnimFromFileInfo{friend class CAnimateThing;private:bool Active;CWideString PathName;float SegmentStartTime,SegmentEndTime,Duration;long KeyFrameCount;};
#endif
