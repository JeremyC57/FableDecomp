// CPersistContext::Transfer<CFaction>(const char* name, CFaction& value) -- the two-argument member template: transfer value under name with the type's default (GetDefaultVal<CFaction>()) through the three-argument Transfer
// Leaf: ??$Transfer@VCFaction@@@CPersistContext@@QAEXPBDAAVCFaction@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BAED0, length 104 bytes (+ int3 padding), `ret 8`.
// Callees: CFaction::CFaction() @ 0x006BA420 / CFaction::~CFaction() @ 0x006BA6D0, CCharString::CCharString(const CCharString&) @
//   0x0099EC30, set<CCharString>::set(const set&) @ 0x00514400 (x2: AlliedFactions, EnemyFactions), the three-argument
//   CPersistContext::Transfer<CFaction>(name, value, default) @ 0x006BABD0.
// Callers: CFactionManager::SaveGameState 0x006B9560 (NF55: binds this spelling) / LoadGameState 0x006B9610.
// Name fix (NF59, queue q10504; NF55): Ego_r.pdb 0x006F6910 + FableWin.pdb 0x02446A80 ??$Transfer@VCFaction@@@CPersistContext@@QAEXPBDAAVCFaction@@@Z
//   (three-argument twin Ego_r 0x006F6700). The value record {CCharString, two STLport string sets} is CFaction (engine/CFaction.h:
//   Name, AlliedFactions, EnemyFactions, sizeof 0x1C; destructor 0x006BA6D0 = FableWin 0x02444350). The local CCharString /
//   CStringMap_00514400 / CNamedMapPair_006ba420 / CPersistContext / CPersistDefault models are replaced by the shared headers:
//   engine/CPersistContext.h FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS (`Transfer(name, value, GetDefaultVal<T>())`) +
//   _GETDEFAULTVAL_BODY (the traits local's DefaultVal returned by value: VC7.1 copies it, no NRVO), engine/CPersistTraits.h
//   FABLE_RETAIL_CPERSISTTRAITS (value-initialising traits constructor -> the out-of-line CFaction() 0x006BA420: NF59 overlay opt-in
//   FABLE_RETAIL_CFACTION_CTOR_OUTOFLINE, Ego_r.pdb ??0CFaction@@QAE@XZ 0x006F64E0), stlport/fable_map.h FABLE_RETAIL_STLP_SET_COPY (the
//   out-of-line set copy 0x00514400) and the implicit CFaction copy constructor.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_SET_COPY
#define FABLE_RETAIL_CFACTION_CTOR_OUTOFLINE
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS
#define FABLE_RETAIL_CPERSISTCONTEXT_GETDEFAULTVAL_BODY
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_CPERSISTTRAITS_EMPTY_DTOR
#include "fable_compiler.h"
#include "rebuild_abi.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/CFaction.h"
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#include "engine/CDataOutputStream.h"
#include "engine/CPersistContext.h"
#include "engine/CPersistTraits.h"

template void CPersistContext::Transfer<CFaction>(const char* name, CFaction& value);
