#ifndef FABLE_ENGINE_CTCINTERESTINGTOVILLAGERS_H
#define FABLE_ENGINE_CTCINTERESTINGTOVILLAGERS_H
#include "engine/CTCBase.h"
#include "engine/CDefPointer.h"
class CInterestingToVillagersDef;
// Full Ego2fb7b/FW32758; native16,original public PDef+c.
// Opt-in FABLE_RETAIL_CTCINTERESTINGTOVILLAGERS_IMPLICIT_DTOR (agentFAM06, row 0x004DC916): no user-declared destructor. The retail
// ??1CTCInterestingToVillagers@@UAE@XZ (0x004DC916, a size-TU COMDAT called only by the scalar deleting destructor 0x004DC8FA)
// stores no vtable: PDef's release, then `jmp ~CTCBase`. A user-declared destructor stores the vtable first (graded 38 v 32);
// the compiler-generated one does not.
class CTCInterestingToVillagers:public CTCBase{
public:
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return TCI_INTERESTING_TO_VILLAGERS;}
 virtual ETCInterfaceType GetInterfaceType() const;
 static CTCBase * FABLE_FASTCALL Alloc(CThing & p0);
 static CCharString FABLE_FASTCALL StatGetClassTypeName();
 virtual CCharString GetClassTypeName() const;
private:
 virtual void OnInitialActivate();
public:
 CDefPointer<CInterestingToVillagersDef const > PDef;
 CTCInterestingToVillagers(const CTCInterestingToVillagers & p0);
 CTCInterestingToVillagers(CThing & p0);
#ifndef FABLE_RETAIL_CTCINTERESTINGTOVILLAGERS_IMPLICIT_DTOR
 virtual ~CTCInterestingToVillagers();
#endif
 CTCInterestingToVillagers & operator=(const CTCInterestingToVillagers & p0);
};
FABLE_STATIC_ASSERT(sizeof(CTCInterestingToVillagers)==16);
#endif
