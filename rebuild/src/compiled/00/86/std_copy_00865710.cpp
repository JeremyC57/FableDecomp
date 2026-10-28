// std::__copy(iterator first, iterator last, iterator result, const random_access_iterator_tag&, int*) -- assign the range forward and return the output end.
// Leaf: ??$__copy@PAVCFlash@NCreatureHitNotification@@PAV12@H@std@@YIPAVCFlash@NCreatureHitNotification@@PAV12@00ABUrandom_access_iterator_tag@0@PAH@Z
// Identity: proven; own neighboring hit-notification Flash vector helpers and CFlash copyctor00864F80; first-member list<CWeaponTrailEntry>::operator=0076FF90, CreationFrame+4; PDB CFlash namespace and fields.
// Layout: no matching nested flash header; shared CCharString data header lacks assignment/destructor APIs, so actual four-byte PStringData view declares real methods. Flash uses actual namespace, node-list4+CreationFrame4; unknown sound value remains explicitly opaque.
class CEnginePrimitiveWeaponTrail{public:class CWeaponTrailEntry;};namespace std{template<class T>class allocator{};template<class T,class Al=allocator<T> >class list{public:void* _M_node;list& operator=(const list&);};}namespace NCreatureHitNotification{class CFlash{public:std::list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry> FlashGeometry;long CreationFrame;};}
#ifndef _MSC_VER
#define __fastcall
#endif
namespace std{struct random_access_iterator_tag{};template<class Iter,class Out,class Distance>Out __fastcall __copy(Iter first,Iter last,Out result,const random_access_iterator_tag&,Distance*){for(Distance n=last-first;n>0;--n){*result=*first;++first;++result;}return result;}}
typedef NCreatureHitNotification::CFlash Element;
template Element* __fastcall std::__copy<Element*,Element*,int>(Element*,Element*,Element*,const std::random_access_iterator_tag&,int*);