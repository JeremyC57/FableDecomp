// std::__copy(iterator first, iterator last, iterator result, const random_access_iterator_tag&, int*) -- assign the range forward and return the output end.
// Leaf: ??$__copy@PAVCUnprovenSoundNameEntry_00A3FCD0@@PAV1@H@std@@YIPAVCUnprovenSoundNameEntry_00A3FCD0@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Identity: descriptive; own erase00A40CB0/clear00A41360 are unreferenced; first-word CCharString assignment0099EFB0 proven, value word type/owner unproved (hand952H009). Descriptive record exposes opaque second word without claiming CSoundBankLH pointer.
// Layout: no matching nested flash header; shared CCharString data header lacks assignment/destructor APIs, so actual four-byte PStringData view declares real methods. Flash uses actual namespace, node-list4+CreationFrame4; unknown sound value remains explicitly opaque.
class CCharStringData;class CCharString{public:CCharStringData* PStringData;CCharString():PStringData(0){}~CCharString();CCharString& operator=(const CCharString&);};namespace std{template<class A,class B>struct pair{A first;B second;};}
class CUnprovenSoundNameEntry_00A3FCD0{public:CCharString Name;unsigned long UnprovenValue;};
#ifndef _MSC_VER
#define __fastcall
#endif
namespace std{struct random_access_iterator_tag{};template<class Iter,class Out,class Distance>Out __fastcall __copy(Iter first,Iter last,Out result,const random_access_iterator_tag&,Distance*){for(Distance n=last-first;n>0;--n){*result=*first;++first;++result;}return result;}}
typedef CUnprovenSoundNameEntry_00A3FCD0 Element;
template Element* __fastcall std::__copy<Element*,Element*,int>(Element*,Element*,Element*,const std::random_access_iterator_tag&,int*);