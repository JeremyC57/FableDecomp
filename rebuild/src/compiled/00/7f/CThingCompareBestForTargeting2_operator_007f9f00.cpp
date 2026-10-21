// CThingCompare_BestForTargeting2::operator()(CThing* a, CThing* b) const -- order two targeting candidates by the vector comparator's value at their positions
// Leaf: ??RCThingCompare_BestForTargeting2
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F9F00, 94 bytes (+ int3 padding).
// Callees: CThingCompare_BestForTargeting2::GetThingCompareMultiplier(CThing*) @ 0x007F9E70 (x2, results discarded:
//          `fstp st(0)`), CThing::GetPosition @ 0x004C73D0 (x2: b first, then a), CVectorCompare_BestForTargeting::
//          GetCompareValueForPoint(const C3DVector&) @ 0x007F9660 (x2, on the comparator member at +4).
// Identity: PDB CThingCompare_BestForTargeting2 (Hero +0, VectorCompare +4, EnemyPreferenceFactor +0x30); the six
//   callers (0x007F9F71..0x007FA243) are the sort helpers of the targeting candidate list.
// Shape: the two multipliers are computed into unused float locals (the multiplication they fed is gone in this build);
//   the positions are references taken b first; both compare values are NAMED float locals (posA's parked in the dead
//   argument slot `[esp+10h]`), then `if (valueA > valueB) return true;` -- `fcomp ; pop edi ; pop esi ; fnstsw ax ;
//   pop ebx` (the inline call-expression compare schedules all three pops before fnstsw); the if-form gives the byte
//   `mov al,1` / `xor al,al` exits.
struct C3DVector;

struct CThing {
    const C3DVector& GetPosition();     // 0x004C73D0
};

struct CVectorCompare_BestForTargeting {
    unsigned char _body[0x2c];
    float GetCompareValueForPoint(const C3DVector& point);     // 0x007F9660
};

struct CThingCompare_BestForTargeting2 {
    CThing* Hero;                                       // +0x00
    CVectorCompare_BestForTargeting VectorCompare;      // +0x04
    float EnemyPreferenceFactor;                        // +0x30
    float GetThingCompareMultiplier(CThing* thing);     // 0x007F9E70
    bool operator()(CThing* a, CThing* b);
};

bool CThingCompare_BestForTargeting2::operator()(CThing* a, CThing* b)
{
    float multiplierA = GetThingCompareMultiplier(a);
    float multiplierB = GetThingCompareMultiplier(b);
    const C3DVector& posB = b->GetPosition();
    const C3DVector& posA = a->GetPosition();
    float valueA = VectorCompare.GetCompareValueForPoint(posA);
    float valueB = VectorCompare.GetCompareValueForPoint(posB);
    if (valueA > valueB)
        return true;
    return false;
}