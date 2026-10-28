#include <stdio.h>
#include <stdlib.h>
#define free TestFree
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#endif
extern "C" void __cdecl free(void*);
namespace std { template<class T> class allocator {}; template<class V,class T,class Alloc> class _STLP_alloc_proxy:public Alloc { public: V _M_data; }; template<class T,class Alloc=allocator<T> > class vector { public: T* _M_start; T* _M_finish; _STLP_alloc_proxy<T*,T,Alloc> _M_end_of_storage; ~vector() { if(_M_start) free(_M_start); } }; template<class T> void __fastcall _Destroy(T* p) { p->~T(); } }
class CThing; class CBaseIntelligentPointer { public: virtual ~CBaseIntelligentPointer(); CBaseIntelligentPointer(); void* PData; }; template<class T> class CIntelligentPointer:public CBaseIntelligentPointer {};
enum EHeroAbility {}; enum EButtonHeldState {};
class CWillButtonHelperTimer { public: long ButtonHeldDownTimer,WillAccumulatedLevel; CIntelligentPointer<CThing> PCreature; EHeroAbility MyAbility; EButtonHeldState CurrentState; std::vector<float> WillPowerUpAccumulatedTimings; float MaxNumberLevels,CurMaxLevels; bool WillIncrementedFlag,UpdateAccLevel,IsAICasting,IsAIHoldingDown; long LevelToCast; ~CWillButtonHelperTimer(); };
CWillButtonHelperTimer::~CWillButtonHelperTimer() {}
#undef free
static void* expectedBuffer; static void* expectedSub; static int calls,bad;
extern "C" void __cdecl TestFree(void* p) { if(p!=expectedBuffer || calls++) bad=1; free(p); }
CBaseIntelligentPointer::CBaseIntelligentPointer():PData(0) {}
CBaseIntelligentPointer::~CBaseIntelligentPointer() { if(this!=expectedSub || calls++!=1) bad=1; }
int main() { CWillButtonHelperTimer* object=new CWillButtonHelperTimer; object->WillPowerUpAccumulatedTimings._M_start=(float*)malloc(2*sizeof(float)); expectedBuffer=object->WillPowerUpAccumulatedTimings._M_start; expectedSub=&object->PCreature; object->~CWillButtonHelperTimer(); if(bad || calls!=2) goto fail;
 object=new CWillButtonHelperTimer; object->WillPowerUpAccumulatedTimings._M_start=0; expectedSub=&object->PCreature; calls=1; object->~CWillButtonHelperTimer(); if(bad || calls!=2) goto fail;
 puts("00792830_TEST PASS"); return 0; fail: puts("00792830_TEST FAIL"); return 1; }