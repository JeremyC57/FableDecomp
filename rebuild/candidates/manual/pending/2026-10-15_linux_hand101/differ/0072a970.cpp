// CTCSpecialAbilities::SetupBulletTime() -- give the creature a "CTCBulletTime" component (activated, no params) unless it already has one
// Grade: DIFFER(63v63) -- only the creature/thing load for AddTC: retail `mov ecx,[esi+N]` right after the CCharString constructor, before `push 0 ; push 1 ; lea eax`; the lane loads it after the pushes (`lea ecx ; push ecx ; mov ecx,[esi+N]`). Tried across the six Setup* rows: plain member, inline accessor, local after the name, inline by-value name helper, optimize("s") (orders the load like retail but adds a frame pointer and narrows the bit test to a byte), optimize("g",off).
// Leaf: SetupBulletTime
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072a970, called once from CTCSpecialAbilities::UseSpecialAbility's
// switch (0x0072CF40).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 with the literal "CTCBulletTime", CThing::AddTC(const
// CCharString&, bool, CTCParamsBase*) @ 0x004C9D60, CCharString::~CCharString @ 0x0099EAE0.
// Layout: Ego_r.pdb CTCSpecialAbilities with the retail one-pointer list: UndeadFollowers +0x14, Creature +0x18
// (PDB +0x20). The thing's component bitset word at +0x30 bit 0x40000 marks an existing CTCBulletTime.
// Shape: an EXPLICIT CCharString(literal, -1) named local (built before the creature pointer is loaded) in its own
// scope, destroyed before the shared `mov al,1` exit.
struct CTCParamsBase;
struct CTCBase;

class CCharString {
public:
    CCharString(const char* text, long length);             // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
    void* m_pStringData;
};

struct CThing {
    unsigned char _pad00[0x24];
    unsigned long TCBits[4];                                // +0x24..+0x30: one bit per thing-component interface
    CTCBase* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
};

struct CTCSpecialAbilities {
    void*         __vftable;                                // +0x00
    CThing*       ParentThing;                              // +0x04
    bool          ToBeRemoved;                              // +0x08
    bool          IsInGlobalUpdateTCs;                      // +0x09
    unsigned char _pad0a[2];
    bool          AbilityMutexLock;                         // +0x0c
    bool          UsingBed;                                 // +0x0d
    long          CurrentAbility;                           // +0x10
    void*         UndeadFollowers;                          // +0x14 retail std::list: one sentinel pointer
    CThing*       Creature;                                 // +0x18
    bool SetupBulletTime();
};

bool CTCSpecialAbilities::SetupBulletTime()
{
    if (!(Creature->TCBits[3] & 0x40000)) {
        CCharString name("CTCBulletTime", -1);
        Creature->AddTC(name, true, 0);
    }
    return true;
}
