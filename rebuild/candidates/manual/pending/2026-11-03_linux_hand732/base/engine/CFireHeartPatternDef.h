#ifndef FABLE_ENGINE_CFIREHEARTPATTERNDEF_H
#define FABLE_ENGINE_CFIREHEARTPATTERNDEF_H
// Original Ego_r and FableWin class records; retail member offsets verified by
// CScriptDef construction and reverse array destruction at 00f28f90/00f29530.
#include "engine/CSubDefClassBase.h"
#include "engine/CDefString.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
class CFireHeartPatternDef : public CSubDefClassBase {
public:
    CFireHeartPatternDef();
    CFireHeartPatternDef(const CFireHeartPatternDef&);
    CFireHeartPatternDef& operator=(const CFireHeartPatternDef&);
    virtual ~CFireHeartPatternDef();
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    static CDefClassBase* __fastcall Alloc();
    static CCharString __fastcall StatGetTypeName();
    virtual void Transfer(CPersistContext&);
    std::vector<bool,std::allocator<bool> > PadActive; // +0x0028
};
FABLE_STATIC_ASSERT(sizeof(CFireHeartPatternDef) == 0x34);
#endif
