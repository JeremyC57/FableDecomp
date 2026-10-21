// Grade: DIFFER(507v507) -- scheduling only: retail stores the vptr before the owner/arg loads and pushes the event ctor's arg3/arg2 between the identity stores (+0x10 / +0x14); ours places the vptr store after the owner load and the pushes one store later. Tried: (0,0,0,0,1)+(0,0,0,1) blocks (507), long + two identity quaternions (507, same residual), polymorphic owner base (512, worse). Needs /EHsc.
// Unnamed audio-library class constructor @ 0x00C11D00 (the catalog's "CTCHero::Initialize" is wrong: a 7-argument
// __thiscall constructor, `ret 1Ch`, storing vtable 0x012B2748 and returning this) -- seeds the owner and the identity
// transforms, builds the event member and the timing member (1000 / owner rate), clears the state block, then allocates
// the four helper objects (two 0x40-byte owners' helpers, the 0x1C-byte source and the 0x18-byte filter at 50.0f)
// Flags: /EHsc
// Leaf: ??0CAudioObjectC11D00
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C11D00, 507 bytes, speed TU with an EH frame (5 unwind states).
// Callees: the event member ctor 0x00C222D0 (3 args), the timing member ctor 0x00C26010 (float), operator new 0x00BFEA1A,
// helper ctors 0x00C26E90 / 0x00C26A30 (owner), 0x00C1C700 (owner, arg 6), 0x00C25B10 (50.0f, 0, source, format*), the
// source's float query 0x00C1C0B0 (result discarded).
// Layout: local model from the retail stores (names descriptive): owner +0x04 (its rate float +0x70), two identity
// blocks +0x08..+0x28, event +0x2C, helpers +0x7B4/+0x7B8/+0x7BC, +0x7C0 1.0f, filter +0x7C4, +0x7C8/+0x7CC, args 4/5
// at +0x7D0/+0x7D4, timing +0x7D8, flag +0x828, 21-dword state +0x82C, +0x880..+0x88C.
extern "C" void* __cdecl memset(void* dest, int value, unsigned int count);
#pragma intrinsic(memset)

struct STransform5 {                                        // (0, 0, 0, 0, 1)
    float V[4];
    float W;
    STransform5() { V[0] = 0.0f; V[1] = 0.0f; V[2] = 0.0f; V[3] = 0.0f; W = 1.0f; }
};
struct STransform4 {                                        // (0, 0, 0, 1)
    float V[3];
    float W;
    STransform4() { V[0] = 0.0f; V[1] = 0.0f; V[2] = 0.0f; W = 1.0f; }
};
struct COwner {
    unsigned char _pad00[0x70];
    float Rate;                                             // +0x70
};
struct CEventMember {
    unsigned char _body[0x7b4 - 0x2c];
    CEventMember(long a, long b, long c);                   // 0x00C222D0
    ~CEventMember();
};
struct CTimingMember {
    unsigned char _body[0x828 - 0x7d8];
    explicit CTimingMember(float period);                   // 0x00C26010
    ~CTimingMember();
};
class CAudioObjectC11D00;
struct CHelperA { unsigned char _body[0x40]; CHelperA(CAudioObjectC11D00* owner); };          // 0x00C26E90
struct CHelperB { unsigned char _body[0x40]; CHelperB(CAudioObjectC11D00* owner); };          // 0x00C26A30
struct CSource {
    unsigned char _body[0x1c];
    CSource(CAudioObjectC11D00* owner, long arg);           // 0x00C1C700
    float Query();                                          // 0x00C1C0B0
};
struct SFormat { long Flags; long Mask; };
struct CFilter {
    unsigned char _body[0x18];
    CFilter(float level, long a, CSource* source, const SFormat* format);   // 0x00C25B10
};

class CAudioObjectC11D00 {
public:
    virtual ~CAudioObjectC11D00();
    COwner*       Owner;                                    // +0x04
    STransform5   A;                                        // +0x08
    STransform4   B;                                        // +0x1C
    CEventMember  Event;                                    // +0x2C
    CHelperA*     HelperA;                                  // +0x7B4
    CHelperB*     HelperB;                                  // +0x7B8
    CSource*      Source;                                   // +0x7BC
    float         Gain;                                     // +0x7C0
    CFilter*      Filter;                                   // +0x7C4
    long          ValueC8;                                  // +0x7C8
    long          ValueCC;                                  // +0x7CC
    long          Arg4;                                     // +0x7D0
    long          Arg5;                                     // +0x7D4
    CTimingMember Timing;                                   // +0x7D8
    bool          Flag828;                                  // +0x828
    long          State[21];                                // +0x82C
    long          Value880;                                 // +0x880
    bool          Flag884;                                  // +0x884
    bool          Flag885;                                  // +0x885
    long          Value888;                                 // +0x888
    long          Value88C;                                 // +0x88C
    CAudioObjectC11D00(COwner* owner, long a1, long a2, long a3, long a4, long a5, long a6);
};

CAudioObjectC11D00::CAudioObjectC11D00(COwner* owner, long a1, long a2, long a3, long a4, long a5, long a6)
    : Owner(owner), Event(a1, a2, a3), HelperA(0), HelperB(0), Gain(1.0f), Filter(0), ValueC8(0), ValueCC(0),
      Arg4(a4), Arg5(a5), Timing(1000.0f / owner->Rate)
{
    Flag828 = false;
    memset(State, 0, sizeof(State));
    Value880 = 0;
    Flag884 = false;
    Flag885 = true;
    Value888 = 500;
    Value88C = 0;
    HelperA = new CHelperA(this);
    HelperB = new CHelperB(this);
    Source = new CSource(this, a6);
    Source->Query();
    SFormat format;
    format.Flags = 0x3000;
    format.Mask = 0xf;
    Filter = new CFilter(50.0f, 0, Source, &format);
}
