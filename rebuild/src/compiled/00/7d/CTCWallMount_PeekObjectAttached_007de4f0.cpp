// CTCWallMount::PeekObjectAttached() const -- the trophy hung on this wall mount (const view)
// Leaf: ?PeekObjectAttached@CTCWallMount@@QBEPBVCThing@@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x007DE4F0, 8 bytes: `add ecx,10h ; jmp 0x00A01B50` (Trophy.GetPItem(), tail call).
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?PeekObjectAttached@CTCWallMount@@QBEPBVCThing@@XZ, second of the Ego_r donor pair
//   GetObjectAttached 0x007DA9A0 / PeekObjectAttached 0x007DA9B0 as retail 0x007DE4F0 follows 0x007DE4E0; its caller is the const
//   valuation CTCBuyableHouse::GetValueOfTrophies (0x006C0088 / 0x006C00A4). The catalog's "_Cons_val<..CParticleMorphs..>" was propagated.
// Callees: ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ 0x00A01B50.
// Header: engine/CTCWallMount.h (new).
// Twin: 0x007DE4E0 GetObjectAttached (same bytes, non-const).
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CTCWallMount.h"
const CThing* CTCWallMount::PeekObjectAttached() const
{
    return Trophy.Get();
}