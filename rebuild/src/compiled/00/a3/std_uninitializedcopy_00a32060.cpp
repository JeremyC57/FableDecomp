// std::__uninitialized_copy(Input first, Input last, Output result, const __false_type&) -- Copy-construct a pair range into raw storage.
// Leaf: ??$__uninitialized_copy@PAU?$pair@KVC3DDummyObject@@@std@@PAU12@@std@@YIPAU?$pair@KVC3DDummyObject@@@0@PAU10@00ABU__false_type@0@@Z
// Identity: Own overflow00a33e80 used by dummy-object vector insert00a349b0 and C3DMesh path00a2fc17;56-byte pair (52-byte Transform/BoneIndex value). Mutable inputs are from original storage in the caller.
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
typedef __SIZE_TYPE__ PlacementSize;
#else
typedef unsigned int PlacementSize;
#endif
inline void* operator new(PlacementSize,void* p){return p;}
namespace std {
template<class A,class B> struct pair {A first;B second;pair(){}pair(const pair& other):first(other.first),second(other.second){}};
struct __false_type {};
template<class T,class V> __forceinline void _Construct(T* p,const V& value){new(p) T(value);}
template<class Input,class Output>
Output __fastcall __uninitialized_copy(Input first,Input last,Output result,const __false_type&) {
    Output current=result;for(;first!=last;++first,++current)_Construct(&*current,*first);return current;
}
}
// Layout: original C3DDummyObject is52bytes: Transform12floats+0, BoneIndex+48. Own vector overflow00a33e80 and PDB confirm56-byte unsigned-key pair.
class CMatrix3x4 {public:float M[12];};
class C3DDummyObject {public:CMatrix3x4 Transform;long BoneIndex;};
typedef std::pair<unsigned long,C3DDummyObject> Element;
template Element* __fastcall std::__uninitialized_copy<Element*,Element*>(Element*,Element*,Element*,const std::__false_type&);