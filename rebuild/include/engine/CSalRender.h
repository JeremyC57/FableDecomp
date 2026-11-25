#ifndef FABLE_CSALRENDER_H
#define FABLE_CSALRENDER_H
#include "engine/CCodaObjectBase.h"
#ifdef FABLE_RETAIL_CSALRENDER_MEMBERS
// NF444 opt-in (rows 0x00C6E6A0 ??0CSalRender@@QAE@XZ, 0x00C6E630 ??1CSalRender@@UAE@XZ and the derived render classes): the instance members
// at retail offsets. SDK CODAApiDbg.lib CodeView names them CSalMutex m_mutexUpdate +0x04, float m_fGain, bool m_bUpdate (SDK 2.10 size 0x2C);
// retail's CSalMutex is 0x1C bytes (vtable 0x012B8FC8 + CRITICAL_SECTION, no m_bActive: the CSalOutputKS.h PASS23 layout), so m_fGain is at
// +0x20 (`fld dword ptr [ecx+20h]` in 0x00C6CEF0 / 0x00C6DE60 / 0x00C6EBA0) and m_bUpdate at +0x24 (0x00C6E6BF), retail size 0x28.
// Users define FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23 (that CSalMutex layout) and, where the member bodies expand,
// FABLE_RETAIL_CSALOUTPUTKS_INLINE_MEMBERS + FABLE_RETAIL_CCODAOBJECTBASE_INLINE_DTOR (the 0x00C5ECA0 / 0x00C5ED90 precedent).
#include "engine/CSalOutputKS.h"
#endif
// GameCODA 2.10 3D voice renderer (hand4c w24, row 0x00C54C10). Class, method names, access and virtual slots: the SDK
// CODAApiDbg.lib SalSw3DVoice object's CodeView (LF_ONEMETHOD VFTableOffset): slot 0 the deleting destructor, +0x04
// ClearFilterHistory, +0x08 Process, +0x0c SetGain, +0x10 SetPosition (native 0x00C54E1F `call dword ptr [eax+10h]` on
// CSalSw3DVoice::m_pRender). Instance members (SDK: CSalMutex m_mutexUpdate +0x04, float m_fGain, bool m_bUpdate) are not
// projected: no landed row reads them yet.
class CSalRender:public CCodaObjectBase {
public:
#ifdef FABLE_RETAIL_CSALRENDER_MEMBERS
 CSalRender();
#endif
 virtual ~CSalRender();
 void Initialize();
 virtual void ClearFilterHistory();
 virtual void Process(const short*,int*(&)[4])=0;
 virtual void SetGain(float,const float*,bool)=0;
 virtual void SetPosition(float,float,float,float,const float*,bool,bool)=0;
#ifdef FABLE_RETAIL_CSALRENDER_MEMBERS
protected:
 CSalMutex m_mutexUpdate;
 float m_fGain;
 bool m_bUpdate;
#endif
};
#ifdef FABLE_RETAIL_CSALRENDER_MEMBERS
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char FableCSalRenderRetailSize[sizeof(CSalRender)==0x28?1:-1];
#endif
#endif
#endif
