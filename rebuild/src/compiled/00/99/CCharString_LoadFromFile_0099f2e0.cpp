// 0099f2e0 CCharString::LoadFromFile
// Flags:
// Leaf: ?LoadFromFile@CCharString@@QAEXAAVCAFile@@PAVCASuspendableProcess@@J@Z
#include <new>
#define CCharString Pass9CharStringStorage
#define CCharStringData Pass9CharStringDataStorage
#define CAFile Pass9FileStaticDeclaration
#include "fable_filesystem.h"
#undef CAFile
#undef CCharStringData
#undef CCharString
#ifndef _MSC_VER
#define __forceinline inline
#define __cdecl
#endif
class CASuspendableProcess;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CBaseClassNonCopyable:public CBaseClass{protected:virtual~CBaseClassNonCopyable();};
enum FileAccessType{};
// Layout: actual RTTI CAFile primary CBaseClassNonCopyable4. Original virtual
// order from CDiskFileWin32 vtable0122d06c, slots1..9 match donor overrides.
class CAFile:public CBaseClassNonCopyable{public:
 virtual~CAFile();virtual void Open(const CWideString&,FileAccessType,unsigned long)=0;
 virtual void Close()=0;virtual void Read(void*,long,CASuspendableProcess*)=0;
 virtual void Write(const void*,long,CASuspendableProcess*)=0;
 virtual void SetPosition(long)=0;virtual void MovePosition(long)=0;
 virtual long GetPosition()const=0;virtual void SetLength(long)=0;virtual long GetLength()const=0;
};
// Layout: original CBasicString<char> is13 packed bytes and NoRefs is13.
#pragma pack(push,1)
template<class T>class CBasicString{public:
 T*PData;unsigned long StringLength;unsigned long DataLength:31;bool UseFastExtend:1;
 __forceinline CBasicString(unsigned long n,T c):PData(0),StringLength(0),DataLength(0),UseFastExtend(true){SetString(n,c);}
protected:void SetString(unsigned long,T);
};
class CCharStringData{public:CBasicString<char>Data;long NoRefs;
 __forceinline CCharStringData(long n,char c):Data(n,c),NoRefs(1){}
};
#pragma pack(pop)
class CCharString{public:CCharStringData*PStringData;
 void LoadFromFile(CAFile&,CASuspendableProcess*,long);
protected:void UnassignString();
};
void CCharString::LoadFromFile(CAFile&file,CASuspendableProcess*process,long length){
 if(PStringData)UnassignString();
 if(length==-1)length=file.GetLength()-file.GetPosition();
 PStringData=new CCharStringData(length,' ');
 if(PStringData&&length>0)file.Read(PStringData->Data.PData,length,process);
}