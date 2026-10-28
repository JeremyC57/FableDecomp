// std::__copy_backward_ptrs(iterator first, iterator last, iterator result) -- copy the range backward and return its new beginning.
// Leaf: ??$__copy_backward_ptrs@UCResourceEntry@CGPUScratchBufferTextureManager@@@std@@YIPAUCResourceEntry@CGPUScratchBufferTextureManager@@PAU12@00ABU__false_type@0@@Z
// Identity: probable helper spelling; proven mutable element: own resource44 fillinsert00F381F0 and resize00F385A0 <- AllocResourceEntry00F372B0, hand953J101/J102 confirm. First row has three arguments; latter two also receive the trivial-assignment tag reference, as shown in their actual fill-insert callers.
// Layout: real PDB record fields/class keys and sizes40/44/44; shared CPixelFormat reused. No matching record header; outer classes supply actual nested naming scope only. Ordinary trivial assignment emits rep movsd per record.
#ifndef _MSC_VER
#define __fastcall
#endif
enum EResourceType{RESOURCE_NONE};enum ESurfaceMultisampleType{MS_NONE};enum ESurfacePool{POOL_NONE};class CTexture;class CSurface;class C2DExtentsI{public:long X,Y;};
#include "engine/CPixelFormat.h"
class CGPUScratchBufferTextureManager{public:struct CResourceEntry{EResourceType ResourceType;CTexture* Texture;CSurface* Surface;long PageIndex;C2DExtentsI Dimensions;long NoLevels;CPixelFormat PixelFormat;ESurfaceMultisampleType MultiSampleType;unsigned long UseageFlags;ESurfacePool MemoryPool;};};
#ifndef _MSC_VER
#define __forceinline inline
#endif
namespace std{struct __false_type{};struct random_access_iterator_tag{};template<class Iter,class Out,class Distance>__forceinline Out __fastcall __copy_backward(Iter first,Iter last,Out result,const random_access_iterator_tag&,Distance*){for(Distance n=last-first;n>0;--n)*--result=*--last;return result;}template<class T>T* __fastcall __copy_backward_ptrs(T* first,T* last,T* result,const __false_type&){return __copy_backward(first,last,result,random_access_iterator_tag(),(int*)0);}}
typedef CGPUScratchBufferTextureManager::CResourceEntry Element;
template Element* __fastcall std::__copy_backward_ptrs<Element>(Element*,Element*,Element*,const std::__false_type&);