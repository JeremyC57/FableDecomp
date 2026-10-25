// CTCInventoryExperience::ProcessButtonYReleased() -- drop the pending experience spend, then the base inventory's ProcessButtonYReleased
// Leaf: ?ProcessButtonYReleased@CTCInventoryExperience@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0056F4B0, 10 bytes: call 0x0056F03D ; jmp 0x005BC692.
// Name: vtable 0x0124F11C (.?AVCTCInventoryExperience@@) slot 61; the tail jump is the base handler of the same slot
// (NInventory::CTCInventoryBase::ProcessButtonYReleased 0x005BC692). Ego_r.exe runs ProcessInventoryInputLeft 0x005E499D, ...Right 0x005E49A7
// (10 bytes each), ProcessExperienceActionReset 0x005E49B1 (5), ProcessButtonYReleased 0x005E49B6 (10); retail repeats it at
// 0x0056F497 / 0x0056F4A1 / 0x0056F4AB / 0x0056F4B0 (this one: Ego_r 0x005E49B6).
// Shape: retail passes `this` to the base handler without reloading ecx: the callee 0x0056F03D only touches eax/edx/edi
// and the compiler knew it, so its body is repeated here under `#pragma auto_inline(off)` (the GOTCHAS same-TU stand-in;
// a separate declaration-only callee costs `push esi ; mov esi,ecx ... mov ecx,esi ; pop esi`, 16 v 10 bytes).
// Local model: no engine header for CTCInventoryExperience / CTCInventoryBase; retail offsets as landed 0x0056F03D.
namespace NInventory {

class CTCInventoryBase {
public:
    virtual ~CTCInventoryBase();
    virtual void _v01();
    virtual void _v02();
    virtual void _v03();
    virtual void _v04();
    virtual void _v05();
    virtual void _v06();
    virtual void _v07();
    virtual void _v08();
    virtual void _v09();
    virtual void _v10();
    virtual void _v11();
    virtual void _v12();
    virtual void _v13();
    virtual void _v14();
    virtual void _v15();
    virtual void _v16();
    virtual void _v17();
    virtual void _v18();
    virtual void _v19();
    virtual void _v20();
    virtual void _v21();
    virtual void _v22();
    virtual void _v23();
    virtual void _v24();
    virtual void _v25();
    virtual void _v26();
    virtual void _v27();
    virtual void OnSelectedCategoryChanged();              // slot 28 (+0x70)
    virtual void OnHighlightedItemChanged();               // slot 29 (+0x74)
    virtual void _v30();
    virtual void _v31();
    virtual void _v32();
    virtual void _v33();
    virtual void _v34();
    virtual void _v35();
    virtual void _v36();
    virtual void _v37();
    virtual void ProcessInventoryInputUp();                // slot 38 (+0x98)
    virtual void ProcessInventoryInputDown();              // slot 39 (+0x9c)
    virtual void ProcessInventoryInputLeft();              // slot 40 (+0xa0)
    virtual void ProcessInventoryInputRight();             // slot 41 (+0xa4)
    virtual void _v42();
    virtual void _v43();
    virtual void _v44();
    virtual void _v45();
    virtual void _v46();
    virtual void _v47();
    virtual void _v48();
    virtual void _v49();
    virtual void _v50();
    virtual void _v51();
    virtual void _v52();
    virtual void _v53();
    virtual void _v54();
    virtual void _v55();
    virtual void _v56();
    virtual void _v57();
    virtual void _v58();
    virtual void _v59();
    virtual void _v60();
    virtual void ProcessButtonYReleased();                 // slot 61 (+0xf4)
};

} // namespace NInventory

struct ExperienceSpendInfo {
    long currentSpend;      // +0x0
    long initialSpend;      // +0x4
    long levelUpSpend;      // +0x8
};

struct SSpendInfoVector {
    ExperienceSpendInfo* _M_start;
    ExperienceSpendInfo* _M_finish;
    ExperienceSpendInfo* _M_end_of_storage;
};

class CTCInventoryExperience : public NInventory::CTCInventoryBase {   // RTTI .?AVCTCInventoryExperience@@, vtable 0x0124F11C
public:
    unsigned char _pad_004[0x1E0 - 4];
    SSpendInfoVector ExperienceSpendValuesPool;     // +0x1E0
    SSpendInfoVector ExperienceSpendValuesStats;    // +0x1EC
    SSpendInfoVector ExperienceSpendValuesAbility;  // +0x1F8
    long ExperienceSpendMagnitude;                  // +0x204
    void ResetExperienceSpendValues();              // 0x0056F03D (Ego_r.pdb QAEXXZ, 83 bytes in both builds)
    virtual void ProcessInventoryInputLeft();         // slot 40 (0056f497)
    virtual void ProcessInventoryInputRight();        // slot 41 (0056f4a1)
    virtual void ProcessButtonYReleased();            // slot 61 (0056f4b0)
};

#pragma auto_inline(off)
void CTCInventoryExperience::ResetExperienceSpendValues()   // same-TU copy of 0x0056F03D (landed separately)
{
    long i;
    for (i = 0; i < 4; ++i)
        ExperienceSpendValuesPool._M_start[i].currentSpend = ExperienceSpendValuesPool._M_start[i].initialSpend;
    for (i = 0; i < 10; ++i)
        ExperienceSpendValuesStats._M_start[i].currentSpend = ExperienceSpendValuesStats._M_start[i].initialSpend;
    for (i = 1; i < 20; ++i)
        ExperienceSpendValuesAbility._M_start[i].currentSpend = ExperienceSpendValuesAbility._M_start[i].initialSpend;
    ExperienceSpendMagnitude = 0;
}
#pragma auto_inline(on)

void CTCInventoryExperience::ProcessButtonYReleased()
{
    ResetExperienceSpendValues();
    CTCInventoryBase::ProcessButtonYReleased();
}
