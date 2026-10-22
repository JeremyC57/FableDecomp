// CTCDExperienceOrb::CTCDExperienceOrb(CThing*) -- driver component of an experience orb: CTCDriverBase(parent), target cleared, two indices at -1, first-update flag set, collected flag cleared
// Grade: RELOCATION_MATCH (retry 10, RTM 3077) -- the two -1 indices as records with an inline (-1) constructor: the zero register is set before `or eax,-1` as in retail
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

struct CIndexM1 { long v; CIndexM1() : v(-1) {} };
struct CZeroWord { long v; CZeroWord() : v(0) {} };
struct CTCDExperienceOrb : CTCDriverBase {
    long          m_target;                  // +0x0c
    CIndexM1      m_index10;                 // +0x10
    CIndexM1      m_index14;                 // +0x14
    long          LifeTimer;               // +0x18 (not initialised)
    bool          m_firstUpdate;             // +0x1c
    bool          m_collected;               // +0x1d
    virtual void v00();
    CTCDExperienceOrb(CThing* parent);
};

CTCDExperienceOrb::CTCDExperienceOrb(CThing* parent)
    : CTCDriverBase(parent)
    , m_target(0)
    , m_firstUpdate(true)
    , m_collected(false)
{
}