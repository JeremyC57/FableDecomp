#ifndef FABLE_CSALRENDER_H
#define FABLE_CSALRENDER_H
#include "engine/CCodaObjectBase.h"
// GameCODA 2.10 3D voice renderer (hand4c w24, row 0x00C54C10). Class, method names, access and virtual slots: the SDK
// CODAApiDbg.lib SalSw3DVoice object's CodeView (LF_ONEMETHOD VFTableOffset): slot 0 the deleting destructor, +0x04
// ClearFilterHistory, +0x08 Process, +0x0c SetGain, +0x10 SetPosition (native 0x00C54E1F `call dword ptr [eax+10h]` on
// CSalSw3DVoice::m_pRender). Instance members (SDK: CSalMutex m_mutexUpdate +0x04, float m_fGain, bool m_bUpdate) are not
// projected: no landed row reads them yet.
class CSalRender:public CCodaObjectBase {
public:
 virtual ~CSalRender();
 void Initialize();
 virtual void ClearFilterHistory();
 virtual void Process(const short*,int*(&)[4])=0;
 virtual void SetGain(float,const float*,bool)=0;
 virtual void SetPosition(float,float,float,float,const float*,bool,bool)=0;
};
#endif
