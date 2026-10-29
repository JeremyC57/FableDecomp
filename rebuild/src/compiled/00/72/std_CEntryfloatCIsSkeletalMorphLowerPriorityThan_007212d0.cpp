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