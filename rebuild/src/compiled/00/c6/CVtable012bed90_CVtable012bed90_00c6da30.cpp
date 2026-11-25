// CVtable_012bed90::~CVtable_012bed90() -- empty virtual destructor of the GameCODA class with vtable 0x012BED90: own vtable, then the base CVtable_012beda4 destructor (tail jump)
// Leaf: ??1CVtable_012bed90@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6DA30, 11 bytes: `mov dword ptr [ecx],offset vtable 0x012BED90 ; jmp 0x00C6E630`.
// Identity: a destructor is named by the vtable it stores first. 0x012BED90 has no complete-object locator (no-RTTI
//   GameCODA module, no PDB), so the class is named by its vtable as its landed constructor 0x00C6D9D0 is (CVtable_<va>, the
//   catalog's ??_7 convention). That constructor calls the base constructor 0x00C6E6A0 first, and the base destructor
//   0x00C6E630 (CVtable_012beda4, named by its own stored vtable) is the jump target here.
//   The base destructor ends by restoring the GameCODA root vtable 0x012B5868 (CCodaObjectBase, owner decision D3), and the
//   class's deleting destructor 0x00C6E1D0 frees through CodaFree 0x00C3C370, so the hierarchy roots at CCodaObjectBase.
//   The catalog label "CChunkedFileChunk::~CChunkedFileChunk" is propagated across this byte-twin shape and is wrong.
// Shape: an empty user destructor of a derived polymorphic class stores its own vtable and tail-jumps to the out-of-line
//   base destructor.
// NF444 binder: the base is the shared engine/CSalRender.h (GameCODA SDK class; was the local model CVtable_012beda4). This row keeps
//   its own symbol; only the base destructor it calls is respelled (??1CVtable_012beda4@@UAE@XZ -> ??1CSalRender@@UAE@XZ, unit 0x00C6E630).
#include "engine/CSalRender.h"   // CSalRender (vtable 0x012BEDA4; destructor 0x00C6E630)
class CVtable_012bed90 : public CSalRender {
public:
    virtual ~CVtable_012bed90();
    virtual void Process(const short*, int*(&)[4]);   // slots 2-4: own overrides in retail 0x012BED90 (SDK signatures;
    virtual void SetGain(float, const float*, bool);  //   the landed method rows still carry their stand-in spellings)
    virtual void SetPosition(float, float, float, float, const float*, bool, bool);
};

CVtable_012bed90::~CVtable_012bed90()
{
}