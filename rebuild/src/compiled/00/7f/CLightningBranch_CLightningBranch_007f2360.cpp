// CLightningBranch::~CLightningBranch() -- kill the branch's effect thing (+0x08) and strip component 0x10b from its target thing (+0x00) unless either is already flagged dead, then destroy both intelligent pointers
// Leaf: ??1CLightningBranch@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F2360, length 112 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three calls per pointer), CThing::Kill(bool) @ 0x004C9B80,
//          CThing::RemoveTC(long) @ 0x004C9840 (manifest "CBaseObject::OnPropertyChanged" is propagated; see GOTCHAS
//          chunk 140), CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10 (members last first, the
//          last one a tail jump).
// Shape: a non-polymorphic class of two 8-byte intelligent pointers; the dead flag is bit 0 of the thing's byte
// +0x91; every use calls GetPItem again (no local).
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#include "engine/CThing.h"

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

struct CLightningBranch {
    CIntelligentPointer<CThing> m_target;              // +0x00
    CIntelligentPointer<CThing> m_effect;              // +0x08
    ~CLightningBranch();
};

CLightningBranch::~CLightningBranch()
{
    if (m_effect.Get() && !m_effect.Get()->IsDead())
        m_effect.Get()->Kill(true);
    if (m_target.Get() && !m_target.Get()->IsDead())
        m_target.Get()->RemoveTC((ETCInterfaceType)(0x10b));
}