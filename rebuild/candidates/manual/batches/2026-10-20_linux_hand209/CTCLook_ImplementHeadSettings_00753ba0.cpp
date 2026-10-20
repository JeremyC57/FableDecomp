// CTCLook::ImplementHeadSettings() -- applies the head angles to the creature's animation: builds the head animations on the first pass, and when the creature is inside the look range, either strips both head-turn combination animations (no head angle) or updates the vertical and horizontal head animations
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
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    SEntry* LowerBound(const long* type);            // 0x0040F020
};
struct CThing {
    unsigned char _pad_0x00[0x20];
    CPresenceBits UsingComponents;                   // +0x20
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (UsingComponents.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            CInterfaceMap::SEntry* found = (it == map->last || wanted < it->type) ? map->last : it;
            *out = (T*)found->component;
            return true;
        }
        return false;
    }
};
struct CAnimateThing {
    void RemoveCombinationAnimsOfType(long type);    // 0x0070D8C0
};
struct CTCAnimation {                                // interface type 0x5A
    CAnimateThing& GetAnimation();                   // 0x0070B460
};
enum { HEAD_ANIM_HORIZONTAL = 11, HEAD_ANIM_VERTICAL = 12 };
extern long g_lookHeadAnimHorizontal;                // 0x013BACE4
extern long g_lookHeadAnimVertical;                  // 0x013BACE8
extern const float g_zero;                           // 0x0122DEDC

struct CTCLook {
    unsigned char _pad00[0x04];
    CThing*       ParentThing;                       // +0x04
    unsigned char _pad08[0x2c - 0x08];
    float         HeadAngleRelToBodyXY;              // +0x2c
    float         HeadAngleRelToBodyYZ;              // +0x30
    unsigned char _pad34[0xcc - 0x34];
    bool          FirstTimeThrough;                  // +0xcc
    void InitialiseHeadAnims();                      // 0x00752640
    bool IsInsideAnimationRange();                   // 0x00751BC0
    void UpdateHeadCreatureAnim(long* headAnim, long type, CAnimateThing& animation);   // 0x00752F30
    void ImplementHeadSettings();
};
void CTCLook::ImplementHeadSettings()
{
    if (FirstTimeThrough) {
        FirstTimeThrough = false;
        InitialiseHeadAnims();
    }
    if (!IsInsideAnimationRange())
        return;
    CTCAnimation* component;
    ParentThing->GetInterface(&component, 0x5a);
    CAnimateThing& animation = component->GetAnimation();
    if (HeadAngleRelToBodyXY == g_zero && HeadAngleRelToBodyYZ == g_zero) {
        animation.RemoveCombinationAnimsOfType(HEAD_ANIM_VERTICAL);
        animation.RemoveCombinationAnimsOfType(HEAD_ANIM_HORIZONTAL);
    } else {
        UpdateHeadCreatureAnim(&g_lookHeadAnimVertical, HEAD_ANIM_VERTICAL, animation);
        UpdateHeadCreatureAnim(&g_lookHeadAnimHorizontal, HEAD_ANIM_HORIZONTAL, animation);
    }
}
