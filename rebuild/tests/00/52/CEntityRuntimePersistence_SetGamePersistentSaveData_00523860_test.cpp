// NThingManager::CEntityRuntimePersistence::SetGamePersistentSaveData(long,const void*,unsigned long) -- resize and compress a script save.
// Retail 00523860,173 bytes; resize entity array0052c6a0, compressor methods00a3a7d0/00a3aea0/00a3aa10/00a3add0/00a3acc0.
#ifdef _MSC_VER
#include "engine/CCompressorZlib.h"
#include "fable_string.h"
#else
// Pointer-size host view of the PDB string; native uses fable_string.h.
class CCharString { public:void* storage; CCharString(); ~CCharString();};
#endif
// Native compressor is 0x58 rather than donor 0x5c: STL vector owns 12 rather than 16 bytes.
// Include the donor header for provenance; use the native ABI view below because its stream is +1c and flags +54.
template<class T> class CArray { public:
 T* first; T* last; T* end;
 unsigned long size() const { return (unsigned long)(last-first); }
 T* data() { return size()?first:(T*)this; }
 const T* data() const { return size()?first:(const T*)this; }
 void resize(unsigned long,const T&); // unsigned char 00486189; entity scripts 0052c6a0 uses single argument below
};
struct CEntityScriptInfo { CCharString Script; CArray<unsigned char> SaveData; long UncompressedSize; bool Valid; };
struct CEntityScriptArray: CArray<CEntityScriptInfo> { void resize(unsigned long); }; //0052c6a0
unsigned long GetZlibWorkspaceSize(); //00a3a7d0 returns zero
class CCompressionInfo;
struct CACompressorNative {
 virtual ~CACompressorNative(); //Base deleting dtor009c6f40 cleans OwnWorkspace then calls ABase dtor0099a2e0; nondeleting cleanup is inlined.
 virtual void CompressAsImage(void*,unsigned long,void*,unsigned long,long,long,long,CCompressionInfo&); //0052add0 slot1
 virtual void Compress(void*,unsigned long,void*,unsigned long,CCompressionInfo&); //0052ade0 slot2
 virtual void Decompress(void*,unsigned long,void*,unsigned long,CCompressionInfo&); //0052adf0 slot3
 virtual void SetQuality(float); //0052ae00 slot4
 void* Workspace; unsigned long WorkspaceSize; bool CompressionOn; unsigned char reserved[3];
 CArray<unsigned char> OwnWorkspace;
 CACompressorNative(long); //00a3aea0
};
struct CCompressorZlibNative: CACompressorNative {
 unsigned char Stream[0x38];bool ZlibInitialised,Compressing,ErrorFlag;
 CCompressorZlibNative():CACompressorNative(GetZlibWorkspaceSize()) { Init(); }
 virtual ~CCompressorZlibNative(); //00a3acc0
 void Init(); //00a3aa10
 // Const input overloads are new virtuals, not overrides of base nonconst input methods.
 virtual void Compress(const void*,unsigned long,void*,unsigned long,CCompressionInfo&); //00a3ac40 slot5
 virtual void CompressAsImage(const void*,unsigned long,void*,unsigned long,long,long,long,CCompressionInfo&); //0052ae10 slot6
 virtual void Decompress(const void*,unsigned long,void*,unsigned long,CCompressionInfo&); //00a3abe0 slot7
 unsigned long InternalCompress(const void*,unsigned long,CArray<unsigned char>&); //00a3add0
 unsigned long InternalDecompress(const void*,unsigned long,void*,unsigned long); //00a3ab00
};
namespace NThingManager {
class CEntityRuntimePersistence { public:
 CArray<CEntityScriptInfo> LevelPersistentEntityScripts;
 CEntityScriptArray GamePersistentEntityScripts;
 bool GetGamePersistentSaveData(long,CArray<unsigned char>&) const;
 void SetGamePersistentSaveData(long,const void*,unsigned long);
};
}
void NThingManager::CEntityRuntimePersistence::SetGamePersistentSaveData(long index,const void* data,unsigned long size) {
 unsigned long needed=index+1;
 GamePersistentEntityScripts.resize(needed>GamePersistentEntityScripts.size()?needed:GamePersistentEntityScripts.size());
 CEntityScriptInfo& info=GamePersistentEntityScripts.first[index];
 info.UncompressedSize=size;
 if(size>0) {
  CCompressorZlibNative compressor;
  compressor.CCompressorZlibNative::InternalCompress(data,size,info.SaveData);
 }
 info.Valid=true;
}

#include <stdio.h>
#include <stddef.h>
#include <string.h>
static int phase,failed,constructs,destructs,resizeCalls;
static unsigned long wantedResize;
static CEntityScriptInfo records[8];
static unsigned char output[512],packed[512];
unsigned long GetZlibWorkspaceSize() { if(phase) failed=1;phase=1;return 0; }
CACompressorNative::CACompressorNative(long size) { if(phase!=1 || size) failed=1; phase=2;++constructs; }
void CCompressorZlibNative::Init() { if(phase!=2) failed=1;phase=3; }
CCompressorZlibNative::~CCompressorZlibNative() { if(phase!=4) failed=1;phase=0;++destructs; }
unsigned long CCompressorZlibNative::InternalCompress(const void* p,unsigned long n,CArray<unsigned char>& out) {
 if(phase!=3 || !n) failed=1;phase=4;
 out.first=packed;out.last=packed+n;out.end=packed+512;
 for(unsigned long i=0;i<n;++i) packed[i]=((const unsigned char*)p)[i]^0x5a;
 return n;
}
unsigned long CCompressorZlibNative::InternalDecompress(const void* p,unsigned long n,void* q,unsigned long size) {
 if(phase!=3 || !n || n!=size) failed=1;phase=4;
 for(unsigned long i=0;i<n;++i) ((unsigned char*)q)[i]=((const unsigned char*)p)[i]^0x5a;
 return n;
}
template<class T> void CArray<T>::resize(unsigned long n,const T& fill) {
 if(n>512 || fill) {failed=1;return;}
 first=(T*)output;last=first+n;end=first+512;
 for(unsigned long i=0;i<n;++i) output[i]=fill;
}
void CEntityScriptArray::resize(unsigned long n) {
 ++resizeCalls;if(n!=wantedResize || n>8) {failed=1;return;}
 unsigned long old=size();first=records;last=records+n;end=records+8;
 for(unsigned long i=old;i<n;++i) {records[i].Valid=false;records[i].UncompressedSize=0;records[i].SaveData.first=records[i].SaveData.last=records[i].SaveData.end=packed;}
}

int main() {
#ifdef _MSC_VER
 if(sizeof(CEntityScriptInfo)!=0x18 || offsetof(CEntityScriptInfo,UncompressedSize)!=0x10 || offsetof(CEntityScriptInfo,Valid)!=0x14 || offsetof(NThingManager::CEntityRuntimePersistence,GamePersistentEntityScripts)!=0xc || sizeof(CCompressorZlibNative)!=0x58 || offsetof(CCompressorZlibNative,Stream)!=0x1c || offsetof(CCompressorZlibNative,ZlibInitialised)!=0x54) return 90;
#endif
 NThingManager::CEntityRuntimePersistence x;
 x.GamePersistentEntityScripts.first=records;x.GamePersistentEntityScripts.last=records;x.GamePersistentEntityScripts.end=records+8;
 unsigned char data[3]={0,17,255};wantedResize=3;
 x.SetGamePersistentSaveData(2,data,3);
 if(x.GamePersistentEntityScripts.size()!=3 || !records[2].Valid || records[2].UncompressedSize!=3 || records[0].Valid || constructs!=1 || destructs!=1 || phase || failed || packed[0]!=(0^0x5a) || packed[1]!=(17^0x5a) || packed[2]!=(255^0x5a)) return 1;
 x.SetGamePersistentSaveData(0,0,0);
 if(!records[0].Valid || records[0].UncompressedSize || x.GamePersistentEntityScripts.size()!=3 || constructs!=1 || destructs!=1 || resizeCalls!=2 || failed) return 2;
 unsigned char* savedFirst=records[2].SaveData.first;unsigned char* savedLast=records[2].SaveData.last;
 x.SetGamePersistentSaveData(2,0,0);
 if(!records[2].Valid || records[2].UncompressedSize || constructs!=1 || resizeCalls!=3 || failed) return 3;
 if(records[2].SaveData.first!=savedFirst||records[2].SaveData.last!=savedLast||packed[1]!=(17^0x5a)) return 4;
 puts("00523860_TEST PASS");return 0;
}

CCharString::CCharString() {}
CCharString::~CCharString() {}
CACompressorNative::~CACompressorNative() {}
void CACompressorNative::CompressAsImage(void*,unsigned long,void*,unsigned long,long,long,long,CCompressionInfo&) {failed=1;}
void CACompressorNative::Compress(void*,unsigned long,void*,unsigned long,CCompressionInfo&) {failed=1;}
void CACompressorNative::Decompress(void*,unsigned long,void*,unsigned long,CCompressionInfo&) {failed=1;}
void CACompressorNative::SetQuality(float) {failed=1;}
void CCompressorZlibNative::Compress(const void*,unsigned long,void*,unsigned long,CCompressionInfo&) {failed=1;}
void CCompressorZlibNative::CompressAsImage(const void*,unsigned long,void*,unsigned long,long,long,long,CCompressionInfo&) {failed=1;}
void CCompressorZlibNative::Decompress(const void*,unsigned long,void*,unsigned long,CCompressionInfo&) {failed=1;}