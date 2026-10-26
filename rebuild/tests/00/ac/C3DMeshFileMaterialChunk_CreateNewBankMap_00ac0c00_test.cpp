// C3DMeshFileMaterialChunk::CreateMap(EMaterialMapType) -- attach and return an MMAP material-map chunk.
// Retail00ac0c00,222bytes. PDB ?CreateMap@C3DMeshFileMaterialChunk@@QAE?AV?$CCountedPointer@VC3DMeshFileMapChunk@@@@W4EMaterialMapType@@@Z.
// Shared donor superchunk34 vs native2c (12-byte vectors); map native3c vs donor44. Native views retain real virtual hierarchy.
// AA8360 is CChunkedFileSuperChunk ctor (RTTI129e4f0), AA8490 its dtor; mapVT129ee34, implicit mapdtor deleting slotAC1960.
// Narrow CCharString ctor99e4b0 increments13bd800; dtor99eae0. AddSubChunkAA84f0; callbackAC1460.
#ifdef _MSC_VER
#include "fable_string.h"
#else
// Exact retained fable_string.h declarations; host64 disables retail pointer-size assertions.


#include <stddef.h>

typedef unsigned int fable_u32;typedef int fable_i32;typedef unsigned char fable_u8;
#define FABLE_FASTCALL
#define FABLE_STATIC_ASSERT(x)

class CWideStringData
{ public:
    wchar_t* text;
    fable_u32 unknown04;
    fable_u32 unknown08;
    fable_i32 owners;
};

#pragma pack(push, 1)
class CCharStringData
{ public:
    char* text;
    fable_u32 unknown04;
    fable_u32 unknown08;
    fable_u8 flags0C;
    fable_i32 owners;
};
#pragma pack(pop)

class CCharString;

class CWideString
{
public:
    CWideString();
    CWideString(const CWideString& other);
    explicit CWideString(const wchar_t* text);
    ~CWideString();
    operator const wchar_t*() const;
    long GetLength() const;
    CWideString& operator=(const CWideString& other);
    const CWideString& operator=(const wchar_t* text);
    const CWideString& operator+=(const CWideString& other);
    const CWideString& operator+=(const wchar_t* text);
    bool operator!=(const wchar_t* other) const;

    static CWideString FABLE_FASTCALL CreateFromCharString(
        const CCharString& text);

private:
    explicit CWideString(const CCharString& text);
    void UnassignString();
    CWideStringData* AllocStringData(
        const wchar_t* text,
        long length);

    CWideStringData* storage_;
};

CWideString FABLE_FASTCALL operator+(
    const CWideString& left,
    const CWideString& right);
CWideString FABLE_FASTCALL operator+(
    const CWideString& left,
    const wchar_t* right);
CWideString FABLE_FASTCALL operator+(
    const wchar_t* left,
    const CWideString& right);

class CCharString
{
public:
    CCharString();
    CCharString(const char* text, long length);
    ~CCharString();
    operator const char*() const;
    CCharString& operator=(const CCharString& other);
    const CCharString& operator=(const char* text);
    CWideString ToWideString() const;

private:
    void UnassignString();
    CCharStringData* AllocStringData(
        const char* text,
        long length);

    CCharStringData* storage_;
};

CCharString FABLE_FASTCALL operator+(
    const CCharString& left,
    const char* right);

// Retail folds equivalent narrow/wide string lifetime helpers. These counters
// remain separate because their addresses and object families are distinct.
extern fable_i32 g_CWideStringInstanceCount_013BCA20;
extern fable_i32 g_CCharStringInstanceCount_013BD800;
extern wchar_t g_FableEmptyWideString_0129A8E0[1];

FABLE_STATIC_ASSERT(sizeof(CWideStringData) == 0x10);
FABLE_STATIC_ASSERT(sizeof(CCharStringData) == 0x11);
FABLE_STATIC_ASSERT(sizeof(CWideString) == 0x04);
FABLE_STATIC_ASSERT(sizeof(CCharString) == 0x04);

#endif
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

#include <cstdio>
#include <cstring>
static bool ok=true;static int stringAlive,stringCtors,stringDtors,chunkDtors,superDtors,registered;static unsigned int dispatched;
static CountedPointer<NativeChunk>* retained;static MaterialChunkMethods* expectedParent;
CCharString::CCharString():storage_(0){++stringAlive;++stringCtors;}
CCharString::CCharString(const char* text,long length):storage_(new CCharStringData){++stringAlive;++stringCtors;if(length<0)length=(long)std::strlen(text);storage_->text=new char[length+1];std::memcpy(storage_->text,text,length);storage_->text[length]=0;storage_->owners=1;}
CCharString::~CCharString(){--stringAlive;++stringDtors;if(storage_&&--storage_->owners==0){delete[] storage_->text;delete storage_;}}
CCharString::operator const char*() const{return storage_?storage_->text:"";}
NativeChunk::~NativeChunk(){++chunkDtors;}
const CCharString NativeChunk::GetChunkDescription() const{return CCharString("base",4);}
const CCharString NativeChunk::GetChunkInfo() const{return CCharString("baseinfo",8);}
int NativeChunk::IsSuperChunk() const{return 0;}int NativeChunk::IsUnknownType() const{return 0;}
const NativeSuperChunk* NativeChunk::GetAsSuperChunk() const{return 0;}NativeSuperChunk* NativeChunk::GetAsSuperChunk(){return 0;}
NativeChunk* NativeChunk::InitNewInstance(NativeChunk* p) const{return p;}
NativeSuperChunk::NativeSuperChunk(const char* type){ok=ok&&std::strcmp(type,"MMAP")==0;std::memcpy(ChunkType,type,5);Parent=0;SubChunks.Start=SubChunks.Finish=SubChunks.EndOfStorage=0;RegisteredSubChunkTypes.Start=RegisteredSubChunkTypes.Finish=RegisteredSubChunkTypes.EndOfStorage=0;CurrentChunkIndex=0;}
NativeSuperChunk::~NativeSuperChunk(){++superDtors;}
int NativeSuperChunk::IsSuperChunk() const{dispatched|=1<<3;return 1;}
const NativeSuperChunk* NativeSuperChunk::GetAsSuperChunk() const{dispatched|=1<<5;return this;}
NativeSuperChunk* NativeSuperChunk::GetAsSuperChunk(){dispatched|=1<<6;return this;}
void NativeSuperChunk::ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&){ok=false;}
void NativeSuperChunk::WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const{ok=false;}
NativeChunk* NativeSuperChunk::InitNewInstance(NativeChunk* p) const{dispatched|=1<<10;return p;}
const CCharString C3DMeshFileMapChunk::GetChunkDescription() const{dispatched|=1<<1;return CCharString("map",3);}
const CCharString C3DMeshFileMapChunk::GetChunkInfo() const{dispatched|=1<<2;return CCharString("info",4);}
class CDataInputStream{ public:};class CDataOutputStream{ public:};class CChunkedFileLoader{ public:};class CChunkedFileSaver{ public:};
void C3DMeshFileMapChunk::ReadFromFile(CDataInputStream&,unsigned long n,CChunkedFileLoader&){dispatched|=1<<7;ok=ok&&n==17;}
void C3DMeshFileMapChunk::WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const{dispatched|=1<<8;}
NativeChunk* C3DMeshFileMapChunk::CreateNewInstance() const{dispatched|=1<<9;return 0;}
void __fastcall DeleteMapChunk(void* p){delete static_cast<C3DMeshFileMapChunk*>(p);}
void NativeSuperChunk::AddSubChunk(const CountedPointer<NativeChunk>& p){
 ok=ok&&this==reinterpret_cast<NativeSuperChunk*>(expectedParent)&&p.Data&&p.Info&&p.Info->RefCount==2&&p.Info->Data==p.Data&&p.Info->DeleteFunc==DeleteMapChunk;
 C3DMeshFileMapChunk* map=static_cast<C3DMeshFileMapChunk*>(p.Data);ok=ok&&map->MapType==registered+5&&std::strcmp(map->MapDescription,"")==0&&std::strcmp(map->Filename,"")==0&&std::strcmp(map->ChunkType,"MMAP")==0;
 ++registered;retained=new CountedPointer<NativeChunk>(p);
}
typedef NativeChunk* (NativeSuperChunk::*InitMethod)(NativeChunk*) const;
struct InitAccess:NativeSuperChunk {static InitMethod GetInit(){return &InitAccess::InitNewInstance;}};
int main(){
#ifdef _MSC_VER
 ok=ok&&sizeof(NativeChunk)==0x10&&sizeof(NativeSuperChunk)==0x2c&&sizeof(C3DMeshFileMapChunk)==0x3c;
#endif
 MaterialChunkMethods parent;expectedParent=&parent;
 for(int n=0;n<2;++n){{MapPointer result=parent.CreateMap(static_cast<EMaterialMapType>(n+5));ok=ok&&result.Data==retained->Data&&result.Info==retained->Info&&result.Info->RefCount==2&&result.Data->MapType==n+5&&chunkDtors==n&&stringAlive==2;
 NativeChunk* base=result.Data;const NativeChunk* cbase=base;dispatched=0;
 {const CCharString description=cbase->GetChunkDescription();const CCharString info=cbase->GetChunkInfo();ok=ok&&std::strcmp(description,"map")==0&&std::strcmp(info,"info")==0&&stringAlive==4;}
 ok=ok&&stringAlive==2&&cbase->IsSuperChunk()==1&&cbase->IsUnknownType()==0&&cbase->GetAsSuperChunk()==result.Data&&base->GetAsSuperChunk()==result.Data;
 CDataInputStream in;CDataOutputStream out;CChunkedFileLoader loader;CChunkedFileSaver saver;base->ReadFromFile(in,17,loader);cbase->WriteToFile(out,saver);ok=ok&&!cbase->CreateNewInstance()&&(cbase->GetAsSuperChunk()->*InitAccess::GetInit())(base)==base;
#ifdef _MSC_VER
 // Call retail numeric slots independently of the C++ declaration order.
 // One-receiver fastcall passes ecx and has no stack args, matching these no-arg thiscall methods.
 typedef const NativeSuperChunk* (__fastcall *ConstSlot)(const NativeChunk*);
 typedef NativeSuperChunk* (__fastcall *MutableSlot)(NativeChunk*);
 void** slots=*reinterpret_cast<void***>(base);unsigned int beforeSlots=dispatched;
 dispatched=0;ok=ok&&reinterpret_cast<ConstSlot>(slots[5])(cbase)==result.Data&&dispatched==(1u<<5);
 dispatched=0;ok=ok&&reinterpret_cast<MutableSlot>(slots[6])(base)==result.Data&&dispatched==(1u<<6);
 dispatched=beforeSlots;
#endif
 ok=ok&&dispatched==((1<<1)|(1<<2)|(1<<3)|(1<<5)|(1<<6)|(1<<7)|(1<<8)|(1<<9)|(1<<10));}
 ok=ok&&retained->Info->RefCount==1&&chunkDtors==n;delete retained;retained=0;ok=ok&&chunkDtors==n+1&&superDtors==n+1&&stringAlive==0;}
 ok=ok&&stringCtors==stringDtors;std::printf("00ac0c00_TEST %s\n",ok?"PASS":"FAIL");return ok?0:1;}