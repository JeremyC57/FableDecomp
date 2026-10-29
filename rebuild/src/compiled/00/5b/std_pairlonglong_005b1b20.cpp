// 005b1b20 std::_Construct
// Leaf: ??$_Construct@U?$pair@QAVCComponent@NUISystem@@U?$pair@JJ@std@@@std@@U12@@std@@YIXPAU?$pair@QAVCComponent@NUISystem@@U?$pair@JJ@std@@@0@ABU10@@Z
// Flags: /O1 /Oy- /Gr
#ifndef _MSC_VER
#define __fastcall
#endif
inline void* operator new(unsigned int,void*p){return p;}
namespace NUISystem {class CComponent;}
namespace std {
template<class A,class B>struct pair {
 typedef A first_type;typedef B second_type;A first;B second;
 pair():first(A()),second(B()){}
 pair(const A&a,const B&b):first(a),second(b){}
 template<class X,class Y>pair(const pair<X,Y>&p):first(p.first),second(p.second){}
 pair(const pair&p):first(p.first),second(p.second){}
};
template<class A,class B>void __fastcall _Construct(A*p,const B&v){new(p)A(v);}
template void __fastcall _Construct<pair<NUISystem::CComponent* const,pair<long,long> >,pair<NUISystem::CComponent* const,pair<long,long> > >(pair<NUISystem::CComponent* const,pair<long,long> >*,const pair<NUISystem::CComponent* const,pair<long,long> >&);
}