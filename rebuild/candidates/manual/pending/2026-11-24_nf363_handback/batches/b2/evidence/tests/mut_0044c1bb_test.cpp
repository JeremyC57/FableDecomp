#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEF_CLASS_BASE_CTORS                  // test only: the inline CDefClassBase / CParentDefClassBase constructors,
#define FABLE_RETAIL_CDEFCLASSBASE_CTOR_SUBDEF_FRIEND      // so the controlled CSubDefClassBase() stub below can construct its base
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTHINGBASEDEF_COMPONENTS
#define FABLE_RETAIL_CTHINGBASEDEF_CTOR_DECL
#define FABLE_RETAIL_CTCNOISEDEF_IMPLICIT_DTOR
#define FABLE_RETAIL_CTCNOISEDEF_CTOR_INLINE
#define FABLE_RETAIL_CTHINGNOISEDEF_VIRTUAL_MODEL
#include "engine/CDefPointeeBase.h"
#include "engine/CThingNoiseDef.h"
#include "engine/CDefString.h"

// Controlled external operations: the out-of-line constructors and destructors the class chain calls record what ran (and on which
// object); the slot-20 GetSizeofClass overrides report the dynamic type, so a vftable store is checked through virtual dispatch; every
// other virtual of the chain is a boundary the test must never reach.
static char g_log[64];
static int g_n = 0;
static void Log(char c) { if (g_n < 63) g_log[g_n++] = c; g_log[g_n] = 0; }
static void ResetLog() { g_n = 0; g_log[0] = 0; }
static const void* g_base_this = 0;
static const void* g_sub_this = 0;
static int g_deletes = 0;
static const void* g_deleted = 0;

void* operator new(size_t size) { return malloc(size); }
void operator delete(void* p) { ++g_deletes; g_deleted = p; free(p); }

CResource::CResource() { }
CResource::~CResource() { Log('R'); }
bool CResource::UnloadResource() { abort(); }
unsigned long CResource::GetUnloadableMemory() const { abort(); }
bool CResource::IsResourceLoaded() const { abort(); }
void CResource::OnResourceLoaded() { abort(); }
bool CResource::CanAbortResourceLoad(long) const { abort(); }
void CResource::OnResourceLoadAborted() { abort(); }
unsigned long CResource::GetDebugMemoryInUse() const { abort(); }
void CResource::ResetDebugMemoryInUse() { abort(); }
void CIVCountedPointeeBase::Delete() const { abort(); }
bool CDefClassBase::UnloadResource() { abort(); }
CDefString CDefClassBase::GetInstantiationName() const { abort(); }
void CDefClassBase::SetInstantiationName(CDefString) { abort(); }
void CDefClassBase::AddSubDef(const CCharString&, long, long) { abort(); }
void CDefClassBase::RemoveSubDef(const CCharString&) { abort(); }
CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&) { abort(); }
const CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&) const { abort(); }
bool CDefClassBase::IsParentDef() const { abort(); }
void CDefClassBase::PostTransfer() { abort(); }
void CDefClassBase::InternalSaveBinary(CDataOutputStream&) { abort(); }
void CDefClassBase::InternalLoadBinary(CDataInputStream&) { abort(); }
void CDefClassBase::Validate(const CValidateDefinitionInitBase&, const CCharString&) const { abort(); }
CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> >* CDefClassBase::GetPSubDefInfoMap() { abort(); }
CParentDefClassBase::~CParentDefClassBase() { Log('P'); }
CDefString CParentDefClassBase::GetInstantiationName() const { abort(); }
void CParentDefClassBase::SetInstantiationName(CDefString) { abort(); }
void CParentDefClassBase::AddSubDef(const CCharString&, long, long) { abort(); }
void CParentDefClassBase::RemoveSubDef(const CCharString&) { abort(); }
CDefClassBase::CSubDefInfo* CParentDefClassBase::GetPSubDefInfo(const CCharString&) { abort(); }
const CDefClassBase::CSubDefInfo* CParentDefClassBase::GetPSubDefInfo(const CCharString&) const { abort(); }
bool CParentDefClassBase::IsParentDef() const { abort(); }
void CParentDefClassBase::InternalSaveBinary(CDataOutputStream&) { abort(); }
void CParentDefClassBase::InternalLoadBinary(CDataInputStream&) { abort(); }
CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> >* CParentDefClassBase::GetPSubDefInfoMap() { abort(); }
CThingComponentSet::~CThingComponentSet() { Log('C'); }
CThingBaseDef::CThingBaseDef() { Log('B'); g_base_this = this; }
CSubDefClassBase::CSubDefClassBase() { Log('S'); }
void CTCNoiseDef::Copy(const CDefClassBase*) { abort(); }
unsigned long CTCNoiseDef::GetSizeofClass() { return 0x34; }
void CTCNoiseDef::Transfer(CPersistContext&) { abort(); }
void CThingNoiseDef::Transfer(CPersistContext&) { abort(); }
void CThingNoiseDef::Copy(const CDefClassBase*) { abort(); }
unsigned long CThingNoiseDef::GetSizeofClass() { return 0xA4; }

// The def TU's own text (rows 0x0045C5F6 / 0x00455EF0 / 0x0044C1BB / 0x0045C5DA): the factory that instantiates the inline constructor.
CDefClassBase* CThingNoiseDef::Alloc()
{
    return new CThingNoiseDef;
}

int main()
{
    // CTCNoiseDef(): CSubDefClassBase() on the object (once), then this class's vftable (slot 20 reports CTCNoiseDef). Built alone and as
    // the Noise member of the def the factory allocates (+0x70).
    CTCNoiseDef* noise = new CTCNoiseDef;
    CDefClassBase* def = noise;
    if (noise == 0 || strcmp(g_log, "S") != 0 || g_sub_this != def || def->GetSizeofClass() != 0x34) {
        printf("0044c1bb_TEST FAIL (alone: %s)\n", g_log);
        return 1;
    }
    ResetLog();
    CThingNoiseDef* owner = static_cast<CThingNoiseDef*>(CThingNoiseDef::Alloc());
    CDefClassBase* member = &owner->Noise;
    if (strcmp(g_log, "BS") != 0 || g_sub_this != member || (const char*)member - (const char*)owner != 0x70 ||
        member->GetSizeofClass() != 0x34) {
        printf("0044c1bb_TEST FAIL (member: %s)\n", g_log);
        return 1;
    }
    printf("0044c1bb_TEST PASS\n");
    return 0;
}
