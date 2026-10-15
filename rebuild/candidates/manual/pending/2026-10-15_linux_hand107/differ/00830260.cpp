// Grade: DIFFER(84v84) -- scheduling only: the seed local's store `mov [esp+4],eax` is placed before `fld [ecx+0B0h]` where retail stores it after `push ecx ; lea ecx,[esp+8]` (as [esp+8]); everything else identical
// CThingAICreature::RandomiseMovementSpeed(float* speed) -- perturb a movement speed by a random amount up to the def's variation fraction, seeded by the creature's unique id; the sign comes from a second hash of the seed
// Retail: Fable.exe (TLC, VC7.1) @ 0x00830260, 82 bytes. Callees: GFFloatRandom(float range, unsigned long* seed)
// Header: engine/CThingAICreature.h exists but names nothing at these offsets (all `_pad_*`, PDB offsets that
// sit 0x30 above retail at the tail), so the class is modelled locally like the landed 0x00833BA0.
// @ 0x004978A0 and GFROR13(unsigned long) @ 0x00497890 (both __fastcall). Called from 0x00830E74.. (four sites).
// Shape: the seed is a copy of CThing::UniqueID's low word (+0x18) in the `push ecx` frame slot (its address goes
// to GFFloatRandom in ecx, the float range on the stack: `fld [def+0B0h] ; fmul [speed]` = `*speed * variation`).
// The random float is parked in the dead `speed` argument slot across the hash call (a named float local);
// `!(hash & 1)` negates it; the update is `delta + *speed`.
struct CCreatureDef {
    unsigned char _pad00[0xb0];
    float         m_speedVariation;                  // +0xb0
};

float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0
long  __fastcall GFROR13(unsigned long value);                      // 0x00497890

struct CThingAICreature {
    unsigned char _pad00[0x18];
    unsigned long m_uniqueIdLow;                     // +0x18 CThing::UniqueID (low word)
    unsigned char _pad1c[0xe0 - 0x1c];
    CCreatureDef* m_pCreatureDef;                    // +0xe0
    void RandomiseMovementSpeed(float* speed);
};

void CThingAICreature::RandomiseMovementSpeed(float* speed)
{
    unsigned long seed = m_uniqueIdLow;
    float delta = GFFloatRandom(*speed * m_pCreatureDef->m_speedVariation, &seed);
    if (!(GFROR13(seed * 0x24a1 + 0x24df) & 1))
        delta = -delta;
    *speed = delta + *speed;
}
