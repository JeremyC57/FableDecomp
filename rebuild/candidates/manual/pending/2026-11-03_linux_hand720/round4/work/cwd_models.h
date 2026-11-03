// ---- ConstructWeaponDescription models (no engine header exists for these classes) ----
class CObjectAugmentationsDef : public CSubDefClassBase {   // RTTI .?AVCObjectAugmentationsDef@@ : CSubDefClassBase; Ego_r.pdb members
public:
    static CCharString __fastcall StatGetTypeName() { return CCharString("CObjectAugmentationsDef", -1); }   // inline (GetSubDef<T> 0x0042AF9E)
    long              NumberOfSlots;                    // +0x28
    long              MaximumNumberOfSlots;             // +0x2c
    unsigned char     AugmentationParticles[0xc];       // +0x30 map<EObjectAugmentationType,CObjectAugmentationParticleSet> (STLport 12)
    unsigned long     EmptySlotName;                    // +0x3c
    std::vector<long> InitialAugmentations;             // +0x40 (Ego_r +0x44)
};
namespace NInventory {
class CPerItemData {                                    // Ego_r.pdb CPerItemData, retail 0x24 (12-byte STLport vector); ctor 0x005C14A6
public:
    CPerItemData()
        : NumberOfKills(0), TimeWielded(0), BestTrophyWitnessesAheadToDate(0), QuestPersistent(true), QuestPersistentRefCount(-1)
    {
    }
    std::vector<long> Augmentations;                    // +0x00
    long              NumberOfKills;                    // +0x0c
    long              TimeWielded;                      // +0x10
    long              BestTrophyWitnessesAheadToDate;   // +0x14
    bool              QuestPersistent;                  // +0x18
    long              QuestPersistentRefCount;          // +0x1c
    float             BestFishWeight;                   // +0x20
};
}
class CTCVolumeContainmentTracker : public CTCBase {    // Ego_r.pdb CTCVolumeContainmentTracker (TCI_VOLUME_CONTAINMENT_TRACKER 117)
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_VOLUME_CONTAINMENT_TRACKER; }
    CIntelligentPointer<CThing> PThingInside;           // +0x0c
};
class CTCObjectAugmentations {
public:
    static float __fastcall GetDamageMultiplierFromAugmentationSet(const std::vector<long>& augmentations);   // 0x007661C0
    static CWideString __fastcall GetAugmentationName(long defIndex);                                        // 0x00765F90
};
namespace { extern const CWideString NEWLINE; }        // 0x013B8BA8 (`anonymous namespace'::NEWLINE of this TU, L"\n"; ESTABLISHED_SPELLINGS)
// Retail-asm: 0x005C6CB4 `fstp dword ptr [ebp-18h]` / 0x005C6CB7 `fld dword ptr [ebp-18h]` / 0x005C6CBA `fistp dword ptr [ebp-14h]` -- the engine's inline-assembly float-to-long helper (the scaled damage stored to its by-value copy and reloaded for a bare dword fistp; again at 0x005C6CC6..0x005C6CCC). VC7.1 C++ `(long)x` calls __ftol2 and /QIfist emits `fistp qword`: no C/C++ form gives the bare dword fistp.
__forceinline long __fastcall GFFastFloatToLong(float value)   // FableWin.pdb ?GFFastFloatToLong@@YIJM@Z (out of line only in the debug build)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
extern const CWideString g_EmptyWideString;             // 0x013BCA24 (established data spelling)
