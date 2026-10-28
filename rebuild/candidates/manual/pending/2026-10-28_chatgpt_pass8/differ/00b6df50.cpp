// CEngineWaterRenderer::GetIceForegroundVertexShader(long lightCount) -- clamp the light-count variant and return its ice foreground shader.
// Leaf: ?GetIceForegroundVertexShader@CEngineWaterRenderer@@AAEAAVCVertexShader@@J@Z
// Identity: proven; own RenderIceForeground caller00B75B6C and original PDB private GetIceForegroundVertexShader(long)->CVertexShader& API; own array offset0x12c exactly shared Water header; propagated LandscapeRenderer/SpotLight names false.
// Layout: shared CVertexShader included unchanged; shared CEngineWaterRenderer data header reused under alias, typed CArray/STLport-vector projection over named opaque shader array. Method is original private nonvirtual API. Requires a nonempty shader array, as retail.
#include "engine/CVertexShader.h"
#define CEngineWaterRenderer CEngineWaterRenderer_HeaderFields
#include "engine/CEngineWaterRenderer.h"
#undef CEngineWaterRenderer
namespace std{template<class T>class allocator{};template<class V,class T,class Al>class _STLP_alloc_proxy:public Al{public:V _M_data;};template<class T,class Al>class _Vector_base{public:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,Al> _M_end_of_storage;};template<class T,class Al=allocator<T> >class vector:public _Vector_base<T,Al>{public:unsigned int size()const{return (unsigned int)(this->_M_finish-this->_M_start);}T& operator[](unsigned int i){return this->_M_start[i];}};}template<class T>class CArray:public std::vector<T>{};
class CEngineWaterRenderer:public CEngineWaterRenderer_HeaderFields{private:CVertexShader& GetIceForegroundVertexShader(long);friend struct TestAccess;};
CVertexShader& CEngineWaterRenderer::GetIceForegroundVertexShader(long lightCount){CArray<CVertexShader>& shaders=*(CArray<CVertexShader>*)&IceForegroundVertexShader;if((unsigned long)lightCount<shaders.size()-1)return shaders[(unsigned int)lightCount];return shaders[shaders.size()-1];}
