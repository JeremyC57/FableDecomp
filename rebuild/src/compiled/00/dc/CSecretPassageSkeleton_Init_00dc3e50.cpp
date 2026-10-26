// CSecretPassageSkeleton::Init() -- reset the entity script: no skeleton part returned yet, the four ceremonial-skeleton object names (sword, shield, armour, helmet) assigned, collected_all cleared
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DC3E50, 68 bytes (vtable entry at 0x012DA49C of the script's table).
// Callees: CCharString::operator=(const char*) @ 0x0099EFE0 (landed), four times with the literals
// "OBJECT_CEREMONIAL_SKELETON_SWORD" @ 0x012DA51C, "..._SHIELD" @ 0x012DA4F8, "..._ARMOUR" @ 0x012DA4D4,
// "..._HELMET" @ 0x012DA4B0.
//
// Shape: the `returned[4]` clearing loop collapses to ONE dword zero store (`mov dword ptr [esi+1Ch],0`),
// scheduled between the first literal push and its `lea ecx`; then the four `push lit ; lea ecx,[esi+N] ;
// call` string assignments and the closing byte store.
// Layout = Ego_r.pdb CSecretPassageSkeleton (52 bytes): CEntityScriptBase {vptr, Game +4, CScriptThing Me
// +8}, ParentClass +0x14, PMasterData +0x18, bool returned[4] +0x1C, CCharString skeleton_object[4] +0x20,
// bool collected_all +0x30 (no engine header exists for the script classes).

class CCharString { public:
    void* m_pStringData;
    CCharString& operator=(const char* text);    // 0x0099EFE0
};

class CGameScriptInterfaceBase;
struct CQ_OpeningGraveyardSecretPassageScript;
struct CQ_SunnyvaleMasterData;

class CScriptThing { public:                            // 12 bytes: vptr + CCountedPointer PImp
    void* __vftable;
    void* PImp_Data;
    void* PImp_Info;
};

class CEntityScriptBase { public:
    virtual void v00();                          // entity-script vtable
    CGameScriptInterfaceBase* Game;              // +0x04
    CScriptThing              Me;                // +0x08
};

struct CSecretPassageSkeleton : CEntityScriptBase {
    CQ_OpeningGraveyardSecretPassageScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData*                 PMasterData;   // +0x18
    bool                                    returned[4];   // +0x1C
    CCharString                             skeleton_object[4];   // +0x20
    bool                                    collected_all; // +0x30
    void Init();
};

void CSecretPassageSkeleton::Init()
{
    for (long i = 0; i < 4; ++i)
        returned[i] = false;
    skeleton_object[0] = "OBJECT_CEREMONIAL_SKELETON_SWORD";
    skeleton_object[1] = "OBJECT_CEREMONIAL_SKELETON_SHIELD";
    skeleton_object[2] = "OBJECT_CEREMONIAL_SKELETON_ARMOUR";
    skeleton_object[3] = "OBJECT_CEREMONIAL_SKELETON_HELMET";
    collected_all = false;
}