#ifndef FABLE_CCODALISTENER_H
#define FABLE_CCODALISTENER_H
#include "engine/CCodaVector.h"
// Layout: SDK original five vectors and update word; native base12 gives76 bytes.
class CCodaListener:public CCodaObject {
public:
 virtual ~CCodaListener();
 void Commit();
 static unsigned int GetCommit(){return m_ffCommit;}
 static void ClearCommit(){m_ffCommit=0;}
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
