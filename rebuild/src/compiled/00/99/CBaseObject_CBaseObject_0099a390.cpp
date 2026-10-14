// CBaseObject::CBaseObject(const CBaseObject&) -- copy constructor: a copy observes nobody's pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099A390, oracle length 16 bytes.
// Callees: none.
//
// Retail bytes:  mov eax,ecx ; mov dword ptr [eax],offset CBaseObject::`vftable' ; mov dword ptr [eax+4],0 ; ret 4
// The vtable stored is 0x0129A7D8 = CBaseObject (rtti-vtables.tsv), so the manifest's
// "CBaseIntelligentPointer" label is a propagated mis-attribution; `ret 4` makes it the one-argument
// (copy) constructor: the IntelligentPointer record (engine/CBaseObject.h, +0x04) is NOT shared with
// the copy, it starts out detached.  The argument is unused.  The vtable is pulled in by a plain
// virtual function so no `??_G` is emitted next to the constructor.
struct CBaseObjectPointer;

struct CBaseObject_Retail {            // engine/CBaseObject.h has the same two members but no real vptr
    virtual void v00();
    CBaseObjectPointer* IntelligentPointer;    // +0x04
    CBaseObject_Retail(const CBaseObject_Retail& other);     // 0x0099A390
};

CBaseObject_Retail::CBaseObject_Retail(const CBaseObject_Retail& other)
    : IntelligentPointer(0)
{
}