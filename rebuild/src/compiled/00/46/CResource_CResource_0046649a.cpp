// CResource::CResource(const CResource& other) -- compiler-generated copy constructor: base CIVCountedPointeeBase (its vtable, the reference count), then CResource's vtable and its five words
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046649A, length 57 bytes (+13 int3 padding).
// Leaf: ??0CResource
// Retail bytes:  mov eax,ecx ; mov ecx,[esp+4] ; mov dword ptr [eax],offset CIVCountedPointeeBase::`vftable' ; mov edx,[ecx+4] ;
//                mov [eax+4],edx ; mov dword ptr [eax],offset CResource::`vftable' ; mov edx,[ecx+8] ; mov [eax+8],edx ; ... ;
//                mov ecx,[ecx+18h] ; mov [eax+18h],ecx ; ret 4
// Vtables (rtti-vtables.tsv): 0x012354D0 = CIVCountedPointeeBase, 0x012354A4 = CResource. Members are the PDB ones
// (engine/CResource.h: +8 ResourceList, +0xC PrevResource, +0x10 NextResource, +0x14 ResourceSize, +0x18 LastUsedFrame;
// the unnamed +4 word is the counted-pointee base's reference count). The generated header holds the vptr as a plain
// word, so a local polymorphic model is used.
// Shape: the base vtable store survives in a copy constructor (the source words read after it may alias `this`,
// so the store is not dead -- a default constructor drops it); the implicit copy constructor moves every member,
// floats included, with integer moves. VC7.1 emits the implicit copy constructor out of line, so the placement-new
// helper only makes the TU emit it -- it is not part of the retail body.
class CResourceList;

// engine/CIVCountedPointeeBase.h names the count IVRefCount (+4); its generated header flattens the vptr.
class CIVCountedPointeeBase { public:               // vtable 0x012354D0
    virtual ~CIVCountedPointeeBase();
    long IVRefCount;                           // +0x04
};

class CResource : public CIVCountedPointeeBase { public:   // vtable 0x012354A4, 0x1C bytes
    CResourceList* ResourceList;             // +0x08
    CResource*     PrevResource;             // +0x0C
    CResource*     NextResource;             // +0x10
    unsigned long  ResourceSize;             // +0x14
    unsigned long  LastUsedFrame;            // +0x18
    virtual ~CResource();
};

inline void* __cdecl operator new(unsigned int, void* where) { return where; }

void CopyResourceInto_0046649a(void* where, const CResource& other)
{
    new (where) CResource(other);
}