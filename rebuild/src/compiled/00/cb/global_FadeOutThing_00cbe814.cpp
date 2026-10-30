// FadeOutThing(CScriptThing& thing, float seconds, float targetAlpha) -- fade the thing's alpha down from 1 until it reaches targetAlpha, in steps of 1/(seconds*15) per script frame
// Leaf: ?FadeOutThing@@YIXAAVCScriptThing@@MM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBE814, 107 bytes (`ret 8`).
// TU: the global quest-script helper functions (__fastcall, size-optimised: `#pragma optimize("s", on)`).
// Model: CScriptThing is polymorphic (vtable 0x01238C8C); the engine header models the vptr as data, so a local
//   virtual model with the retail slot order is used (slots named from Ego_r.pdb / the FSE manifest).
// Model: CGameScriptInterface through g_pGameScriptInterface (0x0143E8F8): slot 7 NewScriptFrame (+0x1C), 108
//   RemoveThing (+0x1B0), 510 EntitySetAlpha (+0x7F8). Constants: 15.0f 0x0124E640, 1.0f 0x0122DED8, 0.0f 0x0122DEDC.
// Identity: Ego_r.pdb `?FadeOutThing@@YIXAAVCScriptThing@@MM@Z`.
// Callees: CScriptThing slot 75 IsAlive (+0x12C); EntitySetAlpha / NewScriptFrame.
// Shape: a top-tested `while (alpha > targetAlpha)` (the `jmp` to the test), alpha starts at 1.
#pragma optimize("s", on)
class CCharString {
public:
    void* m_pStringData;
};
class C3DVector {
public:
    float X, Y, Z;
};
class CScriptThing {                                   // vtable 0x01238C8C
public:
    virtual void v00();
    virtual const CCharString& GetName() const;              // slot 1 (+0x04, 0x004AA8B0)
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual const C3DVector& GetPos() const;                 // slot 6 (+0x18, 0x004AA980)
    virtual void v07(); virtual void v08(); virtual void v09();
    virtual float GetAngleXY() const;                        // slot 10 (+0x28, 0x004AAA40)
    virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual bool MsgIsTriggeredBy(const CCharString& by) const;   // slot 17 (+0x44, 0x004AAAD0)
    virtual bool MsgIsKilledBy(const CCharString& by) const;      // slot 18 (+0x48, 0x004AAAF0)
    virtual void v19(); virtual void v20();
    virtual bool MsgIsHitBy(const CCharString& by) const;         // slot 21 (+0x54, 0x004AAB30)
    virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual bool MsgIsKnockedOutBy(const CCharString& by) const;  // slot 32 (+0x80, 0x004AAC20)
    virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38();
    virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49(); virtual void v50();
    virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56();
    virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62();
    virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68();
    virtual void v69(); virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74();
    virtual bool IsAlive() const;                            // slot 75 (+0x12c, 0x004AB130)
    virtual void v76();
    virtual bool IsNull() const;                             // slot 77 (+0x134, 0x004AB150)
    void* PImp_Data;                                    // +0x04 CCountedPointer<CScriptThing>::Data (PDB name)
    void* PImp_Info;                                    // +0x08 CCountedPointer<CScriptThing>::Info (PDB name)
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8

void __fastcall FadeOutThing(CScriptThing& thing, float seconds, float targetAlpha)
{
    float step = 1.0f / (seconds * 15.0f);
    float alpha = 1.0f;
    while (alpha > targetAlpha) {
        if (thing.IsAlive())
            g_pGameScriptInterface->EntitySetAlpha(thing, alpha, true);
        g_pGameScriptInterface->NewScriptFrame();
        alpha -= step;
    }
}