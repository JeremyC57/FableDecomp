// CTCTalk::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~CTCTalk inlined (destroying DialogueVoice, LastNarrator, then ~CTCBase), and operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GCTCTalk@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E4690, 48 bytes, thiscall, `ret 4`; reached only through vtable slot 0.
// Identity: CTCTalk -- proven; evidence: vtable 0x0125FFEC slot 0 (RTTI .?AVCTCTalk@@ : CTCBase); the two CCharString members are Ego_r.pdb DialogueVoice (+0x38) and LastNarrator (+0x30), and 0x00686830 stores the CTCBase vtable 0x0125BE0C.
// Label: the manifest's "`vector_deleting_destructor'" is propagated across byte-identical bodies.
// Callees: CCharString::~CCharString 0x0099EAE0 (twice), CTCBase::~CTCBase 0x00686830, operator delete 0x00BFE9BC.
// Shape: no vtable store -- VC7.1 resets the vptr only in user-written destructors, and every destructor between this class
//   and CTCBase is implicit (CTCBase's own one stores its vtable); member destructors run in reverse declaration order.
//   The deleting destructor is emitted with the vtable, i.e. in a TU that constructs the class: MakeOne is that
//   construction, nothing more.
// Model: local -- no engine header for this class; the bases are sized to the retail layout.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable : public CBaseClass { // vtable 0x0129A7C4
protected:
    CBaseClassNonCopyable();                      // 0x0099A2F0 (FableWin.pdb ??0CBaseClassNonCopyable@@IAE@XZ)
    virtual ~CBaseClassNonCopyable();             // 0x0099A300 (FableWin.pdb ??1CBaseClassNonCopyable@@MAE@XZ)
};
class CCharString {
public:
    CCharString();                                // 0x0099E4B0
    ~CCharString();                               // 0x0099EAE0
    void* PStringData;
};
class CASoundBank;
enum EConversationAttitude { };
class CTCBase : public CBaseClassNonCopyable {    // Ego_r.pdb 12 bytes
public:
    CTCBase();
    virtual ~CTCBase();                           // 0x00686830
    unsigned char _base[8];
};
class CTCTalk : public CTCBase {
public:
    long SpeechSample;                            // +0x0c
    long SampleStartedFrame;                      // +0x10
    long SampleDurationFrames;                    // +0x14
    EConversationAttitude Attitude;               // +0x18
    float MimeStartTime;                          // +0x1c
    float SampleLoadedTime;                       // +0x20
    CASoundBank* PSpeechBank;                     // +0x24
    unsigned long SampleID;                       // +0x28
    long LastTextIndex;                           // +0x2c
    CCharString LastNarrator;                     // +0x30
    bool IsPureAISpeaker;                         // +0x34
    bool LipSynching;                             // +0x35
    bool UseLipSynching;                          // +0x36
    CCharString DialogueVoice;                    // +0x38
};
void* MakeOne()
{
    return new CTCTalk;
}