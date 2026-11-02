// CCombatSequenceGeneric::~CCombatSequenceGeneric() -- implicit destructor: release the Def def pointer (count down, delete at zero, clear), then ~CCombatSequenceBase
// Leaf: ??1CCombatSequenceGeneric@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006F1770, 35 bytes, thiscall; tail `jmp ~CCombatSequenceBase` 0x008B0A60.
// Identity (proven, two sources): the only caller is the scalar deleting destructor 0x006F1750, slot 0 of vtable 0x01261D74 (RTTI
//   .?AVCCombatSequenceGeneric@@ : CCombatSequenceBase; the vtable is stored by the constructors at 0x006EC5C6 / 0x006F06CA);
//   FableWin.pdb ??1CCombatSequenceGeneric@@UAE@XZ (0x0241BB30, right after ??_G 0x0241BAF0; Ego_r inlines it). It stores no vtable of its own, so it is the
//   compiler-generated destructor. Its only member is Ego_r's Def (CDefPointer<const CCombatSequenceDef>), at retail +0x28 (the
//   Ego_r base has a 16-byte Dinkumware vector; retail's STLport vector<CCombatAction> is 12 bytes). The tail target 0x008B0A60
//   stores the CCombatSequenceBase vtable 0x01278FEC and frees the CombatActions vector at +0x10: ??1CCombatSequenceBase@@UAE@XZ
//   (Ego_r 0x008EBFF0 + FableWin 0x02A37E80; no binding yet -- the two spellings bound near it, ??_7CCombatSequenceBase@@6B@ and ?DestroyRange@@..., are
//   the landed row's vtable and range-destroy callees).
//   The catalog label "C3DAnimationBlendState::~C3DAnimationBlendState" is propagated and wrong. Byte-near-twin 0x007E0B20 is
//   ~CTCHeroSpecialMovement (its own ??_G 0x007E0B00 / vtable 0x0126FE3C; def at +0x18, tail ~CTCBase): two symbols.
// Callees: the def's virtual slot 1 Delete() (through the vtable), ??1CCombatSequenceBase@@UAE@XZ 0x008B0A60 (tail).
// Shape: engine/CDefPointer.h's inline ~CDefPointer = Release(): `if (Object) { if (--count == 0) Delete(); Object = 0; }`.
//   Retail's own constructor (body not reconstructed here) emits the vtable, hence ??_G and this out-of-line ??1.
// Layout: engine/CCombatSequenceGeneric.h retail opt-in (hdr/ overlay, chunk 710); CCombatSequenceBase has no shared header (local
//   model: Ego_r members at retail offsets, the STLport vector as opaque storage).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"                             // CDefPointer<T> over CDefPointeeBase

class CThingCreatureBase;

class CCombatSequenceBase {                                 // vtable 0x01278FEC (RTTI, no base); Ego_r members, retail 0x28 bytes
public:
    CCombatSequenceBase(CThingCreatureBase& creature, CThingCreatureBase& target);   // ??0CCombatSequenceBase@@QAE@AAVCThingCreatureBase@@0@Z
    virtual ~CCombatSequenceBase();                         // 0x008B0A60 ??1CCombatSequenceBase@@UAE@XZ
    bool                Running;                            // +0x04
    long                NoAvailableActions;                 // +0x08
    long                CurrentActionIndex;                 // +0x0C
    unsigned char       CombatActions_[0xc];                // std::vector<CCombatAction> CombatActions +0x10
    CThingCreatureBase* Creature;                           // +0x1C
    CThingCreatureBase* Target;                             // +0x20
    bool                CreateCalled;                       // +0x24
};
FABLE_STATIC_ASSERT(sizeof(CCombatSequenceBase) == 0x28);

#define FABLE_RETAIL_CCOMBATSEQUENCEGENERIC
#include "engine/CCombatSequenceGeneric.h"                  // CCombatSequenceGeneric : CCombatSequenceBase (retail opt-in, hdr/ overlay)

CCombatSequenceGeneric::CCombatSequenceGeneric(CThingCreatureBase& creature, CThingCreatureBase& target,
                                               CDefPointer<const CCombatSequenceDef>& def)
    : CCombatSequenceBase(creature, target), Def(def)
{
}