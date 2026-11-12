#ifndef FABLE_ENGINE_CHEROSOULSROUNDDEF_H
#define FABLE_ENGINE_CHEROSOULSROUNDDEF_H
// Original Ego_r and FableWin class records; retail member offsets verified by
// CScriptDef construction and reverse array destruction at 00f28f90/00f29530.
#include "engine/CSubDefClassBase.h"
#include "engine/CDefString.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
class CHeroSoulsWaveDef;
// Opt-in FABLE_RETAIL_CHEROSOULSROUNDDEF_IMPLICIT_DTOR (agentFAM07, row 0x00F26DF0 ??_GCHeroSoulsRoundDef@@UAEPAXI@Z): no user-declared destructor. The retail
// scalar deleting destructor (vtable 0x012FB430 slot 0, the vtable the ctor 0x00F35780 stores) runs the member vector's destructor and
// then stores the CDefPointeeBase vptr 0x01230BA0 before ~CResource: it stores no vtable of its own, which only the
// compiler-generated destructor does (a user-declared one stores it first; family K050).
class CHeroSoulsRoundDef : public CSubDefClassBase {
public:
    CHeroSoulsRoundDef();
    CHeroSoulsRoundDef(const CHeroSoulsRoundDef&);
    CHeroSoulsRoundDef& operator=(const CHeroSoulsRoundDef&);
#ifndef FABLE_RETAIL_CHEROSOULSROUNDDEF_IMPLICIT_DTOR
    virtual ~CHeroSoulsRoundDef();
#endif
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    static CDefClassBase* __fastcall Alloc();
    static CCharString __fastcall StatGetTypeName();
    virtual void Transfer(CPersistContext&);
    long NumWaves; // +0x0028
    std::vector<CHeroSoulsWaveDef,std::allocator<CHeroSoulsWaveDef> > Waves; // +0x002c
};
FABLE_STATIC_ASSERT(sizeof(CHeroSoulsRoundDef) == 0x38);
#endif
