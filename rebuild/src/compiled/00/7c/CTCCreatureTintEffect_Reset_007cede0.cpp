// CTCCreatureTintEffect::Reset() -- restart the tint: zero the clock, fade times from the def's seconds, copy the def colour
// Naming wave 18: the global stand-in g_GameFramesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007CEDE0, length 95 bytes (+1 int3).
// Callee: __ftol2 @ 0x00BFEA70 (twice). Global: the game frames-per-second long @ 0x01375550 (initial value 15).
// Shape: CurTime (+0x10) = 0, FadeInTime (+0x14) and FadeOutTime (+0x20) = (long)(def seconds * frames-per-second) with
// the def pointer cached in edi (`fild [g] ; fmul [edi+3Ch]`: the extern operand is loaded first); the four Color words
// (+0x30, retail offset: the 12-byte TintList vector) are copied from the def's Color vector (def +0x44 = its first
// pointer) by a four-trip loop the compiler unrolls, PDef re-read after every indexed store (four explicit statements
// keep PDef in edi). Def offsets are the PDB's minus 4 (FadeInTimeSecs +0x3c,
// FadeOutTimeSecs +0x40, Color +0x44).
class CSpecialAbilitiesCreatureTintDef { public:
    unsigned char _pad00[0x3c];
    float FadeInTimeSecs;                   // +0x3c
    float FadeOutTimeSecs;                  // +0x40
    long* Color;                            // +0x44 vector<long> first
    long* ColorLast;                        // +0x48
    long* ColorEnd;                         // +0x4c
};

template <class T> class CDefPointer { public:
    T* Object;
    T* operator->() const { return Object; }
};

class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

class CThing;

class CTCCreatureTintEffect { public:
    void* __vftable;                        // +0x00 (CTCBase)
    CThing* ParentThing;                    // +0x04
    bool ToBeRemoved;                       // +0x08
    bool IsInGlobalUpdateTCs;               // +0x09
    CDefPointer<CSpecialAbilitiesCreatureTintDef> PDef;     // +0x0c
    long CurTime;                           // +0x10
    long FadeInTime;                        // +0x14
    bool StartFadeOut;                      // +0x18
    long FadeOutTimeStart;                  // +0x1c
    long FadeOutTime;                       // +0x20
    void* TintList[3];                      // +0x24 vector<CIntelligentPointer<CThing> > (12 bytes)
    long Color[4];                          // +0x30
    void Reset();
};

void CTCCreatureTintEffect::Reset()
{
    CurTime = 0;
    FadeInTime = (long)(PDef->FadeInTimeSecs * CMainGameComponent::GetConstantFPS());
    FadeOutTime = (long)(PDef->FadeOutTimeSecs * CMainGameComponent::GetConstantFPS());
    for (long i = 0; i < 4; ++i)
        Color[i] = PDef->Color[i];
}