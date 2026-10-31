// CAIStateGroup_SitForMeal::Update() -- with the eating interface and a live cooking pot, take food from it, then sit on a free chair and keep eating while the state group stays valid, or (no chair) stand around and eat; always terminate the update
// Grade: DIFFER(219v219) -- the HasTC(0x46) creature chain loads into ecx (`mov ecx,[eax+18h] ; test byte [ecx+28h],40h`), retail eax->eax; everything else (incl. relocations) agrees. Tried this pass: PDB GetCreature() reference accessor, bool HasTC (if/return and ?: forms), named creature pointer / const reference, raw bitset expression, `== 0` test; earlier: early-return exits, inline GetCreature.
// Leaf: ?Update@CAIStateGroup_SitForMeal@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00945BD0, length 219 bytes (+ int3 padding). Vtable 0x0128BD34 slot 12.
// Callees (PDB name set Ego_r/FableWin + manifest/vtable): CAIStateGroupBase::Terminate @ 0x008FCF40 (`mov byte
// [ecx+10h],1`), CAIStateGroup_SitForMeal::GetPCookingPot @ 0x00945A90 (ABE), the CIntelligentPointer<CThing>
// converting temporary (RTTI .?AV?$CIntelligentPointer@VCThing@@@@, vtable 0x01238C6C) -- CBaseIntelligentPointer
// ctor @ 0x00A01B10, SetPItem @ 0x00A01B90 (twice), GetPItem @ 0x00A01B50, dtor @ 0x00A01C10;
// GetFoodFromCookingPot @ 0x009456A0, GetPFreeChair @ 0x00945B10, ReleaseControlOfPot @ 0x00945530,
// CAIStateGroup_UsesSeat::SetSeat @ 0x009106E0, CAIStateGroup_UsesSeat::MoveToSeatAndSitDown @ 0x009387C0,
// CAIStateGroupBase::AIAction_DoNothing @ 0x0097E200, own virtual slot 14 IsStateGroupStillValid (+0x38;
// SitForMeal's override is 0x00945610), StandAroundAndEat @ 0x009454C0.
// Shape: inline HasTC(0x46) on GetCreature() (bit 6 of the bitset dword at +0x28) with an early Terminate; the
// IS-alive test of the pot inline; `PPot = pot;` is the converting temporary + inline operator=; the eat loop
// `do { if (AIAction_DoNothing() != 4) break; } while (IsStateGroupStillValid());`.
// Layout: local polymorphic base model (engine/CAIStateGroupBase.h flattens the vptr into data and cannot host
// the virtual slots); CAIBrain from engine/CAIBrain.h (Creature +0x18).
#include "engine/CAIBrain.h"

enum ETCInterfaceType { };
enum EAIActionReturn { };
enum EMovementType { };
enum EScriptAIPriority { };
enum EAIStateGroupType { };

class CCharString;
class CWorld;
class CBaseObject;
class CBaseObjectPointer;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    unsigned char _pad1[0x91 - 0x44];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool HasTC(ETCInterfaceType type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
};
class CThingAICreature : public CThing { };
class CThingObject;

class CBaseIntelligentPointer {                       // vtable 0x0129C95C (vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CAIStateGroupBase {                       // vtable 0x01288C60
public:
    virtual ~CAIStateGroupBase();                                                   // slot 0
    virtual void Initialise(CAIBrain& brain, CThingAICreature& creature, long priority,
                            EScriptAIPriority scriptPriority, const CCharString& name);   // slot 1
    virtual void OnActivate();                                                      // slot 2
    virtual bool InitCreaturePosition();                                            // slot 3
    virtual bool AllowPriorityOverlap();                                            // slot 4
    virtual bool IsStillValid() const;                                              // slot 5
    virtual const CCharString* GetDebugText() const;                                // slot 6
    virtual bool IsTimingCritical() const;                                          // slot 7
    virtual EAIStateGroupType GetStateGroupType() const;                            // slot 8
    void Terminate();                                                               // 0x008FCF40
protected:
    virtual void Cleanup();                                                         // slot 9
    virtual void OnReplace();                                                       // slot 10
    virtual bool IsRunnable();                                                      // slot 11
    virtual void Update() = 0;                                                      // slot 12
    virtual void InitStateGroup(const CWorld& world, CThingAICreature& creature);   // slot 13
    CThingAICreature& GetCreature() const { return *PBrain->Creature; }
    EAIActionReturn AIAction_DoNothing();                                           // 0x0097E200

    CAIBrain*     PBrain;                       // +0x04
    long          ScriptAIPriority;             // +0x08
    void*         Name_PStringData;             // +0x0c
    bool          Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    bool          Enabled;                      // +0x12
};

class CAIStateGroup_UsesSeat : public CAIStateGroupBase {   // vtable 0x0128B53C
protected:
    virtual bool IsStateGroupStillValid() const = 0;          // slot 14
    void SetSeat(CThingObject* seat);                         // 0x009106E0
    bool MoveToSeatAndSitDown(EMovementType movementType);    // 0x009387C0

    CIntelligentPointer<CThingObject> PSeat;    // +0x14
    bool  GotSeatPos;                           // +0x1c
    bool  Sitting;                              // +0x1d
    long  SeatPosIndex;                         // +0x20
    float SeatPos[3];                           // +0x24 C3DVector
    bool  SittingOnBench;                       // +0x30
};

class CAIStateGroup_SitForMeal : public CAIStateGroup_UsesSeat {   // vtable 0x0128BD34
protected:
    virtual void Update();
    virtual bool IsStateGroupStillValid() const;   // 0x00945610
private:
    bool GetFoodFromCookingPot();               // 0x009456A0
    CThing* GetPCookingPot() const;             // 0x00945A90
    CThingObject* GetPFreeChair() const;        // 0x00945B10
    void ReleaseControlOfPot();                 // 0x00945530
    bool StandAroundAndEat();                   // 0x009454C0

    CIntelligentPointer<CThing> PPot;           // +0x34
    long ActionPointIndex;                      // +0x3c
    bool HoldingBowl;                           // +0x40
    long DayLastAte;                            // +0x44
};

void CAIStateGroup_SitForMeal::Update()
{
    if (!GetCreature().HasTC((ETCInterfaceType)0x46)) {
        Terminate();
        return;
    }
    CThing* pot = GetPCookingPot();
    if (pot == 0 || (pot->m_flags91 & 1)) {
        Terminate();
        return;
    }
    PPot = pot;
    if (!GetFoodFromCookingPot()) {
        Terminate();
        return;
    }
    CThingObject* chair = GetPFreeChair();
    if (chair != 0) {
        ReleaseControlOfPot();
        SetSeat(chair);
        if (MoveToSeatAndSitDown((EMovementType)3)) {
            do {
                if (AIAction_DoNothing() != 4)
                    break;
            } while (IsStateGroupStillValid());
        }
    } else {
        StandAroundAndEat();
    }
    Terminate();
}
