// CTCDExperienceOrb::CTCDExperienceOrb(CThing*) -- driver component of an experience orb: CTCDriverBase(parent), target cleared, two indices at -1, first-update flag set, collected flag cleared
// Grade: DIFFER(46v46) -- scheduling only: retail sets the zero register first (`xor ecx,ecx ; or eax,-1`), ours `or eax,-1 ; xor ecx,ecx` (the vtable store then follows in both). Tried: init list (this), pointer-typed +0xc, the -1 pair as body assignments, the flag pair as body assignments.
// Leaf: ??0CTCDExperienceOrb
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00779CA0, length 46 bytes (+2 int3 padding).
// Callee: CTCDriverBase::CTCDriverBase(CThing*) @ 0x00771200 (CTCBase(parent), vtable 0x0126A79C, then the parent's
// virtual slot 28 with 1). Vtable 0x0126ABFC = CTCDExperienceOrb (rtti-vtables.tsv).
// Label correction: the manifest "CTCShotDefinitionMultiArrow::CTCShotDefinitionMultiArrow" is propagated; the vtable stored
// is CTCDExperienceOrb's.
// Shape: speed TU; the zero and -1 registers (`xor ecx,ecx ; or eax,-1`) are set up before the vtable store and every
// initialiser follows it in declaration order (+0x18 is left alone).
struct CThing;

struct CTCDriverBase {
    virtual void v00();
    unsigned char _body[0x8];                // CTCBase: ParentThing +4, flags +8/+9
    CTCDriverBase(CThing* parent);           // 0x00771200
};

struct CTCDExperienceOrb : CTCDriverBase {
    long          m_target;                  // +0x0c
    long          m_index10;                 // +0x10
    long          m_index14;                 // +0x14
    long          m_unknown18;               // +0x18 (not initialised)
    bool          m_firstUpdate;             // +0x1c
    bool          m_collected;               // +0x1d
    virtual void v00();
    CTCDExperienceOrb(CThing* parent);
};

CTCDExperienceOrb::CTCDExperienceOrb(CThing* parent)
    : CTCDriverBase(parent)
    , m_target(0)
    , m_index10(-1)
    , m_index14(-1)
    , m_firstUpdate(true)
    , m_collected(false)
{
}
