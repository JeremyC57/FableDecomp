// 005D9B90 CTCInventoryItem::GetContextSensitiveType
// Leaf: ?GetContextSensitiveType@CTCInventoryItem@@SI?AW4EContextSensitiveType@@J@Z
// Flags:
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CONTEXTSENSITIVEITEM_QUERY_PASS28
#define FABLE_RETAIL_CCONTEXTSENSITIVEITEMDEF_CLASS
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_CCONTEXTSENSITIVEITEMDEF_SIZE_BODY
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CACHEDEF
#define FABLE_RETAIL_CTCINVENTORYITEM_PDEF
#define FABLE_RETAIL_INVENTORY_CONTEXT_CACHE_PASS30
#define FABLE_RETAIL_CSUBDEFCLASSBASE_CTOR_INLINE
#define FABLE_RETAIL_CDEFCLASSBASE_CTOR_SUBDEF_FRIEND
#define FABLE_RETAIL_CDEFPOINTER_RESET
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_CTOR
#define FABLE_RETAIL_DEF_CLASS_BASE_CTORS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "fable_string.h"
#include "engine/CContextSensitiveItemDef.h"
#include "engine/CThingBaseDef.h"
#include "engine/CTCInventoryItem.h"
// Test the caller using an instrumented lookup. The linked retail comparison exercises the production lookup.
template<> bool CDefClassBase::GetSubDef<CContextSensitiveItemDef>(CDefPointer<const CContextSensitiveItemDef>*) const;
#include "fable_assert.h"

EContextSensitiveType FABLE_FASTCALL CTCInventoryItem::GetContextSensitiveType(long def_index)
{
    if (CacheDef(def_index)) {
        FABLE_RETAIL_ASSERT(!MostRecentThingDef.IsNull());
        CDefPointer<const CContextSensitiveItemDef> pcontext_def;
        if (MostRecentThingDef->GetSubDef(&pcontext_def))
            return pcontext_def->Type;
    }
    return CONTEXT_SENSITIVE_NULL;
}

#include "engine/CDefString.h"
#include <stdlib.h>
#include <stdio.h>
CResource::~CResource() {}
void CResource::ResetDebugMemoryInUse() { abort(); }
unsigned long CResource::GetDebugMemoryInUse() const { abort(); return 0; }
void CResource::OnResourceLoadAborted() { abort(); }
bool CResource::CanAbortResourceLoad(long) const { abort(); return false; }
void CResource::OnResourceLoaded() { abort(); }
bool CResource::IsResourceLoaded() const { abort(); return false; }
unsigned long CResource::GetUnloadableMemory() const { abort(); return 0; }
bool CResource::UnloadResource() { abort(); return false; }
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
CParentDefClassBase::~CParentDefClassBase() {}


CResource::CResource() {}


static bool cached, found;
static long expectedIndex, calls, lookups, releases;
template<class T> class ConcreteDefinition : public T {
public:
    void Transfer(CPersistContext&) { abort(); }
    void Copy(const CDefClassBase*) { abort(); }
    unsigned long GetSizeofClass() { return sizeof(*this); }
    void Delete() const { ++releases; }
};
static ConcreteDefinition<CThingBaseDef>* expectedThing;
static ConcreteDefinition<CContextSensitiveItemDef>* item;
CDefPointer<const CThingBaseDef> CTCInventoryItem::MostRecentThingDef;

bool FABLE_FASTCALL CTCInventoryItem::CacheDef(long index) {
    ++calls;
    if (index!=expectedIndex) abort();
    MostRecentThingDef.Reset(cached ? expectedThing : 0);
    return cached;
}
template<> bool CDefClassBase::GetSubDef<CContextSensitiveItemDef>(
    CDefPointer<const CContextSensitiveItemDef>* out) const {
    ++lookups;
    if (this!=expectedThing || !out->IsNull()) abort();
    if (found) out->Reset(item);
    return found;
}
int main() {
    ConcreteDefinition<CThingBaseDef> thing;
    ConcreteDefinition<CContextSensitiveItemDef> context;
    expectedThing=&thing; item=&context;
    const long indices[]={-8,0,17};
    const EContextSensitiveType kinds[]={CONTEXT_SENSITIVE_NULL,CONTEXT_SENSITIVE_GUILD_SEAL,
        CONTEXT_SENSITIVE_OPINION_EXPRESSION,CONTEXT_SENSITIVE_TROPHY,CONTEXT_SENSITIVE_SCRIPT};
    for (int c=0;c<2;++c) for (int f=0;f<2;++f)
    for (int i=0;i<3;++i) for (int j=0;j<5;++j) {
        cached=c!=0; found=f!=0; calls=lookups=releases=0; expectedIndex=indices[i]; item->Type=kinds[j];
        const long initial=item->GetRefCount();
        EContextSensitiveType result=CTCInventoryItem::GetContextSensitiveType(expectedIndex);
        if (result!=(cached && found ? item->Type : CONTEXT_SENSITIVE_NULL) ||
            calls!=1 || lookups!=(cached?1:0) || releases!=0 || item->GetRefCount()!=initial) return 1;
    }
    // Clear the static handle before its stack-owned referent leaves scope.
    cached=false; CTCInventoryItem::GetContextSensitiveType(17);
    puts("005d9b90_TEST PASS"); return 0;
}
