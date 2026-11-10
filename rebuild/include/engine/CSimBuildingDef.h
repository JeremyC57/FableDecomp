#ifndef FABLE_ENGINE_CSIMBUILDINGDEF_RETAIL_ENTRY_H
#define FABLE_ENGINE_CSIMBUILDINGDEF_RETAIL_ENTRY_H
#ifdef FABLE_RETAIL_CSIMBUILDINGDEF
#include "engine/CParentDefClassBase.h"
// Original PDB base and three signed long fields; native 0090bc2d reads Property+3c.
class CSimBuildingDef : public CParentDefClassBase {
public:
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    static CDefClassBase* FABLE_FASTCALL Alloc();
    static CCharString FABLE_FASTCALL StatGetTypeName();
    long Property;
    long WorkSpaces;
    long LiveSpaces;
    virtual void Transfer(CPersistContext&);
    CSimBuildingDef();
    CSimBuildingDef(const CSimBuildingDef&);
    virtual ~CSimBuildingDef();
};
FABLE_STATIC_ASSERT(sizeof(CSimBuildingDef)==0x48);
FABLE_STATIC_ASSERT(offsetof(CSimBuildingDef,Property)==0x3c);
#else
#include "engine/_quarantine/CSimBuildingDef.h"
#endif
#endif
