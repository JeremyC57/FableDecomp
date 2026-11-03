// Standalone fixture: active production headers and exact target body are expanded below.
// Host-only long/int mapping preserves native 32-bit stream words; pointers remain 64-bit.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _MSC_VER
#include <math.h>
#else
extern "C" double sqrt(double);
#endif
#include <assert.h>
#include <stddef.h>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#define __declspec(x)
#define long int
#endif
#define FABLE_FASTCALL __fastcall
#define private public
#define protected public


#define FABLE_RETAIL_ENGINE_COMPONENT
#define FABLE_RETAIL_STIPPLE
#define FABLE_ENGINE_CENGINERESOURCEMANAGER_H



#define FABLE_ENGINE_CENGINECOMPONENT_H


#define FABLE_ENGINE_CCHARSTRING_H


class CCharStringData;
class CCharString {
public:
    CCharString();
    CCharString(const char* text, long length = -1);
    CCharString(const CCharString& other);
    ~CCharString();
protected:
    CCharStringData* PStringData;
};
class CCharString;class CEngineComponentParams;class CEngineComponentLoadNewSceneParams;class CEngineComponentRenderParams;
class CEngineComponent {
public:
 CEngineComponent();
 virtual ~CEngineComponent();
 virtual void FrameStart(const CEngineComponentParams&);
 virtual void FrameEnd(const CEngineComponentParams&);
 virtual bool LoadNewScene(const CEngineComponentLoadNewSceneParams&);
 virtual void Render(const CEngineComponentRenderParams&);
 virtual void PreLayerRender(const CEngineComponentRenderParams&);
 virtual void PostLayerRender(const CEngineComponentRenderParams&);
 virtual void WorldFrameUpdate(bool);
 virtual void PausedWorldFrameUpdate();
 virtual CCharString GetConsoleEnableFunctionName();
 virtual unsigned long PeekSceneFilterFlags()const=0;
 virtual void OnReleaseDefaultPoolResources();
 virtual bool OnRestoreDefaultPoolResources();
private:
 void** GlobalPointerAddress;
 bool RenderEnabled;
 unsigned char KeepAlive;
};
#define FABLE_ENGINE_CASYNCLOADINGPROCESSCALLBACK_H

class CAsyncLoadingProcessCallback {public:virtual void ProcessLoading()=0;};
#define FABLE_ENGINE_CARRAY_H


#define FABLE_STLPORT46_STIPPLE_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
inline void* __cdecl operator new(size_t,void* p){return p;}
namespace std {
struct __true_type {};
template<class T>class allocator {public:T* allocate(size_t n,const void* =0);void deallocate(T* p,size_t){if(p)free(p);}};
template<class T>T* allocator<T>::allocate(size_t n,const void*){return n?(T*)malloc(n*sizeof(T)):0;}
template<class V,class T,class A>class _STLP_alloc_proxy:public A {public:V _M_data;_STLP_alloc_proxy(const A&a,V p):A(a),_M_data(p){}};
template<class T>inline const T&(min)(const T&a,const T&b){return b<a?b:a;}
template<class T>inline const T&(max)(const T&a,const T&b){return a<b?b:a;}
template<class T>inline void swap(T&a,T&b){T temp=a;a=b;b=temp;}
template<class T,class U>inline void _Construct(T*p,const U&v){new(p)T(v);}
inline void* __copy_trivial(const void*first,const void*last,void*result){return (last==first)?result:(char*)memmove(result,first,(const char*)last-(const char*)first)+((const char*)last-(const char*)first);}
template<class T,class N>inline T*fill_n(T*first,N count,const T&v){for(;count>0;--count,++first)*first=v;return first;}

template<class T,class A>class _Vector_base {public:_Vector_base(const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){} _Vector_base(size_t n,const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){_M_start=_M_end_of_storage.allocate(n);_M_finish=_M_start;_M_end_of_storage._M_data=_M_start+n;} ~_Vector_base(){if(_M_start)_M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start);}protected:T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{
public:
 typedef unsigned int size_type;typedef T*iterator;typedef const T*const_iterator;
 explicit vector(const A&a=A()):_Vector_base<T,A>(a){}
 explicit vector(size_type n,const T&value=T(),const A&a=A()):_Vector_base<T,A>(n,a){this->_M_finish=fill_n(this->_M_start,n,value);}
 iterator begin(){return this->_M_start;}const_iterator begin()const{return this->_M_start;}
 iterator end(){return this->_M_finish;}const_iterator end()const{return this->_M_finish;}
 size_type size()const{return size_type(this->_M_finish-this->_M_start);}
 T&operator[](size_type n){return this->_M_start[n];}const T&operator[](size_type n)const{return this->_M_start[n];}
 void push_back(const T&x){if(this->_M_finish!=this->_M_end_of_storage._M_data){_Construct(this->_M_finish,x);++this->_M_finish;}else _M_insert_overflow(this->_M_finish,x,__true_type(),1,true);}
 iterator erase(iterator first,iterator last){T*out=(T*)__copy_trivial(last,this->_M_finish,first);this->_M_finish=out;return first;}
 void resize(size_type n,T x){if(n<size())erase(begin()+n,end());else _M_fill_insert(end(),n-size(),x);}
 void resize(size_type n){resize(n,T());}
 __declspec(noinline) void _M_fill_insert(iterator,size_type,const T&);
protected:
 void _M_insert_overflow(iterator pos,const T&x,const __true_type&,size_type fill_len,bool at_end){const size_type old_size=size();const size_type len=old_size+(max)(old_size,fill_len);T*start=this->_M_end_of_storage.allocate(len);T*finish=(T*)__copy_trivial(this->_M_start,pos,start);finish=fill_n(finish,fill_len,x);if(!at_end)finish=(T*)__copy_trivial(pos,this->_M_finish,finish);this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);this->_M_start=start;this->_M_finish=finish;this->_M_end_of_storage._M_data=start+len;}
};
}

template<class T>class CArray:public std::vector<T>{public:CArray(){} CArray(unsigned int n,const T&v=T()):std::vector<T>(n,v){} T&operator[](unsigned int i){return std::vector<T>::operator[](i);}const T&operator[](unsigned int i)const{return std::vector<T>::operator[](i);}};
class CMeshDataBank;class CGraphicDataBank;class CFontBank;
class CEngineResourceManager:public CEngineComponent,public CAsyncLoadingProcessCallback {
public:
 virtual ~CEngineResourceManager();
 virtual void ProcessLoading();
 virtual unsigned long PeekSceneFilterFlags()const;
private:
 void BuildAlphaStippleTexture(long,CArray<unsigned char>&);
 CMeshDataBank*MeshBanks[3];CMeshDataBank*MainMeshBank;
 unsigned char EditorMeshBank[0x460],EngineMeshBank[0x460]; 
 unsigned char LoadingMemoryPool[8]; 
 unsigned char LoadingMemoryFailureHandler[8]; 
 unsigned char AnimationMemoryPool[8],ClothMemoryPool[8],MeshDataMemoryPool[8]; 
 unsigned char AnimationManager[0x4c]; 
 CGraphicDataBank*GraphicBanks[1];CGraphicDataBank*MainGraphicBank;const CFontBank*EngineFont;
 unsigned char VShaderBanks[27*0x128],ShaderBankPixel[0x128]; 
 unsigned char VShaderResources[27*8],ShaderResourcePixel[8]; 
 double MaxLoadingTime;
 unsigned long VertexBufferUsageFlags;
 unsigned char StipleFadeTexture[8]; 
 CArray<unsigned long>HWInverseStipplePatterns;
 unsigned char DitherTextureList[12]; 
 unsigned char EngineCachePath[4],WriteableEngineCachePath[4]; 
 unsigned char ZeroRGBATexture[8],SatRGBATexture[8]; 
 unsigned char VSConstantLayoutBasic[0x14c]; 
 unsigned char VSConstantLayoutLights[0x14c]; 
 unsigned char VSConstantLayoutLightsBones[0x14c]; 
 unsigned char VSConstantLayoutRepeatedMesh[0x17c]; 
 unsigned char VSConstantLayoutParticles[0x14c]; 
 unsigned char GPUScratchBufferTextureManager[0x44]; 
 unsigned char ExternalTexturesAllocatedFromEnginePools[12]; 
};
#define FABLE_ENGINE_C2DVECTOR_H



class C2DVector {
public:
    float X,Y;
    C2DVector() {}
    C2DVector(float x,float y):X(x),Y(y){}
    C2DVector(const C2DVector& p):X(p.X),Y(p.Y){}
    C2DVector operator+(const C2DVector& vector) const {return C2DVector(X+vector.X,Y+vector.Y);}
    C2DVector operator-(const C2DVector& vector) const {return C2DVector(X-vector.X,Y-vector.Y);}
    const C2DVector operator-() const {return C2DVector(-X,-Y);}
    const C2DVector operator*(float scalar) const {return C2DVector(X*scalar,Y*scalar);}
    const C2DVector& operator+=(const C2DVector& vector) {X+=vector.X;Y+=vector.Y;return *this;}
    float Dot(const C2DVector& vector) const {return X*vector.X+Y*vector.Y;}
    float GetSquaredMagnitude()const{return (Y*Y+X*X);}
};
#define FABLE_ENGINE_GFRANDOM_H


long __fastcall GFROR13(unsigned long);
inline unsigned long __fastcall GFRandom(unsigned long range,unsigned long*seed){*seed=(unsigned long)GFROR13(*seed*0x24a1+0x24df);return range?*seed%range:0;}
#include <stdlib.h>
#define min(a,b) (((a)<(b))?(a):(b))
void CEngineResourceManager::BuildAlphaStippleTexture(long size,CArray<unsigned char>&buffer)
{
 CArray<long> point_order_list;
 for(long p=0;p<size*size;++p)point_order_list.push_back(p);
 unsigned long seed=0;
 for(long p=0;p<size*size;++p)std::swap(point_order_list[p],point_order_list[GFRandom(size*size,&seed)]);
 float max_density=0.0f;
 for(long sy=0;sy<size;++sy)for(long sx=0;sx<size;++sx)if(sx||sy){
  C2DVector dist((float)min(min(labs(-sx),labs(-sx+size)),labs(-sx-size)),(float)min(min(labs(-sy),labs(-sy+size)),labs(-sy-size)));
  max_density+=1.0/(dist.GetSquaredMagnitude()*dist.GetSquaredMagnitude());
 }
 CArray<bool>global_point_mask(point_order_list.size(),false);
 CArray<long>point_list;
 float density_step=max_density*0.125f;
 for(float density_threshold=density_step;density_threshold<max_density+density_step*2;density_threshold+=density_step){
  CArray<bool>section_point_mask(point_order_list.size(),false);
  for(long n=0;n<8;++n){
   for(long rp=0;rp<point_order_list.size();++rp){
    long src_point=point_order_list[rp];
    if(!global_point_mask[src_point]){
     long x=src_point%size,y=src_point/size;
     float total_density=0.0f;
     for(long p=0;p<global_point_mask.size();++p){
      if(src_point!=p&&(global_point_mask[p]!=false||section_point_mask[p]!=false)){
       long sx=p%size,sy=p/size;
       C2DVector dist((float)min(min(labs(x-sx),labs(x-sx+size)),labs(x-sx-size)),(float)min(min(labs(y-sy),labs(y-sy+size)),labs(y-sy-size)));
       float density=1.0/(dist.GetSquaredMagnitude()*dist.GetSquaredMagnitude());total_density+=density;
       if(total_density>density_threshold)break;
      }
     }
     if(total_density<density_threshold)section_point_mask[src_point]=true;else section_point_mask[src_point]=false;
    }
   }
  }
  for(long p=0;p<point_order_list.size();++p){long point=point_order_list[p];if(section_point_mask[point]){global_point_mask[point]=true;point_list.push_back(point);}}
 }
 buffer.resize(size*size);
 for(long p=0;p<point_list.size();++p){long point=point_list[p];long sx=point%size,sy=point/size;long alpha=min(p*255/point_list.size()+1,255UL);buffer[sy*size+sx]=(unsigned char)alpha;}
}
CCharString::CCharString():PStringData(0){} CCharString::CCharString(const CCharString& x):PStringData(x.PStringData){} CCharString::~CCharString(){}
CEngineComponent::CEngineComponent():GlobalPointerAddress(0),RenderEnabled(true),KeepAlive(0){}
CEngineComponent::~CEngineComponent(){}
void CEngineComponent::FrameStart(const CEngineComponentParams&){}
void CEngineComponent::FrameEnd(const CEngineComponentParams&){}
bool CEngineComponent::LoadNewScene(const CEngineComponentLoadNewSceneParams&){return true;}
void CEngineComponent::Render(const CEngineComponentRenderParams&){}
void CEngineComponent::PreLayerRender(const CEngineComponentRenderParams&){}
void CEngineComponent::PostLayerRender(const CEngineComponentRenderParams&){}
void CEngineComponent::WorldFrameUpdate(bool){}
void CEngineComponent::PausedWorldFrameUpdate(){}
CCharString CEngineComponent::GetConsoleEnableFunctionName(){return CCharString();}
void CEngineComponent::OnReleaseDefaultPoolResources(){}
bool CEngineComponent::OnRestoreDefaultPoolResources(){return true;}

#undef min
CEngineResourceManager::~CEngineResourceManager(){}
void CEngineResourceManager::ProcessLoading(){}
unsigned long CEngineResourceManager::PeekSceneFilterFlags()const{return 0;}
long __fastcall GFROR13(unsigned long x){return (long)((x>>13)|(x<<19));}
namespace std {template<class T,class A>void vector<T,A>::_M_fill_insert(iterator pos,size_type n,const T&x){size_type off=(size_type)(pos-this->_M_start);if(!n)return;size_type old=size();T*next=(T*)malloc((old+n)*sizeof(T));assert(next);for(size_type i=0;i<off;++i)next[i]=this->_M_start[i];for(size_type j=0;j<n;++j)next[off+j]=x;for(size_type k=off;k<old;++k)next[k+n]=this->_M_start[k];free(this->_M_start);this->_M_start=next;this->_M_finish=next+old+n;this->_M_end_of_storage._M_data=this->_M_finish;}}
static void expected(unsigned n,unsigned char*out){
 unsigned count=n*n,permutation[64],order[64],used=0;bool accepted[64],section[64];float weight[64][64];unsigned state=0;
 memset(accepted,0,sizeof(accepted));memset(out,0,count);for(unsigned i=0;i<count;++i)permutation[i]=i;
 for(unsigned i=0;i<count;++i){unsigned x=state*0x24a1U+0x24dfU;state=(x>>13)|(x<<19);unsigned j=state%count,save=permutation[i];permutation[i]=permutation[j];permutation[j]=save;}
 for(unsigned a=0;a<count;++a)for(unsigned b=0;b<count;++b){unsigned dx=(a%n+n-b%n)%n,dy=(a/n+n-b/n)%n;if(dx>n-dx)dx=n-dx;if(dy>n-dy)dy=n-dy;unsigned square=dx*dx+dy*dy;weight[a][b]=square?(float)(1.0/(square*square)):0.0f;}
 float maxDensity=0;for(unsigned b=1;b<count;++b){unsigned dx=b%n,dy=b/n;if(dx>n-dx)dx=n-dx;if(dy>n-dy)dy=n-dy;unsigned square=dx*dx+dy*dy;maxDensity=(float)(maxDensity+1.0/(square*square));}
 float step=maxDensity*0.125f;for(float threshold=step;threshold<maxDensity+step*2;threshold+=step){
  memset(section,0,sizeof(section));for(unsigned repeat=0;repeat<8;++repeat)for(unsigned q=0;q<count;++q){unsigned a=permutation[q];if(accepted[a])continue;float total=0;for(unsigned b=0;b<count;++b)if(a!=b&&(accepted[b]||section[b])){total+=weight[a][b];if(total>threshold)break;}section[a]=total<threshold;}
  for(unsigned q=0;q<count;++q){unsigned a=permutation[q];if(section[a]){assert(!accepted[a]);accepted[a]=true;order[used++]=a;}}
 }
 for(unsigned i=0;i<used;++i){unsigned alpha=i*255/used+1;out[order[i]]=(unsigned char)(alpha<255?alpha:255);}
}
static void run(unsigned n,unsigned prior){CEngineResourceManager manager;manager.VertexBufferUsageFlags=0x5261;CArray<unsigned char>buffer;buffer.resize(prior);for(unsigned i=0;i<prior;++i)buffer[i]=0x9d;unsigned char answer[64];expected(n,answer);if(n==1&&prior)answer[0]=0x9d;manager.BuildAlphaStippleTexture(n,buffer);assert(buffer.size()==n*n);for(unsigned i=0;i<n*n;++i)if(buffer[i]!=answer[i]){printf("mismatch n=%u i=%u actual=%u expected=%u\n",n,i,(unsigned)buffer[i],(unsigned)answer[i]);assert(false);}assert(manager.VertexBufferUsageFlags==0x5261);if(n>1){unsigned nonzero=0;for(unsigned i=0;i<n*n;++i)nonzero+=buffer[i]!=0;assert(nonzero==n*n);}manager.BuildAlphaStippleTexture(n,buffer);for(unsigned i=0;i<n*n;++i)assert(buffer[i]==answer[i]);}
int main(){const unsigned sizes[]={0,1,2,3,4,5,6,8};for(unsigned i=0;i<8;++i)for(unsigned prior=0;prior<3;++prior)run(sizes[i],prior==0?0:prior==1?3:80);puts("00b3be10_TEST PASS");return 0;}
