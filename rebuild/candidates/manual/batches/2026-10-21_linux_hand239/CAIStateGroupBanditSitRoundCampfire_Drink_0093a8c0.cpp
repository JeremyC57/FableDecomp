// CAIStateGroup_BanditSitRoundCampfire::Drink() -- drink a bottle by the campfire for a random 5..10 seconds, throw it away and get drunker
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093A8C0, length 377 bytes.
// Callees: CThingCreatureBase::SheatheAllWeapons @ 0x006647F0, GetPWorldSeed @ 0x008FCFD0, GFFloatRandom(float, ulong*) @ 0x004978A0,
//          GFGetCurrentFrame @ 0x0049D870 (x2), CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0
//          ("ST_SITTING_THROW_BOTTLE_AWAY" @ 0x01292598, "ST_SITTING_DRINKING_LOOP" @ 0x0129257C, "ST_SITTING_TAKE_BOTTLE_FROM_POCKET"
//          @ 0x01292558, "CARRY_SLOT_RIGHT_HAND" @ 0x01265440), CCreatureAction_PlayIntoLoopOutOfAnimation ctor @ 0x008427E0,
//          AIAction_PerformActionAndWait @ 0x0097DB70, the action base dtor @ 0x00693EF0, CAIStateGroupBase::DestroyItem(const
//          CCharString&) @ 0x008FD1B0, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0xC3, bit 3 of +0x38).
// Globals: 5.0f @ 0x01292458, the drunkenness rate per frame @ 0x013BAE70 (.bss), the 2.5f cap @ 0x01292480.
// Shape: the landed Singsong (0x0093A7A0) body with the start frame taken first (edi); the carry slot is an implicit
//          conversion temporary; the drunk component's level grows by elapsed frames * rate and is clamped to 2.5;
//          `result == 2` returned int-width. A same-TU LowerBound stand-in lets the elapsed-frame fild temporary reuse
//          the lookup key's slot as retail.
// Layout: the landed Singsong model (PBrain +4); the drunk component's level at +0x10.
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020 (stand-in body below)
};
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound (0x0040F020, STLport lower_bound; retail's LTCG saw its body): the key's address
// does not escape, so its frame slot is free again for the later int->float temporary.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CThingCreatureBase
{
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    void SheatheAllWeapons();                   // 0x006647F0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CTCDrunk {                               // interface 0xc3
    unsigned char _00[0x10];
    float Drunkenness;                          // +0x10
};
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0
long __fastcall GFGetCurrentFrame();            // 0x0049D870
extern float g_DrunkennessPerFrame;             // 0x013BAE70
struct CCharString
{
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};
struct CCreatureActionBase
{
    virtual void v00();
    unsigned char _body[0xa4];
};
struct CCreatureAction_PlayAnimationBase : CCreatureActionBase
{
    unsigned char _anim[0xbc - 0xa8];
    ~CCreatureAction_PlayAnimationBase();           // 0x00693EF0
};
struct CCreatureAction_PlayIntoLoopOutOfAnimation : CCreatureAction_PlayAnimationBase
{
    CCharString LoopAnim;                           // +0xbc
    CCharString OutAnim;                            // +0xc0
    CCreatureAction_PlayIntoLoopOutOfAnimation(CThingCreatureBase* creature, const CCharString& into,
                                               const CCharString& loop, const CCharString& out, float duration,
                                               bool a, bool b, bool c);   // 0x008427E0
};
struct CAIBrain
{
    unsigned char _pad[0x18];
    CThingCreatureBase* Creature;                   // +0x18
};
struct CAIStateGroup_BanditSitRoundCampfire
{
    virtual void v00();
    CAIBrain* PBrain;                               // +0x04
    unsigned char _pad08[0x0c];                     // +0x08
    unsigned long* GetPWorldSeed();                 // 0x008FCFD0
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
    void DestroyItem(const CCharString& slot);      // 0x008FD1B0
    long Drink();
};
long CAIStateGroup_BanditSitRoundCampfire::Drink()
{
    PBrain->Creature->SheatheAllWeapons();
    float duration = GFFloatRandom(5.0f, GetPWorldSeed()) + 5.0f;
    long startFrame = GFGetCurrentFrame();
    long result;
    {
        CCharString outAnim("ST_SITTING_THROW_BOTTLE_AWAY");
        CCharString loopAnim("ST_SITTING_DRINKING_LOOP");
        CCharString intoAnim("ST_SITTING_TAKE_BOTTLE_FROM_POCKET");
        CThingCreatureBase* creature = PBrain->Creature;
        result = AIAction_PerformActionAndWait(
            &CCreatureAction_PlayIntoLoopOutOfAnimation(creature, intoAnim, loopAnim, outAnim, duration,
                                                        false, false, false), false);
    }
    DestroyItem("CARRY_SLOT_RIGHT_HAND");
    CTCDrunk* drunk;
    if (PBrain->Creature->GetInterface(&drunk, 0xc3)) {
        drunk->Drunkenness += (float)(GFGetCurrentFrame() - startFrame) * g_DrunkennessPerFrame;
        if (drunk->Drunkenness > 2.5f)
            drunk->Drunkenness = 2.5f;
    }
    return result == 2;
}
