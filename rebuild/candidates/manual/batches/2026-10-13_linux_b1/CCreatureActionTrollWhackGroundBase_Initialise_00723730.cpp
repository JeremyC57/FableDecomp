// CCreatureAction_TrollWhackGroundBase::Initialise()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00723730, oracle length 19 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi] ; call [eax+17Ch] ; mov ecx,esi ; pop esi ; jmp <base Initialise>
// Calls virtual slot 95 (+0x17C) on itself, then tail-jumps into the base-class Initialise at
// 0x006AF780 (__thiscall, no args; its manifest label is a propagated name and not trusted).
// Shape precedent: CCreatureAction_TrollWhackGroundBase::Initialise @ 0x008C5FD0 (landed).
#include "engine/CCreatureAction_TrollWhackGroundBase.h"

struct CTrollWhackGroundVirtuals {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7c();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8c();
    virtual void v90(); virtual void v94(); virtual void v98(); virtual void v9c();
    virtual void va0(); virtual void va4(); virtual void va8(); virtual void vac();
    virtual void vb0(); virtual void vb4(); virtual void vb8(); virtual void vbc();
    virtual void vc0(); virtual void vc4(); virtual void vc8(); virtual void vcc();
    virtual void vd0(); virtual void vd4(); virtual void vd8(); virtual void vdc();
    virtual void ve0(); virtual void ve4(); virtual void ve8(); virtual void vec();
    virtual void vf0(); virtual void vf4(); virtual void vf8(); virtual void vfc();
    virtual void v100(); virtual void v104(); virtual void v108(); virtual void v10c();
    virtual void v110(); virtual void v114(); virtual void v118(); virtual void v11c();
    virtual void v120(); virtual void v124(); virtual void v128(); virtual void v12c();
    virtual void v130(); virtual void v134(); virtual void v138(); virtual void v13c();
    virtual void v140(); virtual void v144(); virtual void v148(); virtual void v14c();
    virtual void v150(); virtual void v154(); virtual void v158(); virtual void v15c();
    virtual void v160(); virtual void v164(); virtual void v168(); virtual void v16c();
    virtual void v170(); virtual void v174(); virtual void v178();
    virtual void PrepareWhack();   // slot 95 (+0x17c)
};

struct CCreatureAction_TrollWhackGroundBase_Methods : CCreatureAction_TrollWhackGroundBase {
    void InitialiseBase();      // 0x006AF780
    void Initialise();
};

void CCreatureAction_TrollWhackGroundBase_Methods::Initialise()
{
    reinterpret_cast<CTrollWhackGroundVirtuals*>(this)->PrepareWhack();
    InitialiseBase();
}
