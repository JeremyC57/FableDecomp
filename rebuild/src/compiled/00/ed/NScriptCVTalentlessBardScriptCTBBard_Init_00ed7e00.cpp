// NScript::CV_TalentlessBardScript::CTB_Bard::Init() -- the talentless bard's entity setup: give him information, set his wander centre (his own position) and the script-def wander distances, script his state group, mute his AI voice, enable only the wander / sit-outside / seek-enemy / seek-target / melee state groups (sit-outside off) and clear the three last-random-speech strings
// Leaf: ?Init@CTB_Bard@CV_TalentlessBardScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ED7E00, 584 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Init) of the RTTI-less entity-script vtable 0x012F3494 that the landed
//   CV_TalentlessBardScript::CTB_Bard::CTB_Bard 0x00ED7D50 stores [dtor 0x00ED7DB0, Main 0x00ED8680, Init 0x00ED7E00,
//   GetParentScript 0x00ED7DA0, OnPersist 0x00CDEBC0 (base), OnPredicateFail 0x00CDEBD0 (base), OnInterrupted 0x00CDEBE0
//   (base)]; the row is referenced only from that slot (0x012F349C). Ego_r.pdb
//   ?Init@CTB_Bard@CV_TalentlessBardScript@NScript@@EAEXXZ. Catalog label sub_00ed7e00.
// Callees: Game (+4) slots 360 SetThingHasInformation, 763 SetWanderCentrePoint, 766 SetWanderMinDistance, 769
//   SetWanderMaxDistance, 784 SetScriptingStateGroup, 362 EntitySetWillBeUsingNarrator, 591 EntitySetAllStategroupsEnabled,
//   590 EntitySetStategroupEnabled (engine/CGameScriptInterfaceBase.h); Me slot 6 GetPos; CCharString(const char*, long)
//   0x0099EBF0, ~CCharString 0x0099EAE0, CCharString::operator=(const char*) 0x0099EFE0; pGlobalScriptDef 0x0143E90C.
// Literals: "NoAIVoice" 0x012F34B0, "SG_MINION_WANDER" 0x01288C48 (true), "SG_MINION_SIT_OUTSIDE" 0x012891B8 (false),
//   "SG_SEEK_ENEMY" 0x0127813C (true), "SG_SEEK_SCRIPT_TARGET" 0x01289310 (true), "SG_MELEE_COMBAT_NEARBY2" 0x0127810C (true),
//   "" 0x0122D70E (x3).
// Layout: retail NScript headers (CEntityScriptBase {vptr, Game +4, Me +8}); CTB_Bard members at their Ego_r.pdb offsets
//   (ParentClass +0x14, PMasterData +0x18, first_time_talked_to +0x1c, given_gold +0x1d, lastRandomSpeech1..3 +0x20/+0x24/
//   +0x28; Alloc size 0x2c). CScriptDef BardWanderDistanceMin / Max at retail +0x7c0 / +0x7c4 (Ego_r +0x820 / +0x824 less
//   0x60: the 0x5c delta of chunk 581 plus SickChildsSisterAttentionAnims, a 0x10 Ego_r vector that is 0xc in retail).
//   EScriptingStateGroups 4 is the enumerator retail passes (its name is not proven; the constant is written as a cast).
// Shape: every by-value CScriptThing is an inline copy of Me (CBaseClass then CScriptThing vtable, PImp words, reference
//   increment), so the copy ctor and the CCountedPointer copy are `__forceinline` after the shared header; the by-value
//   C3DVector needs a memberwise user copy ctor (the implicit copy loads Z before storing Y).
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const char* text);     // 0x0099EFE0
};
class C3DVector {
public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
class CCPPointerInfo {
public:
    long References;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    __forceinline CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->References; }
};
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
__forceinline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CEntityScriptBase.h"

class CScriptDef {
public:
    unsigned char _before[0x7c0];
    float BardWanderDistanceMin;                        // +0x7c0
    float BardWanderDistanceMax;                        // +0x7c4
};
extern CScriptDef* pGlobalScriptDef;                    // 0x0143E90C

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_TalentlessBardScript {
public:
    class CTB_Bard;
};
class CV_TalentlessBardScript::CTB_Bard : public CEntityScriptBase {
public:
    CV_TalentlessBardScript* ParentClass;               // +0x14
    CQ_SunnyvaleMasterData*  PMasterData;               // +0x18
    bool                     first_time_talked_to;      // +0x1c
    bool                     given_gold;                // +0x1d
    CCharString              lastRandomSpeech1;         // +0x20
    CCharString              lastRandomSpeech2;         // +0x24
    CCharString              lastRandomSpeech3;         // +0x28
private:
    virtual void Init();
};

void CV_TalentlessBardScript::CTB_Bard::Init()
{
    Game->SetThingHasInformation(Me, true, true, false);
    Game->SetWanderCentrePoint(Me, Me.GetPos());
    Game->SetWanderMinDistance(Me, pGlobalScriptDef->BardWanderDistanceMin);
    Game->SetWanderMaxDistance(Me, pGlobalScriptDef->BardWanderDistanceMax);
    Game->SetScriptingStateGroup(Me, (EScriptingStateGroups)4);
    Game->EntitySetWillBeUsingNarrator(Me, "NoAIVoice");
    Game->EntitySetAllStategroupsEnabled(Me, false);
    Game->EntitySetStategroupEnabled(Me, "SG_MINION_WANDER", true);
    Game->EntitySetStategroupEnabled(Me, "SG_MINION_SIT_OUTSIDE", false);
    Game->EntitySetStategroupEnabled(Me, "SG_SEEK_ENEMY", true);
    Game->EntitySetStategroupEnabled(Me, "SG_SEEK_SCRIPT_TARGET", true);
    Game->EntitySetStategroupEnabled(Me, "SG_MELEE_COMBAT_NEARBY2", true);
    lastRandomSpeech1 = "";
    lastRandomSpeech2 = "";
    lastRandomSpeech3 = "";
}
}   // namespace NScript