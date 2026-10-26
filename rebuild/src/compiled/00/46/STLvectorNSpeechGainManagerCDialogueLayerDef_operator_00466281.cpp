// 00466281 std::vector<NSpeechGainManager::CDialogueLayerDef>::operator=
// Flags: /O2 /Oy /W3 /O1 /Oy-
// Leaf: ??4?$vector@VCDialogueLayerDef@NSpeechGainManager@@@std@@QAEAAU01@ABU01@@Z
#include <stdlib.h>
#include <new>
namespace NSpeechGainManager {
// Layout: vtable slot0 is a genuine virtual destructor; opaque payload follows vptr. Retail copy assignment and ctor operate on 64-byte CDialogueLayerDef; no engine header exists.
class CDialogueLayerDef { public:unsigned data[15];CDialogueLayerDef();CDialogueLayerDef(const CDialogueLayerDef&);virtual ~CDialogueLayerDef();CDialogueLayerDef&operator=(const CDialogueLayerDef&);};
}
namespace std {
struct __false_type{__false_type(){}};
template<class Tag> __declspec(noinline) void __fastcall _Destroy(NSpeechGainManager::CDialogueLayerDef*a,NSpeechGainManager::CDialogueLayerDef*b,const Tag&){while(a!=b){a->~CDialogueLayerDef();++a;}}
template<class T>__forceinline T*_Copy(T*first,T*last,T*out){for(int n=last-first;n>0;--n,++first,++out)*out=*first;return out;}
template<class T>__forceinline void _Construct(T*p,const T&v){if(p)new(p)T(v);}
template<class T>__forceinline void _UninitializedCopy(T*out,T*first,T*last){while(first!=last){_Construct(out,*first++);++out;}}
template<class T>struct vector{T*_M_start,*_M_finish,*_M_end_of_storage;
 T*_M_allocate_and_copy(unsigned,T*,T*);void _M_clear();vector&operator=(const vector&);
};
template<class T>vector<T>&vector<T>::operator=(const vector&rhs){
 if(&rhs!=this){unsigned count=rhs._M_finish-rhs._M_start;
  if(count>unsigned(_M_end_of_storage-_M_start)){T*fresh=_M_allocate_and_copy(count,rhs._M_start,rhs._M_finish);_M_clear();_M_start=fresh;_M_end_of_storage=fresh+count;}
  else if(unsigned(_M_finish-_M_start)>=count){_Destroy(_Copy(rhs._M_start,rhs._M_finish,_M_start),_M_finish,__false_type());}
  else{_Copy(rhs._M_start,rhs._M_start+(_M_finish-_M_start),_M_start);_UninitializedCopy(_M_finish,rhs._M_start+(_M_finish-_M_start),rhs._M_finish);}
  _M_finish=_M_start+count;
 }return *this;
}

template struct vector<NSpeechGainManager::CDialogueLayerDef>;
}