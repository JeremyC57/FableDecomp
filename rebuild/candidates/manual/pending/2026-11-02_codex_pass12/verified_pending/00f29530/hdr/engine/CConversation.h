#ifndef FABLE_ENGINE_CCONVERSATION_H
#define FABLE_ENGINE_CCONVERSATION_H
// Original Ego_r and FableWin class records; retail member offsets verified by
// CScriptDef construction and reverse array destruction at 00f28f90/00f29530.
#include "engine/CSubDefClassBase.h"
#include "engine/CDefString.h"
#include "engine/CCharString.h"
#include "stlport/fable_containers.h"
class CConversation : public CSubDefClassBase {
public:
    CConversation();
    CConversation(const CConversation&);
    CConversation& operator=(const CConversation&);
    virtual ~CConversation();
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    static CDefClassBase* __fastcall Alloc();
    static CCharString __fastcall StatGetTypeName();
    virtual void Transfer(CPersistContext&);
    long Lines; // +0x0028
    std::vector<CCharString,std::allocator<CCharString> > Speaker; // +0x002c
    std::vector<CCharString,std::allocator<CCharString> > Dialogue; // +0x0038
    std::vector<CCharString,std::allocator<CCharString> > Animation; // +0x0044
    std::vector<CCharString,std::allocator<CCharString> > AnimLoop; // +0x0050
};
FABLE_STATIC_ASSERT(sizeof(CConversation) == 0x5c);
#endif
