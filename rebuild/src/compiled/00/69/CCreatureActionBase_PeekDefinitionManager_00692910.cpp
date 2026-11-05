// CCreatureActionBase::PeekDefinitionManager() const -- the acting creature's definition manager (PCreature->PeekDefinitionManager())
// Leaf: ?PeekDefinitionManager@CCreatureActionBase@@IBEABVCGameDefinitionManager@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00692910, 15 bytes: add ecx,8 ; call GetPItem ; mov ecx,eax ; jmp 0x004C7990.
// Identity (proven, chunk c667): Ego_r.pdb + FableWin.pdb (protected const); the landed 0x0087CA30 binds it so; no
//   GetDefinitionManager / GetPDefinitionManager member of CCreatureActionBase / CCombatActionBase in either PDB.
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50 (PCreature +0x08, through CIntelligentPointer<T>::operator->),
//   CThing::PeekDefinitionManager() const 0x004C7990 (Ego_r ?PeekDefinitionManager@CThing@@QBEABVCGameDefinitionManager@@XZ, tail).
// Layout: Ego_r.pdb CCreatureActionBase PCreature (CIntelligentPointer<CThingCreatureBase>) +0x08, as the landed 0x0087CA30 model.
// Naming wave 16: landed as the free __fastcall Dest_val(void*) calling dv_h2_004c7990; respelled on the PDB member
//   (name-only: the bytes are unchanged).
class CBaseObject;
class CBaseObjectPointer;
class CGameDefinitionManager;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* operator->() const { return (T*)GetPItem(); }
};
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#include "engine/CThing.h"
class CThingCreatureBase : public CThing {
};

class CCreatureActionBase {
public:
    virtual ~CCreatureActionBase();
protected:
    const CGameDefinitionManager& PeekDefinitionManager() const;
    unsigned char _pad_0x04[0x4];                        // +0x04
    CIntelligentPointer<CThingCreatureBase> PCreature;   // +0x08
};

const CGameDefinitionManager& CCreatureActionBase::PeekDefinitionManager() const
{
    return PCreature->PeekDefinitionManager();
}
