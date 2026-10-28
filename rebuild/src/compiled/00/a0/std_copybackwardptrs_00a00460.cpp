// std::__copy_backward_ptrs(iterator first, iterator last, iterator result) -- copy the range backward and return its new beginning.
// Leaf: ??$__copy_backward_ptrs@VCGraphicInfo@@@std@@YIPAVCGraphicInfo@@PAV1@00ABU__false_type@0@@Z
// Identity: probable helper spelling; proven mutable element: own CGraphicInfo44 fillinsert00A01290 and resize00A01790/00A01820; hand952H022 proves actual CGraphicInfo. First row has three arguments; latter two also receive the trivial-assignment tag reference, as shown in their actual fill-insert callers.
// Layout: real PDB record fields/class keys and sizes40/44/44; shared CPixelFormat reused. No matching record header; outer classes supply actual nested naming scope only. Ordinary trivial assignment emits rep movsd per record.
#ifndef _MSC_VER
#define __fastcall
#endif

#include "engine/CPixelFormat.h"
enum ETextureType{TEX_NONE};class CGraphicHeader{public:unsigned short Width,Height,Depth,FrameWidth,FrameHeight,FrameCount;CPixelFormat PixelFormat;unsigned char TransparencyType,MipmapLevels;unsigned char IsFlatAnimatedGraphic:1;unsigned long FrameDataSize,CompressedMipmapSizes[1];};class CGraphicDataBank;class CGraphicInfoFrame;class CGraphicInfo{public:CGraphicHeader Header;ETextureType TextureType;CGraphicDataBank* Bank;long Index;CGraphicInfoFrame* Frames;};
#ifndef _MSC_VER
#define __forceinline inline
#endif
namespace std{struct __false_type{};struct random_access_iterator_tag{};template<class Iter,class Out,class Distance>__forceinline Out __fastcall __copy_backward(Iter first,Iter last,Out result,const random_access_iterator_tag&,Distance*){for(Distance n=last-first;n>0;--n)*--result=*--last;return result;}template<class T>T* __fastcall __copy_backward_ptrs(T* first,T* last,T* result,const __false_type&){return __copy_backward(first,last,result,random_access_iterator_tag(),(int*)0);}}
typedef CGraphicInfo Element;
template Element* __fastcall std::__copy_backward_ptrs<Element>(Element*,Element*,Element*,const std::__false_type&);