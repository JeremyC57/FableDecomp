// CVertexShaderInput::CVertexShaderInput() -- default constructor.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB3B10, length 38 bytes.
// Manifest label "CAIStateGroup_HoverHornetQueen" is propagated; the vtable stored, 0x0129EA08, is
// CVertexShaderInput (rtti-vtables.tsv). Callee: CBase::CBase @ 0x0099A2D0.
//
// Shape: base ctor, own vtable, then the embedded state block's six dwords at +4..+0x18 zeroed from one
// cleared register (the `xor` is hoisted above the vtable store). The member types are not recoverable from the body;
// pointers/counts named descriptively.
namespace N_0099a2d0 { struct CBase {
    virtual void v00();
    CBase();                                    // 0x0099A2D0
}; } using N_0099a2d0::CBase;

struct SVertexShaderInputState {                // embedded block with an inline constructor: that is what keeps
    void* m_pDeclaration;                       // +0x04   the vtable store AHEAD of the zero stores (a plain
    void* m_pStreams;                           // +0x08   initialiser list sinks the vtable store below them)
    long m_streamCount;                         // +0x0c
    long m_elementCount;                        // +0x10
    long m_stride;                              // +0x14
    long m_flags;                               // +0x18
    SVertexShaderInputState() : m_pDeclaration(0), m_pStreams(0), m_streamCount(0), m_elementCount(0), m_stride(0), m_flags(0) { }
};

class CVertexShaderInput : public CBase { public:
    SVertexShaderInputState m_state;            // +0x04
    virtual void v00();
    CVertexShaderInput();
};

CVertexShaderInput::CVertexShaderInput()
    : CBase()
    , m_state()
{
}