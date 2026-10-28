#include <stdio.h>
#include <string.h>
#define protected public
// CPhysicsMesh::GetNoDummyObjects(const CCharString&) const -- Count matching named mesh helpers.
// Leaf: ?GetNoDummyObjects@CPhysicsMesh@@QBEJABVCCharString@@@Z
// Identity: proven. Own CPhysicsMeshObject forwarding wrapper00a11750/00a11800 reached by graphic appearance helper/dummy queries; original PDB exact CPhysicsMesh const GetNoHelperPoints/GetNoDummyObjects signatures. HelperPoints16 and DummyObjects32 reconcile original CVectorMap fields with STLport12-byte vector; record strides20/56 match original pair<CCharString,C3DHelperPoint/C3DDummyObject>. Original CIVCountedPointeeBase ancestry from own RTTI. String helper original protected static CBasicString<char>::Compare00411570.
// Layout: original typed mesh prefix through both maps; unused suffix omitted. StringData packed17/BasicString13; original vector/base/proxy hierarchy. No active owner header.
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
#pragma pack(push,1)
template<class T>class CBasicString{public:T* PData;unsigned long StringLength;unsigned long DataLength:31;bool UseFastExtend:1;protected:static long __fastcall Compare(const T*,const T*);public:bool operator==(const CBasicString& other)const{if(StringLength!=other.StringLength)return false;return Compare(PData,other.PData)==0;}};
class CCharStringData{public:CBasicString<char> Data;long NoRefs;};
#pragma pack(pop)
#pragma auto_inline(off)
template<>long __fastcall CBasicString<char>::Compare(const char* a,const char* b){for(;*a||*b;++a,++b){if(*a<*b)return -1;if(*a>*b)return 1;}return 0;}
#pragma auto_inline(on)
class CCharString{public:CCharStringData* PStringData;CCharString():PStringData(0){}~CCharString();__forceinline bool operator==(const CCharString& other)const{if(PStringData==other.PStringData)return true;if(!PStringData||!other.PStringData)return false;return PStringData->Data==other.PStringData->Data;}};
namespace std{template<class A,class B>struct pair{A first;B second;};template<class T>class allocator{};template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};template<class T,class A>class _Vector_base{protected:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;};template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:const T* begin()const{return this->_M_start;}const T* end()const{return this->_M_finish;}};}
template<class T>class CArray:public std::vector<T>{};template<class T>class CCompareString{};template<class K,class V,class Cmp>class CVectorMap{public:CArray<std::pair<K,V> > Data;Cmp Compare;bool Dirty;};
#pragma pack(push,1)
class C3DVector{public:float X,Y,Z;};class CMatrix3x4{public:C3DVector Rows[4];};
#pragma pack(pop)
class C3DHelperPoint{public:C3DVector Position;long BoneIndex;};class C3DDummyObject{public:CMatrix3x4 Transform;long BoneIndex;};
class CIVCountedPointeeBase{public:virtual ~CIVCountedPointeeBase(){}virtual void Delete()const;mutable unsigned long IVRefCount;};class CPhysicsMesh:public CIVCountedPointeeBase{public:unsigned long ReferenceCount;bool NavigationLineListInfo;CVectorMap<CCharString,C3DHelperPoint,CCompareString<C3DHelperPoint> > HelperPoints;CVectorMap<CCharString,C3DDummyObject,CCompareString<C3DDummyObject> > DummyObjects;long GetNoHelperPoints(const CCharString&)const;long GetNoDummyObjects(const CCharString&)const;};
long CPhysicsMesh::GetNoDummyObjects(const CCharString& name)const{long count=0;for(const std::pair<CCharString,C3DDummyObject>* p=DummyObjects.Data.begin(),*last=DummyObjects.Data.end();p!=last;++p){if(p->first==name)++count;}return count;}

#undef protected
CCharString::~CCharString(){}void CIVCountedPointeeBase::Delete()const{}
int main(){CPhysicsMesh mesh;std::pair<CCharString,C3DDummyObject> records[6];CCharStringData data[4];char text0[]="one",text1[]="one",text2[]="two",text3[]="long";char* texts[]={text0,text1,text2,text3};for(int i=0;i<4;++i){data[i].Data.PData=texts[i];data[i].Data.StringLength=(unsigned long)strlen(texts[i]);}records[0].first.PStringData=0;records[1].first.PStringData=data;records[2].first.PStringData=data+1;records[3].first.PStringData=data+2;records[4].first.PStringData=data+3;records[5].first.PStringData=data;mesh.DummyObjects.Data._M_start=records;mesh.DummyObjects.Data._M_finish=records+6;CCharString key;bool ok=mesh.GetNoDummyObjects(key)==1;key.PStringData=data;ok=ok&&mesh.GetNoDummyObjects(key)==3;key.PStringData=data+2;ok=ok&&mesh.GetNoDummyObjects(key)==1;mesh.DummyObjects.Data._M_finish=records;ok=ok&&mesh.GetNoDummyObjects(key)==0;puts(ok?"00a6c5f0_TEST PASS":"00a6c5f0_TEST FAIL");return ok?0:1;}