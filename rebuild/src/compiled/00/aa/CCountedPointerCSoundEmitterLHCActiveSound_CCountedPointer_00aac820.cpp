// CCountedPointer<CSoundEmitterLH::CActiveSound>::CCountedPointer(CSoundEmitterLH::CActiveSound* data) -- adopt the active sound: store it and, when non-null, allocate its 12-byte count block {1, DeleteData, Data}
// Leaf: ??0?$CCountedPointer@VCActiveSound@CSoundEmitterLH@@@@QAE@PAVCActiveSound@CSoundEmitterLH@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AAC820, 78 bytes, thiscall `ret 4`.
// Name (agentNF3, 2026-11-10): the counted-pointer constructor of the emitter's active-sound list (CSoundEmitterLH::PlayAtmos
//   0x00AAC1D0 calls it out of line for `CCountedPointer<CActiveSound>(new CActiveSound(...))`, landed with this spelling).
//   CActiveSound is a class (`V`): vtable 0x0129E67C (RTTI CSoundEmitterLH::CActiveSound), Anniversary 360 RTTI
//   ??_7CActiveSound@CSoundEmitterUN@@6BCBaseClassNonCopyable@@@ and ?DeleteData@?$CCountedPointer@VCActiveSound@CSoundEmitterUN@@@@SAXPAX@Z.
//   The count block stores 0x00AAC6C0 = DeleteData of the same instantiation (`delete (CActiveSound*)p`, the virtual deleting
//   destructor). Was landed as a descriptive CCountedPointer_00aac6c0::CCountedPointer_00aac6c0 with a Destroy_00aac6c0 deleter.
// Body: engine/CCountedPointer.h (T*) constructor: Data stored, the argument tested, the block built from the stored Data
//   (`mov ecx,[esi]`), Info = 0 when new fails or the pointer is null.
#include "engine/CCountedPointer.h"
#include "engine/CBaseClassNonCopyable.h"
class CSoundEmitterLH {
public:
    class CActiveSound : public CBaseClassNonCopyable {   // vtable 0x0129E67C; DeleteData = virtual deleting dtor
    public:
        virtual ~CActiveSound();
    };
};
template CCountedPointer<CSoundEmitterLH::CActiveSound>::CCountedPointer(CSoundEmitterLH::CActiveSound* data);
