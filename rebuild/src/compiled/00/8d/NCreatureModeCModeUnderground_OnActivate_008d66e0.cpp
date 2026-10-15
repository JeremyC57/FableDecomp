// NCreatureMode::CModeUnderground::OnActivate() -- the CModeStandBase activation (0x008621A0), the thing's targeting flags replaced with none (GetOldTargetFlagsAndReplaceWith(thing, 0) @ 0x0085F830) and the old ones kept at +0x74, then bit 0x40 of the thing's byte +0x1c8 set when bit 0x10 of its byte +0x6c is set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D66E0, length 39 bytes.
// Callees: CModeStandBase::OnActivate @ 0x008621A0, GetOldTargetFlagsAndReplaceWith @ 0x0085F830 (`__fastcall(CThing*, ETargetingType)`). Vtable slot 5 of CModeUnderground (rtti-vtables.tsv). Shape: `this` in esi, the thing re-read from +4 after the call (it may alias), byte flag tests, `ret`.
namespace NCreatureMode {
struct CThing {
    unsigned char _pad00[0x6c];
    unsigned char m_flags6c;                       // +0x6c (bit 0x10)
    unsigned char _pad6d[0x15b];
    unsigned char m_flags1c8;                      // +0x1c8 (bit 0x40)
};

unsigned long __fastcall GetOldTargetFlagsAndReplaceWith(CThing* thing, long targetingType);   // 0x0085F830

struct CModeBase {                                 // CTCCreatureModeManager::CModeBase, vtable 0x012673CC
    virtual ~CModeBase();
    CThing* m_pThing;                              // +0x04
};

struct CModeStandBase : CModeBase {
    unsigned char _pad08[0x60];                    // +0x08 .. +0x68
    void OnActivate();                             // 0x008621A0
};

struct CModeUnderground : CModeStandBase {
    unsigned char _pad68[0xc];                     // +0x68 .. +0x74
    unsigned long m_oldTargetFlags;                // +0x74
    virtual void OnActivate();
};

void CModeUnderground::OnActivate()
{
    CModeStandBase::OnActivate();
    m_oldTargetFlags = GetOldTargetFlagsAndReplaceWith(m_pThing, 0);
    if (m_pThing->m_flags6c & 0x10)
        m_pThing->m_flags1c8 |= 0x40;
}
}