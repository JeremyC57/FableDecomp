// CEAXListener::GetEAX4FXSlot(unsigned long,unsigned long,void*,unsigned long,unsigned long*) -- Read EAX effect-slot and reverb properties.
// Leaf: ?GetEAX4FXSlot@CEAXListener@@QAEHKKPAXKPAK@Z
#include "engine/CEAXListener.h"
#include <string.h>
int CEAXListener::GetEAX4FXSlot(unsigned long slot,unsigned long property,void* data,unsigned long size,unsigned long* returned)
{
 long result=0;
 if(slot>=4) return -3;
 unsigned long parameter=property&0x7fffffff;
 if(parameter<0x10000) {
  if(memcmp(&m_Source.eax4FXSlot[slot].guidLoadEffect,&EAX_REVERB_EFFECT,sizeof(GUID))!=0) { m_lEAX4ErrorCode=-3; return -3; }
  switch(parameter) {
  case EAXREVERB_NONE: *returned=0; break;
  case 1:
   if(data && size >= 112) { *(EAXREVERBPROPERTIES*)data=m_Source.eax4LPSlot[slot]; *returned=112; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 2:
   if(data && size >= 4) { *(unsigned long*)data=m_Source.eax4LPSlot[slot].ulEnvironment; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 3:
   if(data && size >= 4) { *(float*)data=m_Source.eax4LPSlot[slot].flEnvironmentSize; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 4:
   if(data && size >= 4) { *(float*)data=m_Source.eax4LPSlot[slot].flEnvironmentDiffusion; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 5:
   if(data && size >= 4) { *(long*)data=m_Source.eax4LPSlot[slot].lRoom; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 6:
   if(data && size >= 4) { *(long*)data=m_Source.eax4LPSlot[slot].lRoomHF; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 7:
   if(data && size >= 4) { *(long*)data=m_Source.eax4LPSlot[slot].lRoomLF; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 8:
   if(data && size >= 4) { *(float*)data=m_Source.eax4LPSlot[slot].flDecayTime; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 9:
   if(data && size >= 4) { *(float*)data=m_Source.eax4LPSlot[slot].flDecayHFRatio; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 10:
   if(data && size >= 4) { *(float*)data=m_Source.eax4LPSlot[slot].flDecayLFRatio; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 11:
   if(data && size >= 4) { *(long*)data=m_Source.eax4LPSlot[slot].lReflections; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 12:
   if(data && size >= 4) { *(float*)data=m_Source.eax4LPSlot[slot].flReflectionsDelay; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 13:
   if(data && size >= 12) { *(EAXVECTOR*)data=m_Source.eax4LPSlot[slot].vReflectionsPan; *returned=12; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 14:
   if(data && size >= 4) { *(long*)data=m_Source.eax4LPSlot[slot].lReverb; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 15:
   if(data && size >= 4) { *(float*)data=m_Source.eax4LPSlot[slot].flReverbDelay; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 16:
   if(data && size >= 12) { *(EAXVECTOR*)data=m_Source.eax4LPSlot[slot].vReverbPan; *returned=12; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 17:
   if(data && size >= 4) { *(float*)data=m_Source.eax4LPSlot[slot].flEchoTime; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 18:
   if(data && size >= 4) { *(float*)data=m_Source.eax4LPSlot[slot].flEchoDepth; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 19:
   if(data && size >= 4) { *(float*)data=m_Source.eax4LPSlot[slot].flModulationTime; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 20:
   if(data && size >= 4) { *(float*)data=m_Source.eax4LPSlot[slot].flModulationDepth; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 21:
   if(data && size >= 4) { *(float*)data=m_Source.eax4LPSlot[slot].flAirAbsorptionHF; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 22:
   if(data && size >= 4) { *(float*)data=m_Source.eax4LPSlot[slot].flHFReference; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 23:
   if(data && size >= 4) { *(float*)data=m_Source.eax4LPSlot[slot].flLFReference; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 24:
   if(data && size >= 4) { *(float*)data=m_Source.eax4LPSlot[slot].flRoomRolloffFactor; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 25:
   if(data && size >= 4) { *(unsigned long*)data=m_Source.eax4LPSlot[slot].ulFlags; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  default: return -3;
  }
 } else {
  switch(property) {
  case EAXFXSLOT_NONE: *returned=0; break;
  case EAXFXSLOT_ALLPARAMETERS:
   if(data && size >= 28) { *(EAXFXSLOTPROPERTIES*)data=m_Source.eax4FXSlot[slot]; *returned=28; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case EAXFXSLOT_LOADEFFECT:
   if(data && size >= 16) { *(GUID*)data=m_Source.eax4FXSlot[slot].guidLoadEffect; *returned=16; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case EAXFXSLOT_VOLUME:
   if(data && size >= 4) { *(long*)data=m_Source.eax4FXSlot[slot].lVolume; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case EAXFXSLOT_LOCK:
   if(data && size >= 4) { *(long*)data=m_Source.eax4FXSlot[slot].lLock; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case EAXFXSLOT_FLAGS:
   if(data && size >= 4) { *(unsigned long*)data=m_Source.eax4FXSlot[slot].ulFlags; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  default: return -3;
  }
 }
 return result;
}
