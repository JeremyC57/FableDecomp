// NScript::CV_MurderTwistFinalScript::CMurderTwistAssassin::IsFeelingThreatened() -- the assassin feels threatened when the player threatens it and the hero stands within ThreatDistSq (squared); false once the script thread is terminating
// Leaf: ?IsFeelingThreatened@CMurderTwistAssassin@CV_MurderTwistFinalScript@NScript@@AAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EB4710, 76 bytes, thiscall, plain `ret`.
// Identity (proven): Ego_r.pdb names ?IsFeelingThreatened@CMurderTwistAssassin@CV_MurderTwistFinalScript@NScript@@AAE_NXZ
//   (private, bool); its members ThreatDistSq +0x34 is the only float compared, and the callees are the script helpers
//   ?IsPlayerThreateningEntity@@YIHABVCScriptThing@@@Z 0x00CBEED2 (int, fastcall),
//   ?GetSquaredDistanceBetweenThings@@YIMABVCScriptThing@@0@Z 0x00CBE583 (no fsqrt in its body; 0x00CBE512 is the sqrt one),
//   ?GetHero@CGameScriptInterface@@UAEAAVCScriptThing@@XZ slot 70 (+0x118, ghidra_out/cgsi_slots_aligned.tsv) and
//   ?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ 0x00F35B30.
// Layout: Ego_r.pdb CMurderTwistAssassin: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18,
//   MyEntityHandle +0x1C, NextLine +0x2C, HasBeenThreatened +0x30, ThreatDistSq +0x34, HasGivenIntroToHero +0x38,
//   PreemptiveAttackInProgress +0x39.
// Shape: each return is guarded by the script-thread terminating check (both paths call it, the false path discards it);
//   a single `bool threatened = ...; if (terminating) return false; return threatened;` merges into not/and (82v76).
class CScriptThing {                                    // vtable 0x01238C8C, 12 bytes
public:
    virtual ~CScriptThing();
    void* PImp[2];                                      // +0x04 CCountedPointer<CScriptThingImpl>
};
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
int __fastcall IsPlayerThreateningEntity(const CScriptThing& thing);                         // 0x00CBEED2
float __fastcall GetSquaredDistanceBetweenThings(const CScriptThing& a, const CScriptThing& b); // 0x00CBE583
class CEntityScriptBase {
public:
    virtual ~CEntityScriptBase();
    virtual void Main();
    virtual void Init();
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
    CGameScriptInterfaceBase* Game;                     // +0x04
    CScriptThing Me;                                    // +0x08
};
namespace NScript {
class CV_MurderTwistMasterData;
class CV_MurderTwistFinalScript {
public:
    class CMurderTwistAssassin : public CEntityScriptBase {
    public:
        CV_MurderTwistFinalScript* ParentClass;         // +0x14
        CV_MurderTwistMasterData* PMasterData;          // +0x18
        long MyEntityHandle[4];                         // +0x1C
        long NextLine;                                  // +0x2C
        bool HasBeenThreatened;                         // +0x30
        float ThreatDistSq;                             // +0x34
        bool HasGivenIntroToHero;                       // +0x38
        bool PreemptiveAttackInProgress;                // +0x39
    private:
        bool IsFeelingThreatened();
    };
};

bool CV_MurderTwistFinalScript::CMurderTwistAssassin::IsFeelingThreatened()
{
    if (IsPlayerThreateningEntity(Me) &&
        GetSquaredDistanceBetweenThings(Game->GetHero(), Me) <= ThreatDistSq)
    {
        if (IsActiveThreadTerminating())
            return false;
        return true;
    }
    if (IsActiveThreadTerminating())
        return false;
    return false;
}
}