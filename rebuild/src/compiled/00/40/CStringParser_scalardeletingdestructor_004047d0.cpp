// CStringParser::`vector deleting destructor'(unsigned int flags) -- destroy a background landscape patch and free it when bit 0 is set
// Retail: Fable.exe (TLC, VC7.1) @ 0x004047D0, 56 bytes. Callees: member destructors at +0x14, +0x10, +0x0C, the class destructor, operator delete.
//
// Vtable slot 0 of the patch class. A background patch is one tile of the distant landscape (the
// low-detail terrain you see beyond the playable area); it owns a few sub-objects plus its texture and
// buffers. `delete patch` arrives here with flags = 1, destroying the three owned sub-objects in reverse
// order, running the class destructor for the rest (the texture and index/vertex buffers), and then
// freeing the storage; array destruction arrives with flags = 0 and skips the free. Returns `this` so
// the array form can step on. The three sub-object offsets sit inside what the PDB calls EdgeStrips, so
// the attached class name appears to be a propagated label for a shared destructor shape.
//
// Matching: `flags` is the stack argument after the unused edx register; `flags & 1` is a `test ...,1`;
// `operator delete` is the plain global one.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
struct CInner { void* v; };
extern void __fastcall Inner14_dtor(void* p);   // destructor of the sub-object at +0x14
extern void __fastcall Inner_dtor(void* p);     // destructor of the sub-objects at +0x10 and +0x0C
extern void __cdecl operator_delete_stub(void* p);
void operator delete(void* p);

class CStringParser { public:
    void* vftable;
    CInner inner4;                  // parent tree node
    void* f8;
    CInner innerC;                  // owned sub-object (destroyed here)
    unsigned char _pad_0x10[0xa8];  // further owned sub-objects at +0x10 / +0x14, edge strips
    void* tex;                      // the patch texture (released by the class destructor)
};

void* __fastcall CStringParser_scalar_deleting_destructor(CStringParser* parser, int edx_dummy, unsigned int flags)
{
    // Owned sub-objects in reverse order, then the rest of the patch.
    Inner14_dtor((char*)parser + 0x14);
    Inner_dtor((char*)parser + 0x10);
    Inner_dtor((char*)parser + 0xc);
    ((CBaseClass*)(parser))->CBaseClass::~CBaseClass();
    // Bit 0: the caller used `delete`, so give the storage back too.
    if (flags & 1)
        operator delete(parser);
    return parser;
}
