#ifndef FABLE_ENGINE_GFSIZEOF_H
#define FABLE_ENGINE_GFSIZEOF_H
#include "rebuild_abi.h"
// Original FableWin02d96bf0/02d96c10/02d96a60; every helper is expanded in009fe460.
template<class T> inline bool FABLE_FASTCALL GFPIsNull(T* p) {return p==0;}
template<class T> inline unsigned long FABLE_FASTCALL GFSizeof(T& t) {return t.Sizeof();}
template<class T> inline unsigned long FABLE_FASTCALL GFPSizeof(const T& p) {
 return !GFPIsNull(p)?GFSizeof(*p)+sizeof(p):sizeof(p);
}
#endif
