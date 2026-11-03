#ifndef FABLE_ENGINE_NUISYSTEM_CKEYREDEFINER_H
#define FABLE_ENGINE_NUISYSTEM_CKEYREDEFINER_H
// NUISystem::CKeyRedefiner (new, chunk 726). Ego_r.pdb CKeyRedefiner (504 bytes): PActionText +452, PKeyText +456,
// Actions list<EGameAction> +460, SubType +472, Definition CActionInputControl +476. Retail sits on the 0x194-byte retail
// CClickable and STLport's 4-byte list: PActionText +0x194, PKeyText +0x198, Actions +0x19C (the destructor 0x00558660 runs
// `lea ecx,[esi+19Ch]` into the list's ~_List_base 0x00558AA0), SubType +0x1A0, Definition +0x1A4 (28 bytes, cleared by
// the constructor 0x00558540), sizeof 0x1C0. Vtables 0x0124ADBC / 0x0124AD98 / 0x0124AD90. The selected redefiner is the
// global 0x013B8AC8 (established spelling NUISystem::g_pSelectedKeyRedefiner; the data symbol is in neither PDB name list).
#include "engine/NUISystem_CClickable.h"
#include "stlport/fable_containers.h"
enum EGameAction;                                           // global engine enums (as engine/fable_nscript_forward.h)
enum EGameActionSubType;
class CActionInputControl;
namespace NUISystem {
class CText;
class CKeyRedefiner;
extern CKeyRedefiner* g_pSelectedKeyRedefiner;              // 0x013B8AC8
class CKeyRedefiner : public CClickable {
public:
    CKeyRedefiner(long);
    CKeyRedefiner(const CKeyRedefiner&);
    virtual ~CKeyRedefiner();
    void AddAlias(EGameAction);
protected:
    CText* PActionText;                                     // +0x194
    CText* PKeyText;                                        // +0x198
    std::list<EGameAction,std::allocator<EGameAction> > Actions;   // +0x19c
    EGameActionSubType SubType;                             // +0x1a0
    unsigned char Definition[0x1c];                         // +0x1a4 CActionInputControl (GameAction, ControllerType,
                                                            // KeyboardKey, XboxButton, MouseButton, ControlDirection)
};
FABLE_STATIC_ASSERT(sizeof(CKeyRedefiner)==0x1c0);
}
#endif
