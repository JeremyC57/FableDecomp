// 009af7a0 CDefinitionManager::SaveBinaryDefinitions standalone real-object fixture
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#define __int64 long long
#define __declspec(x)
#define FABLE_PORTABLE
#include <strings.h>
#define _stricmp strcasecmp
#define FABLE_RETAIL_CMEMORYALLOCATOR_SAVEBINARY_HOST_PASS23
#endif
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_RETAIL_STLP_HOST_TAGS_PASS23
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_CTOR
#define FABLE_RETAIL_DEFINITION_ORIGINAL_USER_CONSTRUCTORS_PASS22
#define FABLE_RETAIL_CMEMORYALLOCATOR_CTOR
#define private public
#define protected public
// 009af7a0 CDefinitionManager::SaveBinaryDefinitions
// Leaf: ?SaveBinaryDefinitions@CDefinitionManager@@QAEXAAVCDataOutputStream@@@Z
// Flags: none
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_INSTANTIATEDEF
#define FABLE_RETAIL_CDEFINITIONMANAGER_SAVEBINARY_PASS23
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRINGTABLE_SAVEBINARY_PASS23
#define FABLE_RETAIL_CDEFPOINTER_POINTER_CTOR_ADOPT
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE
#define FABLE_RETAIL_CDEFPOINTER_RESET
#define FABLE_RETAIL_CCREATUREDEF_TRANSFER
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAOUTPUTSTREAM
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_WRITESECTIONS
#define FABLE_RETAIL_CARRAY_DEALLOCATEUNUSEDMEMORY
#define FABLE_RETAIL_CARRAY_GETBYTESIZE_PASS23
#define FABLE_RETAIL_CARRAY_GETASCARRAY_START
#define FABLE_RETAIL_CARRAY_PUSHBACK
#define FABLE_RETAIL_CARRAY_ORIGINAL_USER_SPECIAL_MEMBERS_PASS22
#define FABLE_RETAIL_CVECTORMAP_INSERT
#define FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE
#define FABLE_RETAIL_CVECTORMAP_INSERT_VALUE_TYPE
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_CMEMORYALLOCATOR_VARIABLESIZE
#define FABLE_RETAIL_CMEMORYALLOCATOR_SAVEBINARY_PASS23
#include "engine/CDataOutputStream.h"
#include "engine/CMemoryDataOutputStream.h"
#include "engine/CPersistContext.h"
#include "engine/CDefinitionManager.h"
#include "engine/CDefClassBase.h"
#include "engine/CMemoryAllocator.h"

void CDefinitionManager::SaveBinaryDefinitions(CDataOutputStream& os)
{
 GDefStringTable.SaveTable(false);
 os.WriteEBOOL(UseSafeBinaryFormat);
 os.WriteULONG(DependencyCheckCRC);
 os.WriteULONG(GDefStringTable.GetTableRandomID());
 unsigned long no_defs=InstantiatedDefs.size();
 os.WriteULONG(no_defs);
 CMemoryDataOutputStream uncompressed_buffer;
 CArray<unsigned short> def_offsets;
 CompressedDefData.clear();CompressedOffsets.clear();
 unsigned long first_compressed_def=0;
 for(long c_def=0;c_def<no_defs;++c_def){
  CInstantiatedDefInfo& def_info=*InstantiatedDefs[c_def];
  def_info.ClassName.TransferBinaryOut(os);
  def_info.InstantiationName.TransferBinaryOut(os);
  os.WriteULONG(def_info.ClassIndex);
  CMemoryDataOutputStream def_os;
  def_info.PDef->InternalSaveBinary(def_os);
  CMemoryDataOutputStream transfer_os;
  CPersistContext transfer_os_context;
  transfer_os_context.Init(transfer_os,UseSafeBinaryFormat);
  def_info.PDef->Transfer(transfer_os_context);
  unsigned long def_transfer_size=transfer_os.GetLength();
  def_os.Write(transfer_os.PeekData(),def_transfer_size);
  unsigned long current_size=uncompressed_buffer.GetLength()+def_offsets.GetByteSize();
  unsigned long projected_size=def_os.GetLength()+current_size+2;
  if(current_size>0&&projected_size>0x8000){
   CompressDefinitionBlock(uncompressed_buffer,def_offsets,first_compressed_def);
   first_compressed_def=c_def;
  }
  unsigned long current_offset=uncompressed_buffer.GetLength();
  def_offsets.push_back((unsigned short)current_offset);
  uncompressed_buffer.Write(def_os.PeekData(),def_os.GetLength());
 }
 CompressDefinitionBlock(uncompressed_buffer,def_offsets,first_compressed_def);
 CompressedOffsets.Insert(no_defs,CompressedDefData.size());
 CompressedOffsets.Update();
 CompressedDefData.DeallocateUnusedMemory();CompressedOffsets.DeallocateUnusedMemory();
 os.WriteULONG(CompressedOffsets.size());
 os.Write(CompressedOffsets.GetAsCArray(),CompressedOffsets.GetByteSize());
 os.WriteSLONG(CompressedDefData.size());
 os.Write(CompressedDefData.GetAsCArray(),CompressedDefData.GetByteSize());
 long mem_start=0,areas_start=0;
 if(CMemoryAllocator::Get()){
  mem_start=CMemoryAllocator::Get()->GetTotalMemoryAllocated();
  areas_start=CMemoryAllocator::Get()->GetNoAllocatedAreas();
 }
 for(long c_def=0;c_def<no_defs;++c_def){
  CInstantiatedDefInfo& def_info=*InstantiatedDefs[c_def];
  def_info.PDef.Reset(0);
 }
 if(CMemoryAllocator::Get()){
  long mem_freed=mem_start-CMemoryAllocator::Get()->GetTotalMemoryAllocated();
  long areas_freed=areas_start-CMemoryAllocator::Get()->GetNoAllocatedAreas();
 }
}

#undef private
#undef protected

bool CResource::UnloadResource(){abort();}
unsigned long CResource::GetUnloadableMemory() const{abort();}
bool CResource::IsResourceLoaded() const{abort();}
void CResource::OnResourceLoaded(){abort();}
bool CResource::CanAbortResourceLoad(long) const{abort();}
void CResource::OnResourceLoadAborted(){abort();}
unsigned long CResource::GetDebugMemoryInUse() const{abort();}
void CResource::ResetDebugMemoryInUse(){abort();}
bool CResourceList::HandleFailedAllocation(unsigned long){abort();}
bool CDefClassBase::UnloadResource(){abort();}
CDefString CDefClassBase::GetInstantiationName() const{abort();}
void CDefClassBase::SetInstantiationName(CDefString){abort();}
void CDefClassBase::AddSubDef(const CCharString&, long, long){abort();}
void CDefClassBase::RemoveSubDef(const CCharString&){abort();}
CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&){abort();}
const CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&) const{abort();}
bool CDefClassBase::IsParentDef() const{abort();}
void CDefClassBase::PostTransfer(){abort();}
void CDefClassBase::InternalSaveBinary(CDataOutputStream&){abort();}
void CDefClassBase::InternalLoadBinary(CDataInputStream&){abort();}
void CDefClassBase::Validate(const CValidateDefinitionInitBase&, const CCharString&) const{abort();}
CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> >* CDefClassBase::GetPSubDefInfoMap(){abort();}
// External callees are controlled test doubles. Every engine object has its real shared class and normal lifetime.
static bool Active,OutputFinished;static unsigned SaveCalls,InitCalls,HeaderCalls,TransferCalls,DeleteCalls,CompressCalls,DeallocateCalls,StatsCalls;
static CDefinitionManager* Manager;static unsigned DefCount,InternalSizes[5],TransferSizes[5],Refs[5];static bool Safe;
static unsigned char Raw[200000],Expected[300000];static unsigned ExpectedSize,ChunkFirst[8],ChunkCount[8],ChunkSizes[8],Chunks;
static unsigned long AllocBytes,AllocAreas;
static unsigned char Pattern(unsigned n,unsigned part,unsigned pos){return (unsigned char)(17+n*31+part*67+pos*13);}
CBaseClass::CBaseClass(){}CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){}CBaseClassNonCopyable::~CBaseClassNonCopyable(){}CDataStream::~CDataStream(){}CDataOutputStream::~CDataOutputStream(){}
CCharString::CCharString():PStringData(0){}CCharString::~CCharString(){assert(!PStringData);}
CWideString::CWideString():PStringData(0){}CWideString::~CWideString(){assert(!PStringData);}
CFailedAllocationHandler::~CFailedAllocationHandler(){}CResourceList::~CResourceList(){}CDefinitionManager::~CDefinitionManager(){}
CResource::CResource():ResourceList(0),PrevResource(0),NextResource(0),ResourceSize(0),LastUsedFrame(0){}CResource::~CResource(){}
CDefPointeeBase::CDefPointeeBase(){}
CDefClassBase::CDefClassBase():PDefManager(0),GlobalIndex(0),Setup(0),Template(0),DefaultValsApplied(0){}
void CIVCountedPointeeBase::Delete()const{abort();}
CDefinitionManager::CInstantiatedDefInfo::CInstantiatedDefInfo():ClassIndex(0){}
CMemoryAllocator* CMemoryAllocator::PMemoryAllocator=0;
CMemoryAllocator::CMemoryAllocator(void*p):MemoryAllocated(0),MemoryAuditBuffer(0),Heap(p),NoAllocations(0),NoAllocators(0),CurrentTrackingID(0){memset(MemoryAllocatedPerTrackingID,0,sizeof(MemoryAllocatedPerTrackingID));memset(AllocatorExists,0,sizeof(AllocatorExists));memset(MaxFixedAllocationSizes,0,sizeof(MaxFixedAllocationSizes));memset(AllocSizeToAllocator,0,sizeof(AllocSizeToAllocator));}
unsigned long CMemoryAllocator::GetTotalMemoryAllocated(){assert(Active&&this==PMemoryAllocator&&OutputFinished&&(StatsCalls==0||StatsCalls==2));++StatsCalls;return AllocBytes;}
unsigned long CMemoryAllocator::GetNoAllocatedAreas(){assert(Active&&this==PMemoryAllocator&&OutputFinished&&(StatsCalls==1||StatsCalls==3));++StatsCalls;return AllocAreas;}
CDefStringTable GDefStringTable;
void CDefStringTable::SaveTable(bool x){assert(Active&&this==&GDefStringTable&&!x&&SaveCalls==0);++SaveCalls;RandomID=0x12340000UL+DefCount;}
static void Put(unsigned char*b,unsigned&n,unsigned long x,unsigned width){for(unsigned k=0;k<width;++k){b[n++]=(unsigned char)x;x>>=8;}}
static void Grow(CArray<char>&a,unsigned n){if(n>(unsigned)(a._M_end_of_storage._M_data-a._M_start)){unsigned cap=n+32;char*p=(char*)malloc(cap);assert(p);unsigned old=(unsigned)a.size();if(old)memcpy(p,a._M_start,old);free(a._M_start);a._M_start=p;a._M_finish=p+old;a._M_end_of_storage._M_data=p+cap;}}
CMemoryDataOutputStream::CMemoryDataOutputStream():CurrentPosition(0){}
CMemoryDataOutputStream::~CMemoryDataOutputStream(){}
void CMemoryDataOutputStream::SetPosition(unsigned long x){assert(x<=Data.size());CurrentPosition=x;}
unsigned long CMemoryDataOutputStream::GetPosition()const{return CurrentPosition;}
void CMemoryDataOutputStream::Close(){abort();}
void CMemoryDataOutputStream::Write(const void*p,long n){assert(Active&&n>=0&&n<200000);unsigned end=(unsigned)(CurrentPosition+n);Grow(Data,end);if(n)memcpy(Data._M_start+CurrentPosition,p,(unsigned)n);CurrentPosition=end;if((unsigned)Data.size()<end)Data._M_finish=Data._M_start+end;}
unsigned long CMemoryDataOutputStream::GetLength()const{return (unsigned long)Data.size();}
const void* CMemoryDataOutputStream::PeekData()const{return Data.empty()?0:Data._M_start;}
void CDataOutputStream::WriteEBOOL(bool x){unsigned char c=x?1:0;Write(&c,1);}
void CDataOutputStream::WriteULONG(unsigned long x){unsigned char b[4];unsigned n=0;Put(b,n,x,4);Write(b,4);}
void CDataOutputStream::WriteSLONG(long x){WriteULONG((unsigned long)x);}
void CPersistContext::Init(CDataOutputStream&s,bool safe){assert(Active&&safe==Safe);++InitCalls;PDefinitionManager=0;PSymbolMap=0;PParser=0;ErrorCallback=0;PStringToClean=0;UseDefaults=false;UseCRCCheck=safe;SingleTransfer=false;Finished=false;Mode=MODE_SAVE_BINARY;Binary=true;PStringToSaveTo=0;PLoadStream=0;PSaveStream=&s;}
template<class T> void CArray<T>::DeallocateUnusedMemory(){assert(Active);++DeallocateCalls;/* The external compactor preserves all live elements. */unsigned n=(unsigned)this->size();if(!n){free(this->_M_start);this->_M_start=this->_M_finish=this->_M_end_of_storage._M_data=0;}else{T*p=(T*)malloc(n*sizeof(T));assert(p);for(unsigned i=0;i<n;++i)new(p+i)T(this->_M_start[i]);std::_Destroy(this->_M_start,this->_M_finish);free(this->_M_start);this->_M_start=p;this->_M_finish=p+n;this->_M_end_of_storage._M_data=p+n;}}
namespace std {template<class I,class C> void __fastcall sort(I first,I last,C comp){assert(Active);for(I i=first;i!=last;++i)for(I j=i;j!=last;++j)if(comp(*j,*i)){typename iterator_traits<I>::value_type t=*i;*i=*j;*j=t;}}}
class FixtureOutput:public CDataOutputStream{public:unsigned char Bytes[300000];unsigned N;FixtureOutput():N(0){}void SetPosition(unsigned long){abort();}unsigned long GetPosition()const{return N;}void Close(){abort();}void Write(const void*p,long n){assert(Active&&SaveCalls==1&&n>=0&&N+n<sizeof(Bytes));if(n)memcpy(Bytes+N,p,(unsigned)n);N+=(unsigned)n;if(N==ExpectedSize)OutputFinished=true;}};
static void DefinitionBytes(CDataOutputStream&s,unsigned id,unsigned part,unsigned n){for(unsigned k=0;k<n;++k)Raw[k]=Pattern(id,part,k);s.Write(Raw,n);}
class FixtureDef:public CDefClassBase{public:unsigned ID;FixtureDef():ID(0){}void Transfer(CPersistContext&c){assert(Active&&ID<DefCount&&InitCalls==ID+1&&HeaderCalls==ID+1&&c.IsSaving()&&c.Mode==CPersistContext::MODE_SAVE_BINARY&&c.UseCRCCheck==Safe&&c.PSaveStream);++TransferCalls;DefinitionBytes(*c.PSaveStream,ID,1,TransferSizes[ID]);}void InternalSaveBinary(CDataOutputStream&s){assert(Active&&ID<DefCount&&HeaderCalls==ID&&TransferCalls==ID);++HeaderCalls;DefinitionBytes(s,ID,0,InternalSizes[ID]);}void Copy(const CDefClassBase*){abort();}unsigned long GetSizeofClass(){return sizeof(*this);}void Delete()const{assert(Active&&OutputFinished&&TransferCalls==DefCount&&DeallocateCalls==2&&CompressCalls==Chunks&&IVRefCount==0&&ID<DefCount&&Refs[ID]==1);++DeleteCalls;AllocBytes-=100+ID;--AllocAreas;}};
static void AppendByte(CArray<unsigned char>&a,unsigned char x){a.push_back(x);}
void CDefinitionManager::CompressDefinitionBlock(CMemoryDataOutputStream&stream,CArray<unsigned short>&offsets,unsigned long first){assert(Active&&this==Manager&&CompressCalls<Chunks);unsigned chunk=CompressCalls++;assert(first==ChunkFirst[chunk]&&offsets.size()==ChunkCount[chunk]&&stream.GetLength()==ChunkSizes[chunk]);unsigned pos=0;for(unsigned k=0;k<offsets.size();++k){assert(offsets[k]==pos);unsigned id=(unsigned)first+k;pos+=InternalSizes[id]+TransferSizes[id];}assert(pos==stream.GetLength());const unsigned char*p=(const unsigned char*)stream.PeekData();pos=0;for(unsigned id=(unsigned)first;id<first+offsets.size();++id){for(unsigned k=0;k<InternalSizes[id];++k)assert(p[pos++]==Pattern(id,0,k));for(unsigned k=0;k<TransferSizes[id];++k)assert(p[pos++]==Pattern(id,1,k));}
 // Controlled external compression encoding, deliberately simple and independently assembled below.
 CompressedOffsets.Insert(first,(unsigned long)CompressedDefData.size());AppendByte(CompressedDefData,(unsigned char)first);AppendByte(CompressedDefData,(unsigned char)offsets.size());for(unsigned k=0;k<offsets.size();++k){AppendByte(CompressedDefData,(unsigned char)offsets[k]);AppendByte(CompressedDefData,(unsigned char)(offsets[k]>>8));}for(unsigned k=0;k<stream.GetLength();++k)AppendByte(CompressedDefData,p[k]);stream.CurrentPosition=0;stream.Data.clear();offsets.clear();}
static void Oracle(){ExpectedSize=0;Put(Expected,ExpectedSize,Safe?1:0,1);Put(Expected,ExpectedSize,0x789abcUL+DefCount,4);Put(Expected,ExpectedSize,0x12340000UL+DefCount,4);Put(Expected,ExpectedSize,DefCount,4);Chunks=0;unsigned first=0,bytes=0,count=0;for(unsigned n=0;n<DefCount;++n){Put(Expected,ExpectedSize,100+n*2,4);Put(Expected,ExpectedSize,101+n*2,4);Put(Expected,ExpectedSize,300+n,4);unsigned size=InternalSizes[n]+TransferSizes[n];if(bytes+count*2>0&&bytes+count*2+size+2>32768){ChunkFirst[Chunks]=first;ChunkCount[Chunks]=count;ChunkSizes[Chunks++]=bytes;first=n;bytes=count=0;}bytes+=size;++count;}ChunkFirst[Chunks]=first;ChunkCount[Chunks]=count;ChunkSizes[Chunks++]=bytes;
 unsigned char data[200000];unsigned data_n=0;unsigned long keys[9],values[9];for(unsigned c=0;c<Chunks;++c){keys[c]=ChunkFirst[c];values[c]=data_n;data[data_n++]=(unsigned char)ChunkFirst[c];data[data_n++]=(unsigned char)ChunkCount[c];unsigned pos=0;for(unsigned k=0;k<ChunkCount[c];++k){Put(data,data_n,pos,2);unsigned id=ChunkFirst[c]+k;pos+=InternalSizes[id]+TransferSizes[id];}for(unsigned id=ChunkFirst[c];id<ChunkFirst[c]+ChunkCount[c];++id){for(unsigned k=0;k<InternalSizes[id];++k)data[data_n++]=Pattern(id,0,k);for(unsigned k=0;k<TransferSizes[id];++k)data[data_n++]=Pattern(id,1,k);}}
 keys[Chunks]=DefCount;values[Chunks]=data_n;Put(Expected,ExpectedSize,Chunks+1,4);for(unsigned c=0;c<=Chunks;++c){unsigned char pair_bytes[sizeof(std::pair<unsigned long,unsigned long>)];memset(pair_bytes,0,sizeof(pair_bytes));memcpy(pair_bytes,&keys[c],sizeof(unsigned long));memcpy(pair_bytes+sizeof(unsigned long),&values[c],sizeof(unsigned long));memcpy(Expected+ExpectedSize,pair_bytes,sizeof(pair_bytes));ExpectedSize+=sizeof(pair_bytes);}Put(Expected,ExpectedSize,data_n,4);memcpy(Expected+ExpectedSize,data,data_n);ExpectedSize+=data_n;
}
int main(){unsigned cases=0;const unsigned sizes[]={0,1,2,16380,16384,32760,32764,32766,32767,32768,32769};{
 CMemoryAllocator allocator(0);CDefinitionManager manager;Manager=&manager;FixtureDef defs[5];CDefinitionManager::CInstantiatedDefInfo info[5];for(unsigned k=0;k<5;++k){defs[k].ID=k;info[k].ClassName.TablePos=100+k*2;info[k].InstantiationName.TablePos=101+k*2;info[k].ClassIndex=300+k;}
 for(unsigned n=0;n<=5;++n)for(unsigned pattern=0;pattern<11;++pattern)for(unsigned split=0;split<3;++split)for(unsigned safe=0;safe<2;++safe)for(unsigned mem=0;mem<2;++mem)for(unsigned spare=0;spare<2;++spare){
  Active=false;DefCount=n;Safe=safe!=0;AllocBytes=10000;AllocAreas=100;CMemoryAllocator::PMemoryAllocator=mem?&allocator:0;manager.InstantiatedDefs.clear();manager.CompressedDefData.clear();manager.CompressedOffsets.clear();for(unsigned k=0;k<5;++k){unsigned total=sizes[(pattern+k*3)%11];InternalSizes[k]=split==0?total:split==1?total/2:0;TransferSizes[k]=total-InternalSizes[k];Refs[k]=(k+pattern)%2+1;defs[k].IVRefCount=Refs[k];info[k].PDef.Object=0;if(k<n){info[k].PDef.Object=&defs[k];manager.InstantiatedDefs.push_back(&info[k]);}}
  if(spare){for(unsigned k=0;k<100;++k)manager.CompressedDefData.push_back(0xcc);manager.CompressedOffsets.Insert(999,999);}manager.UseSafeBinaryFormat=Safe;manager.DependencyCheckCRC=0x789abcUL+n;Oracle();FixtureOutput output;SaveCalls=InitCalls=HeaderCalls=TransferCalls=DeleteCalls=CompressCalls=DeallocateCalls=StatsCalls=0;unsigned char records[sizeof(info)],manager_before[sizeof(manager)],defs_before[sizeof(defs)];memcpy(records,info,sizeof(info));memcpy(manager_before,&manager,sizeof(manager));memcpy(defs_before,defs,sizeof(defs));OutputFinished=false;Active=true;manager.SaveBinaryDefinitions(output);Active=false;assert(output.N==ExpectedSize&&memcmp(output.Bytes,Expected,ExpectedSize)==0);assert(SaveCalls==1&&InitCalls==n&&HeaderCalls==n&&TransferCalls==n&&CompressCalls==Chunks&&DeallocateCalls==2&&StatsCalls==(mem?4U:0U));unsigned deletes=0;for(unsigned k=0;k<n;++k){assert(info[k].PDef.Object==0&&defs[k].IVRefCount==Refs[k]-1);if(Refs[k]==1)++deletes;}assert(DeleteCalls==deletes&&manager.InstantiatedDefs.size()==n&&manager.UseSafeBinaryFormat==Safe&&manager.DependencyCheckCRC==0x789abcUL+n);for(unsigned k=0;k<5;++k){CDefPointeeBase* zero=0;unsigned off=(unsigned)((unsigned char*)&info[k].PDef.Object-(unsigned char*)info);memcpy(records+off,&zero,sizeof(zero));unsigned long ref=Refs[k]-(k<n?1:0);off=(unsigned)((unsigned char*)&defs[k].IVRefCount-(unsigned char*)defs);memcpy(defs_before+off,&ref,sizeof(ref));}assert(memcmp(records,info,sizeof(info))==0&&memcmp(defs_before,defs,sizeof(defs))==0);unsigned begin=(unsigned)((unsigned char*)&manager.CompressedDefData-(unsigned char*)&manager),end=(unsigned)((unsigned char*)&manager.CompressedOffsets+sizeof(manager.CompressedOffsets)-(unsigned char*)&manager);memcpy(manager_before+begin,(unsigned char*)&manager+begin,end-begin);assert(memcmp(manager_before,&manager,sizeof(manager))==0);++cases;
 }
 CMemoryAllocator::PMemoryAllocator=0;manager.InstantiatedDefs.clear();assert(cases==1584);
 }puts("009af7a0_TEST PASS");return 0;}