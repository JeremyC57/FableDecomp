// 00a48b50 CLipSyncPhonemeSample::WriteToFile
// Leaf: ?WriteToFile@CLipSyncPhonemeSample@@QBEXAAVCDataOutputStream@@@Z
// Flags:
#include <stddef.h>
#include "fable_string.h"
#define CDataOutputStream CDataOutputStreamFields
#include "engine/CDataOutputStream.h"
#undef CDataOutputStream
class CBaseClass{public:virtual~CBaseClass();};
class CBaseClassNonCopyable:public CBaseClass{protected:virtual~CBaseClassNonCopyable();};
class CDataStream:public CBaseClassNonCopyable{public:virtual void SetPosition(unsigned long)=0;virtual unsigned long GetPosition()const=0;virtual void Close()=0;};
class CDataOutputStream:public CDataStream{public:CCharString NewLine;virtual void Write(const void*,long)=0;void WriteULONG(unsigned long);};
// Layout: the callee's original state is unread here; its native 12-byte frequency array plus four floats occupy28 bytes.
class CSpeechFrame{unsigned char unread[28];public:void WriteToFile(CDataOutputStream&)const;};
class CCPPointerInfo;
// Layout: only the original pointer fields and proven const dereference API are exposed. No lifetime operation is performed by this row.
template<class T>class CCountedPointer{public:T*Data;CCPPointerInfo*Info;T*operator->()const{return Data;}};
class CLipSyncPhonemeSample{public:CCharString Symbol;unsigned long ID;CCountedPointer<CSpeechFrame>SampleFrame;void WriteToFile(CDataOutputStream&)const;};
void CLipSyncPhonemeSample::WriteToFile(CDataOutputStream&stream)const{stream.WriteULONG(ID);SampleFrame->WriteToFile(stream);}
FABLE_STATIC_ASSERT(sizeof(CDataOutputStream)==sizeof(CDataOutputStreamFields));
FABLE_STATIC_ASSERT(sizeof(CLipSyncPhonemeSample)==16);