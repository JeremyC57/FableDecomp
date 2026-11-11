#ifndef FABLE_CCODA3DSOURCE_H
#define FABLE_CCODA3DSOURCE_H
#include "engine/CCodaSource.h"
#include "engine/CodaProcessingTypes.h"
class CCodaObjectList;
// Layout: SDK source256->native252; native final commit word472, complete size476.
class CCoda3DSource:public CCodaSource {
public:
#ifdef FABLE_RETAIL_CCODACONTEXT_CREATE3D
 // SDK empty derived constructor calls the complete source constructor and installs the derived table.
 CCoda3DSource() {}
 virtual bool Create(unsigned int);
#endif
 virtual ~CCoda3DSource();
 virtual bool Is3D()const;
 void Commit(const CCodaObjectList&,const CCodaObjectList&,unsigned int);
private:
 CODA3DSOURCE m_3D;
 CCodaVector m_vPosition;
 CCodaVector m_vVelocity;
 CCodaVector m_vConeDirection;
 float m_fMaxDistance;
 float m_fMinDistance;
 float m_fRefDistance;
 float m_fRolloff;
 float m_fDoppler;
 unsigned int m_uiConeInner;
 unsigned int m_uiConeOuter;
 float m_fConeOuterGain;
 int m_iDirect;
 int m_iDirectHF;
 _C3DSOURCEENVFX m_EnvFx;
 CHRTFMODE m_HrtfMode;
 float (FABLE_STDCALL *m_pfnDistanceCallback)(void * ,void * ,float ,unsigned int );
 unsigned int m_DistanceCallbackData;
 unsigned int m_ffCommit;
};
inline CCoda3DSource* CCodaSource::Get3D()const{return Is3D()?static_cast<CCoda3DSource*>(const_cast<CCodaSource*>(this)):0;}
#endif
