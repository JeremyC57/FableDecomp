#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#endif
// 007212d0 std::__insertion_sort
// Leaf: ??$__insertion_sort@PAU?$pair@VCEntry@CSkeletalMorphs@@M@std@@VCIsSkeletalMorphLowerPriorityThan@@@std@@YIXPAU?$pair@VCEntry@CSkeletalMorphs@@M@0@0VCIsSkeletalMorphLowerPriorityThan@@@Z
// Flags:
class CDefString{public:long TablePos;};
// Layout evidence: donor76083-76086 gives originalCEntry8 andusedfields.
// CSkeletalMorphs is usedonlyas originalnested scope; itsownlayoutis unobserved.
class CSkeletalMorphs{public:class CEntry{public:CDefString MorphName;short Priority;bool SupressesLowerMorphs;};};
namespace std{
template<class X,class Y>struct pair{
    typedef X first_type;typedef Y second_type;X first;Y second;
    pair():first(X()),second(Y()){}
    pair(const X&x,const Y&y):first(x),second(y){}
    template<class A,class B>pair(const pair<A,B>&p):first(p.first),second(p.second){}
    pair(const pair&p):first(p.first),second(p.second){}
};
struct input_iterator_tag{};
struct forward_iterator_tag:public input_iterator_tag{};
struct bidirectional_iterator_tag:public forward_iterator_tag{};
struct random_access_iterator_tag:public bidirectional_iterator_tag{};
}
class CIsSkeletalMorphLowerPriorityThan{
public:bool operator()(const std::pair<CSkeletalMorphs::CEntry,float>&x,const std::pair<CSkeletalMorphs::CEntry,float>&y)const{return x.first.Priority<y.first.Priority;}
};
namespace std{
#pragma auto_inline(off)
template<class I,class O,class D>O __fastcall __copy_backward(I first,I last,O result,const random_access_iterator_tag&,D*){
    for(D n=last-first;n>0;--n)*--result=*--last;return result;
}
#pragma auto_inline(on)
template<class I,class T,class C>void __fastcall __unguarded_linear_insert(I,T,C);
template<class I,class O>inline O __fastcall copy_backward(I first,I last,O result){return __copy_backward(first,last,result,random_access_iterator_tag(),(int*)0);}
template<class I,class T,class C>inline void __fastcall __linear_insert(I first,I last,T value,C compare){
    if(compare(value,*first)){copy_backward(first,last,last+1);*first=value;}
    else __unguarded_linear_insert(last,value,compare);
}
template<class I,class C>void __fastcall __insertion_sort(I first,I last,C compare){
    if(first==last)return;
    for(I current=first+1;current!=last;++current)__linear_insert(first,current,*current,compare);
}
template void __fastcall __insertion_sort<std::pair<CSkeletalMorphs::CEntry,float>*,CIsSkeletalMorphLowerPriorityThan>(std::pair<CSkeletalMorphs::CEntry,float>*,std::pair<CSkeletalMorphs::CEntry,float>*,CIsSkeletalMorphLowerPriorityThan);
}

namespace std{
template<class I,class T,class C>void __fastcall __unguarded_linear_insert(I last,T value,C compare){
    I next=last;--next;
    while(compare(value,*next)){*last=*next;last=next;--next;}
    *last=value;
}
}
typedef std::pair<CSkeletalMorphs::CEntry,float>Item;
typedef char CheckEntry[sizeof(CSkeletalMorphs::CEntry)==8?1:-1];
typedef char CheckItem[sizeof(Item)==12?1:-1];
typedef char CheckCompare[sizeof(CIsSkeletalMorphLowerPriorityThan)==1?1:-1];
static short priority(int pattern,int index,unsigned int seed){
    switch(pattern){
    case 0:return(short)(index-32);case 1:return(short)(32-index);
    case 2:return 0;case 3:return(short)(index%2?-32768:32767);
    case 4:return(short)-32768;case 5:return(short)32767;
    case 6:return(short)(index%7-3);
    default:seed=seed*1664525U+1013904223U+(unsigned int)index*747796405U;
        return(short)((int)((seed>>9)%65536U)-32768);
    }
}
int main(){
    Item rows[67];short keys[65];long names[65];bool suppress[65];float weights[65];int expected[65];unsigned int cases=0;
    for(int n=0;n<=64;++n)for(int pattern=0;pattern<12;++pattern)for(unsigned int seed=0;seed<16;++seed){
        rows[0].first.MorphName.TablePos=-777;rows[0].first.Priority=-123;rows[0].first.SupressesLowerMorphs=true;rows[0].second=-16;
        rows[n+1].first.MorphName.TablePos=-888;rows[n+1].first.Priority=123;rows[n+1].first.SupressesLowerMorphs=false;rows[n+1].second=16;
        for(int i=0;i<n;++i){
            keys[i]=priority(pattern,i,seed);names[i]=(long)(i+64*pattern+1024*seed);suppress[i]=((i+(int)seed)&1)!=0;weights[i]=(float)(i*4-(int)seed)/4.0f;
            rows[i+1].first.MorphName.TablePos=names[i];rows[i+1].first.Priority=keys[i];rows[i+1].first.SupressesLowerMorphs=suppress[i];rows[i+1].second=weights[i];
        }
        for(int i=0;i<n;++i){int rank=0;for(int j=0;j<n;++j)if(keys[j]<keys[i]||(keys[j]==keys[i]&&j<i))++rank;expected[rank]=i;}
        std::__insertion_sort(rows+1,rows+1+n,CIsSkeletalMorphLowerPriorityThan());
        for(int i=0;i<n;++i){int j=expected[i];if(rows[i+1].first.MorphName.TablePos!=names[j]||rows[i+1].first.Priority!=keys[j]||rows[i+1].first.SupressesLowerMorphs!=suppress[j]||rows[i+1].second!=weights[j])return 1;}
        if(rows[0].first.MorphName.TablePos!=-777||rows[0].first.Priority!=-123||!rows[0].first.SupressesLowerMorphs||rows[0].second!=-16)return 2;
        if(rows[n+1].first.MorphName.TablePos!=-888||rows[n+1].first.Priority!=123||rows[n+1].first.SupressesLowerMorphs||rows[n+1].second!=16)return 3;
        ++cases;
    }
    if(cases!=12480)return 4;puts("007212d0_TEST PASS");return 0;
}