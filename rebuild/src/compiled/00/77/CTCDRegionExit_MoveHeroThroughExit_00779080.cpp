// CTCDRegionExit::MoveHeroThroughExit() -- when the hero is alive, flag the world and hand it a move-hero-to-region game event (0x2c) carrying the connected entrance's position and heading and three cleared flags, stamped with the hero's player
// Leaf: MoveHeroThroughExit
// Flags: /GS
//
// Retail 0x00779080, 359 bytes (plain `ret`, /GS cookie 0x0139C8A8 checked by __security_check_cookie @ 0x00BFE9F9).
// Callees: CTCBase::GetPlayerManager @ 0x00686D40, CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetPControlledCreature @ 0x00487DC0,
// CTCBase::GetWorld @ 0x00686A70 (twice), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (four times), CThing::GetPos @
// 0x004C73D0 (three times), the physics component's virtual GetAngleXY (slot 63, +0xfc; CTCPhysicsStandard's is an fpatan),
// CMemoryBuffer::Copy @ 0x009F1760 (four floats, three bools), CWorld::HandleMoveHeroToRegionGameEvent @ 0x0049EAF0.
// Shape: the landed CGameEvent model (0x0077FB00 / 0x0077D4B0): the event is built after the world flag store; every Add<T> is
// an inline `Copy(sizeof value, &value)` on its by-value parameter (the floats share one slot, the bools another). The heading goes
// through an inline CThing::GetAngleXY() wrapper over the physics component: spelled out at the call site, VC7.1 loads the
// component into eax and copies it to ecx (`mov eax,[eax+60h] ; mov edx,[eax] ; mov ecx,eax`), which also reshuffles the tail.
// Layout = Ego_r.pdb CTCDRegionExit (packed): PEntranceConnectedTo +0x2a; CThing physics component +0x60 (PDB PhysicsTC +0x70:
// the retail component map and update list are 0x10 smaller), Player byte +0x90, Dead bit 0 of +0x91; CWorld flag byte +0x12f.
class C3DVector { public: float X, Y, Z; };
class CTCPhysicsBase { public:
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
    virtual void v60(); virtual void v61(); virtual void v62();
    virtual float GetAngleXY() const;                   // slot 63 (+0xfc)
};
class CThing {
public:
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
    unsigned char _pad64[0x90 - 0x64];
    signed char Player;                                 // +0x90
    unsigned char Dead : 1;                             // +0x91 bit 0
    const C3DVector& GetPos() const;               // 0x004C73D0
    float GetAngleXY() const { return PhysicsTC->GetAngleXY(); }
};
class CThingPlayerCreature : public CThing { };
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                             // 0x00487DC0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data);  // 0x009F1760
};
class CGameEvent : public CMemoryBuffer { public:
    long          Type;                                 // +0x00
    signed char   Player;                               // +0x04
    unsigned char Data[32];                             // +0x05
    unsigned char EndPos;                               // +0x25
    bool          Valid;                                // +0x26
    bool          Replacement;                          // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddFLOAT(float value) { Copy(4, &value); }
    void AddBOOL(bool value) { Copy(1, &value); }
};
class CWorld { public:
    unsigned char _pad00[0x12f];
    bool          HeroMovingThroughRegionExit;          // +0x12f
    void HandleMoveHeroToRegionGameEvent(const CGameEvent& event);   // 0x0049EAF0
};

class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
protected:
    CPlayerManager& GetPlayerManager();   // 0x00686D40
    CWorld& GetWorld() const;   // 0x00686A70
};
#pragma pack(push, 1)
class CTCDRegionExit : public CTCBase {
public:
    bool  Active;                                       // +0x0c
    bool  InLetterBoxMode;                              // +0x0d
    bool  WaitingForConfirmation;                       // +0x0e
    bool  WaitingForActivationOrPlayerToLeave;          // +0x0f
    bool  WaitingForPlayerToLeaveMessageArea;           // +0x10
    unsigned char _pad11[3];
    float Radius;                                       // +0x14
    float MessageRadius;                                // +0x18
    unsigned char _pad1c[4];
    unsigned __int64 EntranceConnectedToUID;            // +0x20
    bool  ReversedOnMiniMap;                            // +0x28
    bool  HiddenOnMiniMap;                              // +0x29
    CIntelligentPointer<CThing> PEntranceConnectedTo;   // +0x2a
    void MoveHeroThroughExit();
};
#pragma pack(pop)

void CTCDRegionExit::MoveHeroThroughExit()
{
    CThing* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    if (hero && !hero->Dead) {
        GetWorld().HeroMovingThroughRegionExit = true;
        CGameEvent event(0x2c);
        event.AddFLOAT(PEntranceConnectedTo.GetPItem()->GetPos().X);
        event.AddFLOAT(PEntranceConnectedTo.GetPItem()->GetPos().Y);
        event.AddFLOAT(PEntranceConnectedTo.GetPItem()->GetPos().Z);
        event.AddFLOAT(PEntranceConnectedTo.GetPItem()->GetAngleXY());
        event.AddBOOL(false);
        event.AddBOOL(false);
        event.AddBOOL(false);
        event.Player = hero->Player;
        GetWorld().HandleMoveHeroToRegionGameEvent(event);
    }
}