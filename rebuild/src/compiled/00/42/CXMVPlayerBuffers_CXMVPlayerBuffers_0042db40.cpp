// CXMVPlayerBuffers::CXMVPlayerBuffers() -- construct the XMV movie player's buffer holder with no buffers allocated
// Leaf: ??0CXMVPlayerBuffers@@QAE@XZ
// Retail: Fable.exe @ 0x0042DB40, 29 bytes, __fastcall.
// Identity (proven): stores vtable 0x01230C34 (RTTI CXMVPlayerBuffers, bases CBaseClassNonCopyable @0 -> CBaseClass);
// Ego_r.pdb / FableWin.pdb ??0CXMVPlayerBuffers@@QAE@XZ is the class's only constructor; the caller
// CNewFrontendGameComponent::Run (call at 0x0042EF9C) creates the buffers for the intro movies (scoped_ptr<...>).
// Callee: CBaseClassNonCopyable::CBaseClassNonCopyable() @ 0x0099A2F0 (protected in the PDB, ??0...@@IAE@XZ).
// Layout (Ego_r.pdb): vptr +0, InputBuffer +4, PlaybackBuffer +8, CurrentSize +0xC; size 0x10.
// Shape: the base call, the three member zeroes (one `xor eax,eax`), then the vtable store (VC7.1 sinks it below the
// member initialisers).
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();                                // 0x0099A2F0
};

class CXMVPlayerBuffers : public CBaseClassNonCopyable {
public:
    char* InputBuffer;                                      // +0x04
    char* PlaybackBuffer;                                   // +0x08
    unsigned long CurrentSize;                              // +0x0C
    CXMVPlayerBuffers();
    virtual ~CXMVPlayerBuffers();
};

CXMVPlayerBuffers::CXMVPlayerBuffers()
    : InputBuffer(0),
      PlaybackBuffer(0),
      CurrentSize(0)
{
}