#include <stdio.h>
#include <stddef.h>
class CBaseClass{public:CBaseClass();virtual ~CBaseClass();};class CInitBaseClass:public CBaseClass{public:bool Valid;CInitBaseClass();virtual ~CInitBaseClass();virtual void Validate()const;};
class CScratchBuffer:public CInitBaseClass{public:unsigned char* Buffer;bool Locked;long NoBytes;CScratchBuffer(){}};static int constructed;CBaseClass::CBaseClass(){}CBaseClass::~CBaseClass(){}CInitBaseClass::CInitBaseClass(){Valid=false;++constructed;}CInitBaseClass::~CInitBaseClass(){}void CInitBaseClass::Validate()const{}
CScratchBuffer CUnprovenGlobal_013CB498;
int main(){bool ok=constructed==1;ok=ok&&!CUnprovenGlobal_013CB498.Valid;puts(ok?"01224270_TEST PASS":"01224270_TEST FAIL");return ok?0:1;}