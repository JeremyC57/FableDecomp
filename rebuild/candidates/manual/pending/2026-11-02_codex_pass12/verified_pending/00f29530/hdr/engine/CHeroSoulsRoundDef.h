#ifndef FABLE_ENGINE_CHEROSOULSROUNDDEF_H
#define FABLE_ENGINE_CHEROSOULSROUNDDEF_H
// Original Ego_r and FableWin class records; retail member offsets verified by
// CScriptDef construction and reverse array destruction at 00f28f90/00f29530.
#include "engine/CSubDefClassBase.h"
#include "engine/CDefString.h"
#include "engine/CCharString.h"
#include "stlport/fable_containers.h"
class CHeroSoulsWaveDef;
class CHeroSoulsRoundDef : public CSubDefClassBase {
public:
    CHeroSoulsRoundDef();
    CHeroSoulsRoundDef(const CHeroSoulsRoundDef&);
    CHeroSoulsRoundDef& operator=(const CHeroSoulsRoundDef&);
    virtual ~CHeroSoulsRoundDef();
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
