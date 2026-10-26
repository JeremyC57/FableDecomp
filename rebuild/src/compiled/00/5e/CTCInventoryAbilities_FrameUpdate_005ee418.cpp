// CTCInventoryAbilities::FrameUpdate() -- once the inventory is closed and nothing blocks it, gives the pending abilities tutorial through the hero component
// Leaf: FrameUpdate
// Grade: RELOCATION_MATCH (retry 10, RTM 3077) -- the interface lookup written as the ternary entry pick (`((it == last || type < it->type) ? last : it)->component`, the landed 0x00584157 idiom): the component loads straight into ecx
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EE418, 130 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`).
// Callees: CTCBase::PeekWorld() @ 0x00686A80 (+0xA4 Paused), std::bitset::test @ 0x00410DE0, CVectorMap::LowerBound
//          @ 0x0040F020, CTCHero::GiveTutorialForCategory(long, bool) @ 0x006C9FA0 (result dropped).
// Globals: the player GUI @ 0x013B8790 (+0xFD, an unidentified blocking flag).
// Shape: four nested early-outs (pending flag, InventoryOpen, the GUI flag, the world's Paused byte); the parent's
// presence test plus a `__forceinline` GetTCUnchecked<CTCHero>(0x29) returning the component (loaded straight into
// ecx; the out-pointer GetInterface form goes through eax), key in the `push ecx` frame word, null-tested;
// the pending flag is cleared whenever the checks pass.
// Layout: retail CTCInventoryAbilities WaitingToGiveTutorial +0x140 (PDB +0x158); CTCInventoryBase InventoryOpen
// +0x58 (PDB +0x60); CWorld Paused +0xA4.
#pragma optimize("y", off)
#pragma optimize("s", on)

class CTCBase;

struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    SEntry* LowerBound(const long* type);            // 0x0040F020
};

class CThing { public:
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    bool HasTC(long wanted) { return m_present.test(wanted); }
    template<class T> __forceinline T* GetTCUnchecked(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
};

class CTCHero { public:                                     // interface type 0x29
    bool GiveTutorialForCategory(long category, bool force);   // 0x006C9FA0
};

class CWorld { public:
    unsigned char _pad_0x00[0xA4];
    bool          Paused;                            // +0xA4
};

class CPlayerGui { public:
    unsigned char _pad_0x00[0xFD];
    bool          BlockingTutorials;                 // +0xFD
};

extern CPlayerGui* g_pPlayerGui;                     // 0x013B8790

class CTCBase
{
protected:
    const CWorld& PeekWorld() const;   // 0x00686A80
};
namespace NInventory {
class CTCInventoryBase : public CTCBase
{
public:
};
}

class CTCInventoryAbilities : public NInventory::CTCInventoryBase { public:
    void*         __vftable;                         // +0x00
    CThing*       ParentThing;                       // +0x04 CTCBase
    unsigned char _pad_0x08[0x50];
    bool          InventoryOpen;                     // +0x58 (CTCInventoryBase)
    unsigned char _pad_0x59[0xE7];
    bool          WaitingToGiveTutorial;             // +0x140

    void FrameUpdate();
};

void CTCInventoryAbilities::FrameUpdate()
{
    if (WaitingToGiveTutorial && !InventoryOpen && !g_pPlayerGui->BlockingTutorials && !PeekWorld().Paused) {
        CThing* thing = ParentThing;
        if (thing->HasTC(0x29)) {
            CTCHero* hero = thing->GetTCUnchecked<CTCHero>(0x29);
            if (hero)
                hero->GiveTutorialForCategory(2, false);
        }
        WaitingToGiveTutorial = false;
    }
}