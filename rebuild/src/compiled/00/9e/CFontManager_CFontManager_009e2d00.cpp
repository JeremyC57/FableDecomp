// CFontManager::~CFontManager() -- close bank, delete vertex buffers, and release four counted owners.
// Retail009e2d00 length296. No manager header; PDB four counted pointers at4/C/14/1C,
// native vector24 descriptor12 vs donor16. Actual bank/buffer headers retained.
// Counted record is refs0/free-fastcall4/data8, NOT intelligent Object0/refs4.
#include "fable_string.h"
#include "engine/CFontDataBank.h"
#include "engine/CVertexBufferWin32.h"
#include <new>
#include <stdlib.h>
#include <string.h>
struct CFontOwnerRecord {unsigned References;void(__fastcall* DestroyData)(void*);void* Data;};
template<class T>struct CFontCountedPointer {
 T* Data;CFontOwnerRecord* Info;
 void Release(){if(Info){--Info->References;if(!Info->References){Info->DestroyData(Info->Data);delete Info;}}}
 void Reset(){Release();Info=0;Data=0;}
 ~CFontCountedPointer(){Release();Data=0;Info=0;}
};
struct CFontBufferArray {
 CVertexBufferWin32** Begin;CVertexBufferWin32** End;CVertexBufferWin32** Capacity;
 unsigned Size()const{return unsigned(End-Begin);}
 ~CFontBufferArray(){if(Begin)free(Begin);} //00bfea14 is MSVCR71 free
};
// Native RTTI129BA38: CFontManager -> CBaseClassNonCopyable -> CBaseClass.
class CBaseClass { public:virtual ~CBaseClass();};
struct CBaseClassNonCopyable:CBaseClass {virtual ~CBaseClassNonCopyable();}; //0099A300
class CFontManager:public CBaseClassNonCopyable { public:
 CFontManager(const CCharString&,const CCharString&); //fablewin124255,030A2F20
 CFontCountedPointer<CFontDataBank> PFontDataBank;
 CFontCountedPointer<void> PStreamingFontDataBank,PSymbols,PStreamingSymbols;
 CFontBufferArray VertexBuffers;int CurrentVB;unsigned NumberAsyncGlyphsPending;bool LoadFontGlyphsAsynchronously;
 virtual ~CFontManager(); //VT0129ba38 scalar dtor009e3560
};
// Typed PMFs retain native thiscall and full host representation. Native PMF=4.
typedef void(CFontDataBank::*CFontCloseCall)();
typedef void*(CVertexBufferWin32::*CFontBufferDeleteCall)(unsigned);
#ifdef _MSC_VER
typedef char NativeClosePMF[sizeof(CFontCloseCall)==4?1:-1];
typedef char NativeDeletePMF[sizeof(CFontBufferDeleteCall)==4?1:-1];
#endif
CFontManager::~CFontManager(){
 if(PFontDataBank.Data){
  CFontDataBank* bank=PFontDataBank.Data;CFontCloseCall close=0;
  memcpy(&close,static_cast<const CFontCloseCall*>(bank->__vftable)+5,sizeof(close)); //VT129da84 slot5 A635C0 Close
  (bank->*close)();PFontDataBank.Reset();
 }
 for(unsigned i=0;i<VertexBuffers.Size();++i){
  CVertexBufferWin32* buffer=VertexBuffers.Begin[i];
  if(buffer){CFontBufferDeleteCall destroy=0;memcpy(&destroy,buffer->__vftable,sizeof(destroy));(buffer->*destroy)(1);} //VT129ba20 slot0 9E2890
 }
}