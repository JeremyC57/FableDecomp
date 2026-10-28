// std::__copy(iterator first, iterator last, iterator result, const random_access_iterator_tag&, int*) -- assign the range forward and return the output end.
// Leaf: ??$__copy@PAU?$pair@VCCharString@@K@std@@PAU12@H@std@@YIPAU?$pair@VCCharString@@K@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z
// Identity: proven; own erase00AE6A20, resize00AE7D10, CParticleProgram clear00AE3381 on ExportNames+0x88; hand952H009 and actual CCharString assignment0099EFB0.
// Layout: no matching nested flash header; shared CCharString data header lacks assignment/destructor APIs, so actual four-byte PStringData view declares real methods. Flash uses actual namespace, node-list4+CreationFrame4; unknown sound value remains explicitly opaque.
class CCharStringData;class CCharString{public:CCharStringData* PStringData;CCharString():PStringData(0){}~CCharString();CCharString& operator=(const CCharString&);};namespace std{template<class A,class B>struct pair{A first;B second;};}
#ifndef _MSC_VER
#define __fastcall
#endif
namespace std{struct random_access_iterator_tag{};template<class Iter,class Out,class Distance>Out __fastcall __copy(Iter first,Iter last,Out result,const random_access_iterator_tag&,Distance*){for(Distance n=last-first;n>0;--n){*result=*first;++first;++result;}return result;}}
typedef std::pair<CCharString,unsigned long> Element;
template Element* __fastcall std::__copy<Element*,Element*,int>(Element*,Element*,Element*,const std::random_access_iterator_tag&,int*);