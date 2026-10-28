#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __forceinline inline
#endif
class CCharStringData;
class CCharString { public: CCharStringData* PStringData; CCharString(const char*, long = -1); ~CCharString(); };
class CTCBase;
class CTCParamsBase;
class CThing { public: CTCBase* AddTC(const CCharString&, bool, const CTCParamsBase*); };
class CThingCreatureBase : public CThing { };
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer { public: virtual ~CBaseIntelligentPointer(); CBaseObject* GetPItem() const; CBaseObjectPointer* PData; };
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public: T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); } };
class CCreatureActionBase { public:
virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {}
virtual void v04() {} virtual void v05() {} virtual void v06() {} virtual void v07() {}
virtual void v08() {} virtual void v09() {} virtual void v10() {} virtual void v11() {}
virtual void Initialise();
unsigned char _pad04[4]; CIntelligentPointer<CThingCreatureBase> PCreature;
};
class CCreatureAction_SheatheItemToInventory : public CCreatureActionBase { public: virtual void Initialise(); };
class CCreatureAction_SheatheItemToInventory_ForDivineWrath : public CCreatureAction_SheatheItemToInventory { public: virtual void Initialise(); };
void CCreatureAction_SheatheItemToInventory_ForDivineWrath::Initialise()
{
    CCreatureAction_SheatheItemToInventory::Initialise();
    PCreature.GetPItem()->AddTC("CTCDivineWrath", true, 0);
}

static int step = 0, bad = 0;
class CCharStringData { public: const char* Text; };
static CCharStringData stringData;
CCharString::CCharString(const char* s, long n) { stringData.Text=s; PStringData=&stringData; if((step!=1 && step!=2) || n != -1) bad=1; ++step; }
CCharString::~CCharString() { if(step++ != 4) bad=1; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() {}
CBaseObject* CBaseIntelligentPointer::GetPItem() const { if(step!=1 && step!=2) bad=1; ++step; return (CBaseObject*)PData; }
void CCreatureActionBase::Initialise() {}
void CCreatureAction_SheatheItemToInventory::Initialise() { if(step++ != 0) bad=1; }
static CThingCreatureBase* expectedCreature;
CTCBase* CThing::AddTC(const CCharString& s, bool enable, const CTCParamsBase* params) { if(step++ != 3 || this != expectedCreature || strcmp(s.PStringData->Text,"CTCDivineWrath") || !enable || params) bad=1; return 0; }
int main() { CThingCreatureBase thing; CCreatureAction_SheatheItemToInventory_ForDivineWrath action; expectedCreature=&thing; action.PCreature.PData=(CBaseObjectPointer*)&thing; CCreatureActionBase* dispatch=&action; dispatch->Initialise(); if(bad || step!=5) { puts("0069f7b0_TEST FAIL"); return 1; } puts("0069f7b0_TEST PASS"); return 0; }