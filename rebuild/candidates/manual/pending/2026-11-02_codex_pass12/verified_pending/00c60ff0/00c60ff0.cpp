// int CEAXBuffer::Set3DBuffer(unsigned long,void*,unsigned long) -- update immediate/deferred DirectSound 3D source properties.
// Leaf: ?Set3DBuffer@CEAXBuffer@@AAEHKPAXK@Z
#include "engine/CEAXBuffer.h"
bool IsNaN(const D3DVECTOR&);
int CEAXBuffer::Set3DBuffer(unsigned long property,void* value,unsigned long deferred)
{
 long result=0;
 switch(property){
 case 1:{
  const DS3DBUFFER* parameters=(const DS3DBUFFER*)value;
  if(IsNaN((const D3DVECTOR&)parameters->vPosition)||IsNaN((const D3DVECTOR&)parameters->vVelocity)){result=-3;break;}
  if(deferred!=1){
   m_Buffer.vPosition=parameters->vPosition;
   m_Buffer.vVelocity=parameters->vVelocity;
   m_Buffer.dwInsideConeAngle=parameters->dwInsideConeAngle;
   m_Buffer.dwOutsideConeAngle=parameters->dwOutsideConeAngle;
   m_Buffer.vConeOrientation=parameters->vConeOrientation;
   m_Buffer.lConeOutsideVolume=parameters->lConeOutsideVolume;
   m_Buffer.flMinDistance=parameters->flMinDistance;
   m_Buffer.flMaxDistance=parameters->flMaxDistance;
   m_Buffer.dwMode=parameters->dwMode;
   m_dwFlags|=0x754;
  }else m_dwFlags_Defer|=0x754;
  m_Buffer_Defer.vPosition=parameters->vPosition;
  m_Buffer_Defer.vVelocity=parameters->vVelocity;
  m_Buffer_Defer.dwInsideConeAngle=parameters->dwInsideConeAngle;
  m_Buffer_Defer.dwOutsideConeAngle=parameters->dwOutsideConeAngle;
  m_Buffer_Defer.vConeOrientation=parameters->vConeOrientation;
  m_Buffer_Defer.lConeOutsideVolume=parameters->lConeOutsideVolume;
  m_Buffer_Defer.flMinDistance=parameters->flMinDistance;
  m_Buffer_Defer.flMaxDistance=parameters->flMaxDistance;
  m_Buffer_Defer.dwMode=parameters->dwMode;
  long volume=m_Buffer_Defer.lConeOutsideVolume;
  m_lpEAXListener->AddToChangesList(BUFFERUPDATE,0x20,&volume,m_BufferID);
  break;
 }
 case 2:{
  const D3DVECTOR* vector=(const D3DVECTOR*)value;
  if(IsNaN((const D3DVECTOR&)*vector)){result=-3;break;}
  if(deferred!=1){m_Buffer.vPosition=*vector;m_dwFlags|=0x4;}
  else m_dwFlags_Defer|=0x4;
  m_Buffer_Defer.vPosition=*vector;
  break;
 }
 case 4:{
  const D3DVECTOR* vector=(const D3DVECTOR*)value;
  if(IsNaN((const D3DVECTOR&)*vector)){result=-3;break;}
  if(deferred!=1){m_Buffer.vVelocity=*vector;m_dwFlags|=0x10;}
  else m_dwFlags_Defer|=0x10;
  m_Buffer_Defer.vVelocity=*vector;
  break;
 }
 case 8:
  if(deferred!=1){m_Buffer.dwInsideConeAngle=((const unsigned long*)value)[0];m_Buffer.dwOutsideConeAngle=((const unsigned long*)value)[1];m_dwFlags|=0x400;}
  else m_dwFlags_Defer|=0x400;
  m_Buffer_Defer.dwInsideConeAngle=((const unsigned long*)value)[0];m_Buffer_Defer.dwOutsideConeAngle=((const unsigned long*)value)[1];
  break;
 case 16:
  if(deferred!=1){m_Buffer.vConeOrientation=*(const D3DVECTOR*)value;m_dwFlags|=0x40;}
  else m_dwFlags_Defer|=0x40;
  m_Buffer_Defer.vConeOrientation=*(const D3DVECTOR*)value;
  break;
 case 32:{
  long volume=*(const long*)value;
  if(m_Buffer_Defer.lConeOutsideVolume==volume)break;
  m_Buffer_Defer.lConeOutsideVolume=volume;
  if(deferred!=1){m_Buffer.lConeOutsideVolume=volume;m_dwFlags|=0x400;}
  else m_dwFlags_Defer|=0x400;
  break;
 }
 case 64:
  if(deferred!=1){m_Buffer.flMinDistance=*(const float*)value;m_dwFlags|=0x104;}
  else m_dwFlags_Defer|=0x104;
  m_Buffer_Defer.flMinDistance=*(const float*)value;
  break;
 case 128:
  if(deferred!=1){m_Buffer.flMaxDistance=*(const float*)value;m_dwFlags|=0x4;}
  else m_dwFlags_Defer|=0x4;
  m_Buffer_Defer.flMaxDistance=*(const float*)value;
  break;
 case 256:
  if(deferred!=1){m_Buffer.dwMode=*(const unsigned long*)value;m_dwFlags|=0x754;}
  else m_dwFlags_Defer|=0x754;
  m_Buffer_Defer.dwMode=*(const unsigned long*)value;
  break;
 default:result=-3;break;
 }
 if(deferred==1)m_dwFlags|=1;
 else m_dwFlags&=~1;
 if(result>=0){
  if(m_dwFlags&1)return result;
  return CommitEAX3();
 }
 return result;
}
