#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
// BEGIN exact source snapshot SHA256 2946d8f74a6cc1f2f5c42d052be675826ea36bce5bef2c91076a58ff0e9f7a28
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
struct CCharString {
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

// END exact source snapshot
StringData nullHeader={-1,0,0};StringData* NullString_0139c8fc=&nullHeader;void (__fastcall *AssertHook_0143bb2c)(const char*)=0;
void ReleaseData_00c0b9c0(StringData* d){if(--d->refs==0)free(d);}
void CCharString::Release(){ReleaseData_00c0b9c0(GetData());}
void CCharString::Alloc(int n){StringData* d=(StringData*)malloc(sizeof(StringData)+n+1);d->refs=1;d->length=n;d->capacity=n;data=(char*)(d+1);data[n]=0;}
void CCharString::AssignCopy(int n,const char* s){char* copy=(char*)malloc(n);memcpy(copy,s,n);Release();Alloc(n);memcpy(data,copy,n);free(copy);}
void CCharString::CopyBeforeWrite(){if(GetData()->refs>1){StringData* old=GetData();char* s=data;Alloc(old->length);memcpy(data,s,old->length+1);ReleaseData_00c0b9c0(old);}}
StringData* CCharString::GetDataOutOfLine(){return GetData();}
void CCharString::ConcatCopy(int oldn,const char* old,int n,const char* s){Alloc(oldn+n);memcpy(data,old,oldn);memcpy(data+oldn,s,n);}
int main(){CCharString a,b;a.Alloc(2);memcpy(a.data,"ab",3);b.Alloc(1);memcpy(b.data,"x",2);b.Assign(a);assert(b.data==a.data && a.GetData()->refs==2);b.Assign(b);assert(a.GetData()->refs==2);b.Release();a.Release(); puts("00c0c0c0_TEST PASS");return 0;}