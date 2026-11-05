// CThing::IsGameStatePersisted() const -- whether this thing's state is saved with the game (never for co-op spirits)
// Leaf: IsGameStatePersisted@CThing
// Retail: Fable.exe @ 0x004C8280, 22 bytes; Ego_r.pdb ?IsGameStatePersisted@CThing@@QBE_NXZ.
// The first test is HasTC(0xc7): bit 199 of UsingComponents is bit 7 of byte +0x38 (`test al,al; jns`); 0xc7 is the
// interface type CTCCoopSpirit's slot-21 getter returns.
// Layout: retail CThing: UsingComponents +0x20 as in Ego_r.pdb; the flag bytes sit 0x10 lower (GameStatePersisted +0x92 bit 5,
// PDB +0xa2 bit 5; Dead +0x91 bit 0 as in the hero rows).
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#include "engine/CThing.h"
bool CThing::IsGameStatePersisted() const
{
    if (HasTC(TCI_COOP_SPIRIT))
        return false;
    return GameStatePersisted;
}