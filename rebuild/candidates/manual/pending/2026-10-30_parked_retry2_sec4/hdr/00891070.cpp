// CGameScriptInterface::CameraUseCameraPoint(const CScriptThing& cameraPoint, const CScriptThing& subject, float time, long a, long b) const -- script call: forwards to the position/RH-set overload with the live subject's physics position and orientation
// Grade: DIFFER(83v83) -- retail tests PhysicsTC in ecx (`mov ecx,[eax+60h] ; test ecx,ecx ; je`) before `push esi ; mov esi,ecx`; ours loads it straight into esi after the push (one-register placement only)
// Leaf: ?CameraUseCameraPoint@CGameScriptInterface@@UBEXABVCScriptThing@@0MJJ@Z
// Retail: Fable.exe @ 0x00891070, 83 bytes, thiscall `ret 14h`.
// Identity (proven): retail CGameScriptInterface vtable 0x01260F0C slot 413 holds this body; slot 413 is
//   ?CameraUseCameraPoint@CGameScriptInterface@@UBEXABVCScriptThing@@0MJJ@Z (ghidra_out/cgsi_slots_aligned.tsv); it calls
//   slot 411 (+0x66c) ?CameraUseCameraPoint@CGameScriptInterface@@UBEXABVCScriptThing@@PBVC3DVector@@PBVCRightHandedSet@@MJJ@Z.
// Body: t = subject.GetPThing() (CScriptThing slot 11); if t, not dead (+0x91 bit 0) and its PhysicsTC (+0x60; Ego_r.pdb
//   CThing::PhysicsTC +0x70, retail -0x10 after the 4-byte STLport containers) is set: forward cameraPoint,
//   &PhysicsTC->GetPosition() (inline, Position +0x0c; FableWin.pdb ?GetPosition@CTCPhysicsBase@@QBEABVC3DVector@@XZ),
//   &PhysicsTC->GetRHSet() (virtual slot 72, +0x120; ?GetRHSet@CTCPhysicsBase@@UBEABVCRightHandedSet@@XZ), time, a, b.
//   Retail calls GetRHSet before it loads any argument, so the RH set is a named local ahead of the call; the PhysicsTC
//   null test is the third term of the guard (`mov ecx,[eax+60h] ; test ; je` before `push esi`) and the component is
//   then re-read into a local. The two long parameter
//   names are not in the name sets (descriptive).
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet;
class CTCPhysicsBase {
public:
    virtual void _v000(); virtual void _v001(); virtual void _v002(); virtual void _v003(); virtual void _v004(); virtual void _v005(); virtual void _v006(); virtual void _v007();
    virtual void _v008(); virtual void _v009(); virtual void _v010(); virtual void _v011(); virtual void _v012(); virtual void _v013(); virtual void _v014(); virtual void _v015();
    virtual void _v016(); virtual void _v017(); virtual void _v018(); virtual void _v019(); virtual void _v020(); virtual void _v021(); virtual void _v022(); virtual void _v023();
    virtual void _v024(); virtual void _v025(); virtual void _v026(); virtual void _v027(); virtual void _v028(); virtual void _v029(); virtual void _v030(); virtual void _v031();
    virtual void _v032(); virtual void _v033(); virtual void _v034(); virtual void _v035(); virtual void _v036(); virtual void _v037(); virtual void _v038(); virtual void _v039();
    virtual void _v040(); virtual void _v041(); virtual void _v042(); virtual void _v043(); virtual void _v044(); virtual void _v045(); virtual void _v046(); virtual void _v047();
    virtual void _v048(); virtual void _v049(); virtual void _v050(); virtual void _v051(); virtual void _v052(); virtual void _v053(); virtual void _v054(); virtual void _v055();
    virtual void _v056(); virtual void _v057(); virtual void _v058(); virtual void _v059(); virtual void _v060(); virtual void _v061(); virtual void _v062(); virtual void _v063();
    virtual void _v064(); virtual void _v065(); virtual void _v066(); virtual void _v067(); virtual void _v068(); virtual void _v069(); virtual void _v070(); virtual void _v071();
    virtual const CRightHandedSet& GetRHSet() const;   // slot 72 (+0x120)
    unsigned char _pad04[0x08];
    C3DVector Position;                                  // +0x0c
    const C3DVector& GetPosition() const { return Position; }
};
class CThing {
public:
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                           // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char Dead : 1;                              // +0x91 bit 0
    bool IsDead() const { return Dead; }
};
// CScriptThing / CGameScriptInterface: the retail models from the shared NScript headers (FABLE_RETAIL_NSCRIPT).
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::CameraUseCameraPoint(const CScriptThing& cameraPoint, const CScriptThing& subject, float time, long a, long b) const
{
    CThing* t = subject.GetPThing();
    if (t && !t->IsDead() && t->PhysicsTC)
    {
        CTCPhysicsBase* physics = t->PhysicsTC;
        const CRightHandedSet& rhSet = physics->GetRHSet();
        CameraUseCameraPoint(cameraPoint, &physics->GetPosition(), &rhSet, time, a, b);
    }
}
