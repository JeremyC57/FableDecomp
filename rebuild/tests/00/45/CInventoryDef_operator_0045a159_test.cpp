// Behaviour test for 0x0045A159 CInventoryDef::operator= : base part, scalars, bool, vectors, packed 14-byte graphic,
// C2DVector / C3DVector / colour members are all copied and *this is returned (model copy of the row's shape).
#include <cstdio>
#include <cstring>
#include <vector>
#pragma pack(push, 1)
class C2DVector { public: float X, Y; };
class CEngineGraphic { public: unsigned long a, b, c; unsigned short d; };
#pragma pack(pop)
class C3DVector { public: float X, Y, Z; };
class CRGBColour { public: unsigned long IntValue; };
class CParentDefClassBase { public:
    long m_defIndex; long m_parent;
    CParentDefClassBase& operator=(const CParentDefClassBase& o) { m_defIndex = o.m_defIndex; m_parent = o.m_parent; return *this; }
};
struct CInventoryDefModel : CParentDefClassBase {
    float InitialCategoryMovementRepeatDelay;
    long MovementSound;
    bool TrackSelectedItems;
    std::vector<long> InitialInventoryClothingItems;
    C2DVector MeshAreaTLPos;
    CRGBColour BackGroundColour;
    C3DVector DollCentreOffset;
    std::vector<unsigned long> StatLevelsTexts;
    std::vector<CRGBColour> StatLevelsColours;
    CEngineGraphic ExpSpendGreen;
    C2DVector MainDescriptionBoxTLPos;
    float MainQuickAccessItemFlashTime;
    CInventoryDefModel& operator=(const CInventoryDefModel& other)
    {
        CParentDefClassBase::operator=(other);
        InitialCategoryMovementRepeatDelay = other.InitialCategoryMovementRepeatDelay;
        MovementSound = other.MovementSound;
        TrackSelectedItems = other.TrackSelectedItems;
        InitialInventoryClothingItems = other.InitialInventoryClothingItems;
        MeshAreaTLPos = other.MeshAreaTLPos;
        BackGroundColour = other.BackGroundColour;
        DollCentreOffset = other.DollCentreOffset;
        StatLevelsTexts = other.StatLevelsTexts;
        StatLevelsColours = other.StatLevelsColours;
        ExpSpendGreen = other.ExpSpendGreen;
        MainDescriptionBoxTLPos = other.MainDescriptionBoxTLPos;
        MainQuickAccessItemFlashTime = other.MainQuickAccessItemFlashTime;
        return *this;
    }
};
int main()
{
    CInventoryDefModel a, b;
    std::memset(&a.m_defIndex, 0, 8);
    a.m_defIndex = 7; a.m_parent = 3; a.InitialCategoryMovementRepeatDelay = 0.25f; a.MovementSound = 41; a.TrackSelectedItems = true;
    a.InitialInventoryClothingItems.push_back(5); a.InitialInventoryClothingItems.push_back(9);
    a.MeshAreaTLPos.X = 1.5f; a.MeshAreaTLPos.Y = -2.0f; a.BackGroundColour.IntValue = 0x80FF0010;
    a.DollCentreOffset.X = 1; a.DollCentreOffset.Y = 2; a.DollCentreOffset.Z = 3;
    a.StatLevelsTexts.push_back(1234); CRGBColour c; c.IntValue = 0xFF00FF00; a.StatLevelsColours.push_back(c);
    a.ExpSpendGreen.a = 1; a.ExpSpendGreen.b = 2; a.ExpSpendGreen.c = 3; a.ExpSpendGreen.d = 0xBEEF;
    a.MainDescriptionBoxTLPos.X = 10; a.MainDescriptionBoxTLPos.Y = 20; a.MainQuickAccessItemFlashTime = 0.75f;
    b.InitialInventoryClothingItems.push_back(99);
    CInventoryDefModel& r = (b = a);
    int ok = (&r == &b) && b.m_defIndex == 7 && b.m_parent == 3 && b.InitialCategoryMovementRepeatDelay == 0.25f
        && b.MovementSound == 41 && b.TrackSelectedItems && b.InitialInventoryClothingItems.size() == 2
        && b.InitialInventoryClothingItems[1] == 9 && b.MeshAreaTLPos.Y == -2.0f && b.BackGroundColour.IntValue == 0x80FF0010
        && b.DollCentreOffset.Z == 3 && b.StatLevelsTexts.size() == 1 && b.StatLevelsTexts[0] == 1234
        && b.StatLevelsColours[0].IntValue == 0xFF00FF00 && b.ExpSpendGreen.d == 0xBEEF && b.ExpSpendGreen.c == 3
        && b.MainDescriptionBoxTLPos.Y == 20 && b.MainQuickAccessItemFlashTime == 0.75f
        && sizeof(CEngineGraphic) == 14 && sizeof(C2DVector) == 8;
    a.InitialInventoryClothingItems[0] = 77;
    if (b.InitialInventoryClothingItems[0] != 5) ok = 0;   // deep copy, not aliasing
    if (!ok) { std::printf("0045a159_TEST FAIL\n"); return 1; }
    std::printf("0045a159_TEST PASS\n");
    return 0;
}