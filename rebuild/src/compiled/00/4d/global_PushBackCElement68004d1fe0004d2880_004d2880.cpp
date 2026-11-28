// Leaf: ?push_back@?$CArray@VCSubtitledMessage@@@@QAEXABVCSubtitledMessage@@@Z
// Flags: /O2 /Oy /W3 /Gr
#define FABLE_RETAIL_SUBTITLED_MESSAGE_PASS29
#define FABLE_RETAIL_CSUBTITLEDMESSAGE_PASS29
#define FABLE_RETAIL_SUBTITLE_GETMESSAGES_PASS30
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY
#define FABLE_RETAIL_CARRAY_PUSHBACK
#include "fable_string.h"
#include "engine/ETextGroupSelectionMethod.h"
#include "engine/CVectorMap.h"
#include "engine/CSubtitledMessage.h"

template void CArray<CSubtitledMessage>::push_back(const CSubtitledMessage&);
