#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if !defined(_MSC_VER)
#include <strings.h>
#define _stricmp strcasecmp
#define __declspec(x)
#define __cdecl
#define __fastcall
#define __int64 long long
#endif
#define FABLE_RETAIL_DEF_CLASS_BASE_CTOR_DECL
// Fixture-only access for initializing the four proven protected owning pointers.
#define protected public
// CCreatureDef::Transfer(CPersistContext&) -- persist the creature definition and its pointed-to definitions.
// Grade: DIFFER(1912v1912) -- pointer scratch choices and subsequent scratch rotation; no byte match claimed.
// Leaf: ?Transfer@CCreatureDef@@UAEXAAVCPersistContext@@@Z
// Retail: 0x00675e20, 1912 bytes, virtual thiscall, ret 4.
#define FABLE_RETAIL_CPERSIST_ORIGINAL_CLASS_TYPES_PASS22
#define FABLE_RETAIL_CCREATUREDEF_TRANSFER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRING_TRANSFER
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CVECTORMAP_CTOR
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CPERSISTCONTEXT_TRANSFER_BINARY_BODY
#define FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERMAP_BINARY_BODY
#define FABLE_RETAIL_CPERSISTTRAITS_BOOL_CLASS
#include "engine/CDataOutputStream.h"
#include "engine/CCreatureDef.h"
#include "engine/CCreatureStatsDef.h"
#include "engine/CRandomAppearanceMorph.h"
#include "engine/CWoundMorphs.h"
#include "engine/CExpressionSet.h"
#include "engine/CPersistContext.h"
#include "engine/CMemoryDataInputStream.h"

template<> void CPersistContext::Transfer<CWoundMorphs>(CWoundMorphs& value) {
    CheckCRC("");
    CPersistTraits<CWoundMorphs> traits;
    switch (Mode) {
    case MODE_LOAD_BINARY: traits.TransferBinaryIn(*PLoadStream,value); break;
    case MODE_SAVE_BINARY: traits.TransferBinaryOut(*PSaveStream,value); break;
    }
}
template<> void CPersistContext::Transfer<CExpressionSet>(CExpressionSet& value) {
    CheckCRC("");
    CPersistTraits<CExpressionSet> traits;
    switch (Mode) {
    case MODE_LOAD_BINARY: traits.TransferBinaryIn(*PLoadStream,value); break;
    case MODE_SAVE_BINARY: traits.TransferBinaryOut(*PSaveStream,value); break;
    }
}
template<> void CPersistContext::Transfer<long>(long& value) {
    CheckCRC("");
    CPersistTraits<long> traits;
    switch (Mode) {
    case MODE_LOAD_BINARY: traits.TransferBinaryIn(*PLoadStream,value); break;
    case MODE_SAVE_BINARY: traits.TransferBinaryOut(*PSaveStream,value); break;
    }
}
template<> void CPersistContext::Transfer<unsigned long>(unsigned long& value) {
    CheckCRC("");
    CPersistTraits<unsigned long> traits;
    switch (Mode) {
    case MODE_LOAD_BINARY: traits.TransferBinaryIn(*PLoadStream,value); break;
    case MODE_SAVE_BINARY: traits.TransferBinaryOut(*PSaveStream,value); break;
    }
}
template<> void CPersistContext::Transfer<bool>(bool& value) {
    CheckCRC("");
    CPersistTraits<bool> traits;
    switch (Mode) {
    case MODE_LOAD_BINARY: traits.TransferBinaryIn(*PLoadStream,value); break;
    case MODE_SAVE_BINARY: traits.TransferBinaryOut(*PSaveStream,value); break;
    }
}
template<> void CPersistContext::Transfer<CDefString>(CDefString& value) {
    CheckCRC("");
    CPersistTraits<CDefString> traits;
    switch (Mode) {
    case MODE_LOAD_BINARY: traits.TransferBinaryIn(*PLoadStream,value); break;
    case MODE_SAVE_BINARY: traits.TransferBinaryOut(*PSaveStream,value); break;
    }
}

void CCreatureDef::Transfer(CPersistContext& persist)
{
    persist.Transfer(*PWoundMorphs);
    persist.Transfer(*PRandomAppearanceMorph);
    persist.Transfer(*PExpressions);
    persist.TransferSubComponent(*PStats);
    persist.TransferVector(InitialActions);
    persist.TransferVector(InitialAppearanceModifiers);
    persist.TransferMap(PhonemeAnim);
    persist.TransferVector(DialogueVoices);
    persist.Transfer(Inventory);
    persist.Transfer(ClothingInventory);
    persist.Transfer(WeaponsInventory);
    persist.Transfer(HeroAbilitiesScreenInventory);
    persist.Transfer(QuestCardScreenInventory);
    persist.Transfer(MapScreenInventory);
    persist.Transfer(MagicScreenInventory);
    persist.Transfer(StatsScreenInventory);
    persist.Transfer(ExperienceScreenInventory);
    persist.Transfer(TradeScreenInventory);
    persist.Transfer(AugmentScreenInventory);
    persist.Transfer(QuestsScreenInventory);
    persist.Transfer(InGameMenuScreen);
    persist.Transfer(CreatureGroup);
    persist.Transfer(BattleCrySound);
    persist.Transfer(OpinionSourceDef);
    persist.Transfer(RespawnWaitInDaysMin);
    persist.Transfer(RespawnWaitInDaysMax);
    persist.Transfer(RespawnWaitInFramesMin);
    persist.Transfer(RespawnWaitInFramesMax);
    persist.Transfer(UseActualHitPosForHitEffects);
    persist.Transfer(Short);
    persist.Transfer(FlashOnHit);
    persist.Transfer(PickPocketable);
    persist.Transfer(ChildThatCanRegionFollow);
    persist.Transfer(AllowedInProtectedTowns);
    persist.TransferMap(RadiusWeight.StatWeights);
    persist.Transfer(RadiusSpread);
    persist.Transfer(AGhostIsWhatIBe);
}

#undef protected

#include "fable_string.h"
static const void* seen[64];
static int events,crc,seed;
static long saved[64];
static void record(const void* value) { if(events>=64)abort(); seen[events++]=value; }
void CPersistContext::CheckCRC(const char* key) { if(!key||*key)abort();++crc; }
CCharString::~CCharString(){}
CBaseClass::CBaseClass() {} CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {} CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CDataStream::~CDataStream() {}
CResource::CResource() {} CResource::~CResource() {}
void CIVCountedPointeeBase::Delete() const {}
bool CResource::UnloadResource(){return false;}
unsigned long CResource::GetUnloadableMemory()const{return 0;}
bool CResource::IsResourceLoaded()const{return true;}
void CResource::OnResourceLoaded(){}
bool CResource::CanAbortResourceLoad(long)const{return false;}
void CResource::OnResourceLoadAborted(){}
unsigned long CResource::GetDebugMemoryInUse()const{return 0;}
void CResource::ResetDebugMemoryInUse(){}
CDefClassBase::CDefClassBase(){}
CSubDefClassBase::CSubDefClassBase(){}
bool CDefClassBase::UnloadResource(){return false;}
CDefString CDefClassBase::GetInstantiationName()const{return CDefString();}
void CDefClassBase::SetInstantiationName(CDefString){}
void CDefClassBase::AddSubDef(const CCharString&,long,long){}
void CDefClassBase::RemoveSubDef(const CCharString&){}
CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&){return 0;}
const CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&)const{return 0;}
bool CDefClassBase::IsParentDef()const{return false;}
void CDefClassBase::PostTransfer(){}
void CDefClassBase::InternalSaveBinary(CDataOutputStream&){}
void CDefClassBase::InternalLoadBinary(CDataInputStream&){}
void CDefClassBase::Validate(const CValidateDefinitionInitBase&,const CCharString&)const{}
CVectorMap<unsigned long,CDefClassBase::CSubDefInfo,CKeyPairCompareLess<unsigned long,CDefClassBase::CSubDefInfo> >* CDefClassBase::GetPSubDefInfoMap(){return 0;}
void CCreatureStatsDef::Transfer(CPersistContext&){record(this);}
class StatsFixture:public CCreatureStatsDef {public:void Copy(const CDefClassBase*){}unsigned long GetSizeofClass(){return sizeof(*this);}};
class CreatureFixture:public CCreatureDef {public:void Copy(const CDefClassBase*){}unsigned long GetSizeofClass(){return sizeof(*this);}};
CDataInputStream::CDataInputStream(){}
void CDataInputStream::Close(){} void CDataInputStream::SetPosition(unsigned long){}
void CDataInputStream::ReadCompressed(void*,long){} void CDataInputStream::ReadZlibCompressed(void*,long){}
CCharString CDataInputStream::ReadNullTerminatedString(){abort();}
CWideString CDataInputStream::ReadNullTerminatedWString(){abort();}
void CDataInputStream::InternalReadNextSourceChunk(const unsigned char*&,long&){}
bool CDataInputStream::NeedsBufferedRead(long){return false;}
void CDataInputStream::InternalReadUnbuffered(unsigned char*,long){}
void CMemoryDataInputStream::ReadCompressed(void*,long){}
CCharString CMemoryDataInputStream::ReadNullTerminatedString(){abort();}
CWideString CMemoryDataInputStream::ReadNullTerminatedWString(){abort();}
class ContextFixture:public CPersistContext {public:ContextFixture(int mode,CMemoryDataInputStream& input,CDataOutputStream& output){Mode=(EMode)mode;PLoadStream=&input;PSaveStream=&output;}};
void CWoundMorphs::TransferBinaryIn(CMemoryDataInputStream&){record(this);}
void CWoundMorphs::TransferBinaryOut(CDataOutputStream&)const{record(this);}
void CExpressionSet::TransferBinaryIn(CMemoryDataInputStream&){record(this);}
void CExpressionSet::TransferBinaryOut(CDataOutputStream&)const{record(this);}
void CDataOutputStream::WriteSLONG(long v){saved[events]=v;record(0);}
void CDataOutputStream::WriteEBOOL(bool v){saved[events]=v?1:0;record(0);}
static void loadValue(long& v){v=123+seed;}
static void loadValue(unsigned long& v){v=456+seed;}
static void loadValue(bool& v){v=(seed&1)!=0;}
void CPersistTraits<bool>::TransferBinaryIn(CMemoryDataInputStream&,bool& v){record(&v);loadValue(v);}
template<class T> void CPersistTraits<T>::TransferBinaryIn(CMemoryDataInputStream&,T& v){record(&v);loadValue(v);}
void CDefString::TransferBinaryIn(CDataInputStream&){record(this);}
template<class T> void CPersistContext::Transfer(T& v){CheckCRC("");record(&v);}
template<class T>void FABLE_FASTCALL GFSerialiseVectorBinaryIn(CMemoryDataInputStream&,std::vector<T>& v){record(&v);}
template<class T>void FABLE_FASTCALL GFSerialiseVectorBinaryOut(CDataOutputStream&,std::vector<T>& v){record(&v);}
template<class V,class K,class C>void FABLE_FASTCALL GFSerialiseMapBinaryIn(CMemoryDataInputStream&,std::map<K,V,C,std::allocator<std::pair<const K,V> > >& v){record(&v);}
template<class V,class K,class C>void FABLE_FASTCALL GFSerialiseMapBinaryOut(CDataOutputStream&,const std::map<K,V,C,std::allocator<std::pair<const K,V> > >& v){record(&v);}
int main(){
    CreatureFixture creature;CWoundMorphs wounds;CExpressionSet expressions;StatsFixture stats;CRandomAppearanceMorph morph;
    creature.PWoundMorphs.Data=&wounds;creature.PRandomAppearanceMorph.Data=&morph;creature.PExpressions.Data=&expressions;creature.PStats.Data=&stats;
    const void* expected[]={&wounds,&morph,&expressions,&stats,&creature.InitialActions,&creature.InitialAppearanceModifiers,&creature.PhonemeAnim,&creature.DialogueVoices,
    &creature.Inventory,&creature.ClothingInventory,&creature.WeaponsInventory,&creature.HeroAbilitiesScreenInventory,&creature.QuestCardScreenInventory,&creature.MapScreenInventory,
    &creature.MagicScreenInventory,&creature.StatsScreenInventory,&creature.ExperienceScreenInventory,&creature.TradeScreenInventory,&creature.AugmentScreenInventory,&creature.QuestsScreenInventory,&creature.InGameMenuScreen,
    &creature.CreatureGroup,&creature.BattleCrySound,&creature.OpinionSourceDef,&creature.RespawnWaitInDaysMin,&creature.RespawnWaitInDaysMax,&creature.RespawnWaitInFramesMin,&creature.RespawnWaitInFramesMax,
    &creature.UseActualHitPosForHitEffects,&creature.Short,&creature.FlashOnHit,&creature.PickPocketable,&creature.ChildThatCanRegionFollow,&creature.AllowedInProtectedTowns,&creature.RadiusWeight.StatWeights,&creature.RadiusSpread,&creature.AGhostIsWhatIBe};
    long* longs[]={&creature.Inventory,&creature.ClothingInventory,&creature.WeaponsInventory,&creature.HeroAbilitiesScreenInventory,&creature.QuestCardScreenInventory,&creature.MapScreenInventory,&creature.MagicScreenInventory,&creature.StatsScreenInventory,&creature.ExperienceScreenInventory,&creature.TradeScreenInventory,&creature.AugmentScreenInventory,&creature.QuestsScreenInventory,&creature.InGameMenuScreen,&creature.OpinionSourceDef,&creature.RespawnWaitInDaysMin,&creature.RespawnWaitInDaysMax,&creature.RespawnWaitInFramesMin,&creature.RespawnWaitInFramesMax};
    bool* flags[]={&creature.UseActualHitPosForHitEffects,&creature.Short,&creature.FlashOnHit,&creature.PickPocketable,&creature.ChildThatCanRegionFollow,&creature.AllowedInProtectedTowns,&creature.AGhostIsWhatIBe};
    CMemoryDataInputStream input;CDataOutputStream output;
    for(seed=0;seed<64;++seed)for(int mode=0;mode<5;++mode){
        long want[37];memset(want,0,sizeof(want));
        for(int k=0;k<18;++k){*longs[k]=seed*101+k-3000;want[k<13?k+8:k+10]=*longs[k];}
        creature.CreatureGroup=0x7f000000UL+seed;want[21]=(long)creature.CreatureGroup;want[22]=-1;
        for(int k=0;k<7;++k){*flags[k]=(seed&(1<<(k%6)))!=0;want[k<6?k+28:36]=*flags[k]?1:0;}
        ContextFixture context(mode,input,output);events=crc=0;memset(saved,0,sizeof(saved));
        creature.CCreatureDef::Transfer(context);if(crc!=37)return 1;
        if(mode==2){
            if(events!=37)return 2;for(int i=0;i<37;++i)if(seen[i]!=expected[i])return 3;
            for(int k=0;k<18;++k)if(*longs[k]!=123+seed)return 4;
            if(creature.CreatureGroup!=(unsigned long)(456+seed))return 9;
            for(int k=0;k<7;++k)if(*flags[k]!=((seed&1)!=0))return 10;
        }else if(mode==3){
            if(events!=37)return 5;for(int i=0;i<37;++i)if(seen[i]&&seen[i]!=expected[i])return 6;
            for(int i=8;i<34;++i)if(saved[i]!=want[i])return 11;
            if(saved[36]!=want[36])return 12;
        }else{
            const void* other[]={&morph,&stats,&creature.InitialActions,&creature.InitialAppearanceModifiers,&creature.DialogueVoices,&creature.RadiusSpread};
            if(events!=6)return 7;for(int i=0;i<6;++i)if(seen[i]!=other[i])return 8;
        }
        if(mode!=2){for(int k=0;k<18;++k)if(*longs[k]!=want[k<13?k+8:k+10])return 13;for(int k=0;k<7;++k)if((*flags[k]?1:0)!=want[k<6?k+28:36])return 14;}
    }
    creature.PWoundMorphs.Data=0;creature.PRandomAppearanceMorph.Data=0;creature.PExpressions.Data=0;creature.PStats.Data=0;
    puts("00675e20_TEST PASS");return 0;
}
