// CTCInventoryExperience::ProcessButtonAReleased() -- base handler, then clear the "add" spend event flags
// Retail 0x0056E5EC, 30 bytes (vtable 0x0124F108 slot 58). Calls the base
// NInventory::CTCInventoryBase::ProcessButtonAReleased (0x005BC65C, a non-virtual qualified call),
// then, when the screen is in its spending state (InventorySelectionState == 1, +0x1C), clears both flag bytes of the
// ExperienceSpendAdd event (+0x1C8 enable, +0x1C9 initiate).
// Layout: the PDB header (engine/CTCInventoryExperience.h) puts ExperienceSpendAdd at +0x20C; retail
// is 0x44 bytes narrower (12-byte STLport vectors/maps), so this file uses a local retail-offset model.
// Size-optimised module (/Os): the two zero byte stores are immediates, not a shared `xor al,al`.
#pragma optimize("s", on)
struct ExperienceSpendEvent {
    bool enable;              // +0x00
    bool initiate;           // +0x01
    unsigned char _pad[2];
    long time;            // +0x04
};

struct CTCInventoryBase {
    void ProcessButtonAReleased();      // 0x005BC65C
};

class CTCInventoryExperience : public CTCInventoryBase { public:
    void* __vftable;                    // +0x000
    unsigned char _pad_04[0x18];
    long InventorySelectionState;       // +0x01C (CTCInventoryBase; 1 = an item is selected)
    unsigned char _pad_20[0x1A8];
    ExperienceSpendEvent ExperienceSpendAdd;    // +0x1C8
    ExperienceSpendEvent ExperienceSpendRemove; // +0x1D0
    void ProcessButtonAReleased();
};

void CTCInventoryExperience::ProcessButtonAReleased()
{
    CTCInventoryBase::ProcessButtonAReleased();
    if (InventorySelectionState == 1) {
        ExperienceSpendAdd.enable = false;
        ExperienceSpendAdd.initiate = false;
    }
}