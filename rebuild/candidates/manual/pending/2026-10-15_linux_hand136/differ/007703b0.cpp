// Grade: DIFFER(ours v retail) -- 144v144: the empty __false_type tag is homed by retail over the LIVE alpha argument slot (lea ebx,[esp+28h]) where the lane picks the dead interpolation-set slot ([esp+3Ch]); the begin() node load is scheduled after the iterator slot push; the eight member stores after the call are scheduled in another order
// CEnginePrimitiveWeaponTrail::CEnginePrimitiveWeaponTrail(unsigned __int64, const std::list<CWeaponTrailEntry>&, unsigned long, unsigned long, EWeaponTrailType, unsigned char, float, float, float, float, EEngineInterpolationSet) -- primitive type 0x13: copies the trail list, graphic, trail type, alpha and timing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007703B0, length 144 bytes.
// Callees: 0x00BFEA0E malloc (the list's sentinel node), 0x0076FD40 the STLport list range insert
// (`_M_insert_dispatch(pos, first, last, const __false_type&)`, `ret 10h`, copies 28-byte entries).
// Signature from Ego_r.pdb: ??0CEnginePrimitiveWeaponTrail@@QAE@_KABV?$list@VCWeaponTrailEntry@...@std@@KKW4EWeaponTrailType@0@EMMMMW4EEngineInterpolationSet@@@Z
// (12 stack words, `ret 30h`); the 64-bit primitive handle is not stored.
// Layout: PDB CEnginePrimitiveWeaponTrail (0x38) with retail's 4-byte STLport list (the node pointer only), so every
// member after Trail sits 8 bytes below the PDB offset: Trail +0xc, GraphicIndex +0x10, GraphicFrame +0x14,
// TrailType +0x18, SkipPoints +0x1c, FadeStart +0x20, Alpha +0x24, CurrentTime +0x28, FallOffTime +0x2c (0x30).
// The four float parameters feed SkipPoints, CurrentTime, FallOffTime, FadeStart in that order.
// Shape: base inline constructor (interpolation set argument, type immediate, zero channel bytes); the inline
// STLport list copy constructor: the alloc proxy is zeroed, the sentinel node malloc'd and self-linked, then
// `insert(begin(), x.begin(), x.end())` dispatches to the out-of-line range insert with the empty
// `__false_type` tag passed by const reference (homed in a dead argument slot) and the destination iterator
// built in its argument slot (a 4-byte iterator class with a user constructor).
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E

struct CEnginePrimitiveThemeChannelDesc {                // 4 bytes (PDB)
    unsigned char ChannelIndex0, ChannelIndex1, ChannelBlendFactor, Pad;
    CEnginePrimitiveThemeChannelDesc() : ChannelIndex0(0), ChannelIndex1(0), ChannelBlendFactor(0) { }
};
struct CEnginePrimitive {                                // 12 bytes (PDB)
    long                             Type;               // +0x00 EEnginePrimitiveType
    long                             InterpolationSet;   // +0x04 EEngineInterpolationSet
    CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;   // +0x08
    CEnginePrimitive(long type, long interpolationSet = 0) : Type(type), InterpolationSet(interpolationSet) { }
};

struct CWeaponTrailEntry { unsigned long m_data[7]; };   // 28 bytes (the range insert copies 7 dwords)

struct SFalseType { SFalseType() { } };                  // STLport __false_type dispatch tag
struct SListNodeBase { SListNodeBase* m_next; SListNodeBase* m_prev; };
struct SListNode : SListNodeBase { CWeaponTrailEntry m_data; };   // 0x24 bytes
struct SListIterator {                                   // 4-byte iterator with a user constructor
    SListNodeBase* m_node;
    SListIterator(SListNodeBase* node) : m_node(node) { }
    SListIterator(const SListIterator& other) : m_node(other.m_node) { }
};
struct SListConstIterator { SListNodeBase* m_node; };

struct CWeaponTrailList {                                // STLport list: the alloc proxy's node pointer only
    SListNode* m_node;
    CWeaponTrailList(const CWeaponTrailList& x) : m_node(0)
    {
        SListNode* node = (SListNode*)malloc(sizeof(SListNode));
        node->m_next = node;
        node->m_prev = node;
        m_node = node;
        InsertDispatch(begin(), x.cbegin(), x.cend(), SFalseType());
    }
    SListIterator begin() { return SListIterator(m_node->m_next); }
    SListConstIterator cbegin() const { SListConstIterator it; it.m_node = m_node->m_next; return it; }
    SListConstIterator cend() const { SListConstIterator it; it.m_node = m_node; return it; }
    void InsertDispatch(SListIterator pos, SListConstIterator first, SListConstIterator last, const SFalseType& tag);   // 0x0076FD40
};

struct CEnginePrimitiveWeaponTrail : CEnginePrimitive {
    CWeaponTrailList Trail;                              // +0x0c
    unsigned long    GraphicIndex;                       // +0x10
    unsigned long    GraphicFrame;                       // +0x14
    long             TrailType;                          // +0x18 EWeaponTrailType
    float            SkipPoints;                         // +0x1c
    float            FadeStart;                          // +0x20
    unsigned char    Alpha;                              // +0x24
    float            CurrentTime;                        // +0x28
    float            FallOffTime;                        // +0x2c   sizeof == 0x30 (retail)
    CEnginePrimitiveWeaponTrail(unsigned __int64 handle, const CWeaponTrailList& trail, unsigned long graphicIndex,
                                unsigned long graphicFrame, long trailType, unsigned char alpha, float skipPoints,
                                float currentTime, float fallOffTime, float fadeStart, long interpolationSet);
};

CEnginePrimitiveWeaponTrail::CEnginePrimitiveWeaponTrail(unsigned __int64 handle, const CWeaponTrailList& trail,
                                                         unsigned long graphicIndex, unsigned long graphicFrame,
                                                         long trailType, unsigned char alpha, float skipPoints,
                                                         float currentTime, float fallOffTime, float fadeStart,
                                                         long interpolationSet)
    : CEnginePrimitive(0x13, interpolationSet)           // ENGINE_PRIMITIVE_WEAPON_TRAIL
    , Trail(trail)
    , GraphicIndex(graphicIndex)
    , GraphicFrame(graphicFrame)
    , TrailType(trailType)
    , SkipPoints(skipPoints)
    , FadeStart(fadeStart)
    , Alpha(alpha)
    , CurrentTime(currentTime)
    , FallOffTime(fallOffTime)
{
}
