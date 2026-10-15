// CTCEmoteIcon::CTCEmoteIcon(CThing*) -- CTCBase(thing), the CTCEmoteIcon vtable, timer 0, empty emote text, icon/scale 0, type 2, priority 8, alpha 0xff, replaceable + enabled, and an empty CEnginePrimitiveHandle
// Leaf: ??0CTCEmoteIcon@@QAE@PAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071AFE0, length 93 bytes (+ int3 padding). Manifest label "CTCHero::CTCHero"
// is propagated; vtable 0x01265ABC = CTCEmoteIcon (rtti-vtables.tsv).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, CWideString::CWideString() @ 0x0099AED0 (manifest "CCharString" is a
// propagated twin label), CBaseClass::CBaseClass() @ 0x0099A2D0 (base of CEnginePrimitiveHandle; manifest "CBase").
// Shape: Ego_r.pdb layout (0x30 bytes): ActiveTimer +0xc, EmoteText +0x10, EmoteIcon +0x14, EmoteScale +0x18, EmoteType +0x1c,
// EmotePriority +0x20, EmoteAlpha +0x24 (uchar), Replaceable +0x25, Enabled +0x26, PrimitiveHandle +0x28 (vptr + counted
// pointer). Initialisers in declaration order around the out-of-line CWideString constructor (ebx = 0, al = 1 shared by the
// two bools); the handle's inline constructor runs last: CBaseClass(), its vtable 0x0122F598, then the
// CIVCountedPointer member's inline null constructor (`mov [edi+4],ebx` after the vtable store).
// Local model: no engine header exists for CTCEmoteIcon; engine/CTCBase.h is a flattened POD (vptr as a data member), so the
// polymorphic CTCBase is modelled locally.
struct CThing;

struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CTCBase(CThing* thing);                                 // 0x00686800
};

struct CWideString {
    void* PStringData;
    CWideString();                                          // 0x0099AED0
};

struct CBaseClass {
    virtual void v00();
    CBaseClass();                                           // 0x0099A2D0
};

struct CEnginePersistentPrimitiveBase;
struct CIVCountedPointer {                                  // CIVCountedPointer<CEnginePersistentPrimitiveBase>
    CEnginePersistentPrimitiveBase* Object;
    CIVCountedPointer() : Object(0) { }
};
struct CEnginePrimitiveHandle : CBaseClass {                // vtable 0x0122F598
    CIVCountedPointer Data;                                 // +0x04
    virtual void v00();
    CEnginePrimitiveHandle() { }
};

struct CTCEmoteIcon : CTCBase {                             // vtable 0x01265ABC
    long ActiveTimer;                                       // +0x0c
    CWideString EmoteText;                                  // +0x10
    long EmoteIcon;                                         // +0x14
    float EmoteScale;                                       // +0x18
    long EmoteType;                                         // +0x1c EEmoteType
    long EmotePriority;                                     // +0x20 EEmotePriority
    unsigned char EmoteAlpha;                               // +0x24
    bool Replaceable;                                       // +0x25
    bool Enabled;                                           // +0x26
    CEnginePrimitiveHandle PrimitiveHandle;                 // +0x28
    virtual void v00();
    CTCEmoteIcon(CThing* thing);
};

CTCEmoteIcon::CTCEmoteIcon(CThing* thing)
    : CTCBase(thing), ActiveTimer(0), EmoteIcon(0), EmoteScale(0.0f), EmoteType(2), EmotePriority(8),
      EmoteAlpha(0xff), Replaceable(true), Enabled(true)
{
}
