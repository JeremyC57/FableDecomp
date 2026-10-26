// CAIStateGroup_GoToPub::Listen() -- play the LISTENING_INTO / LISTENING_LOOP / LISTENING_OUTOF animation sequence once
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094A190, length 104 bytes (+ int3 padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (x3),
// CAIStateGroup_GoToPub::PlayAnims(const CCharString&, const CCharString&, const CCharString&, long) @ 0x00948D40,
// CCharString::~CCharString @ 0x0099EAE0 (x3).
//
// Shape: three explicit `CCharString("..", -1)` temporaries built right to left inside the call (each
// constructor's returned `this` is pushed straight from eax: explicit temporaries, not implicit
// conversions), the bool result parked in bl across the three destructors (reverse construction order:
// INTO at +8, LOOP at +0xc, OUTOF at +0x10). No engine header for the class; local model.
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length);     // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    void* PBrain;                                   // +0x04
    unsigned char _pad[0x0c];                       // +0x08..+0x13
};

struct CAIStateGroup_GoToPub : CAIStateGroupBase {
    bool PlayAnims(const CCharString& into, const CCharString& loop, const CCharString& outOf, long loops);   // 0x00948D40
    bool Listen();
};

bool CAIStateGroup_GoToPub::Listen()
{
    return PlayAnims(CCharString("LISTENING_INTO", -1), CCharString("LISTENING_LOOP", -1),
        CCharString("LISTENING_OUTOF", -1), 1);
}