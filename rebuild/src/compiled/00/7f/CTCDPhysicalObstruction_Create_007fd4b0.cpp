// CTCDPhysicalObstruction::Create(const C3DVector& startPos, const C3DVector& endPos, float radius) -- create a THING_PHYSICAL_OBSTRUCTION thing for player 4 on the map under the start, give its obstruction driver the line and radius, construct it there and activate it; a thing without the driver is killed; null on any failure
// Leaf: ?Create@CTCDPhysicalObstruction@@SIPAVCThing@@ABVC3DVector@@0M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007FD4B0, 205 bytes (`ret 4` + int3 padding), static `__fastcall` (startPos in ecx,
//   endPos in edx, radius on the stack). Ego_r.pdb `?Create@CTCDPhysicalObstruction@@SIPAVCThing@@ABVC3DVector@@0M@Z`
//   (0xd0 bytes there too); 17 callers (CTCKrakenTentacle::PerformStrikeDamage 0x007F11D0, CTCTrapBase::Trigger, ...).
//   Catalog label `Create` (_global).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CGameDefinitionManager::Get 0x0044C6B0,
//   CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410, CWorldMap::GetMapNumberAt (slot 16) on g_pThingManager
//   (0x013B8A1C) +0x28, CThing::Create(long, long, long, bool) 0x004CA1D0, CTCDPhysicalObstruction::Construct
//   (const C3DVector&) 0x007FD430 (manifest "CTCDCameraPoint::Construct" is propagated), CThing::DoActivationSequence
//   0x004C9CA0, CThing::Kill(bool) 0x004C9B80.
// Layout: CTCDPhysicalObstruction (Ego_r.pdb): StartPos +0x0c, EndPos +0x18, LifeTimer +0x24, Radius +0x28; the driver is
//   presence word 7 bit 13 of the thing (`test ah,20h` on +0x3c), the driver itself at +0x5c.
// Shape: the CTCDRumble::Create 0x007F1AC0 / CTCDExperienceOrb::Create 0x00779E90 pattern (named CCharString in its own
//   block, `result` local), with the inline SetParams (Ego_r 0x007F86A0) storing the two vectors and the radius.
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

class CCharString {
public:
    void* m_pData;
    CCharString(const char* text, long length = -1);           // 0x0099EBF0
    ~CCharString();                                             // 0x0099EAE0
};

class CDefinitionManager {
public:
    long GetDefGlobalIndexFromName(const CCharString& name);    // 0x009AD410
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

class CTCDPhysicalObstruction;

class CThing {
public:
    unsigned char _pad_0x00[0x20];
    unsigned long UsingComponents[9];                           // +0x20 bitset
    unsigned char _pad_0x44[0x5c - 0x44];
    void* PTCDriver;                                            // +0x5c
    static CThing* __fastcall Create(long defIndex, long player, long map, bool levelPersistent);   // 0x004CA1D0
    bool DoActivationSequence();                                // 0x004C9CA0
    void Kill(bool checkLocked);                                // 0x004C9B80
    bool GetTCDriver(CTCDPhysicalObstruction** out)
    {
        if (UsingComponents[7] & 0x2000) {
            *out = (CTCDPhysicalObstruction*)PTCDriver;
            return true;
        }
        return false;
    }
};

class CTCDPhysicalObstruction {
public:
    virtual void v00();
    CThing*       ParentThing;                                  // +0x04
    unsigned char _pad08[4];
    C3DVector     StartPos;                                     // +0x0c
    C3DVector     EndPos;                                       // +0x18
    long          LifeTimer;                                    // +0x24
    float         Radius;                                       // +0x28
    void SetParams(const C3DVector& startPos, const C3DVector& endPos, float radius)
    {
        StartPos = startPos;
        EndPos = endPos;
        Radius = radius;
    }
    static CThing* __fastcall Create(const C3DVector& startPos, const C3DVector& endPos, float radius);
    void Construct(const C3DVector& pos);                       // 0x007FD430
};

CThing* __fastcall CTCDPhysicalObstruction::Create(const C3DVector& startPos, const C3DVector& endPos, float radius)
{
    long defIndex;
    {
        CCharString name("THING_PHYSICAL_OBSTRUCTION");
        defIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name);
    }
    CThing* thing = CThing::Create(defIndex, 4, g_pThingManager->WorldMap->GetMapNumberAt(startPos), true);
    CThing* result = 0;
    if (thing) {
        CTCDPhysicalObstruction* obstruction;
        if (thing->GetTCDriver(&obstruction)) {
            obstruction->SetParams(startPos, endPos, radius);
            obstruction->Construct(startPos);
            if (thing->DoActivationSequence())
                result = thing;
        }
        else
            thing->Kill(false);
    }
    return result;
}