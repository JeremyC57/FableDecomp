// CWorld::SetMiniMapEnabled(bool) -- enable/disable the mini map, remembering whether it was open when disabled
// Leaf: ?SetMiniMapEnabled@CWorld@@QAEX_N@Z
// Retail: Fable.exe @ 0x0049EA10, 46 bytes. PDB ?SetMiniMapEnabled@CWorld@@QAEX_N@Z
// Callee: CPlayerGui::IsMiniMapActive() const 0x00437860 (forwards to the CDrawMiniMap at +0x160; manifest label
// "_Cons_val<...>" is a propagation) on the player GUI singleton g_pPlayerGui 0x013B8790.
// Retail CWorld: MiniMapEnabled +0xDD, MiniMapActiveBeforeDisabled +0xDE (PDB +0xE9/+0xEA: the three 16-byte
// containers InitialActiveQuests / RegisteredQuests / ActiveTestQuests ahead of them are 12 bytes in retail), so
// engine/CWorld.h (PDB layout) is not used and a local model is.
// Shape: a change test on the byte, the store, then on a disable the mini map's open state is captured.
// Layout (supersedes any local-model note above): engine/CWorld.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CWorld CWorldDonor
#include "engine/CWorld.h"
#undef CWorld
class CPlayerGui { public:
    bool IsMiniMapActive() const;               // 0x00437860
};
extern CPlayerGui* g_pPlayerGui;                // 0x013B8790

class CWorld : public CWorldDonor {
 public:
    void SetMiniMapEnabled(bool enabled);
};

void CWorld::SetMiniMapEnabled(bool enabled)
{
    if (enabled != MiniMapEnabled)
    {
        MiniMapEnabled = enabled;
        if (!enabled)
            MiniMapActiveBeforeDisabled = g_pPlayerGui->IsMiniMapActive();
    }
}