// CCombatDialogueDef::Transfer(CPersistContext&) -- transfer three timing floats and seventeen dialogue-state fields through native context helpers.
// Flags: /O1
// Retail: Fable.exe @ 0x00456DFB, 242 bytes. Native vtable slot18; helper overloads at 0x00431061 (float) and 0x00431102 (long).
// The quarantined donor PDB header is 4 bytes larger than retail: native GetSizeofClass at 0x00456DF5 returns 0x8c, and this
// method references twenty dword fields from +0x3c through +0x88; donor names are +0x40..+0x8c. This descriptive native view
// uses those PDB member names shifted by four bytes. The reason for the donor/native layout discrepancy remains unresolved.
#define CCombatDialogueDef CCombatDialogueDef_PDB
#include "engine/_quarantine/CCombatDialogueDef.h"
#undef CCombatDialogueDef
#define CPersistContext CPersistContext_PDB
#include "fable_persist_context.h"
#undef CPersistContext
class CPersistContext : public CPersistContext_PDB { public:
    template <class T> void Transfer(T& value); // 0x00431061
};
class CCombatDialogueDef { public:
    void* __vftable;
    unsigned char nativePrefix[0x38];
    float MinTimeBetweenSpeech;
    float MinTimeBetweenSpeechRepeat;
    float MinDelayAfterBattleCry;
    long EnemySighted;
    long RunForCover;
    long Hiding;
    long Ambushing;
    long WillSeenUsed;
    long Threat;
    long Encourage;
    long FoeKnockedDownByColleague;
    long MenLosing;
    long EnemyKnockedDown;
    long RangedCombatAiming;
    long RangedCombatAimingBlocked;
    long Injured;
    long ReactionToDecapitation;
    long ColleagueInjured;
    long Scared;
    long Flee;
    void Transfer(CPersistContext& context);
};
void CCombatDialogueDef::Transfer(CPersistContext& context) {
    context.Transfer(MinTimeBetweenSpeech);
    context.Transfer(MinTimeBetweenSpeechRepeat);
    context.Transfer(MinDelayAfterBattleCry);
    context.Transfer(EnemySighted);
    context.Transfer(RunForCover);
    context.Transfer(Hiding);
    context.Transfer(Ambushing);
    context.Transfer(WillSeenUsed);
    context.Transfer(Threat);
    context.Transfer(Encourage);
    context.Transfer(FoeKnockedDownByColleague);
    context.Transfer(MenLosing);
    context.Transfer(EnemyKnockedDown);
    context.Transfer(RangedCombatAiming);
    context.Transfer(RangedCombatAimingBlocked);
    context.Transfer(Injured);
    context.Transfer(ReactionToDecapitation);
    context.Transfer(ColleagueInjured);
    context.Transfer(Scared);
    context.Transfer(Flee);
}