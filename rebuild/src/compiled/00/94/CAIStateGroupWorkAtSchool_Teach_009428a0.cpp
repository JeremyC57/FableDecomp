// CAIStateGroup_WorkAtSchool::Teach(long) -- teach the class: the ST_TEACH into/loop/out-of animation triple
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009428A0, length 121 bytes (+ int3 padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (x3), CAIStateGroup_WorkAtSchool::
// PlayLoop(long, const CCharString&, const CCharString&, const CCharString&) @ 0x009426D0, CCharString::
// ~CCharString @ 0x0099EAE0 (x3).
// Label: the manifest's "CrouchAndTalk" is propagated from the byte-identical 0x00942820 (which plays the
// ST_CROUCH_TALK_TO_CHILD_* triple); this body plays ST_TEACH_*, so it is named after its animations.
//
// Shape: `return PlayLoop(time, "ST_TEACH_INTO", "ST_TEACH_LOOP", "ST_TEACH_OUTOF");` -- three implicit-
// conversion temporaries (CCharString(const char*, long = -1)) built right to left (OUTOF first), re-addressed
// with `lea` when pushed, destroyed in reverse order after the call; the bool result rides in bl.
// Leaf: Teach
#include "engine/CAIStateGroup_WorkAtSchool.h"

class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

struct CAIStateGroup_WorkAtSchool_Methods : CAIStateGroup_WorkAtSchool {
    bool PlayLoop(long time, const CCharString& into, const CCharString& loop, const CCharString& outOf);   // 0x009426D0
    bool Teach(long time);
};

bool CAIStateGroup_WorkAtSchool_Methods::Teach(long time)
{
    return PlayLoop(time, "ST_TEACH_INTO", "ST_TEACH_LOOP", "ST_TEACH_OUTOF");
}