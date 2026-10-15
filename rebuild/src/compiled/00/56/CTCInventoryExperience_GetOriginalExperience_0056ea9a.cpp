// CTCInventoryExperience::GetOriginalExperience(long pool) -- the experience a pool held when spending began
// Retail 0x0056EA9A, 60 bytes, ret 4. Pools 0..3 = General, Strength, Skill, Will
// (OrigGeneral..OrigWill at retail +0x1AC..+0x1B8; the PDB header has them at +0x1F0, retail is 0x44
// bytes narrower ahead of them -- 12-byte STLport containers -- so a local retail-offset model is used),
// anything else 0. An if/else chain (`test ; jne`, then `cmp eax,N ; jne`) with every arm jumping to
// the shared `ret 4`.
#pragma optimize("s", on)

struct CTCInventoryExperience {
    unsigned char _pad_000[0x1AC];
    long OrigGeneral;           // +0x1AC
    long OrigStrength;          // +0x1B0
    long OrigSkill;             // +0x1B4
    long OrigWill;              // +0x1B8
    long GetOriginalExperience(long pool);
};

long CTCInventoryExperience::GetOriginalExperience(long pool)
{
    if (pool == 0)
        return OrigGeneral;
    else if (pool == 1)
        return OrigStrength;
    else if (pool == 2)
        return OrigSkill;
    else if (pool == 3)
        return OrigWill;
    return 0;
}