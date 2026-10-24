// CTCEnemy::AreThingsAllies(const CThing& thing1, const CThing& thing2) -- resolve each thing's governing enemy component (and the thing it speaks for) and report whether each side counts the other as an ally; false when either side has no enemy component or is friends with everything
// Leaf: ?AreThingsAllies@CTCEnemy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076D4F0, 176 bytes, static `__fastcall` (thing1 in ecx, thing2 in edx), `ret`.
// Name: FableWin.pdb `?AreThingsAllies@CTCEnemy@@SI_NABVCThing@@0@Z`; landed callers declare `AreThingsAllies @ 0x0076D4F0
//   (__fastcall)` (five call sites: 0x0064A2A5, 0x006A7A46, 0x00744D79, 0x0090F18F, 0x00936B5C). The catalog name is right;
//   the class is CTCEnemy.
// Callees: CTCEnemy::CalculateEnemyTC(const CThing&, const CTCEnemy**, const CThing*&, bool) 0x0076CFE0 (private static
//   __fastcall: `?CalculateEnemyTC@CTCEnemy@@CI_NABVCThing@@PAPBV1@AAPBV2@_N@Z`); CTCEnemy::IsAllyOf(const CThing&) const
//   0x0076D460 (landed).
// Layout: CThing's class type byte +0x10 (Ego_r.pdb CClass::Type; 2 is tested); CTCEnemy FriendsWithEverythingFlag +0x0c
//   (IsFriendsWithEverything() inlined).
// Shape: both enemy pointers start null and both owner pointers start at the things; each lookup's flag is "the OTHER
//   thing is of class type 2" (the constant 2 lives in bl); the two ally answers are computed into named bools before the
//   `return ally1 && ally2;` (`mov eax,1` / `xor eax,eax`), the early rejections are `return false` (`xor al,al`).
class CThing {
public:
    unsigned char _pad_0x00[0x10];
    unsigned char Type;                             // +0x10 (CClass::Type)
};

class CTCEnemy {
public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04 (CTCBase)
    bool ToBeRemoved;                               // +0x08
    bool IsInGlobalUpdateTCs;                       // +0x09
    unsigned char _pad_0x0a[2];
    bool FriendsWithEverythingFlag;                 // +0x0c
    bool EnableFollowersEnemyProxy;                 // +0x0d
    static bool __fastcall AreThingsAllies(const CThing& thing1, const CThing& thing2);
private:
    static bool __fastcall CalculateEnemyTC(const CThing& thing, const CTCEnemy** enemy, const CThing*& owner,
                                            bool flag);   // 0x0076CFE0
    bool IsAllyOf(const CThing& thing) const;       // 0x0076D460
    bool IsFriendsWithEverything() const { return FriendsWithEverythingFlag; }
};

const CTCEnemy* FindEnemyTC_0076cfe0(const CThing& thing, bool flag);    // stand-in callees of the stand-in body below
const CThing* FindEnemyOwner_0076cfe0(const CThing& thing, bool flag);
#pragma auto_inline(off)
// Same-TU stand-in for the private static 0x0076CFE0 (retail defines it in this TU): it writes through both out
// parameters and leaks neither address, so VC7.1 keeps the loaded enemy pointers in registers across the later calls.
__declspec(noinline) bool __fastcall CTCEnemy::CalculateEnemyTC(const CThing& thing, const CTCEnemy** enemy,
                                                                const CThing*& owner, bool flag)
{
    *enemy = FindEnemyTC_0076cfe0(thing, flag);
    owner = FindEnemyOwner_0076cfe0(thing, flag);
    return *enemy != 0;
}
#pragma auto_inline(on)

bool __fastcall CTCEnemy::AreThingsAllies(const CThing& thing1, const CThing& thing2)
{
    const CTCEnemy* enemy1 = 0;
    const CTCEnemy* enemy2 = 0;
    const CThing* owner1 = &thing1;
    const CThing* owner2 = &thing2;
    CalculateEnemyTC(thing1, &enemy1, owner1, thing2.Type == 2);
    CalculateEnemyTC(thing2, &enemy2, owner2, thing1.Type == 2);
    if (enemy1 == 0 || enemy1->IsFriendsWithEverything())
        return false;
    if (enemy2 == 0 || enemy2->IsFriendsWithEverything())
        return false;
    bool ally1 = enemy1->IsAllyOf(*owner2);
    bool ally2 = enemy2->IsAllyOf(*owner1);
    return ally1 && ally2;
}