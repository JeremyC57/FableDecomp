// CTCHeroStats::GetStartingMaxStamina() const -- the stamina cap at magic-power level 0 from the hero-stats definition, read through a counted def-pointer copy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057A939, 41 bytes.
// Callees: CTCBase::GetDefinitions() @ 0x006869D0 (ParentThing's definition holder: the global @ 0x013B8A1C, +0x20),
//          CHeroStatsDef virtual slot 1 (+0x04, destroy when the count drops to zero).
//
// Shape: the holder's CDefPointer<const CHeroStatsDef> (+0xDC) copied into a local (`test ecx,ecx ; je ; inc dword
// ptr [ecx+4]` = the copy constructor's add-ref), MaxStaminaPointsPerMagicPowerLevel[0] read into esi (`push esi`
// after the add-ref), then the local's destructor releases with no null test (`dec dword ptr [ecx+4] ; jne ;
// call [vt+4]`). Size TU (#pragma optimize("s", on)). Two details carry the match: the add-ref / release are inline
// members of the pointee (a release written in the pointer's destructor schedules the `dec` ahead of the value
// read), and the copy comes from an inline holder accessor returning the def pointer BY VALUE
// (`GetDefinitions()->GetHeroStatsDef()`): copying the member directly binds a reference and emits
// `add eax,0DCh ; mov ecx,[eax]` (size) or a dead `add eax,0DCh` (speed) where retail loads `mov ecx,[eax+0DCh]`.
// Layout: retail CHeroStatsDef sits 0x2C below the PDB at this vector (smaller definition head, 12-byte vectors):
// MaxStaminaPointsPerMagicPowerLevel's first pointer +0x114 (PDB +0x134).

#pragma optimize("s", on)

struct CHeroStatsDef {
    virtual void v00();
    virtual void Destroy();                    // slot 1 (+0x04)
    long          RefCount;                    // +0x04 CDefPointeeBase
    unsigned char _pad_0x08[0x10C];
    long*         MaxStaminaPointsPerMagicPowerLevel;   // +0x114 vector<long>::_First (PDB +0x134)
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
    long GetStartingMaxStamina() const;
};

long CTCHeroStats_Retail::GetStartingMaxStamina() const
{
    CHeroStatsDefPointer def = GetDefinitions()->GetHeroStatsDef();
    return def->MaxStaminaPointsPerMagicPowerLevel[0];
}
