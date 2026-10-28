// CThingAICreature::InitialiseFromDef() -- copy the movement speeds and the attack immunity from the creature def, set the speeds up, then give the creature its brain: the overriding brain name when set (forced to BRAIN_STAND_AROUND_LIKE_A_MORON by the debug switch), else the def's BrainDef
// Leaf: ?InitialiseFromDef@CThingAICreature
// Retail: Fable.exe (TLC, VC7.1) @ 0x008338D0, 313 bytes (+7 int3), `ret`.
// Callees: CThingAICreature::InitialiseMovementSpeeds @ 0x00830DD0, CCharString::operator=(const char*) @ 0x0099EFE0
//   ("BRAIN_STAND_AROUND_LIKE_A_MORON" @ 0x01272AC0), CThing::GetDefinitionManager @ 0x004C7990,
//   CDefinitionManager::GetDef<CBrainDef>(long, CDefPointer*) @ 0x0079BD80 and (const CCharString&, CDefPointer*)
//   @ 0x006D3E80 (manifest names propagated instantiations), CThingAICreature::SetNewBrain(CDefPointer<const CBrainDef>)
//   @ 0x00833010 (by value), the def's virtual slot 1 (+4) at a zero count.
// Globals: the debug "every creature stands around" switch byte 0x013B86EA (unnamed in the PDB dump).
// Layout: retail CThingCreatureBase PDef +0xE0; CThingCreatureDef (PDB - 4 up to the bit block, - 8 after):
//   MaxWalkingSpeed +0x90, MaxJoggingSpeed +0x94, MaxRunningSpeed +0x98, MaxFlyingSpeed +0xA0, BrainDef +0xE8,
//   ImmuneToAttack bit 3 of +0x148; CThingAICreature OverridingBrainName +0x190, speeds +0x1B8..+0x1C4, ImmuneToAttack
//   bit 6 of +0x1C8 (the landed ctor 0x00831F80).
// Shape: the bit copy is a masked merge (`shl 3 ; xor ; and 40h ; xor`); the brain def is a CDefPointer local per arm
//   filled by same-TU stand-in GetDefs (no escape: one register copy, the by-value argument built in its stack slot with
//   the copy constructor's add-ref, the local released by its destructor); the name test is the inline IsEmpty(); the
//   GetDef argument goes through an inline const GetCreatureDef() accessor, which makes VC7.1 reload BrainDef after the
//   test (`mov ecx,[eax+0E8h] ; test ; ... ; mov edx,[eax+0E8h]`).
class CCharStringData { public:
    char* Text;                                 // +0
    long  Length;                               // +4
};
class CCharString { public:
    CCharStringData* PData;
    const CCharString& operator=(const char* text);   // 0x0099EFE0
    bool IsEmpty() const { return PData == 0 || PData->Length == 0; }
};
extern bool g_bForceStandAroundBrain;           // 0x013B86EA
class CBrainDef { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_refCount;                            // +0x04
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->m_refCount;
    }
    ~CDefPointer()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Destroy();
    }
};
class CDefinitionManager { public:
    bool GetDef(long globalIndex, CDefPointer<CBrainDef>* out);          // 0x0079BD80
    bool GetDef(const CCharString& name, CDefPointer<CBrainDef>* out);   // 0x006D3E80
};
void* FindDef_CDefinitionManager_GetDef_0();   // stand-in callees of the stand-in bodies below
void* FindDef_CDefinitionManager_GetDef_1();
// Stand-in bodies (the real ones are out of line in retail): they write the out slot only inline, so VC7.1 knows the
// def pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long globalIndex, CDefPointer<CBrainDef>* out)
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
__declspec(noinline) bool CDefinitionManager::GetDef(const CCharString& name, CDefPointer<CBrainDef>* out)
{
    void* def = FindDef_CDefinitionManager_GetDef_1();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

class CThingCreatureDef { public:
    unsigned char _pad00[0x90];
    float MaxWalkingSpeed;                      // +0x90
    float MaxJoggingSpeed;                      // +0x94
    float MaxRunningSpeed;                      // +0x98
    float MaxPushingSpeed;                      // +0x9c
    float MaxFlyingSpeed;                       // +0xa0
    unsigned char _pad0a4[0xe8 - 0xa4];
    long  BrainDef;                             // +0xe8
    unsigned char _pad0ec[0x148 - 0xec];
    bool LeaveDeadCreature : 1;                 // +0x148
    bool AbleToStrafe : 1;
    bool CreateDummyOnDeath : 1;
    bool ImmuneToAttack : 1;                    // bit 3
};
class CThingAICreature { public:
    void*         __vftable;
    unsigned char _pad04[0xe0 - 0x04];
    CThingCreatureDef* PDef;                    // +0xe0 CDefPointer<CThingCreatureDef const>::Object
    const CThingCreatureDef* GetCreatureDef() const { return PDef; }
    unsigned char _pad0e4[0x190 - 0xe4];
    CCharString   OverridingBrainName;          // +0x190
    unsigned char _pad194[0x1b8 - 0x194];
    float MaxWalkingSpeed;                      // +0x1b8
    float MaxJoggingSpeed;                      // +0x1bc
    float MaxRunningSpeed;                      // +0x1c0
    float MaxFlyingSpeed;                       // +0x1c4
    bool PerformingVillageTaskFlag : 1;         // +0x1c8
    bool AISuspended : 1;
    bool HasInformation : 1;
    bool WanderWithInformation : 1;
    bool WaveWithInformation : 1;
    bool ContinueAIWithInformation : 1;
    bool ImmuneToAttack : 1;                    // bit 6
    bool EnableCreatureAutoPlacing : 1;
    CDefinitionManager* GetDefinitionManager(); // 0x004C7990 (CThing)
    void InitialiseMovementSpeeds();            // 0x00830DD0
    void SetNewBrain(CDefPointer<CBrainDef> brainDef);   // 0x00833010
    void InitialiseFromDef();
};

void CThingAICreature::InitialiseFromDef()
{
    MaxWalkingSpeed = PDef->MaxWalkingSpeed;
    MaxJoggingSpeed = PDef->MaxJoggingSpeed;
    MaxRunningSpeed = PDef->MaxRunningSpeed;
    MaxFlyingSpeed = PDef->MaxFlyingSpeed;
    ImmuneToAttack = PDef->ImmuneToAttack;
    InitialiseMovementSpeeds();
    if (g_bForceStandAroundBrain)
        OverridingBrainName = "BRAIN_STAND_AROUND_LIKE_A_MORON";
    if (OverridingBrainName.IsEmpty()) {
        if (PDef->BrainDef) {
            CDefPointer<CBrainDef> brainDef;
            if (GetDefinitionManager()->GetDef(GetCreatureDef()->BrainDef, &brainDef))
                SetNewBrain(brainDef);
        }
    } else {
        CDefPointer<CBrainDef> brainDef;
        GetDefinitionManager()->GetDef(OverridingBrainName, &brainDef);
        SetNewBrain(brainDef);
    }
}