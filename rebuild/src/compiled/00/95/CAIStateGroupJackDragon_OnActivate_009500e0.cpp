// CAIStateGroup_JackDragon::OnActivate() -- preload the dragon's anims, enable boss phase changes, switch gravity off, latch the five arena markers by script name, reset the state and timers, make the dragon targetable, then (landed marker alive with physics) place the dragon on the landed marker facing its heading, and set every fixed flight / ground / strafe position of the fight (the dive-in point from the landed marker plus the two rotated offsets and the def's ground height bodge); finally reset the dragon onto the first dive-in point
// Leaf: ?OnActivate@CAIStateGroup_JackDragon@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009500E0, length 2154 bytes.
// Callees: CThingCreatureBase::PreloadAnims @ 0x00663140, CVectorMap::LowerBound @ 0x0040F020 (inlined GetTC: TCI_BOSS 173,
// TCI_TARGETED 9), CCharString(const char*, long) @ 0x0099EBF0 / copy @ 0x0099EC30 / dtor @ 0x0099EAE0,
// CAIStateGroupBase::PeekThingSearchTools @ 0x008FD000, CThingSearchTools::GetPFirstThing<CThingFilter_HasScriptName, CThing>
// @ 0x0056D050, CBaseIntelligentPointer ctor/GetPItem/SetPItem/dtor @ 0x00A01B10/0x00A01B50/0x00A01B90/0x00A01C10,
// CAIStateGroupBase::GetWorld @ 0x008FCFC0, CWorld::GetFrame @ 0x0049D870 (static fastcall), CDefClassBase::GetSubDef<CJackDragonDef>
// @ 0x008E3300, C3DVector::RotateXY @ 0x00A14040, CThing::GetPos @ 0x004C73D0; physics TC virtual slots 30/31/38/63/66.
// Layout: Ego_r.pdb CAIStateGroup_JackDragon (packed, 0x140); CTCBoss::EnablePhaseChanges +0x21; CTCTargeted::Targetable +0x10;
// CJackDragonDef::DragonGroundHeightOffsetBodge +0x50.
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CSubDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/C3DVector.h"

class CJackDragonDef : public CSubDefClassBase {                    // RTTI .?AVCJackDragonDef@@ : CSubDefClassBase
public:
    static CCharString __fastcall StatGetTypeName() { return CCharString("CJackDragonDef", -1); }
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    unsigned char _pad[0x50 - sizeof(CSubDefClassBase)];
    float DragonGroundHeightOffsetBodge;                            // +0x50
};
class CDefinitionManager {
    friend class CDefClassBase;
protected:
    CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long index);   // 0x009ADA10
};
// Same-TU body of the retail GetSubDef<CJackDragonDef> instance (0x008E3300, the landed body): with it VC7.1 knows the def
// pointer's frame word is not captured by the call.
template <>
__declspec(noinline) bool CDefClassBase::GetSubDef<CJackDragonDef>(CDefPointer<const CJackDragonDef>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(CJackDragonDef::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}

class CTCPhysicsBase {
public:
    virtual void s00();
    virtual void s01();
    virtual void s02();
    virtual void s03();
    virtual void s04();
    virtual void s05();
    virtual void s06();
    virtual void s07();
    virtual void s08();
    virtual void s09();
    virtual void s10();
    virtual void s11();
    virtual void s12();
    virtual void s13();
    virtual void s14();
    virtual void s15();
    virtual void s16();
    virtual void s17();
    virtual void s18();
    virtual void s19();
    virtual void s20();
    virtual void s21();
    virtual void s22();
    virtual void s23();
    virtual void s24();
    virtual void s25();
    virtual void s26();
    virtual void s27();
    virtual void s28();
    virtual void s29();
    virtual void SetPosition(const C3DVector& pos);   // slot 30 (+0x78)
    virtual void ResetPosition(const C3DVector& pos);   // slot 31 (+0x7c)
    virtual void s32();
    virtual void s33();
    virtual void s34();
    virtual void s35();
    virtual void s36();
    virtual void s37();
    virtual void EnableGravity(bool on);   // slot 38 (+0x98)
    virtual void s39();
    virtual void s40();
    virtual void s41();
    virtual void s42();
    virtual void s43();
    virtual void s44();
    virtual void s45();
    virtual void s46();
    virtual void s47();
    virtual void s48();
    virtual void s49();
    virtual void s50();
    virtual void s51();
    virtual void s52();
    virtual void s53();
    virtual void s54();
    virtual void s55();
    virtual void s56();
    virtual void s57();
    virtual void s58();
    virtual void s59();
    virtual void s60();
    virtual void s61();
    virtual void s62();
    virtual float GetFacingAngleXY() const;   // slot 63 (+0xfc)
    virtual void s64();
    virtual void s65();
    virtual void SetFacingAngleXY(float angle);   // slot 66 (+0x108)
};
#include "engine/CThing.h"
class CThingBaseDef : public CDefClassBase { };
inline const CThingBaseDef& CThing::PeekBaseDef() const { return *PBaseDef; }
class CThingCreatureBase : public CThing { public:
    void PreloadAnims();                        // 0x00663140
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // Ego_r: vptr + PData, 8 bytes
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
    CIntelligentPointer(T* const& item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { T* item = other.Get(); SetPItem(item); return *this; }
    T* Get() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    T* operator->() const { return Get(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.Get() != 0) {
        if (!p.Get()->IsDead())
            return true;
    }
    return false;
}

class CTCBoss { public:
    unsigned char _pad[0x21];
    bool EnablePhaseChanges;                    // +0x21
    void SetPhaseChangeEnabled(bool enabled) { EnablePhaseChanges = enabled; }
};
class CTCTargeted { public:
    unsigned char _pad[0x10];
    bool Targetable;                            // +0x10
    void SetTargetable(bool targetable) { Targetable = targetable; }
};
class CThingFilter_HasScriptName { public:
    CCharString ScriptName;
    CThingFilter_HasScriptName(const CCharString& name) : ScriptName(name) { }
};
class CThingSearchTools { public:
    template<class F, class T> bool GetPFirstThing(const F& filter, T** result) const;
};
class CWorld { public:
    static long __fastcall GetFrame();          // 0x0049D870
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
protected:
    const CThingSearchTools& PeekThingSearchTools() const;   // 0x008FD000
    CWorld& GetWorld() const;                   // 0x008FCFC0
};
enum EDragonState { DRAGON_STATE_1 = 1 };
#pragma pack(push, 1)
struct CAIStateGroup_JackDragon : CAIStateGroupBase {
    CIntelligentPointer<CThing> PEnemy;             // +0x14
    CIntelligentPointer<CThing> PLandedMarker;      // +0x1c
    CIntelligentPointer<CThing> PHoverMarker1;      // +0x24
    CIntelligentPointer<CThing> PHoverMarker2;      // +0x2c
    CIntelligentPointer<CThing> PStrafeMarker1;     // +0x34
    CIntelligentPointer<CThing> PStrafeMarker2;     // +0x3c
    CIntelligentPointer<CThing> PInvisibleBodgeCreature;   // +0x44
    long FrameOfLastSlam;                       // +0x4c
    long FrameWhenLastInRange;                  // +0x50
    long FrameWhenLanded;                       // +0x54
    long FramesInSnapRange;                     // +0x58
    EDragonState CurrentState;                  // +0x5c
    long GoLeft;                                // +0x60
    bool BattleTriggered;                       // +0x64
    C3DVector PreviousFlightPos;                // +0x65
    bool IsRangeAttack;                         // +0x71
    C3DVector DragonPosOutOfGround1;            // +0x72
    C3DVector DragonPosOutOfGround2;            // +0x7e
    C3DVector DragonPosOutOfGround3Left;        // +0x8a
    C3DVector DragonPosOutOfGround3Right;       // +0x96
    C3DVector DragonPosOutOfGround4Left;        // +0xa2
    C3DVector DragonPosOutOfGround4Right;       // +0xae
    C3DVector DragonPosIntoGround1Left;         // +0xba
    C3DVector DragonPosIntoGround1Right;        // +0xc6
    C3DVector DragonPosIntoGround2Left;         // +0xd2
    C3DVector DragonPosIntoGround2Right;        // +0xde
    C3DVector DragonPosIntoGround3;             // +0xea
    C3DVector DragonPosIntoStrafeLeft;          // +0xf6
    C3DVector DragonPosIntoStrafeRight;         // +0x102
    C3DVector DragonPosOutOfStrafeLeft;         // +0x10e
    C3DVector DragonPosOutOfStrafeRight;        // +0x11a
    C3DVector DragonPosStrafeFlameRight;        // +0x126
    C3DVector DragonPosStrafeFlameLeft;         // +0x132
    virtual void OnActivate();
};
#pragma pack(pop)

void CAIStateGroup_JackDragon::OnActivate()
{
    GetCreature()->PreloadAnims();
    {
        CTCBoss* boss;
        GetCreature()->GetTC(TCI_BOSS, &boss);
        boss->SetPhaseChangeEnabled(true);
    }
    GetCreature()->GetPhysicsTC()->EnableGravity(false);
    {
        CThing* marker;
        PeekThingSearchTools().GetPFirstThing(CThingFilter_HasScriptName("DragonMarker_Landed"), &marker);
        PLandedMarker = CIntelligentPointer<CThing>(marker);
        PLandedMarker->GetPhysicsTC()->SetFacingAngleXY(0.5f);
        PeekThingSearchTools().GetPFirstThing(CThingFilter_HasScriptName("DragonMarker_Hover1"), &marker);
        PHoverMarker1 = CIntelligentPointer<CThing>(marker);
        PeekThingSearchTools().GetPFirstThing(CThingFilter_HasScriptName("DragonMarker_Hover2"), &marker);
        PHoverMarker2 = CIntelligentPointer<CThing>(marker);
        PeekThingSearchTools().GetPFirstThing(CThingFilter_HasScriptName("DragonMarker_Strafe1"), &marker);
        PStrafeMarker1 = CIntelligentPointer<CThing>(marker);
        PeekThingSearchTools().GetPFirstThing(CThingFilter_HasScriptName("DragonMarker_Strafe2"), &marker);
        PStrafeMarker2 = CIntelligentPointer<CThing>(marker);
    }
    CurrentState = DRAGON_STATE_1;
    GoLeft = 0;
    BattleTriggered = false;
    FramesInSnapRange = 0;
    FrameWhenLastInRange = FrameOfLastSlam = GetWorld().GetFrame();
    FrameWhenLanded = GetWorld().GetFrame();
    {
        CTCTargeted* targeted;
        GetCreature()->GetTC(TCI_TARGETED, &targeted);
        targeted->SetTargetable(true);
    }
    CDefPointer<const CJackDragonDef> def;
    GetCreature()->PeekBaseDef().GetSubDef(&def);
    C3DVector intoGroundOffset(0.0f, -37.93019f, 30.148378f);
    C3DVector intoGroundBack(0.0f, -80.0f, 30.0f);
    if (IS_THING_ALIVE(PLandedMarker) && PLandedMarker->GetPhysicsTC()) {
        intoGroundOffset.RotateXY(PLandedMarker->GetPhysicsTC()->GetFacingAngleXY());
        intoGroundBack.RotateXY(PLandedMarker->GetPhysicsTC()->GetFacingAngleXY());
        GetCreature()->GetPhysicsTC()->SetPosition(PLandedMarker->GetPos());
        GetCreature()->GetPhysicsTC()->SetFacingAngleXY(PLandedMarker->GetPhysicsTC()->GetFacingAngleXY());
    }
    DragonPosOutOfGround1 = C3DVector(1469.0f, 7938.0f, 190.14838f);
    DragonPosOutOfGround2 = C3DVector(1474.0f, 7910.0f, 210.14838f);
    DragonPosOutOfGround3Right = C3DVector(1421.0f, 7910.0f, 200.14838f);
    DragonPosOutOfGround3Left = C3DVector(1527.0f, 7910.0f, 200.14838f);
    DragonPosOutOfGround4Right = C3DVector(1394.0f, PHoverMarker1->GetPos().Y + 20.0f, 185.14838f);
    DragonPosOutOfGround4Left = C3DVector(1544.0f, PHoverMarker1->GetPos().Y + 40.0f, 185.14838f);
    DragonPosIntoGround1Right = C3DVector(1428.0f, 8045.0f, 178.14838f);
    DragonPosIntoGround1Left = C3DVector(1514.0f, 8045.0f, 178.14838f);
    DragonPosIntoGround2Left = C3DVector(1500.0f, 8075.0f, 190.14838f);
    DragonPosIntoGround2Right = C3DVector(1442.0f, 8075.0f, 190.14838f);
    DragonPosIntoGround3 = intoGroundOffset + PLandedMarker->GetPos() + intoGroundBack
                         + C3DVector(0.0f, 0.0f, def->DragonGroundHeightOffsetBodge);
    DragonPosIntoStrafeLeft = C3DVector(1498.0f, 7963.0f, 155.14838f);
    DragonPosIntoStrafeRight = C3DVector(1443.0f, 7963.0f, 155.14838f);
    DragonPosOutOfStrafeLeft = C3DVector(1533.0f, 8010.0f, 195.14838f);
    DragonPosOutOfStrafeRight = C3DVector(1408.0f, 8010.0f, 195.14838f);
    DragonPosStrafeFlameRight = C3DVector(1449.0f, 7959.0f, 169.14838f);
    DragonPosStrafeFlameLeft = C3DVector(1497.0f, 7963.0f, 169.14838f);
    GetCreature()->GetPhysicsTC()->ResetPosition(DragonPosIntoGround1Right);
}