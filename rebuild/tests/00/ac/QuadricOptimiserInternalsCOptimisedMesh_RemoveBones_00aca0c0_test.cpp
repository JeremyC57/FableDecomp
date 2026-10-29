#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#define __stdcall
#define __declspec(x)
#define __forceinline inline
#define __int64 __attribute__((__mode__(__DI__))) long
#endif
#define PASS9_TEST
#include <new>
int main();
extern "C" void*__cdecl TestMalloc(unsigned int);extern "C" void __cdecl TestFree(void*);
#define malloc TestMalloc
#define free TestFree
// 00aca0c0 QuadricOptimiserInternals::COptimisedMesh::RemoveBones
// Leaf: ?RemoveBones@COptimisedMesh@QuadricOptimiserInternals@@UAEXABV?$set@VCCharString@@U?$less@VCCharString@@@std@@V?$allocator@VCCharString@@@3@@std@@@Z
// Flags:
#include <stdlib.h>
#include "rebuild_abi.h"
class CCharStringData;class CCharString{protected:CCharStringData*PStringData;public:CCharString();CCharString(const CCharString&);~CCharString();CCharString GetAsLowerCase()const;bool operator<(const CCharString&)const;
#ifdef PASS9_TEST
friend int ::main();
#endif
};

namespace std {
template<class T>class allocator{public:allocator(){}allocator(const allocator&){}T*allocate(unsigned int count){return count?(T*)malloc(count*sizeof(T)):0;}void deallocate(T*p,unsigned int){if(p)free(p);}};
template<class P,class T,class A>class _STLP_alloc_proxy:public A{public:P _M_data;_STLP_alloc_proxy(const A&a,P p):A(a),_M_data(p){}};
template<class T>struct less{bool operator()(const T&a,const T&b)const{return a<b;}};
template<class T>struct _Identity{const T&operator()(const T&value)const{return value;}};
template<class T1,class T2>struct pair{typedef T1 first_type;typedef T2 second_type;T1 first;T2 second;pair(const T1&a,const T2&b):first(a),second(b){}};
struct _Rb_tree_node_base{bool _M_color;_Rb_tree_node_base*_M_parent,*_M_left,*_M_right;};
template<class V>struct _Rb_tree_node:public _Rb_tree_node_base{V _M_value_field;explicit _Rb_tree_node(const V&value):_M_value_field(value){}};
template<class D>class _Rb_global{public:static _Rb_tree_node_base*__cdecl _M_increment(_Rb_tree_node_base*node){if(node->_M_right){node=node->_M_right;while(node->_M_left)node=node->_M_left;}else{_Rb_tree_node_base*parent=node->_M_parent;while(node==parent->_M_right){node=parent;parent=parent->_M_parent;}if(node->_M_right!=parent)node=parent;}return node;}};
typedef _Rb_global<bool>_Rb_global_inst;
struct _Rb_tree_base_iterator{_Rb_tree_node_base*_M_node;bool operator==(const _Rb_tree_base_iterator&other)const{return _M_node==other._M_node;}bool operator!=(const _Rb_tree_base_iterator&other)const{return _M_node!=other._M_node;}};
template<class T>struct _Nonconst_traits{typedef T&reference;typedef T*pointer;};template<class T>struct _Const_traits{typedef const T&reference;typedef const T*pointer;};
template<class V,class Traits>struct _Rb_tree_iterator:public _Rb_tree_base_iterator{_Rb_tree_iterator(){_M_node=0;}_Rb_tree_iterator(_Rb_tree_node<V>*node){_M_node=node;}_Rb_tree_iterator(const _Rb_tree_iterator<V,_Nonconst_traits<V> >&other){_M_node=other._M_node;}typename Traits::pointer operator->()const{return &operator*();}typename Traits::reference operator*()const{return static_cast<_Rb_tree_node<V>*>(_M_node)->_M_value_field;}_Rb_tree_iterator&operator++(){_M_node=_Rb_global_inst::_M_increment(_M_node);return*this;}};
template<class V,class A>struct _Rb_tree_base{protected:_STLP_alloc_proxy<_Rb_tree_node<V>*,_Rb_tree_node<V>,allocator<_Rb_tree_node<V> > >_M_header;public:_Rb_tree_base(const A&a):_M_header(allocator<_Rb_tree_node<V> >(),0){(void)a;_M_header._M_data=_M_header.allocate(1);}~_Rb_tree_base(){_M_header.deallocate(_M_header._M_data,1);}};
template<class K,class V,class Extract,class Compare,class A>class _Rb_tree:public _Rb_tree_base<V,A>{public:typedef _Rb_tree_node<V>*_Link_type;typedef _Rb_tree_iterator<V,_Nonconst_traits<V> >iterator;typedef _Rb_tree_iterator<V,_Const_traits<V> >const_iterator;
protected:unsigned int _M_node_count;Compare _M_key_compare;_Link_type&_M_root()const{return (_Link_type&)this->_M_header._M_data->_M_parent;}_Link_type&_M_leftmost()const{return (_Link_type&)this->_M_header._M_data->_M_left;}_Link_type&_M_rightmost()const{return (_Link_type&)this->_M_header._M_data->_M_right;}
private:void _M_erase(_Link_type node){while(node){_M_erase(static_cast<_Link_type>(node->_M_right));_Link_type left=static_cast<_Link_type>(node->_M_left);node->_M_value_field.~V();this->_M_header.deallocate(node,1);node=left;}}void _M_empty_initialize(){this->_M_header._M_data->_M_color=false;_M_root()=0;_M_leftmost()=this->_M_header._M_data;_M_rightmost()=this->_M_header._M_data;}
public:_Rb_tree(const Compare&compare,const A&a):_Rb_tree_base<V,A>(a),_M_node_count(0),_M_key_compare(compare){_M_empty_initialize();}_Rb_tree(const _Rb_tree&);~_Rb_tree(){clear();}void clear(){if(_M_node_count){_M_erase(_M_root());_M_leftmost()=this->_M_header._M_data;_M_root()=0;_M_rightmost()=this->_M_header._M_data;_M_node_count=0;}}unsigned int size()const{return _M_node_count;}iterator begin(){return iterator(_M_leftmost());}iterator end(){return iterator(this->_M_header._M_data);}const_iterator find(const K&key)const{return const_iterator(_M_find(key));}private:_Link_type _M_find(const K&)const;public:const_iterator begin()const{return const_iterator(_M_leftmost());}const_iterator end()const{return const_iterator(this->_M_header._M_data);}pair<iterator,bool>insert_unique(const V&);iterator insert_unique(iterator,const V&);
#ifdef PASS9_TEST
friend int ::main();
#endif
};
}
namespace std{template<class P>struct _Select1st{const typename P::first_type&operator()(const P&value)const{return value.first;}};template<class K,class T,class Compare=less<K>,class A=allocator<pair<const K,T> > >class map{public:typedef pair<const K,T>value_type;private:typedef _Rb_tree<K,value_type,_Select1st<value_type>,Compare,A>_Rep_type;_Rep_type _M_t;public:typedef typename _Rep_type::iterator iterator;typedef typename _Rep_type::const_iterator const_iterator;map():_M_t(Compare(),A()){}map(const map&other):_M_t(other._M_t){}map&operator=(const map&);iterator begin(){return _M_t.begin();}iterator end(){return _M_t.end();}const_iterator begin()const{return _M_t.begin();}const_iterator end()const{return _M_t.end();}const_iterator find(const K&key)const{return _M_t.find(key);}pair<iterator,bool>insert(const value_type&v){return _M_t.insert_unique(v);}T&operator[](const K&);bool empty()const{return _M_t.size()==0;}
#ifdef PASS9_TEST
friend int ::main();
#endif
};}

namespace std{template<class K,class C=less<K>,class A=allocator<K> >class set{typedef _Rb_tree<K,K,_Identity<K>,C,A>_Rep_type;_Rep_type _M_t;public:typedef typename _Rep_type::const_iterator iterator;typedef iterator const_iterator;set():_M_t(C(),A()){}const_iterator find(const K&key)const{return _M_t.find(key);}const_iterator end()const{return _M_t.end();}
#ifdef PASS9_TEST
friend int ::main();
#endif
};template<class T,class A>class _Vector_base{protected:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;public:_Vector_base():_M_start(0),_M_finish(0),_M_end_of_storage(A(),0){}};template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:unsigned int size()const{return this->_M_finish-this->_M_start;}T&operator[](unsigned int i){return this->_M_start[i];}};}
template<class T>class CArray:public std::vector<T>{};
class CBaseClass{public:virtual ~CBaseClass();};class CBaseClassNonCopyable:public CBaseClass{CBaseClassNonCopyable(const CBaseClassNonCopyable&);const CBaseClassNonCopyable&operator=(const CBaseClassNonCopyable&);protected:CBaseClassNonCopyable();virtual ~CBaseClassNonCopyable();};class C3DMeshFile;class C3DOptimisedMeshBase:public CBaseClassNonCopyable{public:virtual ~C3DOptimisedMeshBase();virtual void RemoveBones(const std::set<CCharString>&)=0;virtual void CopyTo(C3DMeshFile&)=0;};
class CCPPointerInfo;template<class T>class CCountedPointer{public:T*Data;CCPPointerInfo*Info;T*operator->()const{return Data;}};
// Layout: native mesh16 with public SubMeshes at4, submesh24 with Primitives0/Bones12, bone16. Primitive receiver state is132 opaque unread bytes; no full donor-layout claim.
namespace QuadricOptimiserInternals{class COptimisedPrimitive{unsigned char unread[132];public:void RemapGroups(const std::map<unsigned long,unsigned long>&);};class COptimisedBone{public:CCharString BoneName;long Parent;bool Remapped;long NewIndex;};class COptimisedSubMesh{public:CArray<CCountedPointer<COptimisedPrimitive> >Primitives;CArray<COptimisedBone>Bones;};class COptimisedMesh:public C3DOptimisedMeshBase{public:CArray<COptimisedSubMesh>SubMeshes;virtual void RemoveBones(const std::set<CCharString>&);virtual void CopyTo(C3DMeshFile&);};
void COptimisedMesh::RemoveBones(const std::set<CCharString>&removed){for(unsigned long i=0;i<SubMeshes.size();++i){COptimisedSubMesh&sub=SubMeshes[i];std::map<unsigned long,unsigned long>mapping;for(unsigned long j=0;j<sub.Bones.size();++j){COptimisedBone&bone=sub.Bones[j];if(bone.Parent!=-1){COptimisedBone&parent=sub.Bones[bone.Parent];if(parent.Remapped){bone.NewIndex=parent.NewIndex;bone.Remapped=true;}else if(removed.find(bone.BoneName.GetAsLowerCase())!=removed.end()){bone.NewIndex=bone.Parent;bone.Remapped=true;}if(bone.Remapped)mapping[j]=bone.NewIndex;}}if(!mapping.empty())for(unsigned long j=0;j<sub.Primitives.size();++j)sub.Primitives[j]->RemapGroups(mapping);}}
}
FABLE_STATIC_ASSERT(sizeof(QuadricOptimiserInternals::COptimisedMesh)==16);FABLE_STATIC_ASSERT(sizeof(QuadricOptimiserInternals::COptimisedSubMesh)==24);FABLE_STATIC_ASSERT(sizeof(QuadricOptimiserInternals::COptimisedBone)==16);

#undef malloc
#undef free
static unsigned errors,treeCopies,active,allocationCount;struct Allocation{unsigned char*raw,*data;unsigned size;bool released;};static Allocation records[256];
extern "C" void*__cdecl TestMalloc(unsigned int size){if(allocationCount==256){++errors;return 0;}Allocation&r=records[allocationCount++];r.raw=static_cast<unsigned char*>(malloc(size+32));r.data=r.raw+16;r.size=size;r.released=false;memset(r.raw,0xa5,size+32);++active;return r.data;}
extern "C" void __cdecl TestFree(void*p){Allocation*r=0;for(unsigned i=0;i<allocationCount;++i)if(records[i].data==p&&!records[i].released)r=records+i;if(!r){++errors;return;}for(unsigned i=0;i<16;++i)if(r->raw[i]!=0xa5||r->data[r->size+i]!=0xa5)++errors;r->released=true;--active;free(r->raw);}
namespace std{
template<class K,class V,class Extract,class Compare,class A>pair<typename _Rb_tree<K,V,Extract,Compare,A>::iterator,bool>_Rb_tree<K,V,Extract,Compare,A>::insert_unique(const V&value){_Rb_tree_node_base*parent=this->_M_header._M_data;_Rb_tree_node_base*node=_M_root();bool left=true;while(node){parent=node;const V&current=static_cast<_Rb_tree_node<V>*>(node)->_M_value_field;if(_M_key_compare(Extract()(value),Extract()(current))){left=true;node=node->_M_left;}else if(_M_key_compare(Extract()(current),Extract()(value))){left=false;node=node->_M_right;}else return pair<iterator,bool>(iterator(static_cast<_Rb_tree_node<V>*>(node)),false);} _Link_type result=this->_M_header.allocate(1);new(result)_Rb_tree_node<V>(value);result->_M_color=true;result->_M_parent=parent;result->_M_left=result->_M_right=0;if(parent==this->_M_header._M_data)_M_root()=result;else if(left)parent->_M_left=result;else parent->_M_right=result;if(_M_node_count==0||_M_key_compare(Extract()(value),Extract()(_M_leftmost()->_M_value_field)))_M_leftmost()=result;if(_M_node_count==0||_M_key_compare(Extract()(_M_rightmost()->_M_value_field),Extract()(value)))_M_rightmost()=result;++_M_node_count;return pair<iterator,bool>(iterator(result),true);}
template<class K,class V,class Extract,class Compare,class A>_Rb_tree<K,V,Extract,Compare,A>::_Rb_tree(const _Rb_tree&other):_Rb_tree_base<V,A>(A()),_M_node_count(0),_M_key_compare(other._M_key_compare){_M_empty_initialize();for(const_iterator it=other.begin();it!=other.end();++it)insert_unique(*it);++treeCopies;}

}


struct FixtureStringData{char value;};static FixtureStringData*fixtureString(CCharStringData*p){return reinterpret_cast<FixtureStringData*>(p);}
CCharString::CCharString():PStringData(0){}
CCharString::CCharString(const CCharString&other):PStringData(0){if(other.PStringData){PStringData=static_cast<CCharStringData*>(TestMalloc(sizeof(FixtureStringData)));fixtureString(PStringData)->value=fixtureString(other.PStringData)->value;}}
CCharString::~CCharString(){if(PStringData)TestFree(PStringData);}
CCharString CCharString::GetAsLowerCase()const{CCharString result(*this);if(result.PStringData&&fixtureString(result.PStringData)->value>='A'&&fixtureString(result.PStringData)->value<='Z')fixtureString(result.PStringData)->value+=('a'-'A');return result;}
bool CCharString::operator<(const CCharString&other)const{return (PStringData?fixtureString(PStringData)->value:0)<(other.PStringData?fixtureString(other.PStringData)->value:0);}
CBaseClass::~CBaseClass(){}CBaseClassNonCopyable::CBaseClassNonCopyable(){}CBaseClassNonCopyable::~CBaseClassNonCopyable(){}C3DOptimisedMeshBase::~C3DOptimisedMeshBase(){}
void QuadricOptimiserInternals::COptimisedMesh::CopyTo(C3DMeshFile&){}
namespace std{
template<class K,class V,class Extract,class Compare,class A>typename _Rb_tree<K,V,Extract,Compare,A>::_Link_type _Rb_tree<K,V,Extract,Compare,A>::_M_find(const K&key)const{for(const_iterator it=begin();it!=end();++it)if(!_M_key_compare(Extract()(*it),key)&&!_M_key_compare(key,Extract()(*it)))return static_cast<_Link_type>(it._M_node);return this->_M_header._M_data;}
template<class K,class T,class C,class A>T&map<K,T,C,A>::operator[](const K&key){for(iterator it=begin();it!=end();++it)if(it->first==key)return it->second;return _M_t.insert_unique(value_type(key,T())).first->second;}
}
using namespace QuadricOptimiserInternals;
static COptimisedPrimitive*objects[4];static unsigned expectedMap[4][4],expectedMask[4],seen[4];
void COptimisedPrimitive::RemapGroups(const std::map<unsigned long,unsigned long>&mapping){unsigned slot=4;for(unsigned i=0;i<4;++i)if(objects[i]==this)slot=i;if(slot==4){++errors;return;}++seen[slot];unsigned mask=0;for(std::map<unsigned long,unsigned long>::const_iterator it=mapping.begin();it!=mapping.end();++it){if(it->first>=4||!(expectedMask[slot]&(1U<<it->first))||it->second!=expectedMap[slot][it->first])++errors;else mask|=1U<<it->first;}if(mask!=expectedMask[slot])++errors;}
int main(){unsigned cases=0;for(unsigned seed=0;seed<512;++seed)for(unsigned removedMask=0;removedMask<16;++removedMask)for(unsigned subCount=0;subCount<=2;++subCount){allocationCount=active=errors=treeCopies=0;{
std::set<CCharString>removed;for(unsigned k=0;k<4;++k)if(removedMask&(1U<<k)){CCharString value;value.PStringData=static_cast<CCharStringData*>(TestMalloc(sizeof(FixtureStringData)));fixtureString(value.PStringData)->value='a'+k;removed._M_t.insert_unique(value);}
COptimisedMesh owner;COptimisedSubMesh subs[2];COptimisedBone bones[2][4];COptimisedPrimitive primitiveStorage[4];CCountedPointer<COptimisedPrimitive>refs[2][2];bool expectedRemapped[2][4];long expectedNew[2][4],originalParent[2][4];char originalName[2][4];unsigned boneCounts[2],primitiveCounts[2];COptimisedSubMesh*subState[3]={subs,subs+subCount,subs+2};memcpy(&owner.SubMeshes,subState,12);
for(unsigned i=0;i<4;++i){objects[i]=primitiveStorage+i;expectedMask[i]=seen[i]=0;for(unsigned j=0;j<4;++j)expectedMap[i][j]=0;}
for(unsigned sub=0;sub<subCount;++sub){unsigned n=(seed+sub)%5,primitives=(seed/5+sub)%3;boneCounts[sub]=n;primitiveCounts[sub]=primitives;COptimisedBone*bs[3]={bones[sub],bones[sub]+n,bones[sub]+4};memcpy(&subs[sub].Bones,bs,12);CCountedPointer<COptimisedPrimitive>*rs[3]={refs[sub],refs[sub]+primitives,refs[sub]+2};memcpy(&subs[sub].Primitives,rs,12);
for(unsigned j=0;j<2;++j){refs[sub][j].Data=objects[sub*2+j];refs[sub][j].Info=0;}
for(unsigned j=0;j<n;++j){COptimisedBone&bone=bones[sub][j];bone.Parent=(long)((seed>>(j*2))%(j+1))-1;bone.Remapped=((seed>>(j+4))&1)!=0;bone.NewIndex=(long)((seed+j)%4);bone.BoneName.PStringData=static_cast<CCharStringData*>(TestMalloc(sizeof(FixtureStringData)));fixtureString(bone.BoneName.PStringData)->value=(char)(((seed>>j)&1?'A':'a')+((j+sub)%4));originalParent[sub][j]=bone.Parent;originalName[sub][j]=fixtureString(bone.BoneName.PStringData)->value;expectedRemapped[sub][j]=bone.Remapped;expectedNew[sub][j]=bone.NewIndex;}
for(unsigned j=0;j<n;++j){long parent=originalParent[sub][j];if(parent>=0){if(expectedRemapped[sub][parent]){expectedRemapped[sub][j]=true;expectedNew[sub][j]=expectedNew[sub][parent];}else if(removedMask&(1U<<((j+sub)%4))){expectedRemapped[sub][j]=true;expectedNew[sub][j]=parent;}if(expectedRemapped[sub][j])for(unsigned k=0;k<2;++k){expectedMask[sub*2+k]|=1U<<j;expectedMap[sub*2+k][j]=expectedNew[sub][j];}}}
}
unsigned beforeActive=active;owner.RemoveBones(removed);if(errors||active!=beforeActive)return 1;
for(unsigned sub=0;sub<subCount;++sub){for(unsigned j=0;j<boneCounts[sub];++j){COptimisedBone&bone=bones[sub][j];if(bone.Parent!=originalParent[sub][j]||fixtureString(bone.BoneName.PStringData)->value!=originalName[sub][j]||bone.Remapped!=expectedRemapped[sub][j]||bone.NewIndex!=expectedNew[sub][j])return 2;}for(unsigned j=0;j<2;++j)if(seen[sub*2+j]!=(j<primitiveCounts[sub]&&expectedMask[sub*2+j]?1U:0U))return 3;}
for(unsigned k=0;k<4;++k){CCharString key;key.PStringData=static_cast<CCharStringData*>(TestMalloc(sizeof(FixtureStringData)));fixtureString(key.PStringData)->value='a'+k;if((removed.find(key)!=removed.end())!=((removedMask&(1U<<k))!=0))return 4;}
}if(errors||active)return 5;++cases;}if(cases!=24576)return 6;puts("00aca0c0_TEST PASS");return 0;}