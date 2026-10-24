// IsHitToDaze(const CThingCreatureBase&) -- a hit dazes when the creature's health is below its dazed threshold, it has a decapitation component (0xD1) and it is killable
// Leaf: ?IsHitToDaze@?A0x
// (Leaf is a prefix: the anonymous-namespace hash varies with the file path; it excludes the Call... wrapper.)
// Retail 0x00865E40, 0x2F bytes (+ int3 padding), __fastcall (creature in ecx). PDB (Ego_r): ?IsHitToDaze@?A0x2d832f72@@YI_NABVCThingCreatureBase@@@Z
// (a file-static of the creature combat-response TU, the same anonymous namespace as IsKnockdownToStun 0x00865AC0).
// Callee: CThingCreatureBase::GetDazedHealthThreshold() const @ 0x00662090 (`fild` of the creature def's +0x84 long).
// Layout (retail): CThing UsingComponents bitset +0x20 -- interface type 0xD1 = CTCDecapitation (GetInterfaceType, vtable
// slot 21) is word 6 (+0x38) bit 17; CThingPhysical Health +0xB4, Killable bit 1 of +0xBC (Ego_r +0xCC / +0xD4 less the 0x10
// the retail CThing saves). Modelled locally: the engine CThingPhysical.h keeps the unshifted Ego_r offsets there.
// Shape: `Health < GetDazedHealthThreshold()` (the call first, then `fcomp [esi+0B4h] ; test ah,41h ; jne`), HasTC(0xD1),
// the Killable bit; three guards share the `xor al,al` exit.
class CThingCreatureBase
{
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                  // +0x20 UsingComponents bitset<274>
    unsigned char _pad44[0xb4 - 0x44];
    float         Health;                            // +0xB4
    unsigned char _padb8[0xbc - 0xb8];
    unsigned char Damageable : 1;                    // +0xBC bit 0
    unsigned char Killable : 1;                      // +0xBC bit 1

    float GetDazedHealthThreshold() const;           // 0x00662090
    bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
};

#pragma auto_inline(off)
namespace {
bool __fastcall IsHitToDaze(const CThingCreatureBase& creature)
{
    if (creature.Health < creature.GetDazedHealthThreshold() && creature.HasTC(0xD1) && creature.Killable)
        return true;
    return false;
}
}
#pragma auto_inline(on)

bool __fastcall CallIsHitToDaze(const CThingCreatureBase& creature)
{
    return IsHitToDaze(creature);
}