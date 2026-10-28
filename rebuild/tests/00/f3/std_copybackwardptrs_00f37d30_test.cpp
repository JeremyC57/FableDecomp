#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#endif
enum EResourceType{RESOURCE_NONE};enum ESurfaceMultisampleType{MS_NONE};enum ESurfacePool{POOL_NONE};class CTexture;class CSurface;class C2DExtentsI{public:long X,Y;};class CPixelFormat{public:long FormatIndex;};class CGPUScratchBufferTextureManager{public:struct CResourceEntry{EResourceType ResourceType;CTexture* Texture;CSurface* Surface;long PageIndex;C2DExtentsI Dimensions;long NoLevels;CPixelFormat PixelFormat;ESurfaceMultisampleType MultiSampleType;unsigned long UseageFlags;ESurfacePool MemoryPool;};};
#ifndef _MSC_VER
#define __forceinline inline
#endif
namespace std{struct __false_type{};struct random_access_iterator_tag{};template<class Iter,class Out,class Distance>__forceinline Out __fastcall __copy_backward(Iter first,Iter last,Out result,const random_access_iterator_tag&,Distance*){for(Distance n=last-first;n>0;--n)*--result=*--last;return result;}template<class T>T* __fastcall __copy_backward_ptrs(T* first,T* last,T* result,const __false_type&){return __copy_backward(first,last,result,random_access_iterator_tag(),(int*)0);}}
typedef CGPUScratchBufferTextureManager::CResourceEntry Element;
template Element* __fastcall std::__copy_backward_ptrs<Element>(Element*,Element*,Element*,const std::__false_type&);
int main(){Element data[4],saved[3];memset(data,0,sizeof(data));for(int i=0;i<4;++i){Element& x=data[i];x.PageIndex=i+5;x.Dimensions.X=i+7;x.Dimensions.Y=i+9;x.PixelFormat.FormatIndex=i+11;x.NoLevels=i+1;x.UseageFlags=i+13;}memcpy(saved,data,sizeof(saved));if(std::__copy_backward_ptrs(data,data+3,data+4,std::__false_type())!=data+1 || memcmp(data+1,saved,sizeof(saved)))goto fail;if(std::__copy_backward_ptrs(data,data,data+4,std::__false_type())!=data+4)goto fail;puts("00f37d30_TEST PASS");return 0;fail:puts("00f37d30_TEST FAIL");return 1;}