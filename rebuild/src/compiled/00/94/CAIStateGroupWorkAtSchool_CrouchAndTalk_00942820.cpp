// CAIStateGroup_WorkAtSchool::CrouchAndTalk(long) -- crouch and talk to a child: the into/loop/out-of animation triple
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00942820, length 121 bytes (+ int3 padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (x3), CAIStateGroup_WorkAtSchool::
// PlayLoop(long, const CCharString&, const CCharString&, const CCharString&) @ 0x009426D0, CCharString::
// ~CCharString @ 0x0099EAE0 (x3).
//
// Shape: `return PlayLoop(time, "..._INTO", "..._LOOP", "..._OUTOF");` -- three implicit-conversion
// temporaries (CCharString(const char*, long = -1)) built right to left (OUTOF first, at the highest frame
// word), each re-addressed with `lea` when pushed, destroyed in reverse construction order after the call;
// the bool result rides in bl across the three destructors. `this` is the PDB header layout (no members used).
#include "engine/CAIStateGroup_WorkAtSchool.h"

class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

struct CAIStateGroup_WorkAtSchool_Methods : CAIStateGroup_WorkAtSchool {
    bool PlayLoop(long time, const CCharString& into, const CCharString& loop, const CCharString& outOf);   // 0x009426D0
    bool CrouchAndTalk(long time);
};

bool CAIStateGroup_WorkAtSchool_Methods::CrouchAndTalk(long time)
{
    return PlayLoop(time, "ST_CROUCH_TALK_TO_CHILD_INTO", "ST_CROUCH_TALK_TO_CHILD_LOOP",
                    "ST_CROUCH_TALK_TO_CHILD_OUTOF");
}