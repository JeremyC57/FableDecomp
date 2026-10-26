#include "engine/CTCHeroRangedAttacks.h"  // retyped onto the PDB layout; byte parity re-verified
class CDefPointeeBase { public: char pad[0x136]; unsigned char b136; };
bool __fastcall IsUsingProjectileTargetingAnalogueZoom(CTCHeroRangedAttacks* attacks){ return attacks->PHeroCombatDef_Object->b136 ? true : false; }