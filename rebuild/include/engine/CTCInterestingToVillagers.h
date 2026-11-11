#ifndef FABLE_ENGINE_CTCINTERESTINGTOVILLAGERS_H
#define FABLE_ENGINE_CTCINTERESTINGTOVILLAGERS_H
#include "engine/CTCBase.h"
#include "engine/CDefPointer.h"
class CInterestingToVillagersDef;
// Full Ego2fb7b/FW32758; native16,original public PDef+c.
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
 virtual ~CTCInterestingToVillagers();
 CTCInterestingToVillagers & operator=(const CTCInterestingToVillagers & p0);
};
FABLE_STATIC_ASSERT(sizeof(CTCInterestingToVillagers)==16);
#endif
