// CTCHeroExperience::~CTCHeroExperience() -- releases the three def pointers, destroys the attack-history and pickup lists, frees the three stat vectors and runs the CTCBase destructor
// Leaf: ??1CTCHeroExperience
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DB7E0, length 147 bytes.
// Label: the manifest's "~CEnginePrimitiveAnimatingMesh" is propagated; the only caller is the scalar deleting destructor
// @ 0x006DB7C0, which is slot 0 of the CTCHeroExperience vtable 0x0125F6C4 (rtti-vtables.tsv), and the member walk below
// is the PDB CTCHeroExperience in reverse.
// Callees: the def's virtual slot 1 (destroy at zero count), list destructors @ 0x006DA9A0 (AttackHistory) and @ 0x0053C965
// (Pickups), free @ 0x00BFEA14 (vector storage), CTCBase::~CTCBase @ 0x00686830 (out of line: stores the CTCBase vtable
// 0x0125BE0C and jumps to CBase::~CBase; the tail `jmp`).
// Shape: members destroyed in reverse declaration order; each CDefPointer is `if (m_p) { m_p->Release(); m_p = 0; }` with the
// release inline (`dec [ecx+4] ; jne ; call [vt+4]`); each retail vector is `if (first) free(first)`; the class is
// `__declspec(novtable)` (its destructor stores no vtable of its own).
// Layout: retail std::vector is 12 bytes and std::list 4 bytes (PDB 16/12), so the PDB offsets shrink: vectors at +0x18,
// +0x24, +0x44, lists at +0x40, +0x50, defs at +0x54..+0x5C (the generated header keeps the PDB offsets: modelled locally).
extern "C" void __cdecl free(void* block);          // 0x00BFEA14

struct CDefBase {
    virtual void v00();
    virtual void Destroy();                         // slot 1
    long m_count;                                   // +0x04
    void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
};

struct CDefPointer {
    CDefBase* m_p;
    ~CDefPointer()
    {
        if (m_p) {
            m_p->Release();
            m_p = 0;
        }
    }
};

template <class T> struct CRetailVector {           // 12 bytes: first, last, end of storage
    T* first;
    T* last;
    T* end;
    ~CRetailVector()
    {
        if (first)
            free(first);
    }
};

struct CPickupList {                                // list<pair<long,long>>, sentinel pointer only
    void* head;
    ~CPickupList();                                 // 0x0053C965
};

struct CAttackHistoryList {                         // list<pair<EDamageAttribute,long>>, sentinel pointer only
    void* head;
    ~CAttackHistoryList();                          // 0x006DA9A0
};

struct CThing;

struct CTCBase {
    virtual ~CTCBase();                             // 0x00686830
    CThing* ParentThing;                            // +0x04
    bool    ToBeRemoved;                            // +0x08
    bool    IsInGlobalUpdateTCs;                    // +0x09
};

struct __declspec(novtable) CTCHeroExperience : CTCBase {
    long                FishingLevel;               // +0x0C
    long                TotalExperiencePoints;      // +0x10
    long                ExperiencePointsAvailableToSpend;   // +0x14
    CRetailVector<long> StatExperiencePoints;       // +0x18
    CRetailVector<long> TrainableStatLevels;        // +0x24
    float               ExperienceMultiplierParam;  // +0x30
    float               LastExperienceMultiplierParam;      // +0x34
    long                ExperienceAttenuationTimer; // +0x38
    bool                AbleToGainExperience;       // +0x3C
    CPickupList         Pickups;                    // +0x40
    CRetailVector<long> ExperienceSpentOnStats;     // +0x44
    CAttackHistoryList  AttackHistory;              // +0x50
    CDefPointer         PDef;                       // +0x54 CDefPointer<const CHeroExperienceDef>
    CDefPointer         PHeroCombatDef;             // +0x58 CDefPointer<const CHeroCombatDef>
    CDefPointer         PHeroStatsDef;              // +0x5C CDefPointer<const CHeroStatsDef>
    virtual ~CTCHeroExperience();
};

CTCHeroExperience::~CTCHeroExperience()
{
}
