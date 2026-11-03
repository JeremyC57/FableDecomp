#ifndef FABLE_ENGINE_CARENAROUNDDEF_H
#define FABLE_ENGINE_CARENAROUNDDEF_H
// Original Ego_r and FableWin class records; retail member offsets verified by
// CScriptDef construction and reverse array destruction at 00f28f90/00f29530.
#include "engine/CSubDefClassBase.h"
#include "engine/CDefString.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
class CArenaWaveDef;
class CArenaRoundDef : public CSubDefClassBase {
public:
    CArenaRoundDef();
    CArenaRoundDef(const CArenaRoundDef&);
    CArenaRoundDef& operator=(const CArenaRoundDef&);
    virtual ~CArenaRoundDef();
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    static CDefClassBase* __fastcall Alloc();
    static CCharString __fastcall StatGetTypeName();
    virtual void Transfer(CPersistContext&);
    long NumWaves; // +0x0028
    std::vector<CArenaWaveDef,std::allocator<CArenaWaveDef> > Waves; // +0x002c
};
FABLE_STATIC_ASSERT(sizeof(CArenaRoundDef) == 0x38);
#endif
