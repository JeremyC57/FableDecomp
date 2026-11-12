// CTCLook::ImplementHeadSettings() -- applies the head angles to the creature's animation: builds the head animations on the first pass, and when the creature is inside the look range, either strips both head-turn combination animations (no head angle) or updates the vertical and horizontal head animations
// Leaf: ?ImplementHeadSettings@CTCLook@@QAEXXZ
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00753BA0, length 208 bytes (+ int3 padding); speed TU (no frame pointer).
// Callees: CTCLook::InitialiseHeadAnims @ 0x00752640, IsInsideAnimationRange @ 0x00751BC0, CVectorMap::LowerBound @ 0x0040F020 (the
// inline GetInterface for type 0x5A; the bitset test folds to `test [thing+28h],4000000h`), the component's GetAnimation()
// @ 0x0070B460 (a CAnimateThing&), CAnimateThing::RemoveCombinationAnimsOfType(long) @ 0x0070D8C0, CTCLook::UpdateHeadCreatureAnim
// @ 0x00752F30 with the two head-anim globals 0x013BACE8 (type 12, vertical) and 0x013BACE4 (type 11, horizontal). 0.0f @ 0x0122DEDC.
// Layout: retail CTCLook ParentThing +0x04, HeadAngleRelToBodyXY +0x2C, HeadAngleRelToBodyYZ +0x30, FirstTimeThrough +0xCC (PDB
// +0xD0). CThing components bitset +0x20, interface map +0x44.
// Shape: the animation component comes from the unchecked out-parameter GetInterface (the absent path reads the frame word); the
// two angle tests are `== 0.0f` (`fucompp ; test ah,44h ; jp`); the animation reference is a named local (esi) shared by both arms.
extern "C" int  __cdecl puts(const char* text);    // 0x00BFEAF2
extern "C" __declspec(noreturn) void __cdecl abort();   // 0x00BFEAEC
struct CPresenceBits {                             // std::bitset<274>
    unsigned long words[9];
    bool test(unsigned long position) const
    {
        if (position >= 274) {
            puts("bitset");
            abort();
        }
        return (words[position >> 5] & (1UL << (position & 31))) != 0;
    }
};
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CAnimateThing { public:
    void RemoveCombinationAnimsOfType(long type);    // 0x0070D8C0
};
class CTCAnimationComplex { public:                                // interface type 0x5A
public:
    CAnimateThing& GetAnimation();   // 0x0070B460
};
enum { HEAD_ANIM_HORIZONTAL = 11, HEAD_ANIM_VERTICAL = 12 };
class CCharString;
enum ECreatureAnim;                                  // MS forward enum (Ego_r.pdb)
extern CCharString g_CCharString_013bace4;           // 0x013BACE4 Ego_r.pdb initializer order: ST_HEAD_TURN_H (spelling of the landed definition row 0x0121EBA0)
extern CCharString g_CCharString_013bace8;           // 0x013BACE8 Ego_r.pdb initializer order: ST_HEAD_TURN_V (spelling of the landed definition row 0x0121EBC0)
extern const float g_zero;                           // 0x0122DEDC

class CTCLook { public:
    unsigned char _pad00[0x04];
    CThing*       ParentThing;                       // +0x04
    unsigned char _pad08[0x2c - 0x08];
    float         HeadAngleRelToBodyXY;              // +0x2c
    float         HeadAngleRelToBodyYZ;              // +0x30
    unsigned char _pad34[0xcc - 0x34];
    bool          FirstTimeThrough;                  // +0xcc
    void InitialiseHeadAnims();                      // 0x00752640
    bool IsInsideAnimationRange();                   // 0x00751BC0
    void ImplementHeadSettings();
private:
    void UpdateHeadCreatureAnim(const CCharString& name, ECreatureAnim type, CAnimateThing* animation);   // 0x00752F30 Ego_r/FableWin ?UpdateHeadCreatureAnim@CTCLook@@AAEXABVCCharString@@W4ECreatureAnim@@PAVCAnimateThing@@@Z
};
void CTCLook::ImplementHeadSettings()
{
    if (FirstTimeThrough) {
        FirstTimeThrough = false;
        InitialiseHeadAnims();
    }
    if (!IsInsideAnimationRange())
        return;
    CTCAnimationComplex* component;
    ParentThing->GetTC(TCI_ANIMATION, &component);
    CAnimateThing& animation = component->GetAnimation();
    if (HeadAngleRelToBodyXY == g_zero && HeadAngleRelToBodyYZ == g_zero) {
        animation.RemoveCombinationAnimsOfType(HEAD_ANIM_VERTICAL);
        animation.RemoveCombinationAnimsOfType(HEAD_ANIM_HORIZONTAL);
    } else {
        UpdateHeadCreatureAnim(g_CCharString_013bace8, (ECreatureAnim)HEAD_ANIM_VERTICAL, &animation);
        UpdateHeadCreatureAnim(g_CCharString_013bace4, (ECreatureAnim)HEAD_ANIM_HORIZONTAL, &animation);
    }
}