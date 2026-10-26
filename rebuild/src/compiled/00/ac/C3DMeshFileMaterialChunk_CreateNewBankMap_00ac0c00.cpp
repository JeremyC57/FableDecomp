// C3DMeshFileMaterialChunk::CreateMap(EMaterialMapType) -- attach and return an MMAP material-map chunk.
// Retail00ac0c00,222bytes. PDB ?CreateMap@C3DMeshFileMaterialChunk@@QAE?AV?$CCountedPointer@VC3DMeshFileMapChunk@@@@W4EMaterialMapType@@@Z.
// Shared donor superchunk34 vs native2c (12-byte vectors); map native3c vs donor44. Native views retain real virtual hierarchy.
// AA8360 is CChunkedFileSuperChunk ctor (RTTI129e4f0), AA8490 its dtor; mapVT129ee34, implicit mapdtor deleting slotAC1960.
// Narrow CCharString ctor99e4b0 increments13bd800; dtor99eae0. AddSubChunkAA84f0; callbackAC1460.
#include "fable_string.h"
#ifdef _MSC_VER
#include "engine/CChunkedFileSuperChunk.h"
#include "engine/C3DMeshFileMaterialChunk.h"
#else
#define __fastcall
#define __forceinline inline
#endif
class CDataInputStream;class CDataOutputStream;class CChunkedFileLoader;class CChunkedFileSaver;
struct NativeSuperChunk;
template<class T> struct CountedPointer;
struct NativeChunk {
 virtual ~NativeChunk(); //AA75C0
 virtual const CCharString GetChunkDescription() const; //AA75D0
 virtual const CCharString GetChunkInfo() const; //AA75F0
 virtual int IsSuperChunk() const; //AA7620
 virtual int IsUnknownType() const; //AA7610
 // VC7.1 emits same-name overloads in reverse declaration order: native constslot5,mutableslot6.
 virtual NativeSuperChunk* GetAsSuperChunk(); //AA7630 slot6
 virtual const NativeSuperChunk* GetAsSuperChunk() const; //AA7640 slot5
 virtual void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&)=0;
 virtual void WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const=0;
 virtual NativeChunk* CreateNewInstance() const=0;
protected:
 virtual NativeChunk* InitNewInstance(NativeChunk*) const; //AA7690
public:
 char ChunkType[5];NativeSuperChunk* Parent;
};
struct NativeChunkVector {void* Start;void* Finish;void* EndOfStorage;};
struct NativeSuperChunk:NativeChunk {
 NativeSuperChunk(const char*); //AA8360, four-character type ctor
 virtual ~NativeSuperChunk(); //AA8490
 virtual int IsSuperChunk() const; //AA76A0
 virtual NativeSuperChunk* GetAsSuperChunk(); //AA76B0 slot6
 virtual const NativeSuperChunk* GetAsSuperChunk() const; //AA76C0 slot5
 virtual void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&); //AA8830
 virtual void WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const; //AA78C0
protected:
 virtual NativeChunk* InitNewInstance(NativeChunk*) const; //AA8460
public:
 void AddSubChunk(const CountedPointer<NativeChunk>&); //AA84F0
 NativeChunkVector SubChunks,RegisteredSubChunkTypes;unsigned long CurrentChunkIndex;
};
enum EMaterialMapType { MATERIAL_MAP_UNSPECIFIED=-1 };
class C3DMeshFileMapChunk:public NativeSuperChunk { public:
 EMaterialMapType MapType;CCharString MapDescription,Filename;unsigned long CRC;
 C3DMeshFileMapChunk(EMaterialMapType type):NativeSuperChunk("MMAP"),MapType(type){}
 virtual const CCharString GetChunkDescription() const; //AC1290
 virtual const CCharString GetChunkInfo() const; //ABFE80
 virtual void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&); //AC03F0
 virtual void WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const; //ABFF70
 virtual NativeChunk* CreateNewInstance() const; //ABFFB0
};
void __fastcall DeleteMapChunk(void*); //AC1460: if(p) delete static_cast<C3DMeshFileMapChunk*>(p)
class CCPPointerInfo { public:unsigned long RefCount;void (__fastcall *DeleteFunc)(void*);void* Data;CCPPointerInfo(void* p):RefCount(1),DeleteFunc(DeleteMapChunk),Data(p){} };
template<class T> struct CountedPointer {
 T* Data;CCPPointerInfo* Info;
 __forceinline CountedPointer(T* p):Data(p),Info(p?new CCPPointerInfo(p):0){}
 __forceinline CountedPointer(const CountedPointer& p):Data(p.Data),Info(p.Info){if(Info)++Info->RefCount;}
 template<class U> __forceinline CountedPointer(const CountedPointer<U>& p):Data(p.Data),Info(p.Info){if(Info)++Info->RefCount;}
 __forceinline ~CountedPointer(){if(Info){--Info->RefCount;if(Info->RefCount==0){Info->DeleteFunc(Info->Data);delete Info;}}}
};
typedef CountedPointer<C3DMeshFileMapChunk> MapPointer;
// Material this is a superchunk subobject; no material fields are read by CreateMap.
struct MaterialChunkMethods {MapPointer CreateMap(EMaterialMapType);};
MapPointer MaterialChunkMethods::CreateMap(EMaterialMapType type){MapPointer result(new C3DMeshFileMapChunk(type));reinterpret_cast<NativeSuperChunk*>(this)->AddSubChunk(result);return result;}