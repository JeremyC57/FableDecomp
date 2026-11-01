// CHeroSoulsCreatureDef::~CHeroSoulsCreatureDef() -- implicit destructor: destroy Location and CreatureType, then the inline CDefClassBase / CDefPointeeBase destructors and ~CResource
// Leaf: ??1CHeroSoulsCreatureDef@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F270D0, 33 bytes, thiscall: ~CCharString(+0x2C), ~CCharString(+0x28), store the
//   CDefPointeeBase vtable 0x01230BA0, `jmp ~CResource` 0x009FC550.
// Identity: callers are the scalar deleting destructor 0x00F270B0 (slot 0 of the CHeroSoulsCreatureDef vtable 0x012FB500,
//   the table the landed copy constructor 0x00F27060 stores; slot 18 Transfer 0x00F27100 persists CreatureType / Location /
//   NumCreatures), the landed CPersistContext Transfer of a CHeroSoulsCreatureDef 0x00F274D0 and the landed
//   TransferableVectorOfSubComponents<CHeroSoulsCreatureDef>::Resize 0x00F27B60. Ego_r.pdb ??1CHeroSoulsCreatureDef@@UAE@XZ
//   (donor 0x00DA06F0, next to ??_G 0x00DA0750); layout CDefClassBase + CCharString CreatureType +0x28, CCharString
//   Location +0x2C, long NumCreatures +0x30 (52 bytes). Already the bound spelling in reloc_bindings.tsv. The manifest
//   "CAIStateGroup_HoverHornetQueen::~..." and the twin 0x004D9861 are other functions.
// Shape: compiler-generated destructor (no CHeroSoulsCreatureDef vtable store); the inline ~CDefClassBase store is dead
//   behind the inline ~CDefPointeeBase store, which is the one retail keeps (engine/CDefClassBase.h note: 0x0042DAB0 does
//   the same). An emission helper forces the ??1 COMDAT.
class CCharStringData;
class CCharString {
public:
    ~CCharString();                                     // 0x0099EAE0
    CCharStringData* PStringData;
};

#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                      // : CDefPointeeBase (vtable 0x01230BA0) : CResource (~CResource 0x009FC550)

class CHeroSoulsCreatureDef : public CDefClassBase {    // vtable 0x012FB500; slot 0 0x00F270B0
public:
    CCharString CreatureType;                           // +0x28
    CCharString Location;                               // +0x2C
    long        NumCreatures;                           // +0x30
    virtual void Transfer(CPersistContext&);            // slot 18 0x00F27100
    virtual void Copy(const CDefClassBase*);            // slot 19 0x00F27180
    virtual unsigned long GetSizeofClass();             // slot 20 0x00F270A0
};

// Emission helper (not a retail symbol): forces the implicit virtual destructor out of line as its own COMDAT.
void EmitHeroSoulsCreatureDefDestructor(CHeroSoulsCreatureDef* def)
{
    def->CHeroSoulsCreatureDef::~CHeroSoulsCreatureDef();
}