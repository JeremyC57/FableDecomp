// CTCInventoryExperience::ProcessButtonXReleased() -- base handler, then clear the "remove" spend event flags
// Retail 0x0056E60F, 30 bytes (vtable 0x0124F108 slot 60; the manifest label ProcessButtonAReleased is propagated from slot 58 -- the name here follows the slot order 54 AHeld, 56 XHeld, 58 AReleased, 59 BReleased, 60 = this). Calls the base
// the base slot-60 handler (0x005BC68F, a non-virtual qualified call),
// then, when the screen is in its spending state (InventorySelectionState == 1, +0x1C), clears both flag bytes of the
// ExperienceSpendRemove event (+0x1D0 enable, +0x1D1 initiate).
// Layout: the PDB header (engine/CTCInventoryExperience.h) puts ExperienceSpendAdd at +0x20C; retail
// is 0x44 bytes narrower (12-byte STLport vectors/maps), so this file uses a local retail-offset model.
// Size-optimised module (/Os): the two zero byte stores are immediates, not a shared `xor al,al`.
// Leaf: ProcessButtonXReleased
#pragma optimize("s", on)
struct ExperienceSpendEvent {
    bool enable;              // +0x00
    bool initiate;           // +0x01
    unsigned char _pad[2];
    long time;            // +0x04
};

struct CTCInventoryBase {
    void ProcessButtonXReleased();      // 0x005BC68F
};

class CTCInventoryExperience : public CTCInventoryBase { public:
    void* __vftable;                    // +0x000
    unsigned char _pad_04[0x18];
    long InventorySelectionState;       // +0x01C (CTCInventoryBase; 1 = an item is selected)
    unsigned char _pad_20[0x1A8];
    ExperienceSpendEvent ExperienceSpendAdd;    // +0x1C8
    ExperienceSpendEvent ExperienceSpendRemove; // +0x1D0
    void ProcessButtonXReleased();
};

void CTCInventoryExperience::ProcessButtonXReleased()
{
    CTCInventoryBase::ProcessButtonXReleased();
    if (InventorySelectionState == 1) {
        ExperienceSpendRemove.enable = false;
        ExperienceSpendRemove.initiate = false;
    }
}