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
class CGameScriptInterfaceBase {
public:
    virtual ~CGameScriptInterfaceBase();
    virtual void v001() const; virtual void v002() const; virtual void v003() const; virtual void v004() const; virtual void v005() const; virtual void v006() const;
    virtual void v007() const; virtual void v008() const; virtual void v009() const; virtual void v010() const; virtual void v011() const; virtual void v012() const;
    virtual void v013() const; virtual void v014() const; virtual void v015() const; virtual void v016() const; virtual void v017() const; virtual void v018() const;
    virtual void v019() const; virtual void v020() const; virtual void v021() const; virtual void v022() const; virtual void v023() const; virtual void v024() const;
    virtual void v025() const; virtual void v026() const; virtual void v027() const; virtual void v028() const; virtual void v029() const; virtual void v030() const;
    virtual void v031() const; virtual void v032() const; virtual void v033() const; virtual void v034() const; virtual void v035() const; virtual void v036() const;
    virtual void v037() const; virtual void v038() const; virtual void v039() const; virtual void v040() const; virtual void v041() const; virtual void v042() const;
    virtual void v043() const; virtual void v044() const; virtual void v045() const; virtual void v046() const; virtual void v047() const; virtual void v048() const;
    virtual void v049() const; virtual void v050() const; virtual void v051() const; virtual void v052() const; virtual void v053() const; virtual void v054() const;
    virtual void v055() const; virtual void v056() const; virtual void v057() const; virtual void v058() const; virtual void v059() const; virtual void v060() const;
    virtual void v061() const; virtual void v062() const; virtual void v063() const; virtual void v064() const; virtual void v065() const; virtual void v066() const;
    virtual void v067() const; virtual void v068() const; virtual void v069() const;
    virtual CScriptThing& GetHero();                    // slot 70 (+0x118)
};
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