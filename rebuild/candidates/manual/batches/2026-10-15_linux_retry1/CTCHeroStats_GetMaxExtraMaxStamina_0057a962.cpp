// CTCHeroStats::GetMaxExtraMaxStamina() const -- the hero-stats definition's cap on bonus stamina points, read through a counted def-pointer copy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057A962, 39 bytes.
// Retry (Fable landing session): the sibling-proven by-value accessor form the author left ungraded grades RELOCATION_MATCH.
// Callees: CTCBase::GetDefinitions() @ 0x006869D0 (ParentThing's definition holder: the global @ 0x013B8A1C, +0x20),
//          CHeroStatsDef virtual slot 1 (+0x04, destroy when the count drops to zero).
//
// Shape: the holder's CDefPointer<const CHeroStatsDef> (+0xDC) copied into a local through an inline accessor that
// returns it by value (`test ecx,ecx ; je ; inc dword ptr [ecx+4]`), the cap read into esi, then the release with no
// null test (`dec dword ptr [ecx+4] ; jne ; call [vt+4]`) through inline members of the pointee. Size TU.
// Graded forms: (1) copy ctor by reference + release in the pointer's destructor: `add eax,0DCh` and the `dec`
// hoisted above the read (40v39); (2) raw-pointer constructor + pointee AddRef/Release: right order, object in eax
// (41v39); (3) raw-pointer constructor + destructor release: eax and hoisted `dec` (41v39); (4) reference copy +
// pointee members: only the `add eax,0DCh ; mov ecx,[eax]` remains (40v39).
// Layout: retail CHeroStatsDef sits 0x14 below the PDB here: MaxExtraMaxStaminaPoints +0xDC (PDB +0xF0).
#pragma optimize("s", on)

struct CHeroStatsDef {
    virtual void v00();
    virtual void Destroy();                    // slot 1 (+0x04)
    long          RefCount;                    // +0x04 CDefPointeeBase
    unsigned char _pad_0x08[0xD4];
    long          MaxExtraMaxStaminaPoints;    // +0xDC (PDB +0xF0)
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

struct CDefinitionHolder {
    unsigned char        _pad_0x00[0xDC];
    CHeroStatsDefPointer PHeroStatsDef;        // +0xDC
    CHeroStatsDefPointer GetHeroStatsDef() const { return PHeroStatsDef; }
};

struct CTCHeroStats_Retail {
    void*         __vftable;                   // +0x00
    void*         ParentThing;                 // +0x04 CTCBase

    CDefinitionHolder* GetDefinitions() const; // 0x006869D0
    long GetMaxExtraMaxStamina() const;
};

long CTCHeroStats_Retail::GetMaxExtraMaxStamina() const
{
    CHeroStatsDefPointer def = GetDefinitions()->GetHeroStatsDef();
    return def->MaxExtraMaxStaminaPoints;
}
