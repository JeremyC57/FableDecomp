// CCombatantInfo::CCombatantInfo(const CCombatantInfo& other) -- memberwise copy: parent pointer, the attacking info (scalars, dolly vector, sequence vector) and the defending info
// Leaf: ??0CCombatantInfo
// Retail: Fable.exe (TLC, VC7.1) @ 0x006F4990, 97 bytes (112 with padding).
// Callees: vector<CCountedPointer<CCombatSequenceBase>> copy constructor 0x006F19F0 (manifest label
//          CCombatantInfo::InitActiveScripts is propagated), CDefendingCombatantInfo copy constructor 0x006F46D0.
//
// Shape: PDB layout (struct_layouts_egor.tsv) with retail's 12-byte std::vector (the PDB's 16-byte
// vector puts DefenderInfo at +0x38; retail copies it from +0x34, GOTCHAS wave 23). CCombatantInfo =
// { Parent +0, CAttackingCombatantInfo AttackerInfo +4, CDefendingCombatantInfo DefenderInfo +0x34 }.
// The attacking info has an implicit (inlined) copy constructor: its dwords and the two bool bytes
// move through eax/ecx/edx in member order, the C2DVector is a plain two-float POD (integer moves),
// and only its CombatSequences vector calls an out-of-line copy constructor. The defending info's copy
// constructor is out of line. No engine header exists for CCombatantInfo (local model).
struct CThingCreatureBase;
struct CCombatSequenceBase;

struct C2DVector {
    float X;
    float Y;
};

struct CCountedPointerSequenceVector {      // vector<CCountedPointer<CCombatSequenceBase>> (retail 12 bytes)
    void* _Myfirst;
    void* _Mylast;
    void* _Myend;
    CCountedPointerSequenceVector(const CCountedPointerSequenceVector& other);   // 0x006F19F0
};

struct CAttackingCombatantInfo {
    CThingCreatureBase* Attacked;           // +0x00
    long AttackerNumber;                    // +0x04
    long CombatWheelRingIndex;              // +0x08
    bool PreferredAttackDollySet;           // +0x0c
    long PreferredAttackDollyTimer;         // +0x10
    C2DVector PreferredAttackDollyVector;   // +0x14
    bool UsingCombatWheel;                  // +0x1c
    long CurrentSequence;                   // +0x20
    CCountedPointerSequenceVector CombatSequences;   // +0x24
};

struct CDefendingCombatantInfo {
    unsigned char _body[0x44];
    CDefendingCombatantInfo(const CDefendingCombatantInfo& other);   // 0x006F46D0
};

struct CCombatantInfo {
    CThingCreatureBase* Parent;             // +0x00
    CAttackingCombatantInfo AttackerInfo;   // +0x04
    CDefendingCombatantInfo DefenderInfo;   // +0x34
    CCombatantInfo(const CCombatantInfo& other);
};

CCombatantInfo::CCombatantInfo(const CCombatantInfo& other)
    : Parent(other.Parent), AttackerInfo(other.AttackerInfo), DefenderInfo(other.DefenderInfo)
{
}