// CEngineWaterRenderer::GetIceForegroundShadowedVertexShader(long lightCount) -- clamp the light-count variant and return its ice foreground shader.
// Leaf: ?GetIceForegroundShadowedVertexShader@CEngineWaterRenderer@@AAEAAVCVertexShader@@J@Z
// Identity: proven; own RenderIceForeground caller and original private PDB API, original Water array offsets138/144; own RTTI CEngineWaterRenderer : CEngineComponent.
// Model revision: own RTTI CVertexShader:CShaderBase and two-slot base0129a030, protected CShaderBase ctor/virtualdtor/constGetDebugName; original public Vertex ctor/copy/dtor/GetDebugName. Shared VertexShader flattened layout quarantined under alias (including wrong struct-key Data forward); real class CVertexShaderData remains forward-only. Correct protected vector base/proxy and original CArray::operator[](uint) forwarding API explicitly declared per PDB; clamp expression unchanged, no speculative helper added.
// Layout: genuine original CEngineComponent primary virtual base12 with original GlobalPointerAddress/RenderEnabled/KeepAlive fields. Shared component/water headers retained unchanged under layout aliases; source restores genuine typed CArray/STLport vector shader arrays. Unused intervening buffers/shaders explicitly unmodeled; no array rebinding over fake storage. Shared CVertexShader data header unchanged. Capped00B6DF50 representative retains separate original-base debt; this correction applies only to these two siblings.
#define CVertexShader CVertexShader_HeaderEvidence
#define CVertexShaderData CVertexShaderData_HeaderEvidence
#include "engine/CVertexShader.h"
#undef CVertexShaderData
#undef CVertexShader
#define CEngineComponent CEngineComponent_HeaderLayout
#include "engine/CEngineComponent.h"
#undef CEngineComponent
#define CEngineWaterRenderer CEngineWaterRenderer_HeaderLayout
#include "engine/CEngineWaterRenderer.h"
#undef CEngineWaterRenderer
class CCharString;class CVertexShaderData;class CShaderBase{protected:CShaderBase();virtual ~CShaderBase();virtual CCharString GetDebugName()const;};class CVertexShader:public CShaderBase{public:CVertexShaderData*Data;CVertexShader();CVertexShader(const CVertexShader&);virtual ~CVertexShader();virtual CCharString GetDebugName()const;};
namespace std{template<class T>class allocator{};template<class V,class T,class Al>class _STLP_alloc_proxy:public Al{public:V _M_data;};template<class T,class Al>class _Vector_base{protected:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,Al> _M_end_of_storage;};template<class T,class Al=allocator<T> >class vector:protected _Vector_base<T,Al>{public:unsigned int size()const{return (unsigned int)(this->_M_finish-this->_M_start);}T& operator[](unsigned int i){return this->_M_start[i];}};}template<class T>class CArray:public std::vector<T>{public:T& operator[](unsigned int n){return std::vector<T>::operator[](n);}};class CEngineComponent{public:CEngineComponent();virtual ~CEngineComponent();void** GlobalPointerAddress;bool RenderEnabled;unsigned char KeepAlive;};
class CEngineWaterRenderer:public CEngineComponent{public:unsigned char _unmodeledBuffersAndShaders0C[0x12C-0x0C];CArray<CVertexShader> IceForegroundVertexShader,IceForegroundShadowedVertexShader,IceForegroundColbuffShadowedVertexShader;private:CVertexShader& GetIceForegroundShadowedVertexShader(long);friend struct TestAccess;};CVertexShader& CEngineWaterRenderer::GetIceForegroundShadowedVertexShader(long lightCount){CArray<CVertexShader>& shaders=IceForegroundShadowedVertexShader;if((unsigned long)lightCount<shaders.size()-1)return shaders[(unsigned int)lightCount];return shaders[shaders.size()-1];}

#ifdef _MSC_VER
typedef char ShaderSize[sizeof(CVertexShader)==8?1:-1];typedef char ComponentSize[sizeof(CEngineComponent)==12?1:-1];
#endif
