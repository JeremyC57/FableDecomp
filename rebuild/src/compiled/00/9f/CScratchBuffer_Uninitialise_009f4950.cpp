// CScratchBuffer::Uninitialise() -- when the buffer was initialised, free the scratch block (operator delete[]) and clear the pointer, then run the base uninitialisation
// Leaf: ?Uninitialise@CScratchBuffer@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F4950, 43 bytes, thiscall, tail `jmp CInitBaseClass::Uninitialise`.
// Identity (proven): Ego_r.pdb / FableWin.pdb ?Uninitialise@CScratchBuffer@@QAEXXZ (non-virtual; Ego_r orders Initialise /
//   Uninitialise / Lock / Unlock, retail 0x009F4910 Initialise (landed) / this row / 0x009F4980); the only caller is
//   CSystemManager::Uninitialise 0x009A60D0, which calls it on the global scratch buffer 0x013CB498 (the landed
//   g_ScratchBuffer, a CScratchBuffer: its ctor 0x009F4A00 stores vtable 0x0129C790, RTTI .?AVCScratchBuffer@@ : CInitBaseClass
//   : CBaseClass) after testing IsValid.
// Callees: CInitBaseClass::IsValid 0x0099A330 (?IsValid@CInitBaseClass@@QBE_NXZ), operator delete[] 0x00BFEB1C (??_V@YAXPAX@Z,
//   MSVCR71 import thunk), CInitBaseClass::Uninitialise 0x0099A360 (?Uninitialise@CInitBaseClass@@IAEXXZ: protected, both PDBs;
//   the bound UAEXXZ / QAEXXZ spellings are stand-ins, NAME_FIXES.tsv).
// Layout = Ego_r.pdb CScratchBuffer (0x14): CInitBaseClass {vptr, bool Valid +4}, Buffer +8, Locked +0xc, NoBytes +0x10.
//   CInitBaseClass vtable 0x0129A7CC: slot 0 scalar deleting dtor 0x0099A490, slot 1 Validate 0x0099A340 (UBEXXZ); CScratchBuffer
//   0x0129C790 inherits slot 1.
// Shape: the Initialise twin's nested-if style: the delete is guarded by its own null test, the pointer cleared inside it.
void __cdecl operator delete[](void* block);              // 0x00BFEB1C ??_V@YAXPAX@Z (import thunk)
#include "engine/CBaseClass.h"                              // CBaseClass() 0x0099A2D0, virtual ~CBaseClass 0x0099A2E0

class CInitBaseClass : public CBaseClass {                  // vtable 0x0129A7CC, 8 bytes
public:
    CInitBaseClass();                                       // ??0CInitBaseClass@@QAE@XZ
    virtual ~CInitBaseClass();                              // slot 0 (??_G 0x0099A490)
    bool IsValid() const;                                   // 0x0099A330
    virtual void Validate() const;                          // slot 1  0x0099A340
protected:
    bool Initialise();                                      // 0x0099A350 ?Initialise@CInitBaseClass@@IAE_NXZ
    void Uninitialise();                                    // 0x0099A360 ?Uninitialise@CInitBaseClass@@IAEXXZ
private:
    bool Valid;                                             // +0x04
};

class CScratchBuffer : public CInitBaseClass {              // vtable 0x0129C790, 0x14 bytes
public:
    CScratchBuffer();                                       // 0x009F4A00
    virtual ~CScratchBuffer();
    bool Initialise(long noBytes);                          // 0x009F4910
    void Uninitialise();
    bool Lock(void** pointer);                              // 0x009F4980
    void Unlock();
    long GetByteSize() const;
    bool IsLocked() const;
private:
    unsigned char* Buffer;                                  // +0x08
    bool           Locked;                                  // +0x0c
    long           NoBytes;                                 // +0x10
};

void CScratchBuffer::Uninitialise()
{
    if (IsValid()) {
        if (Buffer) {
            delete[] Buffer;
            Buffer = 0;
        }
    }
    CInitBaseClass::Uninitialise();
}