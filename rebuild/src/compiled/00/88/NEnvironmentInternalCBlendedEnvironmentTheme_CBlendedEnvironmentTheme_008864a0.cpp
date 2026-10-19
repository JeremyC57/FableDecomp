// NEnvironmentInternal::CBlendedEnvironmentTheme::CBlendedEnvironmentTheme() -- construct the 3 embedded STLport texture-set vectors first, then zero 19 colour records and 30 scalers.
// Retail: Fable.exe (TLC, VC7.1) @ 0x008864A0, 281 bytes; no callees. No generated CBlendedEnvironmentTheme header exists, so this local PDB-named view is checked against landed CBlendedEnvironmentTheme::operator= 0x006B5DF0.
// Retail layout: TimeOfDay+0, Colours+4, Scalers+0x134, TextureSets+0x1ac. The native 12-byte CArray/STLport model and its initialization shape follow landed CThemeChannel::CThemeChannel 006B5FB0; PDB CArray's 16-byte donor layout is too large.
template<class T> struct allocator { allocator(){} allocator(const allocator&){} };
template<class Value,class T,class Alloc> struct _STLP_alloc_proxy : Alloc { Value _M_data; _STLP_alloc_proxy(const Alloc& a,Value p):Alloc(a),_M_data(p){} };
template<class T> struct _Vector_base { typedef allocator<T> allocator_type; T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,allocator_type> _M_end_of_storage; _Vector_base(const allocator_type& a):_M_start(0),_M_finish(0),_M_end_of_storage(a,(T*)0){} };
template<class T> struct vector : _Vector_base<T> { typedef allocator<T> allocator_type; explicit vector(const allocator_type& a=allocator_type()):_Vector_base<T>(a){} };
template<class T> struct CArray : vector<T> { CArray(){} };
struct CRGBFloatColour { float R,G,B,A; };
struct CIDrawEnvironmentTextureSet { CArray<long> Textures; CArray<float> Blends; };
namespace NEnvironmentInternal {
struct CBlendedEnvironmentTheme { float TimeOfDay; CRGBFloatColour Colours[19]; float Scalers[30]; CIDrawEnvironmentTextureSet TextureSets[3]; CBlendedEnvironmentTheme(); };
CBlendedEnvironmentTheme::CBlendedEnvironmentTheme() {
 float* blue=&Colours[0].B;
 for(int i=0;i<19;++i){blue[-2]=0.0f;blue[-1]=0.0f;blue[0]=0.0f;blue[1]=0.0f;blue+=4;}
 for(int i=0;i<30;++i) Scalers[i]=0.0f;
}
}