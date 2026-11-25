#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
#include "engine/CDefPointer.h"
class CDefClassBase; class CClockDef;
bool GetDefRaw(const char*, CDefPointer<const CDefClassBase>&) { return true; }
void CIVCountedPointeeBase::Delete() const {}
template<class T> CDefPointer<const T> GetSubDef(const char* n) { CDefPointer<const CDefClassBase> b; GetDefRaw(n,b); CDefPointer<const T> r; r.SetDynamicCast(b); return r; }
class CTCClock { public: CDefPointer<const CClockDef> PDef; int x; void Init(const char*); void Copy(const CTCClock&); ~CTCClock(); virtual int GetType() const { return 3; } };
void CTCClock::Init(const char* n) { PDef = GetSubDef<CClockDef>(n); }
void CTCClock::Copy(const CTCClock& o) { PDef = o.PDef; }
CTCClock::~CTCClock() { }
int main() { CTCClock c; c.Init("x"); return c.GetType(); }
