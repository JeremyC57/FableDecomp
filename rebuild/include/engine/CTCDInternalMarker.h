#ifndef FABLE_ENGINE_CTCDINTERNALMARKER_H
#define FABLE_ENGINE_CTCDINTERNALMARKER_H
// Layout: complete original members from both PDB type records; retail STLport maps use12bytes.
// Evidence: 00783a30/NAMING_EVIDENCE.md; opt-in preserves the previous default.
#include "engine/CTCDriverBase.h"
class C3DVector;
class CTCDInternalMarker:public CTCDriverBase {
public:
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType();
 virtual ETCInterfaceType GetInterfaceType()const;
 static CTCBase* FABLE_FASTCALL Alloc(CThing&);
 static CCharString FABLE_FASTCALL StatGetClassTypeName();
 virtual CCharString GetClassTypeName()const;
 static unsigned long FABLE_FASTCALL StatGetDriverType();
 virtual bool IsDriverType(long)const;
 CTCDInternalMarker(const CTCDInternalMarker&);
 CTCDInternalMarker(CThing&);
 static CThing* FABLE_FASTCALL Create(long,const C3DVector&);
private:
 void Construct(const C3DVector&);
public:
 virtual ~CTCDInternalMarker();
 CTCDInternalMarker& operator=(const CTCDInternalMarker&);
};
FABLE_STATIC_ASSERT(sizeof(CTCDInternalMarker)==12);
#endif
