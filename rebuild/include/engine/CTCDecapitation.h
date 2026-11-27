#ifndef FABLE_ENGINE_CTCDECAPITATION_H
#define FABLE_ENGINE_CTCDECAPITATION_H
#include "engine/CTCBase.h"
#include "engine/CDefPointer.h"
class CDecapitationDef;
// Both original complete records; original native field reads and interface209.
class CTCDecapitation : public CTCBase {
public:
#ifdef FABLE_RETAIL_DECAPITATE_PASS29
 void Decapitate();
#endif
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return TCI_DECAPITATION;}
 bool IsDecapitated()const{return Decapitated;}
private:
 bool Decapitated;
 CDefPointer<const CDecapitationDef> PDef;
};
FABLE_STATIC_ASSERT_AT(15, sizeof(CTCDecapitation)==0x14);
#endif
