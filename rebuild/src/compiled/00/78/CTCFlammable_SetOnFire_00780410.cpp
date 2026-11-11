// CTCFlammable::SetOnFire() -- zero the Resistance (+0xc) of every fire point in FirePoints (+0x10, 0x2c-byte NFire::CFirePoint)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00780410, length 32 bytes.
// Leaf: ?SetOnFire@CTCFlammable@@QAEXXZ
// Name (agentNF22, Codex pass 21 queue): Ego_r.pdb + FableWin.pdb ?SetOnFire@CTCFlammable@@QAEXXZ; Codex: the OnHit native calls and
//   the donor 0x027EC280 named call sequence (evidence/00839940). The body agrees with the Ego_r layout: CTCFlammable::FirePoints
//   +0x10 (vector<NFire::CFirePoint>), CFirePoint 44 bytes with Resistance +0x0c. It sits right after CTCFlammable::IsOnFire
//   0x00780400. Was the propagated stand-in CMusicManager_00780410::ResetMusicEntriesOffsets.
// Layout: the shared engine/CTCFlammable.h under FABLE_RETAIL_CTCFLAMMABLE_FIREPOINTS (PDef +0x0c, FirePoints +0x10, retail
//   STLport vector); NFire::CFirePoint (no engine header) carries only the Ego_r members this body touches.
// Shape: no frame, the finish pointer re-read into esi each iteration, the loop head aligned, `ret`.
#include "engine/ETCInterfaceType.h"                // engine/CTCFlammable.h names ETCInterfaceType / TCI_FLAMMABLE without including it
#define FABLE_RETAIL_CTCFLAMMABLE_FIREPOINTS
#include "engine/CTCFlammable.h"

namespace NFire {
class CFirePoint { public:                          // Ego_r.pdb CFirePoint, 44 bytes
    unsigned char _pad00[0xc];                      // +0x00 HelperName, HelperIndex, EffectIndex
    long          Resistance;                       // +0x0c
    unsigned char _pad10[0x2c - 0x10];              // +0x10 Burning .. PSingeEffect
};
}

void CTCFlammable::SetOnFire()
{
    for (std::vector<NFire::CFirePoint>::iterator it = FirePoints.begin(); it != FirePoints.end(); ++it)
        it->Resistance = 0;
}
