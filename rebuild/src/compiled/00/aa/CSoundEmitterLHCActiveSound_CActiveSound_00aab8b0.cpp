// CSoundEmitterLH::CActiveSound::CActiveSound(long sampleID, CASoundBank& bank, const CCharString& name, float startTime, bool is3D, bool headRelative) -- CBase, the words at +8 / +0xc / +0x10, the flag bytes at +0x14 / +0x15, this class's vtable, a reference count of 1 at +4, the cleared word at +0x18, the name string at +0x1c built out of line, the cleared word at +0x20
// Leaf: ??0CActiveSound@CSoundEmitterLH@@QAE@JAAVCASoundBank@@ABVCCharString@@M_N2@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AAB8B0, length 87 bytes.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0, CCharString::CCharString(const CCharString&) @ 0x0099EC30 (on +0x1c). Vtable 0x0129E67C = CSoundEmitterLH::CActiveSound
// (rtti-vtables.tsv). Shape: the six arguments are loaded in a shuffled order around the vtable store; the string's `push ; lea ecx` sits before the +4 / +0x14 /
// +0x18 stores; `ret 18h`. The chunk label "CEngineSubPrimitiveAnimatedMesh" is propagated.
// Naming (agentNF31, 2026-11-12): signature respelled from (void*, void*, const char*, void*, bool, bool) to the one the landed
//   caller PlaySoundEvent 0x00AABD50 binds (reloc_bindings): its pushes at 0x00AABEF4..0x00AABF09 are the handle, the bank, the
//   name string, GFGetTime as float, Is3D and HeadRelative; 0x0099EC30 is the CCharString copy ctor (Ego_r / FableWin
//   ??0CCharString@@QAE@ABV0@@Z, fable_string.h). Shared headers replace the local CBaseClassNonCopyable / CCharString models.
//   Member names stay descriptive (no PDB layout of CActiveSound).
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CASoundBank.h"
#include "fable_string.h"

class CSoundEmitterLH {
public:
    class CActiveSound : public CBaseClassNonCopyable {                  // vtable 0x0129E67C
    public:
        long         m_references;                 // +0x04
        long         m_sampleID;                   // +0x08
        CASoundBank* m_pBank;                      // +0x0c
        float        m_startTime;                  // +0x10
        bool         m_is3D;                       // +0x14
        bool         m_headRelative;               // +0x15
        long         m_18;                         // +0x18
        CCharString  m_name;                       // +0x1c
        long         m_20;                         // +0x20
        CActiveSound(long sampleID, CASoundBank& bank, const CCharString& name, float startTime, bool is3D, bool headRelative);
    };
};

CSoundEmitterLH::CActiveSound::CActiveSound(long sampleID, CASoundBank& bank, const CCharString& name, float startTime, bool is3D, bool headRelative)
    : CBaseClassNonCopyable()
    , m_references(1)
    , m_sampleID(sampleID), m_pBank(&bank), m_startTime(startTime)
    , m_is3D(is3D), m_headRelative(headRelative)
    , m_18(0)
    , m_name(name)
    , m_20(0)
{
}
