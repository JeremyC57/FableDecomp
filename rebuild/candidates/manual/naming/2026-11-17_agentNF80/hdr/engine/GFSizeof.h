#ifndef FABLE_ENGINE_GFSIZEOF_H
#define FABLE_ENGINE_GFSIZEOF_H
#include "rebuild_abi.h"
// Original FableWin02d96bf0/02d96c10/02d96a60; every helper is expanded in009fe460.
template<class T> inline bool FABLE_FASTCALL GFPIsNull(T* p) {return p==0;}
template<class T> inline unsigned long FABLE_FASTCALL GFSizeof(T& t) {return t.Sizeof();}
#ifdef FABLE_RETAIL_GFPISNULL_CCOUNTEDPOINTER
// Retail opt-in (NF80, q10931: row 0x009C8060): FableWin.pdb ??$GFPIsNull@VCBankFileAsyncData@@@@YI_NABV?$CCountedPointer@VCBankFileAsyncData@@@@@Z
// (0x02F05C60), the counted-pointer overload GFPSizeof uses (with ??$GFPSizeof@$$CBV?$CCountedPointer@VCBankFileAsyncData@@@@@@YIK..
// 0x02F05990); expanded in retail (0x009C809C..0x009C80AD: the Data word tested, `call Sizeof ; add eax,8` or 8). Needs
// engine/CCountedPointer.h for the instantiation.
template<class T> class CCountedPointer;
template<class T> inline bool FABLE_FASTCALL GFPIsNull(const CCountedPointer<T>& p) {return p.IsNull();}
#endif
template<class T> inline unsigned long FABLE_FASTCALL GFPSizeof(const T& p) {
 return !GFPIsNull(p)?GFSizeof(*p)+sizeof(p):sizeof(p);
}
#endif
