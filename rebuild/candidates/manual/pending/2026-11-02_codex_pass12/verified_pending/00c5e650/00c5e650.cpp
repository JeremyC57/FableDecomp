// bool CSalOutputKS::CreateBuffer(unsigned int,unsigned int) -- configure kernel-streaming PCM pin, packets and notification thread.
// Leaf: ?CreateBuffer@CSalOutputKS@@UAE_NII@Z
// Flags: /GS
#include "engine/CSalOutputKS.h"
#include "engine/CodaMemory.h"
#include <string.h>
bool CSalOutputKS::CreateBuffer(unsigned int channels,unsigned int samples)
{
 HMODULE library=LoadLibraryA("ksuser.dll");
 if(!library)return false;
 typedef DWORD (__stdcall* PKSCREATEPIN)(HANDLE,KSPIN_CONNECT*,DWORD,HANDLE*);
 PKSCREATEPIN createPin=(PKSCREATEPIN)GetProcAddress(library,"KsCreatePin");
 if(!createPin){FreeLibrary(library);return false;}
 struct {KSPIN_CONNECT connect;KSDATAFORMAT data;WAVEFORMATEXTENSIBLE wave;} format;
 memset(&format,0,sizeof(format));
 format.connect.Interface.Set=__uuidof(KSINTERFACESETID_Standard);
 format.connect.Interface.Id=0;format.connect.Interface.Flags=0;
 format.connect.Medium.Set=__uuidof(KSMEDIUMSETID_Standard);
 format.connect.Medium.Id=0;format.connect.Medium.Flags=0;
 format.connect.PinId=0;format.connect.PinToHandle=0;
 format.connect.Priority.PriorityClass=0x40000000;
 format.connect.Priority.PrioritySubClass=1;
 format.data.FormatSize=sizeof(KSDATAFORMAT)+sizeof(WAVEFORMATEXTENSIBLE);
 format.data.Flags=0;format.data.Reserved=0;
 format.data.MajorFormat=__uuidof(KSDATAFORMAT_TYPE_AUDIO);
 format.data.SubFormat=__uuidof(KSDATAFORMAT_SUBTYPE_PCM);
 format.data.Specifier=__uuidof(KSDATAFORMAT_SPECIFIER_WAVEFORMATEX);
 format.wave.Format.wFormatTag=0xfffe;
 format.wave.Format.cbSize=22;
 format.wave.Format.nChannels=(WORD)channels;
 format.wave.Format.nSamplesPerSec=44100;
 format.wave.Format.wBitsPerSample=16;
 format.wave.Format.nBlockAlign=(WORD)(channels*2);
 format.wave.Format.nAvgBytesPerSec=44100*format.wave.Format.nBlockAlign;
 format.wave.Samples.wValidBitsPerSample=16;
 switch(channels){
 case 2:format.wave.dwChannelMask=3;break;
 case 4:format.wave.dwChannelMask=0x33;break;
 case 5:format.wave.dwChannelMask=0x37;break;
 case 6:format.wave.dwChannelMask=0x3f;break;
 default:format.wave.dwChannelMask=0x63f;break;
 }
 format.wave.SubFormat=__uuidof(KSDATAFORMAT_SUBTYPE_PCM);
 HANDLE pin;
 DWORD result=createPin(m_filter,&format.connect,0xc0000000,&pin);
 if(result){format.wave.Format.wFormatTag=1;format.wave.Format.cbSize=0;result=createPin(m_filter,&format.connect,0xc0000000,&pin);}
 FreeLibrary(library);
 if(result)return false;
 m_pin.Attach(pin);
 if(!m_eventTerminate.Create(0,false,false,0))return false;
 m_nFrames=samples/128;
 m_nFrameCount=0;m_nFrameIndex=0;
 unsigned int frameBytes=channels*256;
 m_pBuffer=(char*)CodaMalloc(m_nFrames*frameBytes);
 if(!m_pBuffer){m_eventTerminate.Close();m_pin.Close();return false;}
 m_pPacketHeaderTable=(KsPacketHeader*)CodaMalloc(m_nFrames*sizeof(KsPacketHeader));
 if(!m_pPacketHeaderTable){CodaFree(m_pBuffer);m_eventTerminate.Close();m_pin.Close();return false;}
 memset(m_pPacketHeaderTable,0,m_nFrames*sizeof(KsPacketHeader));
 m_pEventTable=(HANDLE*)CodaMalloc((m_nFrames+1)*sizeof(HANDLE));
 if(!m_pEventTable){CodaFree(m_pPacketHeaderTable);CodaFree(m_pBuffer);m_eventTerminate.Close();m_pin.Close();return false;}
 m_pEventTable[m_nFrames]=m_eventTerminate;
 for(unsigned int i=0;i<m_nFrames;++i){
  m_pPacketHeaderTable[i].header.Size=sizeof(KSSTREAM_HEADER);
  m_pPacketHeaderTable[i].header.FrameExtent=frameBytes;
  m_pPacketHeaderTable[i].header.DataUsed=frameBytes;
  m_pPacketHeaderTable[i].header.Data=m_pBuffer+i*frameBytes;
  m_pEventTable[i]=CreateEventA(0,false,false,0);
  m_pPacketHeaderTable[i].signal.hEvent=m_pEventTable[i];
 }
 if(!m_threadNotify.Create(NotifyProc,this,0,0,0))return false;
 m_threadNotify.SetPriority(2);
 if(!m_pin.SetConnectionState(KSSTATE_PAUSE))return false;
 if(!m_pin.SetConnectionState(KSSTATE_RUN))return false;
 return true;
}
