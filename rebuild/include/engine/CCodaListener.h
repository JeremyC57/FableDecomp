#ifndef FABLE_CCODALISTENER_H
#define FABLE_CCODALISTENER_H
#include "engine/CCodaVector.h"
// Layout: SDK original five vectors and update word; native base12 gives76 bytes.
class CCodaListener:public CCodaObject {
public:
 virtual ~CCodaListener();
#ifdef FABLE_RETAIL_PASS22_CCODACONTEXT_CREATE
 CCodaListener();
#endif
 void Commit();
 static unsigned int GetCommit(){return m_ffCommit;}
 static void ClearCommit(){m_ffCommit=0;}
#ifdef FABLE_RETAIL_CCODA3DSOURCE_COMMIT
 // Retail opt-in (hand3c, row 0x00C41FB0 CCoda3DSource::Commit): SDK 2.10 CodeView public accessors ?GetPosition@ / ?GetVelocity@ /
 // ?GetFront@ / ?GetTop@ / ?GetRight@CCodaListener@@QBEABVCCodaVector@@XZ (Dbg COMDATs return the member's address).
 const CCodaVector& GetPosition()const{return m_vPosition;}
 const CCodaVector& GetVelocity()const{return m_vVelocity;}
 const CCodaVector& GetFront()const{return m_vFront;}
 const CCodaVector& GetTop()const{return m_vTop;}
 const CCodaVector& GetRight()const{return m_vRight;}
#endif
private:
 static unsigned int m_ffCommit;
 CCodaVector m_vPosition;
 CCodaVector m_vVelocity;
 CCodaVector m_vFront;
 CCodaVector m_vTop;
 CCodaVector m_vRight;
 unsigned int m_ffUpdate;
};
#endif
