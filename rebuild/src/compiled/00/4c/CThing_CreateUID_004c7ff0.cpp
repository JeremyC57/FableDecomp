// CThing::CreateUID() -- allocate the thing's unique id from the thing manager for its current map, passing the thing-class "global" flag and whether its def is game-state persisted
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C7FF0, 87 bytes. Callees: CThingManager::AllocateNewThingUID(long, bool, bool)
// @ 0x0051E100 (returns the 64-bit id in edx:eax) on the thing manager @ 0x013B8A1C.
// Shape: PBaseDef retail +0x70, def +0x41 bit 3 (LevelLess); the per-class table at manager +4 holds 20-byte
// records indexed by the thing's class byte at +0x10, flag at record +9. Both bool arguments are built in frame
// bytes and pushed as dwords; UniqueID is the 64-bit member at +0x18.
struct SThingClassInfo {                             // 20-byte record
    unsigned char _pad00[9];
    bool          m_flag;                            // +0x09
    unsigned char _pad0a[20 - 10];
};

class CThingManager { public:
    void*            __vftable;                      // +0x00
    SThingClassInfo* m_pClassInfo;                   // +0x04
    unsigned __int64 AllocateNewThingUID(long mapNumber, bool classFlag, bool levelLess); // 0x0051E100
};

extern CThingManager* g_pThingManager;              // 0x013B8A1C

class CThingBaseDef { public:
    unsigned char _pad00[0x41];
    unsigned char AvailableInEditor : 1;             // +0x41 bit 0
    unsigned char AlwaysFrameUpdate : 1;             // bit 1
    unsigned char StartKillLocked : 1;               // bit 2
    unsigned char LevelLess : 1;                     // bit 3
};

class CThing { public:
    void*            __vftable;                      // +0x00
    unsigned char    _pad04[0x10 - 0x04];
    unsigned char    m_thingClass;                   // +0x10
    unsigned char    _pad11[0x18 - 0x11];
    unsigned __int64 UniqueID;                       // +0x18
    unsigned char    _pad20[0x70 - 0x20];
    CThingBaseDef*   PBaseDef;                       // +0x70 (PDB +0x80)
    unsigned char    _pad74[0x8e - 0x74];
    short            CurrentMapNumber;               // +0x8e (PDB +0x9e)
    void CreateUID();
};

void CThing::CreateUID()
{
    bool levelLess = false;
    if (PBaseDef && PBaseDef->LevelLess)
        levelLess = true;
    bool classFlag = g_pThingManager->m_pClassInfo[m_thingClass].m_flag;
    UniqueID = g_pThingManager->AllocateNewThingUID(CurrentMapNumber, classFlag, levelLess);
}