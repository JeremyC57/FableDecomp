// CAIStateGroup_BanditSitRoundCampfire::IsRunnable() -- (at most every 5 s) a bandit with the 0x4a component, outside its village's non-curfew hours, can sit at a live nearby campfire
// Retail: Fable.exe (TLC, VC7.1) @ 0x0093AE80, 313 bytes (virtual, `ret`). Ego_r.pdb: ?IsRunnable@CAIStateGroup_BanditSitRoundCampfire@@UAE_NXZ.
// Callees: CWorld::GetFrame 0x0049D870 (`__fastcall`, no arguments), CVectorMap::LowerBound 0x0040F020 (inlined
// GetInterface 0x23), CTCVillageMember::GetVillage 0x007EE540 (twice; manifest "IS_THING_ALIVE<..>" is propagated),
// CTCVillage::IsSociableTime 0x00658D5D, CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem
// 0x00A01B50 / dtor 0x00A01C10, GetPNearCampfire 0x0093AB70; frames-per-second global 0x01375550.
// Shape: the landed AI state-group model (0x00950A30): two `PCampfire = CIntelligentPointer<CThingObject>(p)`
// temporaries (vtable 0x0125A858; ctor + SetPItem, operator= = SetPItem(GetPItem()), dtor), then IS_THING_ALIVE
// into a named bool (bl preloaded with 1 serves as the dead-bit mask and the result, so the curfew `return false`
// keeps IsSociableTime's zero al); a failure re-arms the check 5 seconds later (the frame rate read into a named
// local before the frame call, held in edi).
// Layout: Ego_r.pdb CAIStateGroup_BanditSitRoundCampfire (PCampfire +0x14, SeatPosIndex +0x1c, GotSeatPos +0x20,
// Sitting +0x21, FrameToNextCheckIsRunnable +0x28).
// Leaf: IsRunnable@CAIStateGroup_BanditSitRoundCampfire
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Naming wave 18: the global stand-in g_framesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
// Name fix (agentNF23): 0x00658D5D is ?IsSociableTime@CTCVillage@@QBE_NXZ (Ego_r 0x0069A63D / FableWin name sets; donor order IsNightTime / IsSociableTime / IsCurfewTime = retail 0x00658D07 / 0x00658D5D / 0x00658DB3, and the body reads CVillageDef SociableStartTime +0xD4 / SociableFinishTime +0xD8, retail STLport offsets of Ego_r 244 / 248); was IsCurfewTime() (non-const; the curfew test is 0x00658DB3).
class CTCBase;
class CThing;
class CWorld;
class CThingAICreature;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020 -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01244344
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem((CBaseObject*)(((CThing*)other.GetPItem()))); return *this; }
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!((CThing*)p.GetPItem())->IsDead())
            return true;
    }
    return false;
}

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void v13();
    virtual long GetTimeToInitiate();           // slot 14 (+0x38)
    virtual void v15(); virtual void v16(); virtual void v17();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CWorld* GetPWorld();                        // 0x008FCFC0 (PBrain->+0x1c->+0x24; manifest "CPlayer::GetCurrentMode" is propagated)
};

class CTCVillage { public:
    bool IsSociableTime() const;                // 0x00658D5D
};
class CTCVillageMember { public:
    CTCVillage* GetVillage();                   // 0x007EE540
};
class CThingObject;

struct CIntelligentPointerThingObject : CBaseIntelligentPointer {   // CIntelligentPointer<CThingObject>, vtable 0x0125A858
    virtual void v00();
    CIntelligentPointerThingObject(CThing* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointerThingObject& operator=(const CIntelligentPointerThingObject& other)
    {
        CThing* item = ((CThing*)other.GetPItem());       // fetched before the member address is formed (GOTCHAS chunk 148)
        SetPItem((CBaseObject*)item);
        return *this;
    }
};

class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};
class CWorld { public: static long __fastcall GetFrame(); };            // 0x0049D870

struct CAIStateGroup_BanditSitRoundCampfire : CAIStateGroupBase {
    CIntelligentPointerThingObject PCampfire;   // +0x14
    long SeatPosIndex;                          // +0x1c
    bool GotSeatPos;                            // +0x20
    bool Sitting;                               // +0x21
    unsigned long FrameToNextSitOnSeat;         // +0x24
    unsigned long FrameToNextCheckIsRunnable;   // +0x28
    bool WasCarryingWeapon;                     // +0x2c
    CThingObject* GetPNearCampfire() const;     // 0x0093AB70
    virtual bool IsRunnable();
};

bool CAIStateGroup_BanditSitRoundCampfire::IsRunnable()
{
    if (FrameToNextCheckIsRunnable > (unsigned long)CWorld::GetFrame())
        return false;
    CThing* creature = PBrain->PCreature;
    if (!creature->HasTC(TCI_BANDIT))
        return false;
    CTCVillageMember* member;
    if (creature->GetTC(TCI_VILLAGE_MEMBER, &member)) {
        if (member->GetVillage() && !member->GetVillage()->IsSociableTime())
            return false;
    }
    GotSeatPos = false;
    Sitting = false;
    SeatPosIndex = -1;
    PCampfire = CIntelligentPointerThingObject(0);
    PCampfire = CIntelligentPointerThingObject((CThing*)GetPNearCampfire());
    bool alive = IS_THING_ALIVE(PCampfire);
    if (!alive) {
        long framesPerSecond = CMainGameComponent::GetConstantFPS();
        FrameToNextCheckIsRunnable = CWorld::GetFrame() + framesPerSecond * 5;
    }
    return alive;
}