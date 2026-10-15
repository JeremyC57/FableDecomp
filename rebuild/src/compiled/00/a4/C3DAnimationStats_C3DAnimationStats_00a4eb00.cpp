// C3DAnimationStats::C3DAnimationStats() -- construct the animation statistics: base object, an empty timing-event multimap, and cleared loop/movement flags, duration and movement vector
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A4EB00, length 79 bytes. Callees: CBaseObject::CBaseObject @ 0x0099A380 (vtable
// 0x0129A7D8, clears +4; manifest label "CBaseIntelligentPointer"), malloc @ 0x00BFEA0E (the tree header node, 0x18 bytes).
// Vtable: C3DAnimationStats 0x0129D514 (its complete-object locator names `.?AVC3DAnimationStats@@`).
// Label correction: the manifest calls this a CRegionDef constructor (propagated); the vtable's RTTI and the neighbouring
// C3DAnimationStats::InitialiseStats @ 0x00A4EBB0 (which writes the same members in the same order) identify the class.
// Layout: Ego_r.pdb C3DAnimationStats: AnimType +0x08 (not initialised), Duration +0x0c, LoopFlag +0x10, MovementFlag +0x11,
// C3DVector MovementVector +0x12 (unaligned: the class is 2-byte packed; the PDB puts TimingEvents at +0x20, so two pad
// bytes follow the vector), multimap<CCharString, float> TimingEvents +0x20. Retail's multimap is the STLport tree: header
// node pointer +0x20, node count +0x24 (the PDB's 16-byte Dinkumware map is from a later build). No engine header exists.
// Shape: the inline STLport `_Rb_tree` constructor -- the header proxy is zeroed, then the header node is allocated, the
// count cleared, and `_M_empty_initialize` marks the header red, clears the root and points leftmost/rightmost at the
// header (each link store re-reads the member because the node links may alias it); the scalars are body assignments in
// InitialiseStats' order (initialisers would precede the malloc). The vector is cleared by an inline C3DVector member
// (`X = Y = Z = 0.0f`: stored Z, Y, X through the zero register); that inlined call is what flushes malloc's deferred
// `add esp,4` right before the vector stores -- written as a plain chained assignment the stack adjust lands straight
// after the tree set-up, as a temporary `C3DVector(0, 0, 0)` it goes through the frame.
// Leaf: ??0C3DAnimationStats
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E

struct CCharString { void* m_pStringData; };

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    void Clear() { X = Y = Z = 0.0f; }
};

struct SRbTreeNodeBase {                                // STLport _Rb_tree_node_base
    bool             _M_color;                          // +0x00 (false = red)
    SRbTreeNodeBase* _M_parent;                         // +0x04
    SRbTreeNodeBase* _M_left;                           // +0x08
    SRbTreeNodeBase* _M_right;                          // +0x0c
};
struct STimingEventNode : SRbTreeNodeBase {             // 0x18 bytes
    CCharString _M_key;
    float       _M_value;
};

struct CTimingEventMultimap {                           // STLport multimap<CCharString, float>
    SRbTreeNodeBase* _M_header;                         // +0x00
    unsigned long    _M_node_count;                     // +0x04
    CTimingEventMultimap() : _M_header(0)
    {
        _M_header = static_cast<SRbTreeNodeBase*>(malloc(sizeof(STimingEventNode)));
        _M_node_count = 0;
        _M_header->_M_color = false;
        _M_header->_M_parent = 0;
        _M_header->_M_left = _M_header;
        _M_header->_M_right = _M_header;
    }
};

struct CBaseObject {
    void* m_pFirstPointer;                              // +0x04
    CBaseObject();                                      // 0x0099A380
    virtual void v00();
};

enum EAnimationType { ANIMATION_TYPE_NONE = 0 };

#pragma pack(push, 2)
struct C3DAnimationStats : CBaseObject {
    EAnimationType       AnimType;                      // +0x08
    float                Duration;                      // +0x0c
    bool                 LoopFlag;                      // +0x10
    bool                 MovementFlag;                  // +0x11
    C3DVector            MovementVector;                // +0x12
    unsigned short       _pad1e;                        // +0x1e
    CTimingEventMultimap TimingEvents;                  // +0x20
    C3DAnimationStats();
    virtual void v00();
};
#pragma pack(pop)

C3DAnimationStats::C3DAnimationStats()
{
    LoopFlag = false;
    Duration = 0.0f;
    MovementFlag = false;
    MovementVector.Clear();
}