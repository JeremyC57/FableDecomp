// CTCHeroStats::GetMaxExtraMaxHealth() const -- the hero-stats definition's cap on bonus hit points, read through a counted def-pointer copy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057A90B, 46 bytes.
// Callees: CTCBase::GetDefinitionManager() @ 0x006869D0 (ParentThing's definition holder: the global @ 0x013B8A1C, +0x20),
//          CHeroStatsDef virtual slot 1 (+0x04, destroy when the count drops to zero).
//
// Shape: GetStartingMaxStamina's (0x0057A939) counted copy -- an inline holder accessor returning the def pointer BY
// VALUE, add-ref / release as inline members of the pointee -- with a float result: the value is moved with integer
// registers into the one-word frame (`mov eax,[ecx+0D8h] ... mov [ebp-4],eax`, a named float local copied, not
// computed) and `fld [ebp-4]` after the release. The frame pointer (`push ebp ; mov ebp,esp ... leave`) is the
// size-optimised TU with frame-pointer omission off: #pragma optimize("y", off) + optimize("s", on). Under that
// pragma pair the by-value holder accessor must be `__forceinline` (plain `inline` is called out of line with the
// frame word as its hidden result).
// Layout: retail CHeroStatsDef sits 0x14 below the PDB here: MaxExtraMaxHitPoints +0xD8 (PDB +0xEC).
#pragma optimize("y", off)
#pragma optimize("s", on)

class CHeroStatsDef { public:
    virtual void v00();
    virtual void Destroy();                    // slot 1 (+0x04)
    long          RefCount;                    // +0x04 CDefPointeeBase
    unsigned char _pad_0x08[0xD0];
    float         MaxExtraMaxHitPoints;        // +0xD8 (PDB +0xEC)
    __forceinline void AddRef() { ++RefCount; }
    __forceinline void Release() { if (--RefCount == 0) Destroy(); }
};

struct CHeroStatsDefPointer {                  // CDefPointer<const CHeroStatsDef>
    CHeroStatsDef* Object;
    __forceinline CHeroStatsDefPointer(const CHeroStatsDefPointer& other) : Object(other.Object)
    {
        if (Object)
            Object->AddRef();
    }
    __forceinline ~CHeroStatsDefPointer() { Object->Release(); }
    CHeroStatsDef* operator->() const { return Object; }
};

class CGameDefinitionManager { public:
    unsigned char        _pad_0x00[0xDC];
    CHeroStatsDefPointer PHeroStatsDef;        // +0xDC
    __forceinline CHeroStatsDefPointer GetHeroStatsDef() const { return PHeroStatsDef; }
};

class CTCBase
{
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0
};

struct CTCHeroStats_Retail : public CTCBase {
    void*         __vftable;                   // +0x00
    void*         ParentThing;                 // +0x04 CTCBase

    float GetMaxExtraMaxHealth() const;
};

float CTCHeroStats_Retail::GetMaxExtraMaxHealth() const
{
    CHeroStatsDefPointer def = GetDefinitionManager().GetHeroStatsDef();
    return def->MaxExtraMaxHitPoints;
}