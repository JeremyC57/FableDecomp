// CArenaCreatureDef::~CArenaCreatureDef() -- implicit destructor of the arena creature definition: destroy HUDType and CreatureType, then the inline CDefClassBase / CDefPointeeBase destructors (CDefPointeeBase vtable reset) and ~CResource
// Leaf: ??1CArenaCreatureDef@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F25E40, 33 bytes, thiscall, tail `jmp ~CResource`.
// Identity (proven): callers are the scalar deleting destructor 0x00F25E20 (slot 0 of the RTTI-less vtable 0x012FB358, the
//   vtable the ctor 0x00F263D0 stores; slot 20 GetSizeofClass 0x00F25E10 returns 0x38) and the two CArenaCreatureDef vector
//   helpers 0x00F26366 (CPersistContext::TransferVectorOfSubComponents<CArenaCreatureDef>) / 0x00F268E1
//   (CPersistContext_TransferableVectorOfSubComponents<CArenaCreatureDef>::Resize: a by-value temporary built by 0x00F263D0
//   and destroyed here; element stride 0x38 in the neighbouring Size/AreDefaultValsApplied rows). Ego_r.pdb names
//   ??1CArenaCreatureDef@@UAE@XZ (donor 0x00CF0310) and lays the class out as 56 bytes with CCharString CreatureType +0x28,
//   long NumCreatures +0x2C, CCharString HUDType +0x30, long DeathScore +0x34: exactly the two ~CCharString calls here.
//   No 0x012FB358 store: the destructor is the implicit one. The catalog label
//   "CAIStateGroup_HoverHornetQueen::~CAIStateGroup_HoverHornetQueen" is propagated.
// Callees: CCharString::~CCharString 0x0099EAE0 (x2), CResource::~CResource 0x009FC550.
// Shape: the two member destructors in reverse order, engine/CDefClassBase.h's inline ~CDefClassBase (dead store dropped) and
//   engine/CDefPointeeBase.h's inline ~CDefPointeeBase (store 0x01230BA0), then the out-of-line ~CResource. DestroyOne (a
//   destruction by name, ungraded; chunk 656/657 pattern) makes VC7.1 emit the out-of-line implicit `??1`.
// Layout: CDefClassBase is 0x25 bytes packed (the header), so the 4-aligned CreatureType sits at +0x28.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"

class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString();                                          // 0x0099E4B0
    ~CCharString();                                         // 0x0099EAE0
};

class CArenaCreatureDef : public CDefClassBase {            // vtable 0x012FB358 (no RTTI), 0x38 bytes
public:
    CArenaCreatureDef();                                    // 0x00F263D0
    virtual void Transfer(CPersistContext& context);       // slot 18 0x00F25E70
    virtual void Copy(const CDefClassBase* other);           // slot 19 0x00F25F00
    virtual unsigned long GetSizeofClass();                 // slot 20 0x00F25E10
    CCharString CreatureType;                               // +0x28
    long        NumCreatures;                               // +0x2c
    CCharString HUDType;                                    // +0x30
    long        DeathScore;                                 // +0x34
};

void DestroyOne(CArenaCreatureDef* def)
{
    def->CArenaCreatureDef::~CArenaCreatureDef();
}