// void CEAXListener::AddToChangesList(IDENTIFIER,unsigned long,void*,long) -- append a typed EAX or source-property notification.
// Leaf: ?AddToChangesList@CEAXListener@@QAEXW4IDENTIFIER@@KPAXJ@Z
#include "engine/CEAXListener.h"
#include "engine/CodaMemory.h"
void CEAXListener::AddToChangesList(IDENTIFIER group,unsigned long property,void* value,long sourceID)
{
 _CHANGES* event=(_CHANGES*)CodaMalloc(sizeof(_CHANGES));
 event->id=group;
 event->property=property;
 switch(group){
 case 4:
  switch(property){
  case 4:event->Listener.flDistanceFactor=*(const float*)value;break;
  case 5:event->Listener.flRolloffFactor=*(const float*)value;break;
  }
  break;
 case 5:
  event->Buffer.BufferID=sourceID;
  switch(property){
  case 0x20:event->Buffer.lConeOutsideVolume=*(const long*)value;break;
  case 0x200:event->Buffer.lVolume=*(const unsigned long*)value;break;
  }
  break;
 case 0:
  switch(property){
  case 1:event->eax30LP=*(const EAX30LISTENERPROPERTIES*)value;break;
  case 2:event->eax30LP.ulEnvironment=*(const unsigned long*)value;break;
  case 3:event->eax30LP.flEnvironmentSize=*(const float*)value;break;
  case 4:event->eax30LP.flEnvironmentDiffusion=*(const float*)value;break;
  case 5:event->eax30LP.lRoom=*(const long*)value;break;
  case 6:event->eax30LP.lRoomHF=*(const long*)value;break;
  case 7:event->eax30LP.lRoomLF=*(const long*)value;break;
  case 8:event->eax30LP.flDecayTime=*(const float*)value;break;
  case 9:event->eax30LP.flDecayHFRatio=*(const float*)value;break;
  case 10:event->eax30LP.flDecayLFRatio=*(const float*)value;break;
  case 11:event->eax30LP.lReflections=*(const long*)value;break;
  case 12:event->eax30LP.flReflectionsDelay=*(const float*)value;break;
  case 13:event->eax30LP.vReflectionsPan=*(const EAXVECTOR*)value;break;
  case 14:event->eax30LP.lReverb=*(const long*)value;break;
  case 15:event->eax30LP.flReverbDelay=*(const float*)value;break;
  case 16:event->eax30LP.vReverbPan=*(const EAXVECTOR*)value;break;
  case 17:event->eax30LP.flEchoTime=*(const float*)value;break;
  case 18:event->eax30LP.flEchoDepth=*(const float*)value;break;
  case 19:event->eax30LP.flModulationTime=*(const float*)value;break;
  case 20:event->eax30LP.flModulationDepth=*(const float*)value;break;
  case 21:event->eax30LP.flAirAbsorptionHF=*(const float*)value;break;
  case 22:event->eax30LP.flHFReference=*(const float*)value;break;
  case 23:event->eax30LP.flLFReference=*(const float*)value;break;
  case 24:event->eax30LP.flRoomRolloffFactor=*(const float*)value;break;
  case 25:event->eax30LP.ulFlags=*(const unsigned long*)value;break;
  }
  break;
 case 1:
  event->Buffer.BufferID=sourceID;
  switch(property){
  case 1:event->Buffer.eax30BP=*(const EAX30BUFFERPROPERTIES*)value;break;
  case 2:*(EAXOBSTRUCTIONPROPERTIES*)&event->Buffer.eax30BP.lObstruction=*(const EAXOBSTRUCTIONPROPERTIES*)value;break;
  case 3:*(EAXOCCLUSIONPROPERTIES*)&event->Buffer.eax30BP.lOcclusion=*(const EAXOCCLUSIONPROPERTIES*)value;break;
  case 4:*(EAXEXCLUSIONPROPERTIES*)&event->Buffer.eax30BP.lExclusion=*(const EAXEXCLUSIONPROPERTIES*)value;break;
  case 5:event->Buffer.eax30BP.lDirect=*(const long*)value;break;
  case 6:event->Buffer.eax30BP.lDirectHF=*(const long*)value;break;
  case 7:event->Buffer.eax30BP.lRoom=*(const long*)value;break;
  case 8:event->Buffer.eax30BP.lRoomHF=*(const long*)value;break;
  case 9:event->Buffer.eax30BP.lObstruction=*(const long*)value;break;
  case 10:event->Buffer.eax30BP.flObstructionLFRatio=*(const float*)value;break;
  case 11:event->Buffer.eax30BP.lOcclusion=*(const long*)value;break;
  case 12:event->Buffer.eax30BP.flOcclusionLFRatio=*(const float*)value;break;
  case 13:event->Buffer.eax30BP.flOcclusionRoomRatio=*(const float*)value;break;
  case 14:event->Buffer.eax30BP.flOcclusionDirectRatio=*(const float*)value;break;
  case 15:event->Buffer.eax30BP.lExclusion=*(const long*)value;break;
  case 16:event->Buffer.eax30BP.flExclusionLFRatio=*(const float*)value;break;
  case 17:event->Buffer.eax30BP.lOutsideVolumeHF=*(const long*)value;break;
  case 18:event->Buffer.eax30BP.flDopplerFactor=*(const float*)value;break;
  case 19:event->Buffer.eax30BP.flRolloffFactor=*(const float*)value;break;
  case 20:event->Buffer.eax30BP.flRoomRolloffFactor=*(const float*)value;break;
  case 21:event->Buffer.eax30BP.flAirAbsorptionFactor=*(const float*)value;break;
  case 22:event->Buffer.eax30BP.ulFlags=*(const unsigned long*)value;break;
  }
  break;
 }
 event->next=0;
 _CHANGES* last=lpChanges;
 if(!last)lpChanges=event;
 else{
  while(last->next)last=last->next;
  last->next=event;
 }

}