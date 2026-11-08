// CPhysicsMesh::GetNoDummyObjects(const CCharString&) const -- Count matching named mesh helpers.
// Leaf: ?GetNoDummyObjects@CPhysicsMesh@@QBEJABVCCharString@@@Z
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Identity: proven. Own CPhysicsMeshObject forwarding wrapper00a11750/00a11800 reached by graphic appearance helper/dummy queries; original PDB exact CPhysicsMesh const GetNoHelperPoints/GetNoDummyObjects signatures. HelperPoints16 and DummyObjects32 reconcile original CVectorMap fields with STLport12-byte vector; record strides20/56 match original pair<CCharString,C3DHelperPoint/C3DDummyObject>. Original CIVCountedPointeeBase ancestry from own RTTI. String helper original protected static CBasicString<char>::Compare00411570.
// Layout: original typed mesh prefix through both maps; unused suffix omitted. StringData packed17/BasicString13; original vector/base/proxy hierarchy. No active owner header.
#include "fable_compiler.h"
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
class CCharString{public:CCharStringData* PStringData;CCharString():PStringData(0){}~CCharString();FABLE_FORCEINLINE bool operator==(const CCharString& other)const{if(PStringData==other.PStringData)return true;if(!PStringData||!other.PStringData)return false;return PStringData->Data==other.PStringData->Data;}};
#include "engine/CArray.h"
template<class T>class CCompareString{};template<class K,class V,class Cmp>class CVectorMap{public:CArray<std::pair<K,V> > Data;Cmp Compare;bool Dirty;};
#pragma pack(push,1)
#include "engine/C3DVector.h"
class CMatrix3x4{public:C3DVector Rows[4];};
#pragma pack(pop)
class C3DHelperPoint{public:C3DVector Position;long BoneIndex;};class C3DDummyObject{public:CMatrix3x4 Transform;long BoneIndex;};
class CIVCountedPointeeBase{public:virtual ~CIVCountedPointeeBase(){}virtual void Delete()const;mutable unsigned long IVRefCount;};class CPhysicsMesh:public CIVCountedPointeeBase{public:unsigned long ReferenceCount;bool NavigationLineListInfo;CVectorMap<CCharString,C3DHelperPoint,CCompareString<C3DHelperPoint> > HelperPoints;CVectorMap<CCharString,C3DDummyObject,CCompareString<C3DDummyObject> > DummyObjects;long GetNoHelperPoints(const CCharString&)const;long GetNoDummyObjects(const CCharString&)const;};
long CPhysicsMesh::GetNoDummyObjects(const CCharString& name)const{long count=0;for(const std::pair<CCharString,C3DDummyObject>* p=DummyObjects.Data.begin(),*last=DummyObjects.Data.end();p!=last;++p){if(p->first==name)++count;}return count;}