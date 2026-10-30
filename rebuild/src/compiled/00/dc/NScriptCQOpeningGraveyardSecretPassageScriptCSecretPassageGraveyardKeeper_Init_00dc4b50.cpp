// NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageGraveyardKeeper::Init() -- reset the script's state: already_talked_to, Helmfound, Swordfound, Shieldfound, Armourfound = 0/false
// Leaf: ?Init@CSecretPassageGraveyardKeeper@CQ_OpeningGraveyardSecretPassageScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DC4B50, 18 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 of the entity-script vtable 0x012DA658 (stored by the landed CSecretPassageGraveyardKeeper::Alloc); FableWin.pdb
//   ?Init@CSecretPassageGraveyardKeeper@CQ_OpeningGraveyardSecretPassageScript@NScript@@EAEXXZ; the members it clears are Ego_r.pdb's CSecretPassageGraveyardKeeper fields at their PDB offsets.
// Shape (family K066, representative 0x00CF06D0): one zero register (al, or eax when a long is cleared) and one store per
//   member in source order; every state member is cleared.
class CScriptDataBase;
class CGameScriptInterfaceBase;
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CScriptBase;
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_OpeningGraveyardSecretPassageScript : public CLevelScriptBase<NScript::CQ_SunnyvaleMasterData> {
public:
    class CSecretPassageGraveyardKeeper : public CEntityScriptBase {
    private:
        virtual void Init();
    public:
        CQ_OpeningGraveyardSecretPassageScript* ParentClass;                             // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        bool  already_talked_to;                       // +0x1C
        bool  Helmfound;                               // +0x1D
        bool  Swordfound;                              // +0x1E
        bool  Shieldfound;                             // +0x1F
        bool  Armourfound;                             // +0x20
    };
};

void CQ_OpeningGraveyardSecretPassageScript::CSecretPassageGraveyardKeeper::Init()
{
    already_talked_to = false;
    Helmfound = false;
    Swordfound = false;
    Shieldfound = false;
    Armourfound = false;
}
}