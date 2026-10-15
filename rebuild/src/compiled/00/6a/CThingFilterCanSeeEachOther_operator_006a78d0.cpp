// CThingFilter_CanSeeEachOther::operator()(const CThingCreatureBase& creature) const -- whether the filter's thing and the creature are both creatures that can see each other
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A78D0, length 94 bytes (+ int3 padding).
// Leaf: ??RCThingFilter_CanSeeEachOther
// Callees: the __fastcall visibility test @ 0x008645E0 (viewer in ecx, target in edx, a false flag on the stack), both ways.
// Identity: Ego_r.pdb `??RCThingFilter_CanSeeEachOther@@QBE_NABVCThingCreatureBase@@@Z`.
// Layout (local model): the filter's thing pointer at +0; CThing's type flag byte at +0x6c (bit 4 = a creature).
// Shape: an inline helper on two things: each is down-cast to a creature by the type bit through a null-initialised local
// assigned in an `if` (`xor edi,edi ; test cl,dl ; je ; mov edi,eax`; a ternary becomes branchless `neg ; sbb ; and`), the
// 10h kept in cl for both tests; with both present the helper returns `see(a, b) && see(b, a)` as a full-register 0/1
// (`mov eax,1` / `xor eax,eax`), otherwise the caller's `return false` (`xor al,al`).
struct CThingCreatureBase;

struct CThing {
    unsigned char _pad00[0x6c];
    unsigned char m_typeFlags;                  // +0x6c (bit 4: a creature)
    const CThingCreatureBase* AsCreature() const
    {
        const CThingCreatureBase* creature = 0;
        if (m_typeFlags & 0x10)
            creature = (const CThingCreatureBase*)this;
        return creature;
    }
};

struct CThingCreatureBase : CThing {
};

bool __fastcall CanCreatureSeeCreature(const CThingCreatureBase* viewer, const CThingCreatureBase* target, bool ignoreLight);   // 0x008645E0

inline bool CanSeeEachOther(const CThingCreatureBase* a, const CThingCreatureBase* b)
{
    return CanCreatureSeeCreature(a, b, false) && CanCreatureSeeCreature(b, a, false);
}

struct CThingFilter_CanSeeEachOther {
    const CThing* m_thing;                      // +0x00
    bool operator()(const CThingCreatureBase& creature) const;
};

bool CThingFilter_CanSeeEachOther::operator()(const CThingCreatureBase& creature) const
{
    const CThingCreatureBase* a = m_thing->AsCreature();
    const CThingCreatureBase* b = creature.AsCreature();
    if (a && b)
        return CanSeeEachOther(a, b);
    return false;
}