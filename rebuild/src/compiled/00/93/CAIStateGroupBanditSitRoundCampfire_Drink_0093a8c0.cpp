// CAIStateGroup_BanditSitRoundCampfire::Drink() -- drink a bottle by the campfire for a random 5..10 seconds, throw it away and get drunker
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
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
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThingCreatureBase
{ public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    void SheatheAllWeapons();                   // 0x006647F0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
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
class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};            // 0x0049D870
extern float g_DrunkennessPerFrame;             // 0x013BAE70
class CCharString
{ public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};
class CCreatureActionBase
{ public:
    virtual void v00();
    unsigned char _body[0xa4];
};
struct CCreatureAction_PlayAnimationBase : CCreatureActionBase
{
    unsigned char _anim[0xbc - 0xa8];
    ~CCreatureAction_PlayAnimationBase();           // 0x00693EF0
};
class CCreatureAction_PlayIntoLoopOutOfAnimation : public CCreatureAction_PlayAnimationBase
{ public:
    CCharString LoopAnim;                           // +0xbc
    CCharString OutAnim;                            // +0xc0
    CCreatureAction_PlayIntoLoopOutOfAnimation(CThingCreatureBase* creature, const CCharString& into,
                                               const CCharString& loop, const CCharString& out, float duration,
                                               bool a, bool b, bool c);   // 0x008427E0
};
class CAIBrain
{ public:
    unsigned char _pad[0x18];
    CThingCreatureBase* Creature;                   // +0x18
};
class CAIStateGroupBase {                         // Ego_r / RTTI base of CAIStateGroup_BanditSitRoundCampfire (offset 0)
public:
    virtual void v00();
protected:
    unsigned long* GetPWorldSeed() const;   // 0x008FCFD0 (Ego_r.pdb ?GetPWorldSeed@CAIStateGroupBase@@IBEPAKXZ)
};
struct CAIStateGroup_BanditSitRoundCampfire : public CAIStateGroupBase {
    virtual void v00();
    CAIBrain* PBrain;                               // +0x04
    unsigned char _pad08[0x0c];                     // +0x08
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
    void DestroyItem(const CCharString& slot);      // 0x008FD1B0
    long Drink();
};
long CAIStateGroup_BanditSitRoundCampfire::Drink()
{
    PBrain->Creature->SheatheAllWeapons();
    float duration = GFFloatRandom(5.0f, GetPWorldSeed()) + 5.0f;
    long startFrame = CWorld::GetFrame();
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
        drunk->Drunkenness += (float)(CWorld::GetFrame() - startFrame) * g_DrunkennessPerFrame;
        if (drunk->Drunkenness > 2.5f)
            drunk->Drunkenness = 2.5f;
    }
    return result == 2;
}