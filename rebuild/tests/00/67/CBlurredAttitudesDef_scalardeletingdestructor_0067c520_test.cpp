#include <stdio.h>
#include <stdlib.h>
static unsigned baseCalls,bufferCalls,objectCalls;
static void* objectAddress;
extern "C" void __cdecl CountBufferFree(void* p) {
    if(p) ++bufferCalls;
    free(p);
}
void __cdecl operator delete(void* p) {
    if(p==objectAddress) ++objectCalls;
    free(p);
}
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_VECTOR_RESERVE
#define free CountBufferFree
// 0067c520 CBlurredAttitudesDef scalar deleting destructor
// The fixture exercises the destructor without invoking a resize of the definition vector.
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_VALUE_DECL_ONLY
// Leaf: ??_GCBlurredAttitudesDef@@UAEPAXI@Z
// Flags: (none)
#define FABLE_RETAIL_BLURRED_ATTITUDES_PASS29
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CPARENTDEFCLASSBASE
#define FABLE_PDCB_TYPED_MEMBERS
#define FABLE_RETAIL_DEF_CLASS_BASE_CTORS
#define FABLE_RETAIL_CVECTORMAP_CTOR
#include "engine/CBlurredAttitudesDef.h"
// Inline-budget: the original definition TU instantiates this resize and constructs and destroys its default element.
template void std::vector<CBlurredAttitudesDef>::resize(unsigned int);
CBlurredAttitudesDef::CBlurredAttitudesDef() {}

#undef free
void CIVCountedPointeeBase::Delete() const {  }
bool CResource::UnloadResource() { return 0; }
unsigned long CResource::GetUnloadableMemory() const { return 0; }
bool CResource::IsResourceLoaded() const { return 0; }
void CResource::OnResourceLoaded() {  }
bool CResource::CanAbortResourceLoad(long) const { return 0; }
void CResource::OnResourceLoadAborted() {  }
unsigned long CResource::GetDebugMemoryInUse() const { return 0; }
void CResource::ResetDebugMemoryInUse() {  }
bool CDefClassBase::UnloadResource() { return 0; }
CDefString CDefClassBase::GetInstantiationName() const { return CDefString(); }
void CDefClassBase::SetInstantiationName(CDefString) {  }
void CDefClassBase::AddSubDef(const CCharString&, long, long) {  }
void CDefClassBase::RemoveSubDef(const CCharString&) {  }
CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&) { return 0; }
const CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&) const { return 0; }
bool CDefClassBase::IsParentDef() const { return 0; }
void CDefClassBase::PostTransfer() {  }
void CDefClassBase::InternalSaveBinary(CDataOutputStream&) {  }
void CDefClassBase::InternalLoadBinary(CDataInputStream&) {  }
void CDefClassBase::Validate(const CValidateDefinitionInitBase&, const CCharString&) const {  }
CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> >* CDefClassBase::GetPSubDefInfoMap() { return 0; }
CDefString CParentDefClassBase::GetInstantiationName() const { return CDefString(); }
void CParentDefClassBase::SetInstantiationName(CDefString) {  }
void CParentDefClassBase::AddSubDef(const CCharString&, long, long) {  }
void CParentDefClassBase::RemoveSubDef(const CCharString&) {  }
CDefClassBase::CSubDefInfo* CParentDefClassBase::GetPSubDefInfo(const CCharString&) { return 0; }
const CDefClassBase::CSubDefInfo* CParentDefClassBase::GetPSubDefInfo(const CCharString&) const { return 0; }
bool CParentDefClassBase::IsParentDef() const { return 0; }
void CParentDefClassBase::InternalSaveBinary(CDataOutputStream&) {  }
void CParentDefClassBase::InternalLoadBinary(CDataInputStream&) {  }
CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> >* CParentDefClassBase::GetPSubDefInfoMap() { return 0; }
void CBlurredAttitudesDef::Transfer(CPersistContext&) {  }
void CBlurredAttitudesDef::Copy(const CDefClassBase*) {  }
unsigned long CBlurredAttitudesDef::GetSizeofClass() { return 0; }

namespace std { template<> void vector<CBlurredAttitudesDef>::resize(size_type,const CBlurredAttitudesDef&) { abort(); } }
CResource::CResource() {}
CResource::~CResource() { ++baseCalls; }
typedef void* (__fastcall *DeletingDtor)(void*,void*,unsigned);
int main() {
    unsigned cases=0;
    for (unsigned flags=0; flags<32; ++flags) {
        for(unsigned mask=0;mask<4;++mask) {
            for(unsigned units=1;units<=4;++units) {
                CBlurredAttitudesDef* p=new CBlurredAttitudesDef();
                if(p->Attitudes.size() || p->SubDefInfoMap.size()) return 1;
                if(mask&1) p->Attitudes.reserve(units);
                if(mask&2) p->SubDefInfoMap.reserve(units);
                baseCalls=bufferCalls=objectCalls=0;
                objectAddress=p;
                DeletingDtor function=**(DeletingDtor**)p;
                void* returned=function(p,0,flags);
                if(returned!=objectAddress || baseCalls!=1 || objectCalls!=(flags&1)
                   || bufferCalls!=unsigned(bool(mask&1))+unsigned(bool(mask&2))) return 2;
                if(!(flags&1)) ::operator delete(objectAddress);
                objectAddress=0;
                ++cases;
            }
        }
    }
    if(cases!=512) return 3;
    puts("0067c520_TEST PASS");return 0;
}