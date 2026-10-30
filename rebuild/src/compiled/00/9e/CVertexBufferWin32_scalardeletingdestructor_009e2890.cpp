// CVertexBufferWin32::`scalar deleting destructor'(unsigned int flags) -- the inline destructor (this class's vtable, its cleanup call, the base destructor), then free this when bit 0 of flags is set
// Leaf: ??_GCVertexBufferWin32@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009E2890, 43 bytes, thiscall, `ret 4`, returns this.
// Identity (proven): it stores vtable 0x0129BA20 (RTTI .?AVCVertexBufferWin32@@, bases CVertexBufferBase : CBaseClassNonCopyable : CBaseClass); the vtable's slot 0.
// Callees: CVertexBufferWin32::Uninitialise() 0x00A63200, CBaseClassNonCopyable::~CBaseClassNonCopyable() 0x0099A300, operator delete 0x00BFE9BC.
// Shape (family K048, representative 0x00987F30): the compiler's scalar deleting destructor around a one-call inline
//   destructor; a `new` of the class in this file makes VC7.1 emit the vtable and the ??_G COMDAT.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();                             // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();                    // 0x0099A300
};
class CVertexBufferBase : public CBaseClassNonCopyable {   // RTTI base; its destructor is implicit (a user-declared
public:                                                  //   empty one would store its own vptr before the base call)
    CVertexBufferBase() { }
};
class CVertexBufferWin32 : public CVertexBufferBase {   // RTTI: CVertexBufferBase : CBaseClassNonCopyable : CBaseClass
public:
    CVertexBufferWin32() { }
    virtual ~CVertexBufferWin32() { Uninitialise(); }
    void Uninitialise();                                 // 0x00A63200 (Ego_r.pdb)
};
CVertexBufferWin32* MakeBuffer_009e2890() { return new CVertexBufferWin32; }