// 00b158a0 standalone behavior test
#define FABLE_RETAIL_CPSCEMITTERGENERIC_COLLECT_PARAMS_PASS30
#define FABLE_RETAIL_CPSCEMITTERGENERIC_CTOR
#define FABLE_RETAIL_CPSCEMITTERGENERIC_ALLOC
#define FABLE_RETAIL_CPARTICLEUPDATEPARAMDESC_CTOR
#define FABLE_RETAIL_CPSCSPLINEBASE_PASS23
#define FABLE_RETAIL_CPSCSPLINEBASE_MEMBERS
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CVECTORMAP_CTOR
#define FABLE_RETAIL_CVECTORMAP_SETDIRTY
#define FABLE_RETAIL_CPARTICLEUPDATEPARAMDESC_GETUPDATEPARAM
#define FABLE_RETAIL_CCRC_ORIGINAL_TYPED_LOOP_PASS27
#define FABLE_RETAIL_CRC32TAB_PASS29
#define FABLE_RETAIL_ORBIT_UPDATE_PARAMS_PASS30
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CARRAY_PUSHBACK
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE
#define FABLE_RETAIL_CVECTORMAP_INSERT_VALUE_TYPE
#include "engine/NParticleEngine_CPSCOrbit.h"
#include "engine/CParticleUpdateParameters.h"
#include "engine/CPSCEmitterGeneric.h"
#include "engine/CPSCSpline.h"

#include "fable_string.h"
#include <stdio.h>
#include <stdlib.h>
static unsigned long last_index;
static void require(bool ok) { if (!ok) { puts("orbit fixture failed"); exit(1); } }
const unsigned long CRC32TAB[256] = {
    0x00000000UL, 0x77073096UL, 0xee0e612cUL, 0x990951baUL, 0x076dc419UL, 0x706af48fUL, 0xe963a535UL, 0x9e6495a3UL,
    0x0edb8832UL, 0x79dcb8a4UL, 0xe0d5e91eUL, 0x97d2d988UL, 0x09b64c2bUL, 0x7eb17cbdUL, 0xe7b82d07UL, 0x90bf1d91UL,
    0x1db71064UL, 0x6ab020f2UL, 0xf3b97148UL, 0x84be41deUL, 0x1adad47dUL, 0x6ddde4ebUL, 0xf4d4b551UL, 0x83d385c7UL,
    0x136c9856UL, 0x646ba8c0UL, 0xfd62f97aUL, 0x8a65c9ecUL, 0x14015c4fUL, 0x63066cd9UL, 0xfa0f3d63UL, 0x8d080df5UL,
    0x3b6e20c8UL, 0x4c69105eUL, 0xd56041e4UL, 0xa2677172UL, 0x3c03e4d1UL, 0x4b04d447UL, 0xd20d85fdUL, 0xa50ab56bUL,
    0x35b5a8faUL, 0x42b2986cUL, 0xdbbbc9d6UL, 0xacbcf940UL, 0x32d86ce3UL, 0x45df5c75UL, 0xdcd60dcfUL, 0xabd13d59UL,
    0x26d930acUL, 0x51de003aUL, 0xc8d75180UL, 0xbfd06116UL, 0x21b4f4b5UL, 0x56b3c423UL, 0xcfba9599UL, 0xb8bda50fUL,
    0x2802b89eUL, 0x5f058808UL, 0xc60cd9b2UL, 0xb10be924UL, 0x2f6f7c87UL, 0x58684c11UL, 0xc1611dabUL, 0xb6662d3dUL,
    0x76dc4190UL, 0x01db7106UL, 0x98d220bcUL, 0xefd5102aUL, 0x71b18589UL, 0x06b6b51fUL, 0x9fbfe4a5UL, 0xe8b8d433UL,
    0x7807c9a2UL, 0x0f00f934UL, 0x9609a88eUL, 0xe10e9818UL, 0x7f6a0dbbUL, 0x086d3d2dUL, 0x91646c97UL, 0xe6635c01UL,
    0x6b6b51f4UL, 0x1c6c6162UL, 0x856530d8UL, 0xf262004eUL, 0x6c0695edUL, 0x1b01a57bUL, 0x8208f4c1UL, 0xf50fc457UL,
    0x65b0d9c6UL, 0x12b7e950UL, 0x8bbeb8eaUL, 0xfcb9887cUL, 0x62dd1ddfUL, 0x15da2d49UL, 0x8cd37cf3UL, 0xfbd44c65UL,
    0x4db26158UL, 0x3ab551ceUL, 0xa3bc0074UL, 0xd4bb30e2UL, 0x4adfa541UL, 0x3dd895d7UL, 0xa4d1c46dUL, 0xd3d6f4fbUL,
    0x4369e96aUL, 0x346ed9fcUL, 0xad678846UL, 0xda60b8d0UL, 0x44042d73UL, 0x33031de5UL, 0xaa0a4c5fUL, 0xdd0d7cc9UL,
    0x5005713cUL, 0x270241aaUL, 0xbe0b1010UL, 0xc90c2086UL, 0x5768b525UL, 0x206f85b3UL, 0xb966d409UL, 0xce61e49fUL,
    0x5edef90eUL, 0x29d9c998UL, 0xb0d09822UL, 0xc7d7a8b4UL, 0x59b33d17UL, 0x2eb40d81UL, 0xb7bd5c3bUL, 0xc0ba6cadUL,
    0xedb88320UL, 0x9abfb3b6UL, 0x03b6e20cUL, 0x74b1d29aUL, 0xead54739UL, 0x9dd277afUL, 0x04db2615UL, 0x73dc1683UL,
    0xe3630b12UL, 0x94643b84UL, 0x0d6d6a3eUL, 0x7a6a5aa8UL, 0xe40ecf0bUL, 0x9309ff9dUL, 0x0a00ae27UL, 0x7d079eb1UL,
    0xf00f9344UL, 0x8708a3d2UL, 0x1e01f268UL, 0x6906c2feUL, 0xf762575dUL, 0x806567cbUL, 0x196c3671UL, 0x6e6b06e7UL,
    0xfed41b76UL, 0x89d32be0UL, 0x10da7a5aUL, 0x67dd4accUL, 0xf9b9df6fUL, 0x8ebeeff9UL, 0x17b7be43UL, 0x60b08ed5UL,
    0xd6d6a3e8UL, 0xa1d1937eUL, 0x38d8c2c4UL, 0x4fdff252UL, 0xd1bb67f1UL, 0xa6bc5767UL, 0x3fb506ddUL, 0x48b2364bUL,
    0xd80d2bdaUL, 0xaf0a1b4cUL, 0x36034af6UL, 0x41047a60UL, 0xdf60efc3UL, 0xa867df55UL, 0x316e8eefUL, 0x4669be79UL,
    0xcb61b38cUL, 0xbc66831aUL, 0x256fd2a0UL, 0x5268e236UL, 0xcc0c7795UL, 0xbb0b4703UL, 0x220216b9UL, 0x5505262fUL,
    0xc5ba3bbeUL, 0xb2bd0b28UL, 0x2bb45a92UL, 0x5cb36a04UL, 0xc2d7ffa7UL, 0xb5d0cf31UL, 0x2cd99e8bUL, 0x5bdeae1dUL,
    0x9b64c2b0UL, 0xec63f226UL, 0x756aa39cUL, 0x026d930aUL, 0x9c0906a9UL, 0xeb0e363fUL, 0x72076785UL, 0x05005713UL,
    0x95bf4a82UL, 0xe2b87a14UL, 0x7bb12baeUL, 0x0cb61b38UL, 0x92d28e9bUL, 0xe5d5be0dUL, 0x7cdcefb7UL, 0x0bdbdf21UL,
    0x86d3d2d4UL, 0xf1d4e242UL, 0x68ddb3f8UL, 0x1fda836eUL, 0x81be16cdUL, 0xf6b9265bUL, 0x6fb077e1UL, 0x18b74777UL,
    0x88085ae6UL, 0xff0f6a70UL, 0x66063bcaUL, 0x11010b5cUL, 0x8f659effUL, 0xf862ae69UL, 0x616bffd3UL, 0x166ccf45UL,
    0xa00ae278UL, 0xd70dd2eeUL, 0x4e048354UL, 0x3903b3c2UL, 0xa7672661UL, 0xd06016f7UL, 0x4969474dUL, 0x3e6e77dbUL,
    0xaed16a4aUL, 0xd9d65adcUL, 0x40df0b66UL, 0x37d83bf0UL, 0xa9bcae53UL, 0xdebb9ec5UL, 0x47b2cf7fUL, 0x30b5ffe9UL,
    0xbdbdf21cUL, 0xcabac28aUL, 0x53b39330UL, 0x24b4a3a6UL, 0xbad03605UL, 0xcdd70693UL, 0x54de5729UL, 0x23d967bfUL,
    0xb3667a2eUL, 0xc4614ab8UL, 0x5d681b02UL, 0x2a6f2b94UL, 0xb40bbe37UL, 0xc30c8ea1UL, 0x5a05df1bUL, 0x2d02ef8dUL,
};
CBaseClass::CBaseClass() { }
CBaseClass::~CBaseClass() { }
CBaseClassNonCopyable::CBaseClassNonCopyable() { }
CBaseClassNonCopyable::~CBaseClassNonCopyable() { }
CCharString::CCharString() { }
CCharString::~CCharString() { }

namespace NParticleEngine {
template<class K, class V> CParticleUpdateParamCallback<K,V>::~CParticleUpdateParamCallback() { }
template<class K, class V> bool CParticleUpdateParamCallback<K,V>::GetUpdateParam(C3DVector&,V,float,bool,const CParticleUpdateParameters<K,V>&,unsigned long) { return false; }
CPSCOrbit::COrbitState::COrbitState() { }
CPSCOrbit::CPSCOrbit() { }
CPSCOrbit::~CPSCOrbit() { }
CPSCBase::~CPSCBase() { }
static const CPSCOrbit* last_orbit;
bool CPSCOrbit::GetUpdateParam(C3DVector&,unsigned long index,float,bool,const CParticleUpdateParameters<unsigned long,unsigned long>&,unsigned long)
{
    last_orbit = this; last_index = index; return true;
}
CCharString CPSCOrbit::GetClassTypeName() const { return CCharString(); }
EPSCType CPSCOrbit::GetComponentType() const { return static_cast<EPSCType>(0); }
EPSCSubType CPSCOrbit::GetComponentSubType() const { return static_cast<EPSCSubType>(0); }
CCharString CPSCOrbit::GetGUIName() const { return CCharString(); }
CPSCBase* CPSCOrbit::DatabaseClone() const { return 0; }
void CPSCOrbit::InterpolateTo(const CPSCBase&,const float) {  }
void CPSCOrbit::DatabaseCopy(const CPSCBase&) {  }
void CPSCOrbit::DatabaseSerialise(CPersistContext&,const CEmitterDatabaseSerialiseInfo&) {  }
void CPSCOrbit::ReadBinary(CDataInputStream&) {  }
void CPSCOrbit::WriteBinary(CDataOutputStream&) const {  }
void CPSCOrbit::StartUpdate(const CParticleUpdateParameters<unsigned long,unsigned long>&) {  }
void CPSCOrbit::GetComponentCost(CParticleSystemCost&,const CParticleSystem&,const float,const float) {  }
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

namespace NParticleEngine {

typedef unsigned long TKey;
typedef CParticleUpdateParameters<TKey,TKey> TParams;
typedef CParticleUpdateParamDesc<TKey,TKey> TDesc;
typedef CParticleUpdateParamCallback<TKey,TKey> TCallback;

static const TCallback* callback_receiver;
static unsigned long callback_index;
static const TParams* callback_params;
static float callback_time;
static bool callback_local;
static unsigned long callback_flags;

class TestEmitter : public CPSCEmitterGeneric {
public:
    virtual bool GetUpdateParam(C3DVector&, TKey index, float time, bool local,
                                const TParams& params, unsigned long flags)
    {
        callback_receiver=this;
        callback_index=index;
        callback_params=&params;
        callback_time=time;
        callback_local=local;
        callback_flags=flags;
        return true;
    }
};

class TestSpline : public CPSCSpline {
public:
    virtual bool GetUpdateParam(C3DVector&, TKey index, float time, bool local,
                                const TParams& params, unsigned long flags)
    {
        callback_receiver=this;
        callback_index=index;
        callback_params=&params;
        callback_time=time;
        callback_local=local;
        callback_flags=flags;
        return true;
    }
};

CPSCEmitterGeneric::CPSCEmitterGeneric() {}
CPSCBase* CPSCEmitterGeneric::DatabaseClone() const { abort(); return 0; }
CCharString CPSCEmitterGeneric::GetClassTypeName() const { abort(); return CCharString(); }
EPSCType CPSCEmitterGeneric::GetComponentType() const { abort(); return static_cast<EPSCType>(0); }
EPSCSubType CPSCEmitterGeneric::GetComponentSubType() const { abort(); return static_cast<EPSCSubType>(0); }
CCharString CPSCEmitterGeneric::GetGUIName() const { abort(); return CCharString(); }
void CPSCEmitterGeneric::InterpolateTo(const CPSCBase&,const float) { abort(); }
void CPSCEmitterGeneric::ReadBinary(CDataInputStream&) { abort(); }
void CPSCEmitterGeneric::WriteBinary(CDataOutputStream&) const { abort(); }
void CPSCEmitterGeneric::DatabaseCopy(const CPSCBase&) { abort(); }
void CPSCEmitterGeneric::DatabaseSerialise(CPersistContext&,const CEmitterDatabaseSerialiseInfo&) { abort(); }
void CPSCEmitterGeneric::GetComponentCost(CParticleSystemCost&,const CParticleSystem&,const float,const float) { abort(); }

CPSCSplineBase::CPSCSplineBase() {}
CPSCSplineBase::~CPSCSplineBase() {}
void CPSCSplineBase::StartUpdate(const TParams&) { abort(); }
void CPSCSplineBase::InterpolateTo(const CPSCBase&,const float) { abort(); }
void CPSCSplineBase::ReadBinary(CDataInputStream&) { abort(); }
void CPSCSplineBase::WriteBinary(CDataOutputStream&) const { abort(); }
void CPSCSplineBase::DatabaseSerialise(CPersistContext&,const CEmitterDatabaseSerialiseInfo&) { abort(); }
void CPSCSplineBase::DatabaseCopy(const CPSCBase&) { abort(); }
bool CPSCSplineBase::Initialise(CIEngine*,CParticleSystem&,const TParams&,CEnginePrimitiveHandle&) { abort(); return false; }
float CPSCSplineBase::GetApproximateSplineLength() { abort(); return 0; }
void CPSCSplineBase::GetComponentCost(CParticleSystemCost&,const CParticleSystem&,const float,const float) { abort(); }

CPSCSpline::CPSCSpline() {}
CPSCBase* CPSCSpline::DatabaseClone() const { abort(); return 0; }
CCharString CPSCSpline::GetClassTypeName() const { abort(); return CCharString(); }
EPSCType CPSCSpline::GetComponentType() const { abort(); return static_cast<EPSCType>(0); }
EPSCSubType CPSCSpline::GetComponentSubType() const { abort(); return static_cast<EPSCSubType>(0); }
CCharString CPSCSpline::GetGUIName() const { abort(); return CCharString(); }
void CPSCSpline::InterpolateTo(const CPSCBase&,const float) { abort(); }
void CPSCSpline::DatabaseCopy(const CPSCBase&) { abort(); }
void CPSCSpline::WriteBinary(CDataOutputStream&) const { abort(); }
bool CPSCSpline::GetUpdateParam(C3DVector&,TKey,float,bool,const TParams&,unsigned long) { abort(); return false; }
void CPSCSpline::StartUpdate(const TParams&) { abort(); }
void CPSCSpline::ReadBinary(CDataInputStream&) { abort(); }
void CPSCSpline::DatabaseSerialise(CPersistContext&,const CEmitterDatabaseSerialiseInfo&) { abort(); }
void CPSCSpline::GetComponentCost(CParticleSystemCost&,const CParticleSystem&,const float,const float) { abort(); }

}

typedef CVectorMap<NParticleEngine::TKey,NParticleEngine::TDesc,
                   NParticleEngine::TParams::CParticleUpdateCCompareParamDesc<NParticleEngine::TKey> > TMap;

namespace NParticleEngine {
void CPSCOrbit::CollectUpdateParameters(CParticleUpdateParameters<unsigned long, unsigned long>& dest, const unsigned long& instance_name)
{
    dest.UpdateParams.Insert(MAKE_UPDATE_PARAM_FROM_BASE(instance_name, ".Primary"), CParticleUpdateParamDesc<unsigned long, unsigned long>(this, 0));
    dest.UpdateParams.Insert(MAKE_UPDATE_PARAM_FROM_BASE(instance_name, ".Secondary"), CParticleUpdateParamDesc<unsigned long, unsigned long>(this, 1));
    dest.UpdateParams.Insert(MAKE_UPDATE_PARAM_FROM_BASE(instance_name, ".Tertiary"), CParticleUpdateParamDesc<unsigned long, unsigned long>(this, 2));
}
}

namespace NParticleEngine {
void CPSCEmitterGeneric::CollectUpdateParameters(
    CParticleUpdateParameters<unsigned long,unsigned long>& dest, const unsigned long& instance_name)
{
    dest.UpdateParams.Insert(MAKE_UPDATE_PARAM_FROM_BASE(instance_name, ".Centre"),
        CParticleUpdateParamDesc<unsigned long,unsigned long>(this, 0));
}
}

namespace NParticleEngine {
void CPSCSpline::CollectUpdateParameters(
    CParticleUpdateParameters<unsigned long,unsigned long>& dest, const unsigned long& instance_name)
{
    dest.UpdateParams.Insert(instance_name, CParticleUpdateParamDesc<unsigned long,unsigned long>(this, 0));
}
}

static unsigned long bitwise_crc(unsigned long seed,const char* suffix)
{
    for (;*suffix;++suffix) {
        seed^=static_cast<unsigned char>(*suffix);
        for (int bit=0;bit<8;++bit)
            seed=(seed>>1)^((seed&1)!=0 ? 0xedb88320UL : 0);
    }
    return seed;
}

static void check_descriptor(const NParticleEngine::TParams& params,unsigned int pos,
                             unsigned long key,const NParticleEngine::TCallback& receiver,unsigned long index)
{
    using namespace NParticleEngine;
    require(params.UpdateParams[pos].first==key);
    C3DVector out(0,0,0);
    callback_receiver=0;
    require(params.UpdateParams[pos].second.GetUpdateParam(out,0.75f,true,params,0x1234));
    require(callback_receiver==&receiver && callback_index==index);
    require(callback_params==&params && callback_time==0.75f && callback_local && callback_flags==0x1234);
}

static bool dirty(const TMap& map)
{
    // The actual retail CVectorMap contains a 12-byte vector, one Compare byte, then Dirty.
    require(sizeof(TMap)==16);
    return reinterpret_cast<const unsigned char*>(&map)[13]!=0;
}

static void test_collectors(NParticleEngine::CPSCBase& first,NParticleEngine::CPSCBase& second,
                            const char* suffix)
{
    using namespace NParticleEngine;
    TParams params;
    const unsigned long keys[]={0,0x12345678UL,0xffffffffUL,0x87654321UL};
    CPSCBase* collectors[]={&first,&second,&first,&second};
    for (unsigned int i=0;i<4;++i) {
        params.UpdateParams.SetDirty(false);
        require(!dirty(params.UpdateParams));
        const TMap::value_type* const old=params.UpdateParams.begin();
        collectors[i]->CollectUpdateParameters(params,keys[i]);
        require(params.UpdateParams.size()==i+1 && dirty(params.UpdateParams));
        if (i==0) require(params.UpdateParams.begin()!=old);
        if (i==3) require(params.UpdateParams.begin()==old);
        for (unsigned int j=0;j<=i;++j) {
            const unsigned long key=suffix ? bitwise_crc(keys[j],suffix) : keys[j];
            check_descriptor(params,j,key,*collectors[j],0);
        }
    }

}


int main()
{
    using namespace NParticleEngine;
    TestEmitter first, second;
    test_collectors(first, second, ".Centre");
    puts("00b158a0_TEST PASS");
    return 0;
}
