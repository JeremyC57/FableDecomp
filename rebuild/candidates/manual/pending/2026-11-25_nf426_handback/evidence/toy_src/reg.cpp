#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
#include "engine/CDefPointer.h"
class CDefClassBase;
int Plain1(int a) { return a*3; }
int Plain2(int a) { return a+7; }
bool GetDefRaw(const char*, CDefPointer<const CDefClassBase>&);
template<class T> bool GetSubDef(const char* n, CDefPointer<const T>* out) { CDefPointer<const CDefClassBase> b; if(!GetDefRaw(n,b)) return false; out->SetDynamicCast(b); return true; }
struct CBase { virtual ~CBase() {} virtual int Size() const = 0; };
class CTargetingDef;
struct CUser0 : CBase { CDefPointer<const CTargetingDef> P; int Size() const { return 0; } void Init(const char* n) { GetSubDef(n,&P); } };
class CActionUseDef;
struct CUser1 : CBase { CDefPointer<const CActionUseDef> P; int Size() const { return 1; } void Init(const char* n) { GetSubDef(n,&P); } };
class CReadableDef;
struct CUser2 : CBase { CDefPointer<const CReadableDef> P; int Size() const { return 2; } void Init(const char* n) { GetSubDef(n,&P); } };
class CInventoryItemDef;
struct CUser3 : CBase { CDefPointer<const CInventoryItemDef> P; int Size() const { return 3; } void Init(const char* n) { GetSubDef(n,&P); } };
CBase* Alloc0(const char* n) { CUser0* u = new CUser0; u->Init(n); return u; }
CBase* Alloc1(const char* n) { CUser1* u = new CUser1; u->Init(n); return u; }
CBase* Alloc2(const char* n) { CUser2* u = new CUser2; u->Init(n); return u; }
CBase* Alloc3(const char* n) { CUser3* u = new CUser3; u->Init(n); return u; }
int Plain3(int a) { return a-1; }
