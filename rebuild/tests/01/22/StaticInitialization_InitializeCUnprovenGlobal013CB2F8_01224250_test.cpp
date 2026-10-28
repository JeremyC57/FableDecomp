#include <stdio.h>
#include <stddef.h>
class CBaseClass{public:CBaseClass();virtual ~CBaseClass();};class CInitBaseClass:public CBaseClass{public:bool Valid;CInitBaseClass();virtual ~CInitBaseClass();virtual void Validate()const;};
class CUnprovenCharEvent_013CB2F8{public:wchar_t Char;bool Pressed;};class CInkeyBuffer:public CInitBaseClass{public:CUnprovenCharEvent_013CB2F8 Buffer[100];unsigned long FirstChar,LastChar;CInkeyBuffer(){}};static int constructed;CBaseClass::CBaseClass(){}CBaseClass::~CBaseClass(){}CInitBaseClass::CInitBaseClass(){Valid=false;++constructed;}CInitBaseClass::~CInitBaseClass(){}void CInitBaseClass::Validate()const{}
CInkeyBuffer CUnprovenGlobal_013CB2F8;
int main(){bool ok=constructed==1;ok=ok&&!CUnprovenGlobal_013CB2F8.Valid;puts(ok?"01224250_TEST PASS":"01224250_TEST FAIL");return ok?0:1;}