// CAIStateGroup_SitForMeal::IsRunnable() -- a meal is due on a new day, during meal hours, when a cooking pot exists
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00945B90, length 49 bytes (+ int3 padding).
// Callees: CAIStateGroupBase::GetPWorld @ 0x008FCFC0 (PBrain -> +0x1c -> +0x24), the world's
// day counter read through its clock object (+0x1c -> +0x14), IsInMealHours @ 0x009455B0 (the
// manifest's propagated "IsInSchoolHours": the creature's CTCVillage interface asks for the hour
// window), GetPCookingPot @ 0x00945A90 (CThing* or null).
// Shape: three tests in a row with one shared `xor al,al` false tail; the day comparison is
// against DayLastAte at +0x44 (local model, this class has no PDB layout). Bool result (`mov al,1`).
struct CThing;

struct CWorldClock {
    unsigned char _pad[0x14];
    long m_day;                                 // +0x14
};

struct CWorld {
    unsigned char _pad[0x1c];
    CWorldClock* m_pClock;                      // +0x1c
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    unsigned char _pad[0x10];                   // PBrain, ScriptAIPriority, Name, flags (+0x4..+0x13)
    CWorld* GetPWorld();                        // 0x008FCFC0
};

struct CAIStateGroup_WorkBase : CAIStateGroupBase {
    bool IsInMealHours();                       // 0x009455B0
};

struct CAIStateGroup_SitForMeal : CAIStateGroup_WorkBase {
    unsigned char _pad2[0x30];                  // +0x14..+0x43 seat / pot pointers
    long DayLastAte;                            // +0x44
    virtual bool IsRunnable();
    CThing* GetPCookingPot();                   // 0x00945A90
};

bool CAIStateGroup_SitForMeal::IsRunnable()
{
    if (GetPWorld()->m_pClock->m_day != DayLastAte) {
        if (IsInMealHours()) {
            if (GetPCookingPot() != 0)
                return true;
        }
    }
    return false;
}