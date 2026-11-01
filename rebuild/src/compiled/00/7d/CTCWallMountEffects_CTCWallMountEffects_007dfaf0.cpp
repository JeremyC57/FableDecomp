// CTCWallMountEffects::~CTCWallMountEffects() -- implicit component destructor: destroy CreaturePerformingAction (+0x14), release PDef (+0xc), then CTCBase::~CTCBase (tail jump)
// Leaf: ??1CTCWallMountEffects@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007DFAF0, 43 bytes: push esi ; mov esi,ecx ; lea ecx,[esi+14h] ; call 0x00A01C10 ;
//   mov ecx,[esi+0Ch] ; test ecx,ecx ; je ; dec dword ptr [ecx+4] ; jne ; mov eax,[ecx] ; call [eax+4] ;
//   mov dword ptr [esi+0Ch],0 ; mov ecx,esi ; pop esi ; jmp 0x00686830.
// Identity (proven): the only caller is 0x007DFAD0 (call at 0x007DFAD3), slot 0 (scalar deleting destructor) of vtable
//   0x0126FC84, whose RTTI is .?AVCTCWallMountEffects@@ (rtti-vtables.tsv); RTTI base CTCWallMountEffects : CTCBase.
//   Ego_r.pdb lists CTCWallMountEffects' Alloc, ctor ??0CTCWallMountEffects@@QAE@AAVCThing@@@Z, OnInitialActivate ... ??_E and
//   NO destructor (FableWin.pdb, debug, has the generated ??1CTCWallMountEffects@@UAE@XZ): compiler-generated, hence no
//   vtable store and the straight tail jump to CTCBase::~CTCBase 0x00686830. The manifest label
//   "CCreatureAction_PerformExpressionExtended::~CCreatureAction_PerformExpressionExtended" is propagated.
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 (CreaturePerformingAction's base, out of line), the
//   def's virtual slot 1 (CIVCountedPointeeBase::Delete, `call [eax+4]`) at count zero, CTCBase::~CTCBase 0x00686830.
// Layout: Ego_r.pdb CTCWallMountEffects (36 bytes) : CTCBase (0xC): PDef (CDefPointer<const CWallMountEffectsDef>) +0x0C,
//   WaitingForConfirmation (EWallMountEffectsConfirmationType) +0x10, CreaturePerformingAction
//   (CIntelligentPointer<CThingCreatureBase>, 8 bytes) +0x14, FrameEffectLastUsed +0x1C, ObjectWasGamePersistant +0x20;
//   retail identical. CDefPointer is engine/CDefPointer.h; CBaseIntelligentPointer the landed model (vptr + PData,
//   0x00750CD0); the CTCBase chain is the landed CTCHeroPodium model (0x00771F30).
// Shape: the class declares no destructor; members are destroyed in reverse order (the intelligent pointer out of line,
//   then the inline def release); the constructor is defined here so the vftable, ??_G and the implicit ??1 are emitted.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"
#include "engine/CBaseClassNonCopyable.h"
class CThing;
class CThingCreatureBase;
class CWallMountEffectsDef;
class CBaseObjectPointer;

class CBaseIntelligentPointer {                             // vtable 0x0129C95C (vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
private:
    CBaseObjectPointer* PData;                              // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
};

enum EWallMountEffectsConfirmationType { };                 // values not needed here

class CTCBase : public CBaseClassNonCopyable {
public:
    CTCBase(CThing& parent);                                // 0x00686800
    virtual ~CTCBase();                                     // 0x00686830
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
};

class CTCWallMountEffects : public CTCBase {                // vtable 0x0126FC84
public:
    CTCWallMountEffects(CThing& parent);
    CDefPointer<const CWallMountEffectsDef> PDef;                   // +0x0C
    EWallMountEffectsConfirmationType       WaitingForConfirmation; // +0x10
    CIntelligentPointer<CThingCreatureBase> CreaturePerformingAction; // +0x14
    long                                    FrameEffectLastUsed;    // +0x1C
    bool                                    ObjectWasGamePersistant; // +0x20
};

CTCWallMountEffects::CTCWallMountEffects(CThing& parent)
    : CTCBase(parent)
{
}