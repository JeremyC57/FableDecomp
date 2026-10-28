// CSoundEventManager::CSoundEventManager(CAudioOwner* g, float a4, float a0, float a8, float w, float h, CSteeringWorldEntry* entry) -- libpk sound-event manager: seed the owner and the identity transforms, build the event set and the fader (1000 / owner rate), clear the state block, then allocate the two script-module helpers, the audio resources (on the world entry) and the 50.0f filter
// Grade: DIFFER(507v507) -- residue: scheduling only: retail stores the vptr before the owner/arg loads and pushes the event-set ctor's arg3/arg2 between the identity stores (+0x10 / +0x14); ours stores the vptr after the owner load and the pushes one store later | tried: 2026-10-21 (0,0,0,0,1)+(0,0,0,1) blocks, long + two identity quaternions, polymorphic owner base (512); 2026-10-28 respelled only (owner rule: CSoundEventManager per the landed CAudioOwner ctor 0x00C094A0 and the sweep1 libpk class set) | next: park (lane gap in the vptr-store scheduling)
// Flags: /EHsc
// Leaf: ??0CSoundEventManager@@QAE@PAUCAudioOwner@@MMMMMPAUCSteeringWorldEntry@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C11D00, 507 bytes, speed TU with an EH frame (5 unwind states), `ret 1Ch`.
// Name (probable, libpk has no PDB or RTTI): the landed CAudioOwner::CAudioOwner 0x00C094A0 declares and calls this
//   `CSoundEventManager(CAudioOwner* g, float a4, float a0, float a8, float w, float h, CSteeringWorldEntry* entry)`, and the
//   landed CSoundEvent sources (sweep1, 2026-10-25) name vtable 0x012B2748 CSoundEventManager from the TraceEvents strings.
//   The catalog's "CTCHero::Initialize" and the stale binding `??0CSteeringSolver@@...` are wrong.
// Callees: CSoundEventSet ctor 0x00C222D0 (3 floats), CFader::CFader(float) 0x00C26010 (landed), operator new 0x00BFEA1A,
//   CVtable_012b4030 / CVtable_012b401c ctors 0x00C26E90 / 0x00C26A30 (landed), CAudioResources::CAudioResources(void*, void*)
//   0x00C1C700 (landed), CAudioDevice::GetDefaultRateRatio 0x00C1C0B0 (landed; result discarded), the filter ctor 0x00C25B10.
// Layout: local model from the retail stores: owner +0x04 (rate float +0x70), two identity blocks +0x08..+0x28, event set
//   +0x2C, helpers +0x7B4/+0x7B8, resources +0x7BC, +0x7C0 1.0f, filter +0x7C4, +0x7C8/+0x7CC, w/h +0x7D0/+0x7D4, fader +0x7D8,
//   flag +0x828, 21-dword state +0x82C, +0x880..+0x88C.
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
struct CSteeringWorldEntry;
struct CAudioOwner {
    unsigned char _pad00[0x70];
    float Rate;                                             // +0x70
};
struct CSoundEventSet {                                     // +0x2C member (vtable store 0x012B3EC8 inside)
    unsigned char _body[0x7b4 - 0x2c];
    CSoundEventSet(float a, float b, float c);              // 0x00C222D0
    ~CSoundEventSet();
};
struct CFader {                                             // landed 0x00C26010
    unsigned char _body[0x828 - 0x7d8];
    CFader(float timeStep);                                 // 0x00C26010
    ~CFader();
};
struct CSoundEventManager;
struct CVtable_012b4030 { unsigned char _body[0x40]; CVtable_012b4030(void* owner); };   // 0x00C26E90 (landed)
struct CVtable_012b401c { unsigned char _body[0x40]; CVtable_012b401c(void* owner); };   // 0x00C26A30 (landed)
struct CAudioDevice {
    float GetDefaultRateRatio();                            // 0x00C1C0B0 (landed; `this` unused)
};
struct CAudioResources {                                    // vtable 0x012B3290 (landed 0x00C1C700)
    unsigned char _body[0x1c];
    CAudioResources(void* owner, void* stream);             // 0x00C1C700
};
struct SAudioFilterFormat { long Flags; long Mask; };
struct CAudioFilter00c25b10 {                               // descriptive: no landed name, no RTTI (catalog label propagated)
    unsigned char _body[0x18];
    CAudioFilter00c25b10(float level, long a, CAudioResources* source, const SAudioFilterFormat* format);   // 0x00C25B10
};

struct CSoundEventManager {                                 // vtable 0x012B2748
    virtual ~CSoundEventManager();
    CAudioOwner*  Owner;                                    // +0x04
    STransform5   A;                                        // +0x08
    STransform4   B;                                        // +0x1C
    CSoundEventSet Event;                                   // +0x2C
    CVtable_012b4030* HelperA;                              // +0x7B4
    CVtable_012b401c* HelperB;                              // +0x7B8
    CAudioResources* Resources;                             // +0x7BC
    float         Gain;                                     // +0x7C0
    CAudioFilter00c25b10* Filter;                           // +0x7C4
    long          ValueC8;                                  // +0x7C8
    long          ValueCC;                                  // +0x7CC
    float         Width;                                    // +0x7D0
    float         Height;                                   // +0x7D4
    CFader        Timing;                                   // +0x7D8
    bool          Flag828;                                  // +0x828
    long          State[21];                                // +0x82C
    long          Value880;                                 // +0x880
    bool          Flag884;                                  // +0x884
    bool          Flag885;                                  // +0x885
    long          Value888;                                 // +0x888
    long          Value88C;                                 // +0x88C
    CSoundEventManager(CAudioOwner* g, float a4, float a0, float a8, float w, float h, CSteeringWorldEntry* entry);
};

CSoundEventManager::CSoundEventManager(CAudioOwner* g, float a4, float a0, float a8, float w, float h, CSteeringWorldEntry* entry)
    : Owner(g), Event(a4, a0, a8), HelperA(0), HelperB(0), Gain(1.0f), Filter(0), ValueC8(0), ValueCC(0),
      Width(w), Height(h), Timing(1000.0f / g->Rate)
{
    Flag828 = false;
    memset(State, 0, sizeof(State));
    Value880 = 0;
    Flag884 = false;
    Flag885 = true;
    Value888 = 500;
    Value88C = 0;
    HelperA = new CVtable_012b4030(this);
    HelperB = new CVtable_012b401c(this);
    Resources = new CAudioResources(this, entry);
    ((CAudioDevice*)Resources)->GetDefaultRateRatio();
    SAudioFilterFormat format;
    format.Flags = 0x3000;
    format.Mask = 0xf;
    Filter = new CAudioFilter00c25b10(50.0f, 0, Resources, &format);
}
