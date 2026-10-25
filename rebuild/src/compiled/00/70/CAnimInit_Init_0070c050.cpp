// CAnimInit::Init(unsigned char type, unsigned long typeFlags, long anim, EAnimUpdateMode updateMode, bool looping, unsigned long flags, long priority) -- (re)initialise an animation request: unit speed and alpha, frame zeroed, then the arguments
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070C050, length 73 bytes (+ int3 padding), `ret 1Ch`.
// Leaf: ?Init@CAnimInit@@QAEXEKJW4EAnimUpdateMode@@_NKJ@Z
// Naming: Ego_r.pdb ?Init@CAnimInit@@QAEXEKJW4EAnimUpdateMode@@_NKJ@Z (seven arguments = `ret 1Ch`). Ego_r.pdb CAnimInit
//   (44 bytes, the retail extent written here): EAnimUpdateMode UpdateMode +0x00, uchar Type +0x04, bool Looping +0x05,
//   long Anim +0x08, float Speed +0x0C, float Alpha +0x10, float LastAlpha +0x14, CAnimFrameInfo Frame {float Current,
//   Last} +0x18, ulong Flags +0x20, ulong TypeFlags +0x24, long Priority +0x28; each argument goes to the member of its
//   type (byte Type +4, ulong TypeFlags +0x24, long Anim +8, enum UpdateMode +0, bool Looping +5, ulong Flags +0x20,
//   long Priority +0x28). FINDINGS chunk 402. Was landed as the setter stand-in "CInitRumble::Set" (catalog
//   "CAnimInit::Set"; the "CInitRumble::CInitRumble" label is propagated).
// Shape: one `mov eax,3F800000h` for the three 1.0f stores, one zero register for the frame, then the arguments.
enum EAnimUpdateMode { };

class CAnimFrameInfo {
public:
    float Current;                                 // +0x00
    float Last;                                    // +0x04
};

class CAnimInit {
public:
    void Init(unsigned char type, unsigned long typeFlags, long anim, EAnimUpdateMode updateMode, bool looping,
              unsigned long flags, long priority);
    EAnimUpdateMode UpdateMode;                    // +0x00
    unsigned char   Type;                          // +0x04
    bool            Looping;                       // +0x05
    long            Anim;                          // +0x08
    float           Speed;                         // +0x0c
    float           Alpha;                         // +0x10
    float           LastAlpha;                     // +0x14
    CAnimFrameInfo  Frame;                         // +0x18
    unsigned long   Flags;                         // +0x20
    unsigned long   TypeFlags;                     // +0x24
    long            Priority;                      // +0x28
};

void CAnimInit::Init(unsigned char type, unsigned long typeFlags, long anim, EAnimUpdateMode updateMode, bool looping,
                     unsigned long flags, long priority)
{
    Speed = 1.0f;
    Alpha = 1.0f;
    LastAlpha = 1.0f;
    Frame.Current = 0.0f;
    Frame.Last = 0.0f;
    UpdateMode = updateMode;
    Type = type;
    Anim = anim;
    Looping = looping;
    Flags = flags;
    TypeFlags = typeFlags;
    Priority = priority;
}
