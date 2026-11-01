// CThingHolySite::~CThingHolySite() -- compiler-generated destructor: release the Def pointer (count at +4, virtual Delete at zero, cleared), then ~CThingGameObject by tail jump
// Leaf: ??1CThingHolySite@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00529AD0, 41 bytes: `push esi ; mov esi,ecx ; mov ecx,[esi+0D0h] ; test ecx,ecx ; je T ;
//   dec dword ptr [ecx+4] ; jne N ; mov eax,[ecx] ; call [eax+4] ; N: mov dword ptr [esi+0D0h],0 ; T: mov ecx,esi ;
//   pop esi ; jmp 0x00838A00`.
// Identity (proven): the only caller is CThingHolySite's scalar deleting destructor 0x00529AB0 (vtable 0x012452DC slot 0, call
//   at 0x00529AB3), which sits beside it; the member at +0xD0 is CThingHolySite::Def (Ego_r.pdb CDefPointer<const
//   CThingHolySiteDef> at +0xE8 after the 0xE8-byte PDB CThingGameObject; retail CThingGameObject is 0xD0, and the landed
//   CThingHolySite::Alloc 0x0052AC90 inlines the constructor that zeroes +0xD0 after storing vtable 0x012452DC). The tail jump
//   goes to CThingGameObject::~CThingGameObject 0x00838A00 (??1CThingGameObject@@UAE@XZ in both PDBs). No ??1CThingHolySite in
//   either PDB: compiler-generated, hence no vtable store. The catalog label `~C3DAnimationBlendState` is propagated.
// Callees: the def's virtual Delete (CIVCountedPointeeBase slot 1, `call [eax+4]`), CThingGameObject::~CThingGameObject 0x00838A00.
// Shape: the Def member's inline CDefPointer<T>::~CDefPointer -> Release() from engine/CDefPointer.h.
// Layout: retail CThingGameObject is 0xD0 bytes (12-byte STLport containers in CThing); engine/CThingGameObject.h is the
//   flattened PDB layout without virtuals, so the base is modelled locally at its retail size. PlayerStart +0xD4 (PDB +0xEC).
// Emission: the TU harness below destroys a holy site by name; it is not a retail function (retail allocates holy sites
//   through CThingHolySite::Alloc and deletes them through the vtable).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"                 // CDefPointer<T> over CDefPointeeBase : CResource : CIVCountedPointeeBase

class CThingHolySiteDef;

class CThingGameObject {                                    // retail sizeof 0xD0
public:
    virtual ~CThingGameObject();                            // 0x00838A00
private:
    unsigned char _body[0xd0 - 4];
};

class CThingHolySite : public CThingGameObject {            // vtable 0x012452DC
public:
    CThingHolySite();                                       // inlined into CThingHolySite::Alloc 0x0052AC90
    CDefPointer<const CThingHolySiteDef> Def;               // +0xD0
    unsigned char                        PlayerStart;       // +0xD4
};

// TU harness (not a retail function).
void TUHarness_DestroyHolySite()
{
    CThingHolySite site;
}