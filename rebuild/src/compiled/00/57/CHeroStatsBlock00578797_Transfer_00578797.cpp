#pragma optimize("s", on)
// CHeroStatsBlock_00578797::Transfer(CPersistContext& context, long) -- persist the "OpenedExperienceSpending" flag at +0x1a0 and the "TutorialsPlayed" byte array at +0x248
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00578797, length 49 bytes.
// Callees: CPersistContext::Transfer<bool> @ 0x004107C0 (the manifest label "CEngineSceneGrid::GetCellBoundingBox" is propagated: the callee builds a one-byte
// temporary), CPersistContext::Transfer<signed char> @ 0x0049B8D0. Strings @ 0x0124F298 / 0x0124F288. Shape: the context is re-read from the stack for each
// call (ecx is clobbered by the first), the first member address is a `lea`, the second an `add esi,248h` (esi is dead afterwards); the second argument is unused.
// The chunk label "CEngineLocalDetailLayerDef::Transfer" is propagated from an unrelated body; the class is named by address.
#include "stlport/fable_containers.h"
class CCharString;
class CPersistContext { public:
    template <class T> void Transfer(const char* name, T& value);   // 0x004107C0 <bool> (Ego_r / FableWin ??$Transfer@_N@...AA_N@Z)
    template <class T> void TransferVector(const char* name, std::vector<T, std::allocator<T> >& value);   // 0x0049B8D0 <CCharString> (Ego_r / FableWin ??$TransferVector@VCCharString@@@)
};

struct CHeroStatsBlock_00578797 {
    unsigned char _pad0[0x1a0];
    bool          m_openedExperienceSpending;      // +0x1a0
    unsigned char _pad1[0xa7];
    std::vector<CCharString, std::allocator<CCharString> > m_tutorialsPlayed;   // +0x248 (Ego_r CTCInventoryExperience::TutorialsPlayed, vector<CCharString>)
    void Transfer(CPersistContext& context, long version);
};

void CHeroStatsBlock_00578797::Transfer(CPersistContext& context, long version)
{
    context.Transfer("OpenedExperienceSpending", m_openedExperienceSpending);
    context.TransferVector("TutorialsPlayed", m_tutorialsPlayed);
}