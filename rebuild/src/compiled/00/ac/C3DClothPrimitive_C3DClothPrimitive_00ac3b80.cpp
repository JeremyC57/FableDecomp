// C3DClothPrimitive::~C3DClothPrimitive() -- release the counted ParticleProgram pointer, then the CBaseClassNonCopyable base destructor
// Leaf: ??1C3DClothPrimitive@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC3B80, 66 bytes, thiscall, tail `jmp ~CBaseClassNonCopyable`.
// Identity (proven): stores vtable 0x0129DF74 first -- RTTI .?AVC3DClothPrimitive@@ (rtti-vtables.tsv), whose slot 0 `??_G`
//   0x00A96000 is the sole caller; Ego_r.pdb and FableWin.pdb both spell ??1C3DClothPrimitive@@UAE@XZ (virtual through
//   CBaseClass). Ego_r members: ParticleProgram CCountedPointer<CParticleProgram> +4, PrimitiveIndex +0xC, MaterialIndex +0x10
//   (0x14 bytes). The masked byte twin 0x004AA840 is ~CScriptThing (chunk 696; ~CBaseClass tail).
// Callees: operator delete 0x00BFE9BC (the CCPPointerInfo block), the info block's DeleteFunc through +4,
//   CBaseClassNonCopyable::~CBaseClassNonCopyable 0x0099A300 (protected virtual, ??1CBaseClassNonCopyable@@MAE@XZ).
// Shape: an empty user destructor (the vtable store comes first); the ParticleProgram member destructor is the inline
//   CCountedPointer<T>::~CCountedPointer of engine/CCountedPointer.h (dec, Info reload, DeleteFunc, delete Info, Data/Info zeroed).
// Layout: local C3DClothPrimitive (no engine header exists) over engine/CBaseClassNonCopyable.h.
void __cdecl operator delete(void* block);              // 0x00BFE9BC
#include "engine/CCountedPointer.h"
#include "engine/CBaseClassNonCopyable.h"

class CParticleProgram;
class C3DMeshFileClothChunk;

class C3DClothPrimitive : public CBaseClassNonCopyable {     // vtable 0x0129DF74, 0x14 bytes
public:
    C3DClothPrimitive(const C3DMeshFileClothChunk& chunk);
    virtual ~C3DClothPrimitive();
    CCountedPointer<CParticleProgram> ParticleProgram;      // +0x04
    unsigned long PrimitiveIndex;                            // +0x0C
    unsigned long MaterialIndex;                             // +0x10
};

C3DClothPrimitive::~C3DClothPrimitive()
{
}