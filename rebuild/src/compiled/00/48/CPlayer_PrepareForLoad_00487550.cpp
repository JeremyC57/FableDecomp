// CPlayer::PrepareForLoad() -- before a saved game loads: clear the player's hero log book, then reset its modes
// Leaf: ?PrepareForLoad@CPlayer@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00487550, 22 bytes, thiscall: `mov ecx,[esi+0xF8] ; call CHeroLogBook::Clear`, then
//   `jmp CPlayer::ResetModes` (tail call).
// Identity: the two callers are the landed CPlayerManager::PrepareForLoad 0x00449C30 (each Players[i]->PrepareForLoad()) and
//   CWorld::PrepareForLoad 0x004A0220 (PlayerManager->GetMainPlayer()->PrepareForLoad()); Ego_r.pdb / FableWin.pdb
//   ?PrepareForLoad@CPlayer@@QAEXXZ (donors 0x00492D40 / 0x01ACC9C0), already the bound spelling in reloc_bindings.tsv.
//   Callees: the landed CHeroLogBook::Clear 0x0047AE50 (Ego_r ?Clear@CHeroLogBook@@QAEXXZ; its source names this row as its
//   only caller) on PHeroLogBook (engine/CPlayer.h +0xF8, CCountedPointer<CHeroLogBook>::Data), and CPlayer::ResetModes
//   0x00634870 (Ego_r ?ResetModes@CPlayer@@QAEXXZ). The manifest "_Dest_val<allocator<pair<long,CMusicManager::
//   CRegisteredMusicEntry>>...>" and the twin 0x00628F10 are other functions.
// Shape: `PHeroLogBook->Clear(); ResetModes();` -- CCountedPointer's operator-> is the plain Data load.
#define CPlayer CPlayerLayout
#include "engine/CPlayer.h"                            // PHeroLogBook_Data +0xF8 (flattened CCountedPointer<CHeroLogBook>)
#undef CPlayer

class CHeroLogBook {
public:
    void Clear();                                       // 0x0047AE50
};

class CPlayer : public CPlayerLayout {
public:
    void PrepareForLoad();
    void ResetModes();                                  // 0x00634870
};

void CPlayer::PrepareForLoad()
{
    PHeroLogBook_Data->Clear();
    ResetModes();
}