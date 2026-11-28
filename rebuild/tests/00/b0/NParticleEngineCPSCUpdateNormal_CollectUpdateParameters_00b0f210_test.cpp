#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CVECTORMAP_CTOR
#define FABLE_RETAIL_CPARTICLEUPDATEPARAMDESC_GETUPDATEPARAM
#include <stdio.h>
// 00b0f210 NParticleEngine::CPSCUpdateNormal::CollectUpdateParameters
// Leaf: ?CollectUpdateParameters@CPSCUpdateNormal@NParticleEngine@@UAEXAAV?$CParticleUpdateParameters@KK@2@ABK@Z
// Flags: /Gr
#define FABLE_RETAIL_CPSCUPDATENORMAL_COLLECT_PARAMS_PASS30
#define FABLE_RETAIL_ORBIT_UPDATE_PARAMS_PASS30
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CARRAY_PUSHBACK
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE
#define FABLE_RETAIL_CVECTORMAP_INSERT_VALUE_TYPE
#include "engine/CPSCUpdateNormal.h"
#include "engine/CParticleUpdateParameters.h"

namespace NParticleEngine {
void CPSCUpdateNormal::CollectUpdateParameters(CParticleUpdateParameters<unsigned long,unsigned long>& params, const unsigned long& prefix)
{
    unsigned long key = CCRC::Calc(0, "System Offset", 13);
    params.UpdateParams.Insert(key, CParticleUpdateParamDesc<unsigned long,unsigned long>(this, 0));
}
}
CBaseClass::CBaseClass() { }
CBaseClass::~CBaseClass() { }
CBaseClassNonCopyable::CBaseClassNonCopyable() { }
CBaseClassNonCopyable::~CBaseClassNonCopyable() { }
CCharString::CCharString() { }
CCharString::~CCharString() { }

namespace NParticleEngine {
template<class K, class V> CParticleUpdateParamCallback<K,V>::~CParticleUpdateParamCallback() { }
template<class K, class V> bool CParticleUpdateParamCallback<K,V>::GetUpdateParam(C3DVector&,V,float,bool,const CParticleUpdateParameters<K,V>&,unsigned long) { return false; }
CPSCBase::~CPSCBase() { }
void CPSCUpdateNormal::Update(CParticleSystem&,const CParticleUpdateParameters<unsigned long,unsigned long>&,bool&) { }
class NormalProbe : public CPSCUpdateNormal {
public:
 CCharString GetClassTypeName() const { return CCharString(); }
 EPSCType GetComponentType() const { return static_cast<EPSCType>(0); }
 EPSCSubType GetComponentSubType() const { return static_cast<EPSCSubType>(0); }
 CCharString GetGUIName() const { return CCharString(); }
 CPSCBase* DatabaseClone() const { return 0; }
 void InterpolateTo(const CPSCBase&,const float) { }
 void DatabaseCopy(const CPSCBase&) { }
 void ReadBinary(CDataInputStream&) { }
 void WriteBinary(CDataOutputStream&) const { }
 void GetComponentCost(CParticleSystemCost&,const CParticleSystem&,const float,const float) { }
 bool GetUpdateParam(C3DVector&,unsigned long index,float,bool,const CParticleUpdateParameters<unsigned long,unsigned long>&,unsigned long) {
    last_receiver = this;
    return index == 0;
 }
 static NormalProbe* last_receiver;
};
NormalProbe* NormalProbe::last_receiver = 0;
void CPSCBase::OnCreate() {  }
void CPSCBase::StartUpdate(const CParticleUpdateParameters<unsigned long, unsigned long>&) {  }
void CPSCBase::Update(CParticleSystem&,const CParticleUpdateParameters<unsigned long, unsigned long>&,bool&) {  }
void CPSCBase::CollectUpdateParameters(CParticleUpdateParameters<unsigned long, unsigned long>&,const unsigned long&) {  }
void CPSCBase::TransferUpdateParametersToDialog(const CSymbolMap&) {  }
void CPSCBase::UpdateAddParticle(CParticleSystem&,const CParticleUpdateParameters<unsigned long, unsigned long>&,C3DVector,C3DVector,float) {  }
void CPSCBase::PreparePrimitives(const float,const CEnginePrimitivePrepareInfo&,CParticleSystem&,const CParticleUpdateParameters<unsigned long, unsigned long>&,CEnginePrimitiveHandle&) {  }
void CPSCBase::DatabaseSerialise(CPersistContext&,const CEmitterDatabaseSerialiseInfo&) {  }
bool CPSCBase::AddToBoundingSphere(C3DBoundingSphere&,const float,CParticleSystem&,const CParticleUpdateParameters<unsigned long, unsigned long>&) { return false; }
bool CPSCBase::GetParticleLifeSecs(float&) { return false; }
bool CPSCBase::GetNumberOfParticles(float&,const float) { return false; }
bool CPSCBase::GetDecalLifeSecs(float&) { return false; }
bool CPSCBase::GetNumberOfDecals(float&,const float) { return false; }
void CPSCBase::EnterTimelineEnd() {  }
long CPSCBase::GetParticleCount() { return 0; }
long CPSCBase::GetDecalCount() { return 0; }
bool CPSCBase::Initialise(CIEngine*,CParticleSystem&,const CParticleUpdateParameters<unsigned long, unsigned long>&,CEnginePrimitiveHandle&) { return false; }
void CPSCBase::ClearAllPrimitives(CIEngine*,CEnginePrimitiveHandle&) {  }
bool CPSCBase::GetUserPoint(C3DVector&) const { return false; }
float CPSCBase::GetApproximateSplineLength() { return 0; }
}

CBaseIntelligentPointer::~CBaseIntelligentPointer() { }
unsigned long FABLE_FASTCALL CCRC::Calc(unsigned long seed,const void* bytes,unsigned long count)
{
    const unsigned char* p = static_cast<const unsigned char*>(bytes);
    while (count--) {
        seed ^= *p++;
        for (unsigned int bit = 0; bit < 8; ++bit)
            seed = (seed >> 1) ^ ((seed & 1) ? 0xedb88320UL : 0UL);
    }
    return seed;
}
int main()
{
    using namespace NParticleEngine;
    NormalProbe first, second;
    CParticleUpdateParameters<unsigned long,unsigned long> params;
    unsigned long prefixes[] = {0, 1, 0xffffffffUL, 0x12345678UL};
    for (unsigned int i = 0; i < 4; ++i) {
        NormalProbe* receiver = i & 1 ? &second : &first;
        receiver->CollectUpdateParameters(params, prefixes[i]);
        if (params.UpdateParams.size() != i + 1) return 1;
        for (unsigned int j = 0; j <= i; ++j) {
            if (params.UpdateParams[j].first != 0xe802d792UL) return 2;
            C3DVector out(0,0,0);
            if (!params.UpdateParams[j].second.GetUpdateParam(out,0,false,params,0)) return 3;
            if (NormalProbe::last_receiver != (j & 1 ? &second : &first)) return 4;
        }
    }
    puts("00b0f210_TEST PASS");
    return 0;
}
