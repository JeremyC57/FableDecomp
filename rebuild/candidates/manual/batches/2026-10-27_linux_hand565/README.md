# Chunk 565 (agent565, 2026-10-29): 10 of 12 never-tried rows land (RELOCATION_MATCH), 2 parked

~CTCAssassinRush 0x007A9D60, NThingShot::CTCShotDefinitionBase::~CTCShotDefinitionBase 0x007BF2B0, ~CTCHealLife 0x007C3F10
(catalog CTCActionUseBuyHouse), CTCGhostSword::MakeSwordsAppear 0x007C5DB0, ~CTCTurncoat 0x007C7960,
CTCFireballSpell::CastFireball 0x007C89F0 (1000 bytes), ~CTCHeroReceiveItems 0x007CA8E0, ~CTCBurntEffect 0x007CDC20,
CPersistContext::Transfer<CDefString> 0x007E39D0 (catalog Transfer<signed_char>), ~CThingAICreature 0x008320C0 (catalog
CTCTavernGame). Every jump table checked entry by entry.

Parked (pending/2026-10-27_linux_hand565/differ):
- 0x007A8BE0 CTCBattleCharge::FrameUpdate, 576v617: over-spanned row, byte-exact over its true 576 bytes; re-split at
  0x007A8E20 and land as is.
- 0x007CBF60 ~CTCMultiArrow, 241v241: ammo-loop address computed before vs after the argument push (lane gap; eight
  forms and the QFE-4035 toolset tried).

NAME_FIXES.tsv: 22 rows, deferred (naming pass): NThingShot::CTCShotDefinitionBase::SetCanBeDeflected 0x007BED50 (TC 0x77
from vtable slot 21), CEnvironmentSpellEffectInterpolator::RadialBlurFadeOut 0x006B82E0, CRightHandedSet::Rotate
0x00A22F80, CDefString::TransferBinaryIn 0x0044FC00, STLport list clear / _M_erase instantiations.
