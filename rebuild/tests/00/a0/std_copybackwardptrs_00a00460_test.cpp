#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#endif
class CPixelFormat{public:long FormatIndex;};enum ETextureType{TEX_NONE};class CGraphicHeader{public:unsigned short Width,Height,Depth,FrameWidth,FrameHeight,FrameCount;CPixelFormat PixelFormat;unsigned char TransparencyType,MipmapLevels;unsigned char IsFlatAnimatedGraphic:1;unsigned long FrameDataSize,CompressedMipmapSizes[1];};class CGraphicDataBank;class CGraphicInfoFrame;class CGraphicInfo{public:CGraphicHeader Header;ETextureType TextureType;CGraphicDataBank* Bank;long Index;CGraphicInfoFrame* Frames;};
#ifndef _MSC_VER
#define __forceinline inline
#endif
namespace std{struct __false_type{};struct random_access_iterator_tag{};template<class Iter,class Out,class Distance>__forceinline Out __fastcall __copy_backward(Iter first,Iter last,Out result,const random_access_iterator_tag&,Distance*){for(Distance n=last-first;n>0;--n)*--result=*--last;return result;}template<class T>T* __fastcall __copy_backward_ptrs(T* first,T* last,T* result,const __false_type&){return __copy_backward(first,last,result,random_access_iterator_tag(),(int*)0);}}
typedef CGraphicInfo Element;
template Element* __fastcall std::__copy_backward_ptrs<Element>(Element*,Element*,Element*,const std::__false_type&);
int main(){Element data[4],saved[3];memset(data,0,sizeof(data));for(int i=0;i<4;++i){Element& x=data[i];x.Header.Width=i+1;x.Header.PixelFormat.FormatIndex=i+3;x.Header.IsFlatAnimatedGraphic=i%2!=0;x.Index=i+7;}memcpy(saved,data,sizeof(saved));if(std::__copy_backward_ptrs(data,data+3,data+4,std::__false_type())!=data+1 || memcmp(data+1,saved,sizeof(saved)))goto fail;if(std::__copy_backward_ptrs(data,data,data+4,std::__false_type())!=data+4)goto fail;puts("00a00460_TEST PASS");return 0;fail:puts("00a00460_TEST FAIL");return 1;}