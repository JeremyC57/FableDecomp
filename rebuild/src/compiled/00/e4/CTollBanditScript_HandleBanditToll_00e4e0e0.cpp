// CTollBanditScript::HandleBanditToll() -- place the toll bandits: teleport Me to the nearer blockade point (M_TollBanditBlockWay when it is closer to the hero than Me, else M_TollBandit), pick the near or far blockade markers, spawn archer/grunt/archer "BlockingBandit" creatures on them and face them along the markers
// Leaf: HandleBanditToll
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E4E0E0, length 1811 bytes. Called from 0x00E4B68D.
// Label: catalog/autoname-t5 "CCreatureAction_TrollWhackGroundBase::HandleBanditToll" is wrong -- `this` is an entity script (Game +4, Me +8),
//   called from the entity Main at 0x00E4B5B0 (vtable slot 0x012E4D8C), next to CM_TollBandit / CM_TauntingBandit.
// Shape: the handle getters (slot 6 GetPos, slot 10 GetHeading) inline on the exact-type locals with the empty-handle
//   default first; CCountedPointer::operator= reads the source pair before the self-compare; the distance test as
//   `dist(Me, hero) > dist(blockWay, hero)`; early returns under inline_depth(0) (out-of-line ~CScriptThing).
// Callees: CCharString 0x0099EBF0 / ~ 0x0099EAE0, CBaseClass 0x0099A2D0 / ~ 0x0099A2E0, ~CScriptThing 0x004AA840,
//   GetSquaredDistanceBetweenThings 0x00CBE512 (__fastcall), IsActiveThreadTerminating 0x00F35B30, operator delete 0x00BFE9BC;
//   CGameScriptInterface slots 70 GetHero, 72 GetThingWithScriptName, 91 CreateCreature, 472 EntityTeleportToThing,
//   474 EntitySetFacingAngle. Default position 0x0143E8E0 for an empty handle.
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
void operator delete(void* p);                           // 0x00BFE9BC
struct CCPPointerInfo {
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
struct CScriptThingImp {
    virtual void v00() const; virtual void v01() const; virtual void v02() const; virtual void v03() const; virtual void v04() const; virtual void v05() const;
    virtual const C3DVector& GetPos() const;             // slot 6 (+0x18)
    virtual void v07() const; virtual void v08() const; virtual void v09() const;
    virtual float GetHeading() const;                    // slot 10 (+0x28)
};
extern const C3DVector g_ZeroPos;                        // 0x0143E8E0
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    __forceinline CCountedPointer& operator=(const CCountedPointer& other)
    {
        T* data = other.Data;
        CCPPointerInfo* info = other.Info;
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct CBaseClass {
    CBaseClass();                                        // 0x0099A2D0
    virtual ~CBaseClass();                               // 0x0099A2E0
};
struct CScriptThing : CBaseClass {                       // 12 bytes, vtable 0x01238C8C
    CCountedPointer<CScriptThingImp> PImp;               // +0x04
    CScriptThing() { }
    CScriptThing(const CScriptThing& other);             // 0x004ABE90
    ~CScriptThing() { }                                  // out of line 0x004AA840
    __forceinline CScriptThing& operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
    virtual void v01() const; virtual void v02() const; virtual void v03() const; virtual void v04() const; virtual void v05() const;
    virtual const C3DVector& GetPos() const { return PImp.Data == 0 ? g_ZeroPos : PImp.Data->GetPos(); }   // slot 6
    virtual void v07() const; virtual void v08() const; virtual void v09() const;
    virtual float GetHeading() const { return PImp.Data == 0 ? 0.0f : PImp.Data->GetHeading(); }          // slot 10
};
float __fastcall GetSquaredDistanceBetweenThings(const CScriptThing& a, const CScriptThing& b);   // 0x00CBE512
#define VPAD1(a) virtual void a() const;
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
#define VPAD256(a) VPAD128(a##0) VPAD128(a##1)
struct CGameScriptInterface {                              // vtable 0x01260F0C (cgsi_slots_aligned.tsv)
    VPAD64(p0_0) VPAD4(p0_1) VPAD2(p0_2)                                         // slots 0..69
    virtual CScriptThing& GetHero() const;                                       // slot 70 (+0x118)
    VPAD1(p71_0)                                                                 // slots 71..71
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const;  // slot 72 (+0x120)
    VPAD16(p73_0) VPAD2(p73_1)                                                   // slots 73..90
    virtual CScriptThing CreateCreature(const CCharString& creature, const C3DVector& pos, const CCharString& scriptName, bool b) const; // slot 91 (+0x16c)
    VPAD256(p92_0) VPAD64(p92_1) VPAD32(p92_2) VPAD16(p92_3) VPAD8(p92_4) VPAD4(p92_5) // slots 92..471
    virtual void EntityTeleportToThing(const CScriptThing& entity, const CScriptThing& target, bool b) const; // slot 472 (+0x760)
    VPAD1(p473_0)                                                                // slots 473..473
    virtual void EntitySetFacingAngle(const CScriptThing& entity, float angle, bool b) const; // slot 474 (+0x768)
};
struct CTollBanditScript {
    void*                 __vftable;          // +0x00
    CGameScriptInterface* Game;               // +0x04
    CScriptThing          Me;                 // +0x08
    bool IsActiveThreadTerminating();         // 0x00F35B30
    void HandleBanditToll();
};

void CTollBanditScript::HandleBanditToll()
{
    CScriptThing blockWay = Game->GetThingWithScriptName("M_TollBanditBlockWay");
    CScriptThing point1;
    CScriptThing point2;
    CScriptThing point3;
    if (GetSquaredDistanceBetweenThings(Me, Game->GetHero()) > GetSquaredDistanceBetweenThings(blockWay, Game->GetHero())) {
        if (IsActiveThreadTerminating())
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        Game->EntityTeleportToThing(Me, blockWay, false);
        point1 = Game->GetThingWithScriptName("BanditBlockadeNear1");
        point2 = Game->GetThingWithScriptName("BanditBlockadeNear2");
        point3 = Game->GetThingWithScriptName("BanditBlockadeNear3");
    } else {
        if (IsActiveThreadTerminating())
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        Game->EntityTeleportToThing(Me, Game->GetThingWithScriptName("M_TollBandit"), false);
        point1 = Game->GetThingWithScriptName("BanditBlockadeFar1");
        point2 = Game->GetThingWithScriptName("BanditBlockadeFar2");
        point3 = Game->GetThingWithScriptName("BanditBlockadeFar3");
    }
    CScriptThing bandit1 = Game->CreateCreature("CREATURE_BANDIT_ARCHER_LEVEL1", point1.GetPos(), "BlockingBandit", false);
    CScriptThing bandit2 = Game->CreateCreature("CREATURE_BANDIT_GRUNT_LEVEL1", point2.GetPos(), "BlockingBandit", false);
    CScriptThing bandit3 = Game->CreateCreature("CREATURE_BANDIT_ARCHER_LEVEL1", point3.GetPos(), "BlockingBandit", false);
    Game->EntitySetFacingAngle(bandit1, point1.GetHeading(), true);
    Game->EntitySetFacingAngle(bandit2, point2.GetHeading(), true);
    Game->EntitySetFacingAngle(bandit3, point3.GetHeading(), true);
}