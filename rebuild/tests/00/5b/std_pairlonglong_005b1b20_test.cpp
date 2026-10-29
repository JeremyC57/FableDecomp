#include <stdio.h>
#include <string.h>
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

extern "C" void abort();
static void check(bool p){if(!p)abort();}
int main(){typedef std::pair<long,long> Pair;typedef std::pair<NUISystem::CComponent*const,Pair> Value;check(sizeof(Value)==12);const long values[]={0,1,-1,2,-2,127,-128,255,256,-256,32767,-32768,65535,65536,2147483647L,(-2147483647L-1L)};int identities[8];unsigned cases=0;for(int who=0;who<8;++who)for(int i=0;i<16;++i)for(int j=0;j<16;++j){NUISystem::CComponent*ptr=who?reinterpret_cast<NUISystem::CComponent*>(&identities[who]):0;Value source(ptr,Pair(values[i],values[j]));unsigned char original[12];memcpy(original,&source,12);union{double align;unsigned char bytes[32];}storage;memset(storage.bytes,0x5a,sizeof storage.bytes);Value*dest=reinterpret_cast<Value*>(storage.bytes+8);std::_Construct(dest,source);check(dest->first==ptr&&dest->second.first==values[i]&&dest->second.second==values[j]);check(memcmp(&source,original,12)==0);for(int k=0;k<32;++k)if(k<8||k>=20)check(storage.bytes[k]==0x5a);dest->~Value();++cases;}check(cases==2048);puts("005b1b20_TEST PASS");return 0;}