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