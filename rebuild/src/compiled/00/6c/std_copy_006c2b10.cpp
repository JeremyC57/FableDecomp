// std::__copy(Input first, Input last, Output result, const random_access_iterator_tag& tag, int* distance) -- assign the input range forwards.
// Leaf: ??$__copy@PAVCLevelFiles@CObject@CLevelLoader@@PAV123@H@std@@YIPAVCLevelFiles@CObject@CLevelLoader@@PAV123@00ABUrandom_access_iterator_tag@0@PAH@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Identity: probable; CLevelLoader::CObject::CLevelFiles::operator=0x6C2940; level-file-vector mutable erase caller0x6C2C00 and assignment0x6C2D40; exact28-byte three-counted-file layout.
class CAFile; class CCPPointerInfo;
#include "engine/CCountedPointer.h"
class CLevelLoader { public: class CObject { public: class CLevelFiles { public: long LevelID; CCountedPointer<CAFile> PTopologyFile,PNavigationFile,PThingFile; CLevelFiles& operator=(const CLevelFiles&); }; }; };
namespace std {
struct input_iterator_tag {}; struct forward_iterator_tag : input_iterator_tag {}; struct bidirectional_iterator_tag : forward_iterator_tag {}; struct random_access_iterator_tag : bidirectional_iterator_tag {};
template<class Input,class Output,class Distance> Output __fastcall __copy(Input first,Input last,Output result,const random_access_iterator_tag&,Distance*) {
 for(Distance n=last-first;n>0;--n) { *result=*first; ++first; ++result; } return result;
}
}
template CLevelLoader::CObject::CLevelFiles* __fastcall std::__copy<CLevelLoader::CObject::CLevelFiles*,CLevelLoader::CObject::CLevelFiles*,int>(CLevelLoader::CObject::CLevelFiles*,CLevelLoader::CObject::CLevelFiles*,CLevelLoader::CObject::CLevelFiles*,const std::random_access_iterator_tag&,int*);