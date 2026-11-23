// CAIStateGroup_BanditSitRoundCampfire::Drink() -- drink a bottle by the campfire for a random 5..10 seconds, throw it away and get drunker
// NF83 (round 7, q11027..q11029 self): leaf respelt to the private bool PDB spelling ?Drink@CAIStateGroup_BanditSitRoundCampfire@@AAE_NXZ
//   (both PDBs; NF79 0x0093AD70 binds it so) by an in-place edit of the row's local model (no shared header exists for this class).
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
// NF134 (2026-11-20, q5021): AIAction_PerformActionAndWait 0x0097DB70 respelled in place onto the row-local CAIStateGroupBase as the PDB
//   ?AIAction_PerformActionAndWait@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@ABVCCreatureActionBase@@_N@Z (Ego_r.pdb 0x00848940 + FableWin.pdb
//   0x02892570: protected, const CCreatureActionBase&, EAIActionReturn; engine/fable_ai_action_enums.h); was the derived stand-in
//   ?AIAction_PerformActionAndWait@CAIStateGroup_BanditSitRoundCampfire@@QAEJPAVCCreatureActionBase@@_N@Z.
// NF167 (2026-11-20, q4151): ported off the row-local `struct CTCDrunk` stand-in and `extern float g_DrunkennessPerFrame` onto the shared
//   engine/CTCDrunkenness.h (opt-in FABLE_RETAIL_CTCDRUNKENNESS_DRANKFRAMES: Ego_r / FableWin DrankFrames(long), GetDrunkenness,
//   SetDrunkenness and the private static ?DrunkennessDeltaPerFrameOfDrinking@CTCDrunkenness@@0MB 0x013BAE70, Ego_d.pdb PUB32): retail's
//   0x0093AA03 `fild ; fmul [0x013BAE70] ; fadd [esi+10h]` is DrankFrames expanded (its out-of-line copy is 0x0093A4E0, ret 4), and the
//   2.5 clamp goes through the public accessors because Drunkenness is private. The local CCharString model is replaced by fable_string.h.
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_GREETING_PASS23
#define FABLE_RETAIL_CTCDRUNKENNESS_DRANKFRAMES
#include "fable_string.h"
#include "engine/CTCDrunkenness.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThingCreatureBase
{ public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    void SheatheAllWeapons();                   // 0x006647F0
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
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
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0
class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};            // 0x0049D870
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
    CCreatureAction_PlayIntoLoopOutOfAnimation(CThingCreatureBase& creature, const CCharString& into,
                                               const CCharString& loop, const CCharString& out, float duration,
                                               bool a, long b, long c);   // 0x008427E0 (Ego_r + FableWin AAV ... M_NJJ)
};
class CAIBrain
{ public:
    unsigned char _pad[0x18];
    CThingCreatureBase* Creature;                   // +0x18
};
#include "engine/fable_ai_action_enums.h"
class CAIStateGroupBase {                         // Ego_r / RTTI base of CAIStateGroup_BanditSitRoundCampfire (offset 0)
public:
    virtual void v00();
protected:
    unsigned long* GetPWorldSeed() const;   // 0x008FCFD0 (Ego_r.pdb ?GetPWorldSeed@CAIStateGroupBase@@IBEPAKXZ)
    EAIActionReturn AIAction_PerformActionAndWait(const CCreatureActionBase& action, bool wait);   // 0x0097DB70
};
struct CAIStateGroup_BanditSitRoundCampfire : public CAIStateGroupBase {
    virtual void v00();
    CAIBrain* PBrain;                               // +0x04
    unsigned char _pad08[0x0c];                     // +0x08
    void DestroyItem(const CCharString& slot);      // 0x008FD1B0
private:
    bool Drink();                               // private (Ego_r.pdb 0x0095DD10 / FableWin.pdb 0x02B7F7B0 AAE_NXZ)
};
bool CAIStateGroup_BanditSitRoundCampfire::Drink()
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
            CCreatureAction_PlayIntoLoopOutOfAnimation(*creature, intoAnim, loopAnim, outAnim, duration,
                                                        false, false, false), false);
    }
    DestroyItem("CARRY_SLOT_RIGHT_HAND");
    CTCDrunkenness* drunk;
    if (PBrain->Creature->GetInterface(&drunk, TCI_DRUNKENNESS)) {
        drunk->DrankFrames(CWorld::GetFrame() - startFrame);
        if (drunk->GetDrunkenness() > 2.5f)
            drunk->SetDrunkenness(2.5f);
    }
    return result == 2;
}