// CTCVillage::IsTimeToGoToTavern(CThingCreature* villager, const CWorldClock& clock, bool run, float speed) -- true during cook time, else when the villager's walk/run time from home to the village entrance covers the time left before the tavern opens
// Grade: DIFFER(413v415) -- register allocation and frame slots: retail keeps the villager in ebx, &Home in esi, the TC type 0x23 in edi (`push 23h ; pop edi`) and reuses the dead villager argument slot [ebp+8] for home, the type and the tavern start; ours keeps the villager in esi/edi and gives home/start their own frame words
// Leaf: IsTimeToGoToTavern
// Callees: IsCookTime 0x658C0A, GetPItem 0xA01B50, IS_THING_ALIVE<CThingBuilding*> 0x65D624, CThing::GetPosition 0x4C73D0, bitset::test 0x410DE0, LowerBound 0x40F020,
//   TC 0x23 GetVillage 0x7EE540 (label IS_THING_ALIVE<CIntelligentPointer> is wrong), CTCVillage::GetTavernTimes 0x658C5B (label GetSchoolTimes is wrong; landed), creature vslots +0x13c/+0x144, appearance vslot +0x118.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <math.h>
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    __forceinline C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    friend __forceinline float Distance(const C3DVector& a, const C3DVector& b)
    {
        float dx = a.X - b.X;
        float dy = a.Y - b.Y;
        float dz = a.Z - b.Z;
        return (float)sqrt(dx * dx + (dy * dy + dz * dz));
    }
    float GetLength() const { return (float)sqrt(X * X + (Y * Y + Z * Z)); }
};
struct CBitset {
    unsigned long Bits[9];
    bool test(unsigned int pos) const;                      // 0x00410DE0
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* end;
    SEntry* LowerBound(const long& type);                   // 0x0040F020
};
struct CThing;
struct CBaseObject;
struct CBaseIntelligentPointer {
    unsigned char _pad00[0x08];
    CBaseObject* GetPItem() const;                          // 0x00A01B50
};
struct CThingBuilding;
bool __fastcall IS_THING_ALIVE(CThingBuilding* const& thing);          // 0x0065D624 (__fastcall)
struct CTCVillage;
struct CTCVillageMember {                                   // TC 0x23
    CTCVillage* GetVillage() const;                         // 0x007EE540
};
struct CThingGraphic {
    virtual void v00();
};
struct CThing {
    virtual void v00();
    unsigned char _pad04[0x1c];
    CBitset       m_tcBits;                                 // +0x20
    CInterfaceMap m_tcs;                                    // +0x44
    unsigned char _pad50[0x10];
    struct CAppearance* PAppearance;                        // +0x60
    const C3DVector& GetPosition() const;                   // 0x004C73D0
    __forceinline bool HasTC(long type) const { return m_tcBits.test(type); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_tcs;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};
#define VF(n) virtual void v##n();
struct CAppearance {
    VF(00) VF(01) VF(02) VF(03) VF(04) VF(05) VF(06) VF(07) VF(08) VF(09) VF(10) VF(11) VF(12) VF(13)
    VF(14) VF(15) VF(16) VF(17) VF(18) VF(19) VF(20) VF(21) VF(22) VF(23) VF(24) VF(25) VF(26) VF(27)
    VF(28) VF(29) VF(30) VF(31) VF(32) VF(33) VF(34) VF(35) VF(36) VF(37) VF(38) VF(39) VF(40) VF(41)
    VF(42) VF(43) VF(44) VF(45) VF(46) VF(47) VF(48) VF(49) VF(50) VF(51) VF(52) VF(53) VF(54) VF(55)
    VF(56) VF(57) VF(58) VF(59) VF(60) VF(61) VF(62) VF(63) VF(64) VF(65) VF(66) VF(67) VF(68) VF(69)
    virtual C3DVector GetEntranceOffset() const;            // slot 70 (+0x118)
};
struct CThingCreature : CThing {
    unsigned char _pad64[0x170 - 0x64];
    CBaseIntelligentPointer Home;                           // +0x170
    VF(01) VF(02) VF(03) VF(04) VF(05) VF(06) VF(07) VF(08) VF(09) VF(10) VF(11) VF(12) VF(13)
    VF(14) VF(15) VF(16) VF(17) VF(18) VF(19) VF(20) VF(21) VF(22) VF(23) VF(24) VF(25) VF(26) VF(27)
    VF(28) VF(29) VF(30) VF(31) VF(32) VF(33) VF(34) VF(35) VF(36) VF(37) VF(38) VF(39) VF(40) VF(41)
    VF(42) VF(43) VF(44) VF(45) VF(46) VF(47) VF(48) VF(49) VF(50) VF(51) VF(52) VF(53) VF(54) VF(55)
    VF(56) VF(57) VF(58) VF(59) VF(60) VF(61) VF(62) VF(63) VF(64) VF(65) VF(66) VF(67) VF(68) VF(69)
    VF(70) VF(71) VF(72) VF(73) VF(74) VF(75) VF(76) VF(77) VF(78)
    virtual float GetWalkSpeedTo(const C3DVector& target);  // +0x13c
    VF(80)
    virtual float GetRunSpeedTo(const C3DVector& target);   // +0x144
};
struct CClockDef { unsigned char _pad00[0x10]; float HoursPerSecond; };
struct CWorldClock { unsigned char _pad00[0x1c]; const CClockDef* PDef; };
extern long g_FramesPerSecond;                              // 0x01375550
struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
};
struct CTCVillage : CTCBase {
    bool IsCookTime() const;                                // 0x00658C0A
    bool GetTavernTimes(float& start, float& end) const;    // 0x00658C5B
    bool IsTimeToGoToTavern(CThingCreature* villager, const CWorldClock& clock, bool run, float speed);
};

bool CTCVillage::IsTimeToGoToTavern(CThingCreature* villager, const CWorldClock& clock, bool run, float speed)
{
    if (IsCookTime())
        return true;
    CThingBuilding* home = (CThingBuilding*)villager->Home.GetPItem();
    if (!IS_THING_ALIVE(home))
        return false;
    C3DVector villagerPos = villager->GetPosition();
    C3DVector homePos = ((CThing*)villager->Home.GetPItem())->GetPosition();
    float distance = Distance(villagerPos, homePos);
    if (!villager->HasTC(0x23))
        return false;
    CTCVillage* village = ((CTCVillageMember*)villager->GetTC(0x23))->GetVillage();
    if (!village)
        return false;
    float start, end;
    if (!village->GetTavernTimes(start, end))
        return false;
    start = start / clock.PDef->HoursPerSecond;
    C3DVector target = ParentThing->PAppearance->GetEntranceOffset() + ParentThing->GetPosition();
    if (speed == 0.0f)
        speed = run ? villager->GetRunSpeedTo(target) : villager->GetWalkSpeedTo(target);
    if (start > distance * 0.5f / (g_FramesPerSecond * speed))
        return false;
    return true;
}
