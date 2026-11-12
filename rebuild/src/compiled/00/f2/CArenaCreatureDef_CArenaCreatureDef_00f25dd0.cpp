// CArenaCreatureDef::CArenaCreatureDef(const CArenaCreatureDef&) -- copy constructor: CDefClassBase(const&), this class's vtable
// 0x012FB358, then the members in declaration order (CreatureType, NumCreatures, HUDType, DeathScore)
// Leaf: ??0CArenaCreatureDef@@QAE@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F25DD0, length 61 bytes, `ret 4`.
// Identity: stores vtable 0x012FB358, the vtable the landed default constructor 0x00F263D0 also stores (slot 0 the landed
//   ??_GCArenaCreatureDef 0x00F25E20, slots 18 / 19 / 20 the landed CArenaCreatureDef::Transfer 0x00F25E70 / Copy 0x00F25F00 /
//   GetSizeofClass 0x00F25E10); reloc_bindings.tsv already binds ??0CArenaCreatureDef@@QAE@ABV0@@Z to 0x00F25DD0 (caller
//   0x00F25D20, the element copy of vector<CArenaCreatureDef>); the member offsets +0x28 / +0x2C / +0x30 / +0x34 are Ego_r.pdb
//   CArenaCreatureDef's (CCharString CreatureType, long NumCreatures, CCharString HUDType, long DeathScore).
// Callees: CDefClassBase::CDefClassBase(const CDefClassBase&) 0x0046644C, CCharString::CCharString(const CCharString&) 0x0099EC30.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CARENACREATUREDEF_IMPLICIT_DTOR
#include "engine/CArenaCreatureDef.h"

CArenaCreatureDef::CArenaCreatureDef(const CArenaCreatureDef& other)
    : CSubDefClassBase(other)
    , CreatureType(other.CreatureType)
    , NumCreatures(other.NumCreatures)
    , HUDType(other.HUDType)
    , DeathScore(other.DeathScore)
{
}
