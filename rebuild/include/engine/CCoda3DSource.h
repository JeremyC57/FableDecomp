#ifndef FABLE_CCODA3DSOURCE_H
#define FABLE_CCODA3DSOURCE_H
#include "engine/CCodaSource.h"
#include "engine/CodaProcessingTypes.h"
class CCodaObjectList;
// Layout: SDK source256->native252; native final commit word472, complete size476.
class CCoda3DSource:public CCodaSource {
public:
#if defined(FABLE_RETAIL_PASS22_CCODACONTEXT_CREATE) && !defined(FABLE_RETAIL_CCODACONTEXT_CREATE3D)
 CCoda3DSource();
#endif
#ifdef FABLE_RETAIL_CCODACONTEXT_CREATE3D
 // SDK empty derived constructor calls the complete source constructor and installs the derived table.
 CCoda3DSource() {}
 virtual bool Create(unsigned int);
#endif
#if defined(FABLE_RETAIL_PASS22_CCODA3D_CREATE) && !defined(FABLE_RETAIL_CCODACONTEXT_CREATE3D)
 virtual bool Create(unsigned int);
#endif
 virtual ~CCoda3DSource();
 virtual bool Is3D()const;
 void Commit(const CCodaObjectList&,const CCodaObjectList&,unsigned int);
#ifdef FABLE_RETAIL_CSALSW3D_CALCPARAMS
 // Retail opt-in (hand4c w24, row 0x00C54C10 CSalSw3DVoice::CalcParams): SDK 2.10 CodeView public accessor
 // ?Get3DParams@CCoda3DSource@@QBEABUCODA3DSOURCE@@XZ (the CODAApiDbg CalcParams calls it); the release reads m_3D in place.
 const CODA3DSOURCE& Get3DParams()const{return m_3D;}
#endif
#ifdef FABLE_RETAIL_CCODA3DSOURCE_HRTFMODE
 // NF445 opt-in (row 0x00C556E0 CSalSw3DVoice::Create): SDK 2.10 CodeView public accessor CCoda3DSource::GetHrtfMode (returns
 // CHRTFMODE m_HrtfMode); the release reads it in place (0x00C55727 `mov eax,[eax+1CCh]`, retail offset of m_HrtfMode).
 CHRTFMODE GetHrtfMode()const{return m_HrtfMode;}
#endif
#ifdef FABLE_RETAIL_CCODA3DSOURCE_COMMIT
 // Retail opt-in (hand3c, row 0x00C41FB0 Commit): the SDK 2.10 private helpers Commit uses. IsHeadRelative / IsMaxDistanceMuted are the Dbg
 // COMDAT ?IsHeadRelative@ / ?IsMaxDistanceMuted@CCoda3DSource@@ABE_NXZ bodies (status bits 0x40000 / 0x80000); the five Calc* are out of line (CODAApi.lib Commit
 // relocations at the retail call sites): 0x00C418F0, 0x00C41990, 0x00C41A20, 0x00C41A70, 0x00C41BB0.
private:
 bool IsHeadRelative()const{return (m_ffStatus & 0x40000) != 0;}
 bool IsMaxDistanceMuted()const{return (m_ffStatus & 0x80000) != 0;}
 float CalcDirectionAndDistance(CCodaVector& vDirection, const CCodaVector& cvPosition);
 float CalcDistanceGain()const;
 float CalcDoppler()const;
 float CalcConeGain()const;
 float CalcEnvFxGain(float fRolloff)const;
#endif
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
