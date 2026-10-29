#include <cstdio>
#include <cstddef>
// Behaviour model of CVertexShaderInput::CVertexShaderInput @ 0x00AB3B10.
class CBaseClass {
public:  virtual void v00() {} CBaseClass() {} };
struct SVertexShaderInputState { void* m_pDeclaration; void* m_pStreams; long m_streamCount, m_elementCount, m_stride, m_flags;
    SVertexShaderInputState() : m_pDeclaration(0), m_pStreams(0), m_streamCount(0), m_elementCount(0), m_stride(0), m_flags(0) {} };
class CVertexShaderInput : public CBaseClass { public: SVertexShaderInputState m_state; virtual void v00() {} CVertexShaderInput() : CBaseClass(), m_state() {} };
int main()
{
    CVertexShaderInput v;
    bool ok = v.m_state.m_pDeclaration == 0 && v.m_state.m_pStreams == 0 && v.m_state.m_streamCount == 0 && v.m_state.m_elementCount == 0 && v.m_state.m_stride == 0 && v.m_state.m_flags == 0;
    if (sizeof(void*) == 4) ok = ok && offsetof(CVertexShaderInput, m_state) == 0x4 && offsetof(SVertexShaderInputState, m_flags) == 0x14;
    if (!ok) { std::printf("00ab3b10_TEST FAIL\n"); return 1; }
    std::printf("00ab3b10_TEST PASS\n");
    return 0;
}