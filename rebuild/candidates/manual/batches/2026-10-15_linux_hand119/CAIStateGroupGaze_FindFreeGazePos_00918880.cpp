// CAIStateGroup_Gaze::FindFreeGazePos() -- a free gaze marker of the primary definition (else of the secondary one when it differs), preferring by whether the time of day is inside the creature's sleep window
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00918880, length 116 bytes (+ int3 padding).
// Callees: `this`'s virtual slot 15 (+0x3c), the state group's world accessor @ 0x008FCFC0 (game time at +0x1c),
// CThingAICreature::GetSleepTimes(long*, long*) @ 0x00833390, the game time's IsTimeOfDayInRange(long, long)
// @ 0x006BB390 (thiscall; the manifest's "_global" label is a propagated guess),
// CAIStateGroup_Gaze::FindFreeParticularGazePos(long, bool) @ 0x00918640 (twice).
// Shape: the `bool` flag is a frame byte initialised false (`mov byte ptr [esp+8],0`) and later reloaded as a whole
// dword (`mov edi,[esp+8]`) to be pushed twice; the game time is a named local taken BEFORE the sleep-time call
// (edi); the sleep window lives in two frame longs filled through pointers; the secondary lookup runs only when
// the first returned null and the two definitions differ; the last call's result is returned as is.
// Model: local (engine/CAIStateGroup_Gaze.h flattens the base; member names from it).
struct CThingGameObject;

struct CGameTime {
    bool IsTimeOfDayInRange(long start, long end);          // 0x006BB390
};

struct CWorld {
    unsigned char _pad[0x1c];
    CGameTime* GameTime;                                    // +0x1c
};

struct CThingAICreature {
    void GetSleepTimes(long* start, long* end);             // 0x00833390
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThingAICreature* Creature;                             // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual bool IsStateGroupStillValid();                  // slot 15 (+0x3c)
    CAIBrain* PBrain;                                       // +0x04
    long ScriptAIPriority;                                  // +0x08
    void* Name_PStringData;                                 // +0x0c
    unsigned char Terminated;                               // +0x10
    unsigned char Priority;                                 // +0x11
    unsigned char Enabled;                                  // +0x12
    unsigned char _pad_0x13;
    CWorld* GetPWorld();                                    // 0x008FCFC0
};

struct CAIStateGroup_Gaze : CAIStateGroupBase {
    unsigned char GazeMarker[8];                            // +0x14 CIntelligentPointer<CThingGameObject>
    float ChanceOfRunning;                                  // +0x1c
    float MinJourneyForRunning;                             // +0x20
    float MaxDistanceForCreaturesToLookAt;                  // +0x24
    long PrimaryMarkerDef;                                  // +0x28
    long SecondaryMarkerDef;                                // +0x2c
    CThingGameObject* FindFreeParticularGazePos(long markerDef, bool inSleepTime);   // 0x00918640
    CThingGameObject* FindFreeGazePos();
};

CThingGameObject* CAIStateGroup_Gaze::FindFreeGazePos()
{
    bool inSleepTime = false;
    if (IsStateGroupStillValid()) {
        CGameTime* gameTime = GetPWorld()->GameTime;
        long start, end;
        PBrain->Creature->GetSleepTimes(&start, &end);
        inSleepTime = gameTime->IsTimeOfDayInRange(start, end);
    }
    CThingGameObject* marker = FindFreeParticularGazePos(PrimaryMarkerDef, inSleepTime);
    if (marker == 0 && PrimaryMarkerDef != SecondaryMarkerDef)
        marker = FindFreeParticularGazePos(SecondaryMarkerDef, inSleepTime);
    return marker;
}
