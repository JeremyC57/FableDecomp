// CParticleUpdateParamDesc::CParticleUpdateParamDesc(void* name, void* param) -- CBase, the name at +4, own vtable, the parameter at +8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B0F310, length 34 bytes.
// Callees: CBaseClass::CBaseClass @ 0x0099A2D0. Vtable 0x0125FEE4 = CParticleUpdateParamDesc (rtti-vtables.tsv); the
// chunk label "CVertexShaderData::CVertexShaderData" is propagated over this byte-identical shell.
// Shape: first argument stored at +4 before the vtable store (`__declspec(novtable)` argument base), the
// second at +8 after it; `ret 8`.
class CBaseClass {
public:
    virtual void v00();
    CBaseClass();                                  // 0x0099A2D0
};

struct __declspec(novtable) CParticleUpdateParamDescBase : CBaseClass {
    void* m_pName;                            // +0x04
    CParticleUpdateParamDescBase(void* name) : CBaseClass(), m_pName(name) { }
};

struct CParticleUpdateParamDesc : CParticleUpdateParamDescBase {   // vtable 0x0125FEE4
    void* m_pParam;                           // +0x08
    virtual void v00();
    CParticleUpdateParamDesc(void* name, void* param);
};

CParticleUpdateParamDesc::CParticleUpdateParamDesc(void* name, void* param)
    : CParticleUpdateParamDescBase(name), m_pParam(param)
{
}