// CTCBoss::GetMinHealthForPhase(long phase) const -- the health threshold of a boss phase: the def's percentage for that phase times 0.01 times the physical thing's MaxHealth, else 0
//
// Retail 0x007AEE40, 69 bytes (`ret 4`, float in st0). No callees. Constants: 0x01230A5C = 0.01f,
// 0x0122DEDC = 0.0f (a `fld` of the pool zero, never `fldz`).
// The parent thing (+0x04) must carry the physical-thing type flag (ThingTypeFlags +0x6c, bit 2;
// retail CThing sits 0x10 below the Ego_r.pdb offsets) -- it is then read as a CThingPhysical whose
// MaxHealth is +0xb0 (PDB +0xc8). The def's PhaseHealthPercent is the retail 12-byte vector<long>
// at +0x28; the phase must be >= 0 (signed `jl`) and < size() (unsigned `jae`).
// Shape: `fild [first+phase*4] ; fmul [0.01] ; fmul [MaxHealth]` = MaxHealth * ((float)percent * 0.01f):
// the left-to-right `percent * 0.01f * MaxHealth` makes VC7.1 move the literal to the end
// (`fmul [MaxHealth] ; fmul [0.01]`), the parenthesised product keeps it first. Every failed test
// shares the 0.0f tail.

template <class T>
struct CRetailVector                                // retail std::vector: {first, last, end}
{
    T* _First;
    T* _Last;
    T* _End;
    unsigned int size() const { return _Last - _First; }
    const T& operator[](unsigned int i) const { return _First[i]; }
};

struct CBossDef
{
    unsigned char       _pad_0x00[0x28];
    CRetailVector<long> PhaseHealthPercent;         // +0x28
};

struct CThing
{
    unsigned char _pad_0x00[0x6c];
    unsigned long ThingTypeFlags;                   // +0x6c
};

struct CThingPhysical : CThing
{
    unsigned char _pad_0x70[0x40];
    float         MaxHealth;                        // +0xb0
    float         Health;                           // +0xb4
};

enum { THING_TYPE_PHYSICAL = 4 };

#pragma pack(push, 1)
struct CTCBoss
{
    void*         __vftable;                        // +0x00 (CTCBase)
    CThing*       ParentThing;                      // +0x04 (CTCBase)
    unsigned char _pad_0x08[0x4];                   // +0x08 (CTCBase flags)
    const CBossDef* PBossDef;                       // +0x0c CDefPointer<const CBossDef>::Object

    float GetMinHealthForPhase(long phase) const;
};
#pragma pack(pop)

float CTCBoss::GetMinHealthForPhase(long phase) const
{
    CThing* thing = ParentThing;
    if (thing->ThingTypeFlags & THING_TYPE_PHYSICAL)
    {
        if (PBossDef != 0 && phase >= 0 && (unsigned int)phase < PBossDef->PhaseHealthPercent.size())
            return static_cast<CThingPhysical*>(thing)->MaxHealth * ((float)PBossDef->PhaseHealthPercent[phase] * 0.01f);
    }
    return 0.0f;
}
