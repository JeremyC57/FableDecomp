// CTCSoundPlayer::IsAttachedSoundPlaying(unsigned long sampleID) const -- while the sound system exists and the attached emitter (PAttachedEmitter, +0xc) is set and not dead, ask the emitter whether the sample is playing; else false
// Leaf: ?IsAttachedSoundPlaying@CTCSoundPlayer@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0066AE10, length 69 bytes.
// Callees: CBaseIntelligentPointer::GetPItem() const @ 0x00A01B50 three times (re-fetched for every access),
// CThingSoundEmitter::IsSamplePlaying(unsigned long) const @ 0x00836870.
// Identity: the PDB name sets hold ?IsAttachedSoundPlaying@CTCSoundPlayer@@QBE_NK@Z and
// ?IsSamplePlaying@CThingSoundEmitter@@QBE_NK@Z (no CGameScriptThing::IsSamplePlaying); Ego_r gives CTCSoundPlayer
// CIntelligentPointer<CThingSoundEmitter> PAttachedEmitter at +0xc, and the callee is the landed
// CThingSoundEmitter::IsSamplePlaying.  Callers are sound/dialogue code (CTCWife::GlobalUpdate,
// NSpeechGainManager OnDialogueStart / UpdateDialogueElement, which already binds this name).
// Shape: the global test first and `push esi` hoisted above its branch; the pointer's address kept in esi;
// nested ifs with one `return false` at the end (`xor al,al`).
// No engine header exists for CTCSoundPlayer or CThingSoundEmitter: local models (CTCBase part as padding).
extern void* g_pSoundSystem;                        // 0x013B8394 (descriptive name)

class CBaseObject;
class CBaseIntelligentPointer {                     // 8 bytes: vptr + the pointee link
public:
    virtual ~CBaseIntelligentPointer();
    void* PItem;
    CBaseObject* GetPItem() const;                  // 0x00A01B50
};

class CThingSoundEmitter {
public:
    unsigned char _pad00[0x91];
    unsigned char Dead : 1;                         // +0x91 bit 0 (CThing flag)
    unsigned char _flags91 : 7;
    bool IsSamplePlaying(unsigned long sampleID) const;   // 0x00836870
};

class CTCSoundPlayer {
public:
    unsigned char _tcbase[0xc];                     // CTCBase
    CBaseIntelligentPointer PAttachedEmitter;        // +0x0c, CIntelligentPointer<CThingSoundEmitter>
    bool IsAttachedSoundPlaying(unsigned long sampleID) const;
};

bool CTCSoundPlayer::IsAttachedSoundPlaying(unsigned long sampleID) const
{
    if (g_pSoundSystem) {
        if (PAttachedEmitter.GetPItem()) {
            if (!((const CThingSoundEmitter*)PAttachedEmitter.GetPItem())->Dead)
                return ((const CThingSoundEmitter*)PAttachedEmitter.GetPItem())->IsSamplePlaying(sampleID);
        }
    }
    return false;
}
