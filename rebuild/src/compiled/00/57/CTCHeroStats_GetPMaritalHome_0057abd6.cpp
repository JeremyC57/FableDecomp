// CTCHeroStats::GetPMaritalHome(const CThingAICreature*) const -- the building the hero shares with this spouse (first marriage record naming her whose home still exists as a building), else null
// Leaf: ?GetPMaritalHome@CTCHeroStats@@QBEPBVCThingBuilding@@PBVCThingAICreature@@@Z
// Naming wave 31 (family d3): DECISIONS_W30 D3: GetThingManager is the inherited protected ?GetThingManager@CTCBase@@IBEABVCThingManager@@XZ
//   0x00686AF0 (engine/CTCBase.h retail branch; was the transitional ?GetThingManager@CTCHeroStats_Retail@@QBE...), and the own row is
//   the PDB ?GetPMaritalHome@CTCHeroStats@@QBEPBVCThingBuilding@@PBVCThingAICreature@@@Z (Ego_r.pdb / FableWin.pdb; was the invented
//   ?GetPMaritalHome@CTCHeroStats_Retail@@QBEPAVCThing@@PBV2@@Z)
// Naming wave 30 (family c699): the UID lookup is the const PDB pair CTCBase::GetThingManager() const 0x00686AF0 (const CThingManager&) and
//   ?PeekPThingWithUID@CThingManager@@QBEPBVCThing@@_K@Z 0x0051ED60 (both PDBs); was CWorld::GetThingFromUID through GetWorld()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057ABD6, 83 bytes.
// Callees: CTCBase::GetWorld() @ 0x00686AF0 (returns the world global @ 0x013B8A1C),
//          CWorld::GetThingFromUID(unsigned __int64) @ 0x0051ED60 (thing map at +0x58; manifest label
//          "CStopWatch::GetTicks" is propagated).
//
// Shape: a list walk over Marriages (retail head-node pointer +0x1D8): `node = head->next`, the empty test before the
// spouse argument is loaded into ebp; a 64-bit WifeUID == UniqueID compare (`jne` on each half); the home lookup pushes
// the by-value MaritalHomeUID BEFORE fetching the world (`GetWorld()->GetThingFromUID(uid)`: arguments first, then
// the object expression); a found thing whose type byte (+0x10) is 3 is returned straight from eax, the loop exit
// shares `xor eax,eax`. Size TU: the UID halves are pushed straight from memory (`push dword ptr [esi+14h]`) and the
// register colouring (ebx = this, edi = head, ebp = spouse) follows; the speed form loads through edx/eax and
// colours edi = this.
// Layout: Marriages is +0x1D8 in retail (PDB +0x1F0; see HasMarriageInState 0x0057AC29, landed); CHeroMarriageInfo
// from the PDB (WifeUID +0x00, MaritalHomeUID +0x08 -> node +0x08/+0x10); CThing::UniqueID +0x18 (PDB).
#pragma optimize("s", on)

#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                              // GetThingManager() const 0x00686AF0 (protected)

#include "engine/CThing.h"
class CThingBuilding : public CThing { };     // the PDB return type (single inheritance from CThing)
class CThingAICreature : public CThing { };   // the PDB parameter type

class CThingManager { public:
    const CThing* PeekPThingWithUID(unsigned __int64 uid) const;   // 0x0051ED60 (Ego_r.pdb / FableWin.pdb)
};

struct CHeroMarriageInfoNode {                 // std::list<CHeroMarriageInfo> node
    CHeroMarriageInfoNode* Next;               // +0x00
    CHeroMarriageInfoNode* Prev;               // +0x04
    unsigned __int64       WifeUID;            // +0x08
    unsigned __int64       MaritalHomeUID;     // +0x10
    long                   State;              // +0x18 EMarriageState
    bool                   GayMarriage;        // +0x1C
};

class CTCHeroStats : public CTCBase {          // RTTI .?AVCTCHeroStats@@ : CTCBase
public:
    unsigned char          _pad_0x0c[0x1D8 - 0x0C];
    CHeroMarriageInfoNode* Marriages_Head;     // +0x1D8 (PDB +0x1F0)

    const CThingBuilding* GetPMaritalHome(const CThingAICreature* wife) const;
};

const CThingBuilding* CTCHeroStats::GetPMaritalHome(const CThingAICreature* wife) const
{
    CHeroMarriageInfoNode* head = Marriages_Head;
    for (CHeroMarriageInfoNode* node = head->Next; node != head; node = node->Next) {
        if (node->WifeUID == wife->GetUniqueID()) {
            const CThing* home = GetThingManager().PeekPThingWithUID(node->MaritalHomeUID);
            if (home && home->GetType() == 3)
                return static_cast<const CThingBuilding*>(home);
        }
    }
    return 0;
}