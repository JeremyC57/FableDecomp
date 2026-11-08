// 008e3aa0 std::vector<CIcicleData>::_M_insert_overflow
// Flags: /O2 /Oy /W3 
// Leaf: _M_insert_overflow
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
#include "fable_compiler.h"
#include <stdlib.h>
#include <new>
namespace Icicles {
// Layout: retail vector has start/finish/storage at +0,+4,+8; 36-byte objects use the landed copy constructor 008e2fc0.
struct CBaseObjectPointer{void*object;unsigned refcount;};
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct CThing;struct C3DVector{float X,Y,Z;};
struct CIcicleData{CIntelligentPointer<CThing>PEffect,PTrap;int EffectTimer,TrapTimer;C3DVector Position;CIcicleData();CIcicleData(const CIcicleData&);};
struct __false_type {__false_type(){}};
CIcicleData* __fastcall uninitialized_copy(CIcicleData*,CIcicleData*,CIcicleData*,const __false_type&);
namespace std {
template<class T>FABLE_FORCEINLINE const T&max(const T&a,const T&b){return a<b?b:a;}
template<class T>FABLE_FORCEINLINE T*uninitialized_fill_n(T*out,unsigned n,const T&v){for(;n>0;--n,++out)if(out)new(out)T(v);return out;}
template<class T>struct vector {T*_M_start,*_M_finish,*_M_end_of_storage;
 void _M_clear();void _M_insert_overflow(T*,const T&,const __false_type&,unsigned,bool);
};
template<class T>void vector<T>::_M_insert_overflow(T*pos,const T&v,const __false_type&,unsigned n,bool atEnd){
 unsigned old=_M_finish-_M_start;unsigned capacity=old+max(old,n);
 T*buffer=capacity?(T*)malloc(capacity*sizeof(T)):0;
 T*out=buffer;
 for(T*p=_M_start;p!=pos;++p,++out)if(out)new(out)T(*p);
 if(n==1){if(out)new(out)T(v);++out;}
 else out=uninitialized_fill_n(out,n,v);
 if(!atEnd)out=uninitialized_copy(pos,_M_finish,out,__false_type());
 _M_clear();_M_start=buffer;_M_finish=out;_M_end_of_storage=buffer+capacity;
}
template struct vector<CIcicleData>;
}
}