// 0064a3ed NPlayerGui::CDrawQuestInfo::AddElement
// Codex parked-50 best attempt: DIFFER(99v99); residue: 99/99 bytes; corrected element/reference model still has a three-way loop register-allocation mismatch.
// Flags:
// Leaf: ?AddElement@CDrawQuestInfo@NPlayerGui@@AAEXV?$CCountedPointer@VCQuestInfoElement@CDrawQuestInfo@NPlayerGui@@@@@Z
#pragma optimize("s",on)
#include "engine/CCountedPointer.h"
#define CQuestManager CQuestManagerLayout
#include "engine/CQuestManager.h"
#undef CQuestManager
namespace std {
template<class T>inline const T&__fastcall max(const T&a,const T&b){return a<b?b:a;}
template<class T>class allocator{};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
template<class T,class A>class _Vector_base{protected:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{};
struct _List_node_base{_List_node_base*_M_next,*_M_prev;};
template<class T>struct _List_node:public _List_node_base{T _M_data;};
template<class T>struct _Nonconst_traits{typedef T*pointer;typedef T&reference;};
struct _List_iterator_base{
 _List_node_base*_M_node;
 _List_iterator_base(_List_node_base*n):_M_node(n){}
 void _M_incr(){_M_node=_M_node->_M_next;}
 bool operator!=(const _List_iterator_base&b)const{return _M_node!=b._M_node;}
};
template<class T,class Traits>struct _List_iterator:public _List_iterator_base{
 typedef _List_iterator<T,_Nonconst_traits<T> >iterator;
 _List_iterator(_List_node<T>*n):_List_iterator_base(n){}
 _List_iterator(const iterator&i):_List_iterator_base(i._M_node){}
 typename Traits::reference operator*()const{return ((_List_node<T>*)_M_node)->_M_data;}
 _List_iterator&operator++(){this->_M_incr();return *this;}
};
template<class T,class A>class _List_base{protected:_STLP_alloc_proxy<_List_node<T>*,_List_node<T>,allocator<_List_node<T> > >_M_node;};
template<class T,class A=allocator<T> >class list:public _List_base<T,A>{
public:typedef _List_iterator<T,_Nonconst_traits<T> >iterator;
 iterator begin(){return (_List_node<T>*)this->_M_node._M_data->_M_next;}
 iterator end(){return this->_M_node._M_data;}
 void push_back(const T&);
};
}
template<class T>class CTBaseSingletonNoDefaultAlloc{
 static CCountedPointer<T>PClass;
public:virtual~CTBaseSingletonNoDefaultAlloc();static T&__fastcall Get(){return *PClass;}
};
// Layout: original singleton4, PQuestCurrentlyRunning native88; shared header corroborates containers.
class CQuestManager:public CTBaseSingletonNoDefaultAlloc<CQuestManager>{
public:class CActiveQuest{long ID;public:long GetID()const{return ID;}};
private:unsigned char unreadManager[0x88-4];CActiveQuest*PQuestCurrentlyRunning;
public:CActiveQuest*GetPActiveQuest()const{return PQuestCurrentlyRunning;}
};
class CPlayerGui;class CPlayerManager;class CWorld;class CMainGameComponent;class CGraphicDataBank;
class CEnginePrimitiveHandle;class CEnginePrimitivePrepareInfo;class C2DVector;
namespace NPlayerGui {
// Layout: raw PDB CDrawBase vector shrinks16 to12, native base40.
class CDrawBase{
public:virtual void ClearPrimitivesIfNecessary();virtual void ClearPrimitives();
protected:CPlayerGui&PlayerGui;CPlayerManager&PlayerManager;CWorld&World;
 const CMainGameComponent&Component;const CGraphicDataBank&Bank;bool Drawing;
 std::vector<CEnginePrimitiveHandle>PrimitiveHandles;
};
// Layout: raw PDB nested element ID4, ScriptID8, Scale12, vector16.
class CDrawQuestInfo:public CDrawBase{
public:enum EElementType{};
 class CQuestInfoElement{
 public:virtual float DrawAt(const C2DVector&,const CEnginePrimitivePrepareInfo&)=0;
  virtual bool Update();virtual void Clear();virtual EElementType GetType()const=0;
  long ID,ScriptID;float Scale;std::vector<CEnginePrimitiveHandle>PrimitiveHandles;
 };
private:unsigned char unreadQuest[0x58-0x28];std::list<CCountedPointer<CQuestInfoElement> >Elements;
 void AddElement(CCountedPointer<CQuestInfoElement>);
};
void CDrawQuestInfo::AddElement(CCountedPointer<CQuestInfoElement>element){
 CQuestManager::CActiveQuest*quest=CQuestManager::Get().GetPActiveQuest();
 CQuestInfoElement&object=*element;
 if(quest)object.ScriptID=quest->GetID();
 long maxID=0;
 for(std::list<CCountedPointer<CQuestInfoElement> >::iterator it=Elements.begin();it!=Elements.end();++it){
  const long&id=(*it)->ID;if(id>maxID)maxID=id;
 }
 object.ID=maxID+1;object.Scale*=0.7f;
 Elements.push_back(element);
}
}
