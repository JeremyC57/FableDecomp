// 00681ba0 CParticleMorphs_TransferEntry
// Flags: /O2 /Oy /W3
// Leaf: Transfer
// Shape: the write/read choice is a named bool (gives retail's write-first block order); the default argument of Write is
//   DefaultString() (default ctor into the inline local, copy ctor into the result, dtor) so all three strings home in the
//   dead ctx/name/arr argument slots and no frame is allocated.
#include <new>
void* __cdecl operator new[](unsigned int);
void __cdecl operator delete[](void*);

// Layout: 00681ba0 string data has buffer +0 and length +4; context mode +0x18.
// The CCharString empty text 0x0129AAF4 is isolated so it does not rebind the pooled "" another landed file names EmptyString.
namespace NCharStringPool {extern const char EmptyString[];} using NCharStringPool::EmptyString;
#pragma pack(push,1)
struct Data {char* text;int size;unsigned capacity;bool fast;int refs;};
#pragma pack(pop)
struct CCharString { Data* data;CCharString();CCharString(const char*,int=-1);CCharString(const CCharString&);~CCharString();void AppendCharacter(char);const char* Text()const{return !data?EmptyString:data->text;}int Size()const{return data?data->size:0;} CCharString Mutate();};
Data* MakeData(const char*,int);Data* ShareData(Data*);void DropData(Data*);

struct CPersistContext {char pad[24];int mode;void Write(const char*,CCharString&,CCharString&);void Read(const char*,CCharString&);};
namespace _STL {template<class T>struct vector {T* first;T* last;T* end;};}
typedef _STL::vector<bool> BoolVector;
__forceinline CCharString DefaultString(){CCharString s;return s;}
struct CPersistTraits {void Transfer(CPersistContext*,CCharString*,BoolVector*);};
void CPersistTraits::Transfer(CPersistContext* ctx,CCharString* name,BoolVector* arr){
 bool writing=ctx->mode==1||ctx->mode==3;if(writing){CCharString value("");for(bool* p=arr->first,*end=arr->last;p!=end;++p)value.AppendCharacter(*p?'T':'F');const char* key=name->Text();ctx->Write(key,value,DefaultString());return;}
{CCharString value("");ctx->Read(name->Text(),value);if(value.data&&value.data->size>0&&value.data->size==arr->last-arr->first){const char* p=value.data->text;int n=arr->last-arr->first;for(int i=0;i<n;++i)arr->first[i]=p[i]=='T';}}

}
