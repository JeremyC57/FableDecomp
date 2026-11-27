#ifndef FABLE_ENGINE_NUISYSTEM_CFRONTENDLIST_H
#define FABLE_ENGINE_NUISYSTEM_CFRONTENDLIST_H
// NUISystem::CFrontEndList (new, chunk 753): RTTI .?AVCFrontEndList@NUISystem@@, single direct base NUISystem::CList (the landed
// ??_G 0x0054DA70 calls ~CList 0x0053B7F0; the ctor 0x0054C3A0 calls CList(long) 0x0053B63E). Vtables 0x01249224 (+0, 190 slots),
// 0x012491FC (+4: slot 1 ProcessEvent 0x0054C420), 0x012491F4 (+0x18). Overrides read from the primary vtable: 0 ??_G 0x0054DA70,
// 1 Update 0x0054C430, 43 Initialise 0x0054D660, 48 ChangeState(unsigned long) 0x0054CBF0, 152 AddScrollingChildAndSetOffsets
// 0x0054D170, 156 InitialiseOffsets 0x0054C480; new protected virtuals 188 ScrollUp() 0x0054C4C0, 189 ScrollDown() 0x0054C810.
// Name set: Ego_r.pdb / FableWin.pdb (??0..QAE@J@Z, ??0..QAE@ABV01@@Z, ??1..UAE@XZ, ??_G, AddScrollingChildAndSetOffsets,
// ChangeState UAEXK@Z, Initialise, InitialiseOffsets UAEXK@Z, ProcessEvent, ScrollDown MAEXXZ, ScrollUp MAEXXZ, Update UAEXM@Z).
// No own members: CManager::CreateComponent 0x0041D21B news 0x1FC bytes, the retail sizeof CList (FABLE_RETAIL_CLIST).
// The destructor is empty and inline: the ??_G stores the three vptrs and calls ~CList.
#include "engine/NUISystem_CList.h"
namespace NUISystem {
class CFrontEndList : public CList {
public:
    CFrontEndList(long);
    CFrontEndList(const CFrontEndList&);
    virtual ~CFrontEndList() { }
    virtual void Initialise();
    virtual void Update(float);
    virtual void ChangeState(unsigned long);
    virtual void ProcessEvent(EEvent);
    virtual void AddScrollingChildAndSetOffsets(CCountedPointer<CComponent>&);
    virtual void InitialiseOffsets(unsigned long);
protected:
    virtual void ScrollUp();                                                // slot 188
    virtual void ScrollDown();                                              // slot 189
};
}
#ifdef FABLE_RETAIL_CLIST
FABLE_STATIC_ASSERT(sizeof(NUISystem::CFrontEndList) == 0x1fc);
#endif

#ifdef FABLE_RETAIL_FRONT_END_LIST_INITIALISE_PASS29
// CFrontEndList::Initialise uses the original size-compiled CCountedPointer and
// STLport owners from the UI TUs. Their bodies remain visible to the speed-built
// Initialise. Ordinary inline allocation/overflow emitted 1231 bytes in that
// leaf or the incorrect 174-byte overflow; retail calls both owners out of line.
#include "engine/NUISystem_CListArrow.h"
#ifdef _MSC_VER
#define FABLE_LIST_INITIALISE_OWNER_NOINLINE __declspec(noinline)
#else
#define FABLE_LIST_INITIALISE_OWNER_NOINLINE
#endif
#if defined(_MSC_VER)
#pragma optimize("s",on)
#endif
template<> inline CCountedPointer<NUISystem::CComponent>::CCountedPointer(NUISystem::CComponent* data):Data(data) {
    if(Data) Info=new CCPPointerInfo(1,DeleteData,Data); else Info=0;
}
template<> inline void CCountedPointer<NUISystem::CComponent>::ReleaseData() {
    if(Info && --Info->RefCount == 0){ Info->DeleteFunc(Info->Data); delete Info; }
    Data=0; Info=0;
}
template<> inline CCountedPointer<NUISystem::CComponent>::~CCountedPointer() { ReleaseData(); }
template<> inline void CCountedPointer<NUISystem::CListArrow>::ReleaseData() {
    if(Info && --Info->RefCount==0){Info->DeleteFunc(Info->Data); delete Info;}
    Data=0; Info=0;
}
template<> inline CCountedPointer<NUISystem::CListArrow>::~CCountedPointer() { ReleaseData(); }
template<> inline void CCountedPointer<NUISystem::CListArrow>::ShareData(NUISystem::CListArrow* data,CCPPointerInfo* info) {
    if(Info!=info) { ReleaseData(); Data=data; Info=info; if(info)++info->RefCount; }
}
template<> template<> inline void CCountedPointer<NUISystem::CListArrow>::SetStaticCast<NUISystem::CComponent>(const CCountedPointer<NUISystem::CComponent>& other) { ShareData(static_cast<NUISystem::CListArrow*>(other.Data),other.Info); }
#if defined(_MSC_VER)
#pragma optimize("",on)
#endif

namespace std {
#if defined(_MSC_VER)
#pragma optimize("s",on)
#endif
template<> inline FABLE_LIST_INITIALISE_OWNER_NOINLINE map<unsigned long, NUISystem::CUIState>* allocator<map<unsigned long, NUISystem::CUIState> >::allocate(size_t n, const void*) {
    return n ? static_cast<map<unsigned long, NUISystem::CUIState>*>(malloc(n * sizeof(map<unsigned long, NUISystem::CUIState>))) : 0;
}
template<> inline  map<unsigned long, NUISystem::CUIState>* FABLE_FASTCALL __uninitialized_copy(map<unsigned long, NUISystem::CUIState>* first,map<unsigned long, NUISystem::CUIState>* last,map<unsigned long, NUISystem::CUIState>* dest,const __false_type&) {
    map<unsigned long, NUISystem::CUIState>* current=dest;
    for(;first!=last;++first,++current) _Construct(current,*first);
    return current;
}
template<> inline  map<unsigned long, NUISystem::CUIState>* FABLE_FASTCALL __uninitialized_fill_n(map<unsigned long, NUISystem::CUIState>* first,unsigned int count,const map<unsigned long, NUISystem::CUIState>& value,const __false_type&) {
    for(;count>0;--count,++first) _Construct(first,value);
    return first;
}
template<> inline  void vector<map<unsigned long, NUISystem::CUIState> >::_M_clear() {
    _Destroy(this->_M_start,this->_M_finish);
    this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);
}
#if defined(_MSC_VER)
#pragma optimize("y",off)
#endif
template<> inline FABLE_LIST_INITIALISE_OWNER_NOINLINE void vector<map<unsigned long, NUISystem::CUIState> >::_M_insert_overflow(iterator position, const map<unsigned long, NUISystem::CUIState>& value, const __false_type&,size_type count,bool at_end) {
    const size_type old_size=size();
    const size_type len=old_size+(max)(old_size,count);
    iterator new_start=this->_M_end_of_storage.allocate(len);
    iterator new_finish=__uninitialized_copy(this->_M_start,position,new_start,__false_type());
    if(count==1){ _Construct(new_finish,value);++new_finish; }
    else new_finish=__uninitialized_fill_n(new_finish,count,value,__false_type());
    if(!at_end)new_finish=__uninitialized_copy(position,this->_M_finish,new_finish,__false_type());
    _M_clear();
    _M_set(new_start,new_finish,new_start+len);
}
template<> inline void vector<map<unsigned long, NUISystem::CUIState> >::push_back(const map<unsigned long, NUISystem::CUIState>& x) {
    if(this->_M_finish != this->_M_end_of_storage._M_data){
        _Construct(this->_M_finish,x); ++this->_M_finish;
    }else _M_insert_overflow(this->_M_finish,x,__false_type(),1,true);
}
#if defined(_MSC_VER)
#pragma optimize("",on)
#endif
}

#undef FABLE_LIST_INITIALISE_OWNER_NOINLINE
#endif
#endif
