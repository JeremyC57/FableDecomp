// NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageGraveyardKeeper::Alloc(CQ_OpeningGraveyardSecretPassageScript& parent, CScriptDataBase* data, const CScriptThing& me) -- entity-script factory of the "SecretPassageGraveyardKeeper" binding: new CSecretPassageGraveyardKeeper(parent, data, me) (CEntityScriptBase(*parent.Game, me), ParentClass = &parent, PMasterData = data) handed to a fresh CCountedPointer<CEntityScriptBase>
// Leaf: ?Alloc@CSecretPassageGraveyardKeeper@CQ_OpeningGraveyardSecretPassageScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DC4AA0, 145 bytes (+ int3 padding), static `__fastcall` (hidden result
//   pointer ecx, parent edx, data and me on the stack), `ret 8`, returns the result pointer.
// Identity: Ego_r.pdb ?Alloc@CSecretPassageGraveyardKeeper@CQ_OpeningGraveyardSecretPassageScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z is exactly the symbol this source emits.
//   CQ_OpeningGraveyardSecretPassageScript::Main 0x00DC27C0 (slot 2 of the script vtable 0x012DA11C, which the "Q_OpeningGraveyardSecretPassage" script's Alloc stores at its CScriptInfoManager::RegisterScripts registration) stores this address as the factory of the CEntityScriptBinding<CQ_OpeningGraveyardSecretPassageScript> record named "SecretPassageGraveyardKeeper" (site 0x00DC2A0F).
//   Was landed as _global::Create_CVtable_012da658 (named by the entity vtable address); same shape as the family-pass F014 Alloc rows.
// Callees: scalar operator new 0x00BFEA1A (0x24 = sizeof(CSecretPassageGraveyardKeeper), then 0x0C for the count block),
//   CScriptThing::CScriptThing(const CScriptThing&) 0x004ABE90 (Me), and the count block's deleter
//   0x00CDEE00 (stored, not called: `if (p) p->~CEntityScriptBase()` deleting, `__fastcall`).
// Data: vtables CEntityScriptBase 0x012C3224 (stored before the Me copy) and CSecretPassageGraveyardKeeper 0x012DA658.
// Layout: Ego_r.pdb CQ_OpeningGraveyardSecretPassageScript::CSecretPassageGraveyardKeeper (36 bytes = the retail new size): ParentClass +0x14, PMasterData +0x18, already_talked_to +0x1C, Helmfound +0x1D, Swordfound +0x1E, Shieldfound +0x1F, Armourfound +0x20.
//   CEntityScriptBase: vptr, Game +0x04, Me +0x08 (Ego_r 20 bytes). The parent's Game is CLevelScriptBase's +0x40.
// Shape: the PDB ctor (parent, data, me) inlined: base and ParentClass in the init list, PMasterData assigned in the body;
//   STLport-free counted pointer ctor: Data first, the null test on the argument, the 12-byte {1, deleter, Data} block
//   read back from Data, three exits (block, failed new, null object).
class CGameScriptInterfaceBase;
class CScriptDataBase;

void __fastcall Destroy_00cdee00(void* object);        // 0x00CDEE00 (catalog name; the CEntityScriptBase deleter)

class CCPPointerInfo {                                  // 12 bytes
public:
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void*);
    void* Data;
    CCPPointerInfo(void* data, void (__fastcall* deleteFunc)(void*)) : RefCount(1), DeleteFunc(deleteFunc), Data(data) { }
};

template <class T> class CCountedPointer {
public:
    T* Data;
    CCPPointerInfo* Info;
    CCountedPointer(T* data) : Data(data)
    {
        if (data)
            Info = new CCPPointerInfo(Data, Destroy_00cdee00);
        else
            Info = 0;
    }
};

// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_OpeningGraveyardSecretPassageScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CSecretPassageGraveyardKeeper : public CEntityScriptBase {
    public:
        CQ_OpeningGraveyardSecretPassageScript* ParentClass; // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        bool already_talked_to;                         // +0x1C
        bool Helmfound;                                 // +0x1D
        bool Swordfound;                                // +0x1E
        bool Shieldfound;                               // +0x1F
        bool Armourfound;                               // +0x20
        CSecretPassageGraveyardKeeper(CQ_OpeningGraveyardSecretPassageScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        virtual ~CSecretPassageGraveyardKeeper();
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_OpeningGraveyardSecretPassageScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

CCountedPointer<CEntityScriptBase> __fastcall CQ_OpeningGraveyardSecretPassageScript::CSecretPassageGraveyardKeeper::Alloc(CQ_OpeningGraveyardSecretPassageScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CSecretPassageGraveyardKeeper(parent, data, me));
}
}
