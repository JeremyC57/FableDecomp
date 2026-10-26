// 00c0c0c0 CCharString::Assign
// Flags: /O2 /Oy
// Leaf: Assign
// Layout: retail object is one char pointer; header precedes it by 12 bytes with refs,length,capacity at -12,-8,-4. No shared CCharString header exists.
#include <string.h>
struct StringData {long refs;int length,capacity;};
extern StringData* NullString_0139c8fc;
extern void (__fastcall *AssertHook_0143bb2c)(const char*);
struct CDebugBreak {CDebugBreak();};
extern "C" __declspec(dllimport) void __stdcall DebugBreak();
#pragma auto_inline(off)
CDebugBreak::CDebugBreak(){DebugBreak();}
#pragma auto_inline(on)
static __forceinline void Check(bool ok,const char* text){if(!ok){if(AssertHook_0143bb2c)AssertHook_0143bb2c(text);CDebugBreak token;}}
class CCharString { public:
 char* data;
 __forceinline StringData* GetData()const{Check(data!=0,"m_pchData != NULL\n");return (StringData*)data-1;}
 StringData* GetDataOutOfLine();
 void Release(); void AssignCopy(int,const char*); void CopyBeforeWrite();void Alloc(int);void ConcatCopy(int,const char*,int,const char*);
 CCharString& Assign(const CCharString&);
};
void ReleaseData_00c0b9c0(StringData*);
extern "C" __declspec(dllimport) long __stdcall InterlockedIncrement(long*);
CCharString& CCharString::Assign(const CCharString& r){
 if(data!=r.data){
  if((GetData()->refs<0 && GetData()!=NullString_0139c8fc) || r.GetData()->refs<0) {AssignCopy(r.GetData()->length,r.data);}
  else {Release();Check(r.GetData()!=NullString_0139c8fc,"stringSrc.GetData() != _NullString\n");data=r.data;InterlockedIncrement(&GetData()->refs);}
 }
 return *this;
}