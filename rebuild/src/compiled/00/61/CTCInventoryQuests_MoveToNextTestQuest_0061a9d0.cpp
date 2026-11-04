// CTCInventoryQuests::MoveToNextTestQuest() -- in the test-quest list mode, selects the next displayed test quest (wrapping to the first and scrolling the window of 12 rows along)
// Leaf: ?MoveToNextTestQuest@CTCInventoryQuests@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0061A9D0, 168 bytes (+ int3 padding), plain `ret`. Ego_r.pdb ?MoveToNextTestQuest@CTCInventoryQuests@@QAEXXZ.
// Callees: CTCInventoryQuests::GetTestQuestsToDisplay(vector<CTestQuest>&) 0x0061A8A0, std::__destroy_aux<CTestQuest*> 0x004ABD90
//   (__fastcall, the vector's destructor), free 0x00BFEA14 (~_Vector_base).
// Shape: the TU of the landed twin 0x0061AA80 MoveToPreviousTestQuest (copied verbatim: its STLport vector<CTestQuest> model and
//   the same-TU noinline bodies of GetTestQuestsToDisplay and __destroy_aux, EXTRA_RULES chunk 422). With those bodies visible
//   VC7.1 keeps `this` in esi, the start pointer in ebp (shared by size() and the destructor) and the index in edi, as retail;
//   with declaration-only callees it rotated this/index/start through edi/ebp/esi (chunks 209, 628). The wrap test is unsigned
//   (size() is unsigned).
extern "C" void __cdecl free(void*);extern "C" void*__cdecl malloc(unsigned int);
inline void*__cdecl operator new(unsigned int,void*p){return p;}
// Layout: original CTestQuest has six CCharString handles and ColourIndex at +8, native size28.
class CCharStringData;class CCharString{protected:CCharStringData*PStringData;public:~CCharString();};
class CTestQuest{public:CTestQuest(const CTestQuest&);CCharString QuestName,HSPName;long ColourIndex;CCharString DisplayName,IniFile,OutiFile,QuestCardDefName;};
namespace std{
struct __false_type{};template<class A,class B,class R>struct binary_function{};
template<class T>class allocator{public:allocator()throw(){}allocator(const allocator&)throw(){}template<class U>allocator(const allocator<U>&)throw(){}~allocator()throw(){}T*allocate(unsigned int n,const void* =0){return static_cast<T*>(malloc(n*sizeof(T)));}};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;_STLP_alloc_proxy(const A&a,V d):A(a),_M_data(d){}};
template<class I>__declspec(noinline)void __fastcall __destroy_aux(I first,I last,const __false_type&){for(;first!=last;++first)first->~CTestQuest();}
template<class T,class A>class _Vector_base{protected:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;public:_Vector_base(const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){}~_Vector_base(){if(_M_start)free(_M_start);}};
template<class In,class Out>Out copy(In,In,Out);template<class In,class Out>Out uninitialized_copy(In,In,Out);
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:typedef unsigned int size_type;typedef T*iterator;typedef const T*const_iterator;vector(const A&a=A()):_Vector_base<T,A>(a){}~vector(){__destroy_aux(this->_M_start,this->_M_finish,__false_type());}
size_type size()const{return this->_M_finish-this->_M_start;}size_type capacity()const{return this->_M_end_of_storage._M_data-this->_M_start;}
iterator begin(){return this->_M_start;}iterator end(){return this->_M_finish;}const_iterator begin()const{return this->_M_start;}const_iterator end()const{return this->_M_finish;}T&operator[](size_type i){return this->_M_start[i];}
protected:template<class It>T*_M_allocate_and_copy(size_type n,It first,It last){T*p=this->_M_end_of_storage.allocate(n);uninitialized_copy(first,last,p);return p;}
public:__declspec(noinline)vector&operator=(const vector&x){if(&x!=this){size_type n=x.size();if(n>capacity()){T*p=_M_allocate_and_copy(n,x.begin(),x.end());__destroy_aux(this->_M_start,this->_M_finish,__false_type());if(this->_M_start)free(this->_M_start);this->_M_start=p;this->_M_end_of_storage._M_data=p+n;}else if(size()>=n){T*p=copy(x.begin(),x.end(),begin());__destroy_aux(p,this->_M_finish,__false_type());}else{copy(x.begin(),x.begin()+size(),begin());uninitialized_copy(x.begin()+size(),x.end(),this->_M_finish);}this->_M_finish=this->_M_start+n;}return *this;}
protected:__declspec(noinline)void _M_insert_overflow(T*pos,const T&x,const __false_type&,size_type n,bool atEnd){size_type old=size(),cap=old+(old>n?old:n);T*start=this->_M_end_of_storage.allocate(cap);T*finish=uninitialized_copy(this->_M_start,pos,start);for(size_type i=0;i<n;++i){new(finish)T(x);++finish;}if(!atEnd)finish=uninitialized_copy(pos,this->_M_finish,finish);__destroy_aux(this->_M_start,this->_M_finish,__false_type());if(this->_M_start)free(this->_M_start);this->_M_start=start;this->_M_finish=finish;this->_M_end_of_storage._M_data=start+cap;}
public:__forceinline void push_back(const T&x){if(this->_M_finish!=this->_M_end_of_storage._M_data){new(this->_M_finish)T(x);++this->_M_finish;}else _M_insert_overflow(this->_M_finish,x,__false_type(),1,true);}
};
template<class It,class Compare>void __fastcall sort(It,It,Compare);

}
#define CTCBase CTCBaseFields
#include "engine/CTCBase.h"
#undef CTCBase
#define CTCInventoryBase CTCInventoryBaseFields
#include "engine/_quarantine/CTCInventoryBase.h"
#undef CTCInventoryBase
#define CWorld CWorldFields
#include "engine/CWorld.h"
#undef CWorld
#define CQuestManager CQuestManagerFields
#include "engine/CQuestManager.h"
#undef CQuestManager
#include "engine/CCountedPointer.h"
class CWorld:public CWorldFields{};
#include "engine/CBaseClassNonCopyable.h"
class CTCBase:public CBaseClassNonCopyable{protected:CThing&ParentThing;private:bool ToBeRemoved,IsInGlobalUpdateTCs;protected:const CWorld&PeekWorld()const;};
class CWideString;enum EPlayerMode{};
namespace NHeroInformationScreens{enum EScreenType{};class CBase{public:~CBase();void Copy(const CBase&);virtual CWideString GetScreenName()const=0;virtual EPlayerMode GetPlayerMode()const=0;virtual EScreenType GetScreenType()const=0;virtual bool IsInventoryScreen()const;};}
namespace NInventory{class CTCInventoryBase:public CTCBase{unsigned char unreadInventory[0x130-0xc];};}
class CQuestManager:public CQuestManagerFields{public:bool IsQuestActive(const CCharString&)const;};
template<class T>class CTBaseSingletonNoDefaultAlloc{private:static CCountedPointer<T>PClass;public:static T&__fastcall Get(){return *PClass;}};
class CTestQuestNameCompare:public std::binary_function<CTestQuest,CTestQuest,bool>{public:bool operator()(const CTestQuest&,const CTestQuest&)const;};
// Layout: supplied native inventory base130; original derived ActiveTestQuestMode157, selected158, top15c and show-all160.
class CTCInventoryQuests:public NInventory::CTCInventoryBase,public NHeroInformationScreens::CBase{unsigned char unreadQuestMode[0x157-0x134];bool ActiveTestQuestMode;long TestQuestSelectedIndex,TestQuestTopIndex;bool TestQuestShowAll;public:__declspec(noinline)void GetTestQuestsToDisplay(std::vector<CTestQuest>&);void MoveToNextTestQuest();};
__declspec(noinline)void CTCInventoryQuests::GetTestQuestsToDisplay(std::vector<CTestQuest>&quests){quests=reinterpret_cast<const std::vector<CTestQuest>&>(PeekWorld().ActiveTestQuests);std::sort(quests.begin(),quests.end(),CTestQuestNameCompare());if(TestQuestShowAll)return;std::vector<CTestQuest>active;for(unsigned long i=0;i<quests.size();++i)if(CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().IsQuestActive(quests[i].QuestName))active.push_back(quests[i]);quests=active;}
void CTCInventoryQuests::MoveToNextTestQuest()
{
    if (ActiveTestQuestMode) {
        std::vector<CTestQuest> quests;
        GetTestQuestsToDisplay(quests);
        if (TestQuestSelectedIndex >= quests.size() - 1) {
            TestQuestSelectedIndex = 0;
            TestQuestTopIndex = 0;
        } else {
            ++TestQuestSelectedIndex;
            if (TestQuestSelectedIndex - TestQuestTopIndex >= 12)
                ++TestQuestTopIndex;
        }
    }
}