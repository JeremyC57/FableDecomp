#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>
#ifndef _MSC_VER
#define __cdecl
typedef unsigned long test_uint64;
#else
typedef unsigned __int64 test_uint64;
#endif
static void check(bool ok) { if(!ok) { fprintf(stderr,"check failed\n"); exit(1); } }
#ifndef FABLE_ENGINE_CCHARSTRING_H
#define FABLE_ENGINE_CCHARSTRING_H
// Retail declarations (Codex pass 11): Ego_r TPI 0x27dc, class key V,
// sizeof 4, protected PStringData +0; native array construction advances by 4.
class CCharStringData;
class CCharString {
public:
    CCharString();
    CCharString(const char* text, long length = -1);
    CCharString(const CCharString& other);
    ~CCharString();
protected:
    CCharStringData* PStringData;
};
#endif

// Test dependency: stable strings model the value/lifetime contract of the API.
CCharString::CCharString() : PStringData(0) { }
CCharString::CCharString(const char* t,long) : PStringData((CCharStringData*)t) { }
CCharString::CCharString(const CCharString& x) : PStringData(x.PStringData) { }
CCharString::~CCharString() { }
static const char* text_of(const CCharString& x) {
    const char* p=*(const char* const*)&x; return p?p:"";
}
bool operator<(const CCharString& a,const CCharString& b) { return strcmp(text_of(a),text_of(b))<0; }
#ifndef FABLE_ENGINE_CRUNTIMESCRIPTEDENTITYPERSIST_H
#define FABLE_ENGINE_CRUNTIMESCRIPTEDENTITYPERSIST_H

// Nested-type declaration: no enclosing object is instantiated by these rows.
// Layout: Ego_r CPersistInfo sizeof 16/alignment 8, UID +0/PersistScript +8;
// native pair copy cbcb3b..cbcb56 confirms second +8 and its string +16.
namespace NScript {
class CRuntimeScriptedEntityPersist {
public:
    class CPersistInfo {
    public:
        test_uint64 UID;
        CCharString PersistScript;
    };
};
}
#endif
namespace std { template<class _T1,class _T2> struct pair { _T1 first; _T2 second; }; }
template<class K,class V> class CKeyPairCompareLess {
public: bool operator()(const std::pair<K,V>& a,const std::pair<K,V>& b) const { return a.first<b.first; }
};
// std::__introsort_loop<std::pair<unsigned __int64,NScript::CRuntimeScriptedEntityPersist::CPersistInfo> *,std::pair<unsigned __int64,NScript::CRuntimeScriptedEntityPersist::CPersistInfo>,int,CKeyPairCompareLess<unsigned __int64,NScript::CRuntimeScriptedEntityPersist::CPersistInfo> >(std::pair<unsigned __int64,NScript::CRuntimeScriptedEntityPersist::CPersistInfo> *,std::pair<unsigned __int64,NScript::CRuntimeScriptedEntityPersist::CPersistInfo> *,std::pair<unsigned __int64,NScript::CRuntimeScriptedEntityPersist::CPersistInfo> *,int,CKeyPairCompareLess<unsigned __int64,NScript::CRuntimeScriptedEntityPersist::CPersistInfo>) -- SGI depth-limited partition sort.
// Leaf: ??$__introsort_loop@PAU?$pair@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@@std@@U12@HV?$CKeyPairCompareLess@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@@@@std@@YIXPAU?$pair@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@@0@00HV?$CKeyPairCompareLess@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@@@@Z
// Flags: /Gr
/*
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Copyright (c) 1996,1997
 * Silicon Graphics Computer Systems, Inc.
 *
 * Copyright (c) 1997
 * Moscow Center for SPARC Technology
 *
 * Copyright (c) 1999 
 * Boris Fomitchev
 *
 * This material is provided "as is", with absolutely no warranty expressed
 * or implied. Any use is at your own risk.
 *
 * Permission to use or copy this software for any purpose is hereby granted 
 * without fee, provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 *
 */
// Modified excerpt: only the algorithms required by this instantiation are retained.
#pragma inline_recursion(on)
namespace std {
template<class _Tp,class _Compare>
inline const _Tp& __median(const _Tp& __a,const _Tp& __b,const _Tp& __c,_Compare __comp) {
    if (__comp(__a,__b))
        if (__comp(__b,__c)) return __b;
        else if (__comp(__a,__c)) return __c;
        else return __a;
    else if (__comp(__a,__c)) return __a;
    else if (__comp(__b,__c)) return __c;
    else return __b;
}
template<class _RandomAccessIter,class _Tp,class _Compare>
_RandomAccessIter __unguarded_partition(_RandomAccessIter,_RandomAccessIter,_Tp,_Compare);
template<class _RandomAccessIter,class _Tp,class _Compare>
void __partial_sort(_RandomAccessIter,_RandomAccessIter,_RandomAccessIter,_Tp*,_Compare);
template<class _RandomAccessIter,class _Compare>
void partial_sort(_RandomAccessIter __first,_RandomAccessIter __middle,_RandomAccessIter __last,_Compare __comp) {
    __partial_sort(__first,__middle,__last,(_RandomAccessIter)0,__comp);
}
template<class _RandomAccessIter,class _Tp,class _Size,class _Compare>
void __introsort_loop(_RandomAccessIter __first,_RandomAccessIter __last,_Tp*,_Size __depth_limit,_Compare __comp) {
    while (__last-__first > 16) {
        if (__depth_limit == 0) {
            partial_sort(__first,__last,__last,__comp);
            return;
        }
        --__depth_limit;
        _RandomAccessIter __cut = __unguarded_partition(__first,__last,
            _Tp(__median(*__first,*(__first+(__last-__first)/2),*(__last-1),__comp)),__comp);
        __introsort_loop(__cut,__last,(_Tp*)0,__depth_limit,__comp);
        __last=__cut;
    }
}
}
template void std::__introsort_loop<std::pair<test_uint64,NScript::CRuntimeScriptedEntityPersist::CPersistInfo>*,std::pair<test_uint64,NScript::CRuntimeScriptedEntityPersist::CPersistInfo>,int,CKeyPairCompareLess<test_uint64,NScript::CRuntimeScriptedEntityPersist::CPersistInfo> >(std::pair<test_uint64,NScript::CRuntimeScriptedEntityPersist::CPersistInfo>*,std::pair<test_uint64,NScript::CRuntimeScriptedEntityPersist::CPersistInfo>*,std::pair<test_uint64,NScript::CRuntimeScriptedEntityPersist::CPersistInfo>*,int,CKeyPairCompareLess<test_uint64,NScript::CRuntimeScriptedEntityPersist::CPersistInfo>);
static int partition_calls=0, fallback_calls=0;
namespace std {
template<class I,class T,class C> I __unguarded_partition(I first,I last,T pivot,C comp) {
 ++partition_calls;
 for(;;) {
  while(comp(*first,pivot)) ++first;
  --last; while(comp(pivot,*last)) --last;
  if(!(first<last))return first;
  T temporary=*first;*first=*last;*last=temporary;++first;
 }
}
template<class I,class T,class C> void __partial_sort(I first,I middle,I last,T*,C comp) {
 ++fallback_calls;check(middle==last);
 // An independently simple dependency implementation satisfies partial_sort's contract.
 for(I current=first;current!=last;++current) {
  I least=current;for(I other=current+1;other!=last;++other)if(comp(*other,*least))least=other;
  T temporary=*current;*current=*least;*least=temporary;
 }
}
}
static unsigned int random_state=17;
static unsigned int random_value() { random_state=random_state*1664525u+1013904223u;return random_state; }
typedef std::pair<test_uint64,NScript::CRuntimeScriptedEntityPersist::CPersistInfo> element_type;
typedef CKeyPairCompareLess<test_uint64,NScript::CRuntimeScriptedEntityPersist::CPersistInfo> compare_type;
static char keys[512][16];
static test_uint64 key_value(int k) { return ((test_uint64)(k%17)<<32)+(unsigned int)k; }
static void initialise(element_type& x,int k) {
 x.first=key_value(k);x.second.UID=(test_uint64)k;x.second.PersistScript=CCharString(keys[k]);
}
static int payload_id(const element_type& x) { return (int)x.second.UID; }
static void check_payload(const element_type& x) {
 int k=(int)x.second.UID;check(k>=0&&k<512);check(x.first==key_value(k));
 check(strcmp(text_of(x.second.PersistScript),keys[k])==0);
}
int main() {
 for(int k=0;k<512;++k)sprintf(keys[k],"key_%04d",k);
 const int sizes[]={0,1,2,15,16,17,18,31,32,33,63,64,127,256,511};
 const int depths[]={0,1,2,4,16};
 compare_type comp;
 for(int si=0;si<15;++si)for(int mode=0;mode<5;++mode)for(int di=0;di<5;++di) {
  int count=sizes[si];element_type values[513];int before[512],after[512];memset(before,0,sizeof(before));memset(after,0,sizeof(after));
  initialise(values[0],510);initialise(values[count+1],509);
  for(int i=0;i<count;++i) {
   int k=mode==0?i:mode==1?count-1-i:mode==2?7:mode==3?(i%11):(int)(random_value()%512);
   initialise(values[i+1],k);++before[k];
  }
  partition_calls=0;fallback_calls=0;
  std::__introsort_loop(values+1,values+count+1,(element_type*)0,depths[di],comp);
  check_payload(values[0]);check_payload(values[count+1]);
  if(count<=16)check(partition_calls==0&&fallback_calls==0);
  if(count>16&&depths[di]==0)check(partition_calls==0&&fallback_calls==1);
  // Introsort intentionally leaves short partitions for final insertion sort.
  // No inversion can span more than a threshold-sized partition.
  for(int i=0;i<count;++i) {
   check_payload(values[i+1]);++after[payload_id(values[i+1])];
   for(int j=i+16;j<count;++j)check(!comp(values[j+1],values[i+1]));
  }
  for(int c=0;c<512;++c)check(before[c]==after[c]);
  for(int p=2;p<=count;++p) {
   element_type value=values[p];int j=p;
   while(j>1&&comp(value,values[j-1])){values[j]=values[j-1];--j;}
   values[j]=value;
  }
  for(int q=2;q<=count;++q)check(!comp(values[q],values[q-1]));
 }
 printf("00cbca50_TEST PASS\n");return 0;
}
