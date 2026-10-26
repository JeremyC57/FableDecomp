// CResource::CResource() -- default constructor: reference count 1 (counted-pointee base), own vtable, not in a list, unlinked, size and last-used frame 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FBEC0, length 33 bytes (+15 int3 padding).
// Leaf: ??0CResource
// Retail bytes:  mov eax,ecx ; xor ecx,ecx ; mov dword ptr [eax+4],1 ; mov dword ptr [eax],offset CResource::`vftable' ;
//                mov [eax+8],ecx ; mov [eax+0Ch],ecx ; mov [eax+10h],ecx ; mov [eax+14h],ecx ; mov [eax+18h],ecx ; ret
// Vtables (rtti-vtables.tsv): 0x012354A4 = CResource; its base CIVCountedPointeeBase (0x012354D0) is constructed inline
// and its own vtable store is dead (dropped). Members are the PDB ones (engine/CResource.h; the unnamed +4 word is the
// counted-pointee reference count). A local polymorphic model is used (the generated header flattens the vptr).
// Shape: the base's count store precedes the derived vtable store; the five derived initialisers share one zero register.
class CResourceList;

// engine/CIVCountedPointeeBase.h names the count IVRefCount (+4); its generated header flattens the vptr.
class CIVCountedPointeeBase { public:               // vtable 0x012354D0
    virtual ~CIVCountedPointeeBase();
    long IVRefCount;                           // +0x04
    CIVCountedPointeeBase() : IVRefCount(1) { }
};

class CResource : public CIVCountedPointeeBase { public:   // vtable 0x012354A4, 0x1C bytes
    CResourceList* ResourceList;             // +0x08
    CResource*     PrevResource;             // +0x0C
    CResource*     NextResource;             // +0x10
    unsigned long  ResourceSize;             // +0x14
    unsigned long  LastUsedFrame;            // +0x18
    CResource();
    virtual ~CResource();
};

CResource::CResource()
    : ResourceList(0), PrevResource(0), NextResource(0), ResourceSize(0), LastUsedFrame(0)
{
}