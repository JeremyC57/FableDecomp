// C3DMeshStripifier::C3DMeshStripifier() -- default constructor.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A8DF70, length 46 bytes.
// Manifest label "CAIStateGroup_HoverHornetQueen" is propagated; the vtable stored, 0x0129DF20, is
// C3DMeshStripifier (rtti-vtables.tsv). Callee: CBase::CBase @ 0x0099A2F0.
//
// Shape: base ctor, own vtable, then the embedded buffer block's inline constructor: four null pointers at
// +4..+0x10, a 3.0f cache-cost weight at +0x14 and a 500 iteration/strip-length limit at +0x18 (the
// scheduler shares one zero register and hoists the `xor` above the vtable store). Member names are
// descriptive.
struct CBase {
    virtual void v00();
    CBase();                                    // 0x0099A2F0
};

struct SStripifierBuffers {                     // embedded block with an inline constructor: that is what keeps
    void* m_pVertices;                          // +0x04   the vtable store AHEAD of the four null stores (a plain
    void* m_pIndices;                           // +0x08   initialiser list sinks the vtable store below them)
    void* m_pStrips;                            // +0x0c
    void* m_pAdjacency;                         // +0x10
    SStripifierBuffers() : m_pVertices(0), m_pIndices(0), m_pStrips(0), m_pAdjacency(0) { }
};

class C3DMeshStripifier : public CBase { public:
    SStripifierBuffers m_buffers;               // +0x04
    float m_cacheWeight;                        // +0x14 = 3.0f
    long m_maxStripLength;                      // +0x18 = 500
    virtual void v00();
    C3DMeshStripifier();
};

C3DMeshStripifier::C3DMeshStripifier()
    : CBase()
    , m_buffers()
    , m_cacheWeight(3.0f)
    , m_maxStripLength(500)
{
}