// CTCDRumble::Create(const C3DVector& pos, float strength, float length, bool globalRumble) -- create a THING_RUMBLE thing for player 4 on the map under the position, set its rumble driver's strength/length/global flag, construct it there and activate it; a thing without the rumble driver is killed; null on any failure
// Leaf: ?Create@CTCDRumble@@SIPAVCThing@@ABVC3DVector@@MM_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F1AC0, 165 bytes (`ret 8` + int3 padding), static `__fastcall` (pos in ecx,
//   globalRumble in dl, the two floats on the stack). FableWin.pdb `?Create@CTCDRumble@@SIPAVCThing@@ABVC3DVector@@MM_N@Z`;
//   callers: the EQuakeStrength/EQuakeLength overload 0x007F1C20, 0x0088ED98, 0x0089FDB2. Catalog label `Create` (_global).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CGameDefinitionManager::Get 0x0044C6B0,
//   CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&) 0x009AD410, CWorldMap::GetMapNumberAt (slot 16)
//   on g_pThingManager (0x013B8A1C) +0x28, CThing::Create(long, long, long, bool) 0x004CA1D0, CTCDRumble::Construct
//   (const C3DVector&) 0x007F18B0 (manifest "CTCDCameraPoint::Construct" is propagated), CThing::DoActivationSequence
//   0x004C9CA0, CThing::Kill(bool) 0x004C9B80.
// Layout: CTCDRumble (Ego_r.pdb, 0x20): PDef +0x0c, Strength +0x10, Length +0x14, GlobalRumble +0x18, LifeTimer +0x1c;
//   the rumble driver is presence word 7 bit 12 of the thing (`test ah,10h` on +0x3c), the driver itself at +0x5c.
// Shape: the landed CTCDExperienceOrb::Create 0x00779E90 with the def index looked up by name first through a NAMED
//   CCharString in its own block (retail pushes `lea eax,[esp+10h]`; a temporary passes the ctor's returned this and
//   moves g_pThingManager into eax) and the three member stores before Construct.
class C3DVector { public: float X, Y, Z; };

class CCharString {
public:
    void* m_pData;
    CCharString(const char* text, long length = -1);           // 0x0099EBF0
    ~CCharString();                                             // 0x0099EAE0
};

class CDefinitionManager {
public:
public:
    long GetDefGlobalIndexFromName(const CCharString&) const;   // 0x009AD410
};

class CGameDefinitionManager : public CDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();           // 0x0044C6B0
};

class CWorldMap {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual long GetMapNumberAt(const C3DVector& pos) const;   // slot 16 (+0x40)
};

class CThingManager {
public:
    unsigned char _pad_0x00[0x28];
    CWorldMap* WorldMap;                                        // +0x28
};
extern CThingManager* g_pThingManager;                          // 0x013B8A1C

class CTCDRumble;

class CThing {
public:
    unsigned char _pad_0x00[0x20];
    unsigned long UsingComponents[9];                           // +0x20 bitset
    unsigned char _pad_0x44[0x5c - 0x44];
    void* PTCDriver;                                            // +0x5c
    static CThing* __fastcall Create(long defIndex, long player, long map, bool levelPersistent);   // 0x004CA1D0
    bool DoActivationSequence();                                // 0x004C9CA0
    void Kill(bool checkLocked);                                // 0x004C9B80
    bool GetTCDriver(CTCDRumble** out)
    {
        if (UsingComponents[7] & 0x1000) {
            *out = (CTCDRumble*)PTCDriver;
            return true;
        }
        return false;
    }
};

class CTCDRumble {
public:
    virtual void v00();
    CThing*       ParentThing;                                  // +0x04
    unsigned char _pad08[4];
    void*         PDef;                                         // +0x0c
    float         Strength;                                     // +0x10
    float         Length;                                       // +0x14
    bool          GlobalRumble;                                 // +0x18
    long          LifeTimer;                                    // +0x1c
    static CThing* __fastcall Create(const C3DVector& pos, float strength, float length, bool globalRumble);
    void Construct(const C3DVector& pos);                       // 0x007F18B0
};

CThing* __fastcall CTCDRumble::Create(const C3DVector& pos, float strength, float length, bool globalRumble)
{
    long defIndex;
    {
        CCharString name("THING_RUMBLE");
        defIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name);
    }
    CThing* thing = CThing::Create(defIndex, 4, g_pThingManager->WorldMap->GetMapNumberAt(pos), true);
    CThing* result = 0;
    if (thing) {
        CTCDRumble* rumble;
        if (thing->GetTCDriver(&rumble)) {
            rumble->Strength = strength;
            rumble->Length = length;
            rumble->GlobalRumble = globalRumble;
            rumble->Construct(pos);
            if (thing->DoActivationSequence())
                result = thing;
        }
        else
            thing->Kill(false);
    }
    return result;
}