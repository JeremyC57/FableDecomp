// CAIStateGroup_BanditPlayDice::Update() -- claim the free dice-game position, walk to it, join the game and face it; put the weapon away while playing, then on leaving free the place, clear the current player and draw the weapon again
// Grade: DIFFER(506v507) nd 77->63 -- lever: the LookAt sum written GetPosition() + Forward (retail calls GetOrientation first). Residue: frame 0x110 v 0x120 (retail action temporaries larger/8-aligned), ++NumberOfPlayers folded to inc [mem] (retail load/inc/store), DiceGamePosition temp registers. Tried a named orientation ref (108), a players load/store pair (59 but 510v507). Next: the action temporary sizes
// Leaf: Update@CAIStateGroup_BanditPlayDice
#include "engine/CAIStateGroup_BanditPlayDice.h"
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
struct CThingPlacement {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual const CRightHandedSet& GetOrientation() const;   // slot 72 (+0x120)
};
struct CThing {
    unsigned char _pad[0x60];
    CThingPlacement* Placement;                    // +0x60 (PDB PhysicsTC +0x70; retail CThing is 0x10 smaller)
    const C3DVector& GetPosition() const;          // 0x004C73D0
};
struct CThingCreatureBase : CThing { };
struct CAIBrain { unsigned char _pad[0x18]; CThingCreatureBase* PCreature; };
bool __fastcall IsThingCarryingWeapon(CThing* thing);   // 0x006A1540
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                      // 0x00A01B50
};
struct CTCExclusiveUse {
    void* __vftable;
    CThing* Owner;                                 // +0x04
    unsigned char _pad08[0x4];
    CBaseIntelligentPointer User;                  // +0x0c
    void SetUser(CThing* user);                    // 0x00916640
};
struct CTCDiceGame {
    unsigned char _pad[0xc];
    long NumberOfPlayers;                          // +0x0c
    CBaseIntelligentPointer CurrentPlayer;         // +0x10
    void SetCurrentPlayer(CThing* player);         // 0x00929550
};
struct CCreatureActionBase { unsigned char _body[0xa8]; };
struct CCreatureAction_SheatheWeapons : CCreatureActionBase {
    unsigned char _own[0x58]; __int64 _aligned8;   // retail places the temporary 8-aligned (+0x18)
    CCreatureAction_SheatheWeapons(CThingCreatureBase* creature);   // 0x008CC370
    ~CCreatureAction_SheatheWeapons();             // 0x0062E900 (folded)
};
struct CCreatureAction_UnsheatheWeapons : CCreatureActionBase {
    unsigned char _own[0x58]; __int64 _aligned8;   // retail places the temporary 8-aligned (+0x18)
    CCreatureAction_UnsheatheWeapons(CThingCreatureBase* creature); // 0x008B8590
    ~CCreatureAction_UnsheatheWeapons();           // 0x0062E900 (folded)
};
enum EAIActionReturn { AI_ACTION_RETURN_0, AI_ACTION_RETURN_1, AI_ACTION_DONE };
struct CDiceDispatch {                             // the state group's own vtable (0x0128AD54)
    virtual void s00(); virtual void s01(); virtual void s02(); virtual void s03(); virtual void s04(); virtual void s05();
    virtual void s06(); virtual void s07(); virtual void s08(); virtual void s09(); virtual void s10(); virtual void s11();
    virtual void s12(); virtual void s13(); virtual void s14(); virtual void s15();
    virtual bool PutWeaponAway();                  // slot 16 (+0x40)
    virtual EAIActionReturn Act();                 // slot 17 (+0x44)
};
struct DiceMethods : CAIStateGroup_BanditPlayDice {
    void Update();
    void Terminate();                              // 0x008FCF40
    long AIAction_MoveToPos(long repeat, C3DVector pos, bool* reached, long movement, bool a, bool b, CThing* target, bool c, float d);   // 0x0097EC00
    long AIAction_LookAtPos(long repeat, C3DVector pos, long frames, float speed);   // 0x0097D580
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);       // 0x0097DB70
    __forceinline CThingCreatureBase* GetCreature() { return (*(CAIBrain**)_pad_0x04)->PCreature; }
};
void DiceMethods::Update()
{
    WasCarryingWeapon = false;
    if (DiceGamePosition->User.GetPItem()) {
        DiceGame = 0;
        Terminate();
        return;
    }
    DiceGamePosition->SetUser(GetCreature());
    bool reached;
    if (AIAction_MoveToPos(1, DiceGamePosition->Owner->GetPosition(), &reached, 3, true, true, 0, true, 0.0f) != AI_ACTION_DONE) {
        DiceGamePosition->SetUser(0);
        DiceGame = 0;
        Terminate();
        return;
    }
    ++DiceGame->NumberOfPlayers;
    AIAction_LookAtPos(1, DiceGamePosition->Owner->GetPosition() + DiceGamePosition->Owner->Placement->GetOrientation().Forward, 0x11, 0.0027777f);
    WasCarryingWeapon = ((CDiceDispatch*)this)->PutWeaponAway() && IsThingCarryingWeapon(GetCreature());
    if (WasCarryingWeapon && AIAction_PerformActionAndWait(&CCreatureAction_SheatheWeapons(GetCreature()), false) != AI_ACTION_DONE) {
        Terminate();
        return;
    }
    ((CDiceDispatch*)this)->Act();
    --DiceGame->NumberOfPlayers;
    if (DiceGame->CurrentPlayer.GetPItem() == GetCreature())
        DiceGame->SetCurrentPlayer(0);
    DiceGamePosition->SetUser(0);
    DiceGame = 0;
    if (!(WasCarryingWeapon && AIAction_PerformActionAndWait(&CCreatureAction_UnsheatheWeapons(GetCreature()), false) != AI_ACTION_DONE))
        WasCarryingWeapon = false;
    Terminate();
}
