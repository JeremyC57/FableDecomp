// CCombatTypeDef::Transfer(CPersistContext&) -- serialize combat-def fields in retail order
// Retail 0x0046F7C8, 274 bytes. The real generated PDB declarations are included as aliased evidence;
// this native view models the retail 32-bit STL members (12 bytes each), shifting fields from donor offsets.
// NativeVector32/NativeMap32 are opaque three-word storage views, not claims about pointer C++ types.
// Owner is global CCombatTypeDef (decorated symbol/RTTI); context is the real global CPersistContext.
#pragma optimize("s", on)
#define CCombatTypeDef CCombatTypeDef_PDB
#include "engine/CCombatTypeDef.h"
#undef CCombatTypeDef
#define CPersistContext CPersistContext_PDB
#include "fable_persist_context.h"
#undef CPersistContext
struct NativeVector32 { unsigned int first, last, capacity; };
struct NativeMap32 { unsigned int root, first, size; };
class CPersistContext { public:
    void TransferLongVector(NativeVector32* value);   // 0x00466A47
    template <class T> void Transfer(T& value);                   // 0x00431102
    void TransferStringVector(NativeVector32* value); // 0x004331F9
    void TransferMap(NativeMap32* value);            // 0x0046FA22
    void TransferCombatType(int* value);             // 0x0046F8DA
    void TransferRecoil(int* value);                 // 0x0046F97E
};
class CCombatTypeDef { public:
    unsigned char prefix[0x3c];
    NativeVector32 DefenderCombatRingOccupationNumbers; // +0x3c; PDB donor +0x40
    int DefenderWheelOuterRingStartIndex;              // +0x48; donor +0x50
    bool DefenderWheelAllowAttackerStrafing;            // +0x4c
    unsigned char pad4d[3];
    NativeVector32 CombatSequence;                      // +0x50; donor +0x58
    NativeVector32 CombatSequenceDef;                   // +0x5c; donor +0x68
    bool PreferOuterRing;                               // +0x68; donor +0x78
    unsigned char pad69[3];
    float FrontFarLimit;                                // +0x6c
    float FrontMiddleLimit;                             // +0x70
    float FrontNearLimit;                               // +0x74
    float SideLimit;                                    // +0x78
    float RearLimit;                                    // +0x7c
    float PreferredCombatDistance;                      // +0x80
    NativeMap32 AttackPatterns;                         // +0x84; donor +0x94
    int CombatCreatureType;                             // +0x90; donor +0xa4
    bool AlwaysAttackIfNearest;                         // +0x94
    bool KnockdownCausesStun;                           // +0x95
    unsigned char pad96[2];
    int RecoilStyle;                                    // +0x98
    bool CanBeKnockedDown;                              // +0x9c
    unsigned char pad9d[3];
    float ComboContinueSpeed;                           // +0xa0
    float CombatFOV;                                    // +0xa4
    bool StartsAsReadyForMelee;                         // +0xa8
    void Transfer(CPersistContext& context);
};
typedef char NativeCombatTypeSize[(sizeof(CCombatTypeDef) == 0xac) ? 1 : -1];
void CCombatTypeDef::Transfer(CPersistContext& context)
{
    context.TransferLongVector(&DefenderCombatRingOccupationNumbers);
    context.Transfer(*(long*)(&DefenderWheelOuterRingStartIndex));
    context.Transfer(*(bool*)(&DefenderWheelAllowAttackerStrafing));
    context.TransferStringVector(&CombatSequence);
    context.TransferLongVector(&CombatSequenceDef);
    context.Transfer(*(bool*)(&PreferOuterRing));
    context.Transfer(*(float*)(&FrontFarLimit));
    context.Transfer(*(float*)(&FrontMiddleLimit));
    context.Transfer(*(float*)(&FrontNearLimit));
    context.Transfer(*(float*)(&SideLimit));
    context.Transfer(*(float*)(&RearLimit));
    context.Transfer(*(float*)(&PreferredCombatDistance));
    context.TransferMap(&AttackPatterns);
    context.TransferCombatType(&CombatCreatureType);
    context.Transfer(*(bool*)(&AlwaysAttackIfNearest));
    context.Transfer(*(bool*)(&KnockdownCausesStun));
    context.TransferRecoil(&RecoilStyle);
    context.Transfer(*(bool*)(&CanBeKnockedDown));
    context.Transfer(*(float*)(&ComboContinueSpeed));
    context.Transfer(*(float*)(&CombatFOV));
    context.Transfer(*(bool*)(&StartsAsReadyForMelee));
}