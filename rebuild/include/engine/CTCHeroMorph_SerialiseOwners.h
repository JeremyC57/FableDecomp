#ifndef FABLE_ENGINE_CTCHEROMORPH_SERIALISEOWNERS_H
#define FABLE_ENGINE_CTCHEROMORPH_SERIALISEOWNERS_H
#ifdef FABLE_RETAIL_HERO_MORPH_SERIALISE_PASS29
// Native inventory template owners at 005B1479, 005B149F, 005B26E9,
// 005B31AC, and 005B31EC use size optimization. The allocation helper
// at 0071EA40 instead expands the same copy loop under speed optimization.
// Plain inline for the copy permits both native expansions.
// Inline-budget: native range destruction, fill, overflow and allocation call their separately audited owners.
#ifdef _MSC_VER
#define FABLE_HERO_MORPH_NOINLINE __declspec(noinline)
#else
#define FABLE_HERO_MORPH_NOINLINE
#endif
typedef CDefPointer<const CThingBaseDef> TattooPtr;
#ifdef _MSC_VER
#pragma optimize("s",on)
#endif
namespace std {
template<> inline void FABLE_FASTCALL _Destroy(TattooPtr* p) { p->~CDefPointer(); }
template<> inline void vector<TattooPtr>::_M_clear() { _Destroy(this->_M_start,this->_M_finish); this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start); }
template<> inline vector<TattooPtr>::~vector() { _Destroy(this->_M_start,this->_M_finish); }
template<> FABLE_HERO_MORPH_NOINLINE inline void FABLE_FASTCALL __destroy_aux(TattooPtr* first,TattooPtr* last,const __false_type&) { for(;first!=last;++first) _Destroy(&*first); }
template<> inline TattooPtr* FABLE_FASTCALL __uninitialized_copy(TattooPtr* first,TattooPtr* last,TattooPtr* result,const __false_type&) { TattooPtr* cur=result;for(;first!=last;++first,++cur) _Construct(&*cur,*first);return cur;}
template<> FABLE_HERO_MORPH_NOINLINE inline TattooPtr* FABLE_FASTCALL __uninitialized_fill_n(TattooPtr* first,unsigned n,const TattooPtr& x,const __false_type&) { TattooPtr* cur=first;for(;n>0;--n,++cur) _Construct(&*cur,x);return cur;}
}
#ifdef _MSC_VER
#pragma optimize("",on)
#endif
#ifdef _MSC_VER
#pragma optimize("s",on)
#pragma optimize("y",off)
#endif
namespace std {
// Native 005B3275 saves EBP and calls range copy, range fill and _M_clear.
template<> FABLE_HERO_MORPH_NOINLINE inline void vector<TattooPtr>::_M_insert_overflow(iterator position,const TattooPtr& x,const __false_type&,size_type fillLength,bool atEnd) {
    const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
    TattooPtr* newStart=this->_M_end_of_storage.allocate(len);TattooPtr* newFinish=newStart;
    newFinish=__uninitialized_copy(this->_M_start,position,newStart,__false_type());
    if(fillLength==1) {_Construct(newFinish,x);++newFinish;}
    else newFinish=__uninitialized_fill_n(newFinish,fillLength,x,__false_type());
    if(!atEnd) newFinish=__uninitialized_copy(position,this->_M_finish,newFinish,__false_type());
    _M_clear();_M_set(newStart,newFinish,newStart+len);
}
template<> inline void vector<TattooPtr>::push_back(const TattooPtr& x) { if(this->_M_finish!=this->_M_end_of_storage._M_data){_Construct(this->_M_finish,x);++this->_M_finish;}else _M_insert_overflow(this->_M_finish,x,_IsPODType(),1,true); }
}
#ifdef _MSC_VER
#pragma optimize("",on)
#endif
namespace std {
template<> template<> FABLE_HERO_MORPH_NOINLINE inline TattooPtr* vector<TattooPtr>::_M_allocate_and_copy(unsigned int n,TattooPtr* first,TattooPtr* last){ TattooPtr* result=this->_M_end_of_storage.allocate(n);__uninitialized_copy(first,last,result,_IsPODType());return result;}
}
#undef FABLE_HERO_MORPH_NOINLINE
#endif
#endif
