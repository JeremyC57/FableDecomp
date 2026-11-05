// CThing::IsLevelPersistent() const -- whether the thing survives its level: game-state persisted, or serialised by name, else the LevelPersistent bit
// Leaf: ?IsLevelPersistent@CThing@@QBE_NXZ
// Retry 10b: GameStatePersisted bound to a `const bool&` (a bit-field reference binds a bool TEMPORARY whose address is taken): retail's dead frame byte `mov [esp+7],al` and the separate `mov al,1` exit (`return true`); a plain bool local folds into a test.
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C82A0, 104 bytes (+8 int3). __fastcall, returns bool in al.
// Callees: CCharString::operator!=(const char*) const 0x0099E960 (x2: "" 0x0122D70E, "NULL" 0x012393E4).
// Shape: a thing holding component 199 (bitset word 6 bit 7: `mov al,[esi+38h] ; test al,al ; jns`) is never level
//        persistent; GameStatePersisted (bit 5 of +0x92) bound to a const bool& first (`mov [esp+7],al`) returns true;
//        without KillOnLevelUnload (bit 2 of +0x93) a real SerialisationSectionName (not "" / "NULL") returns true;
//        otherwise the LevelPersistent bit (bit 4 of +0x92) as an unsigned char bitfield (`shr al,4 ; and al,1`).
// Layout: retail CThing = Ego_r.pdb - 0x10 here: UsingComponents +0x20, SerialisationSectionName +0x58, flag bytes +0x91..
//         +0x93 with the PDB bit order.
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_STATEACCESS
#include "engine/CThing.h"

bool CThing::IsLevelPersistent() const
{
    if (HasTC(TCI_COOP_SPIRIT))
        return false;
    const bool& persistent = GameStatePersisted;
    if (persistent)
        return true;
    if (!KillOnLevelUnload && SerialisationSectionName != "" && SerialisationSectionName != "NULL")
        return true;
    return LevelPersistent;
}