// Test for 0x006928C0 CCreatureActionBase::SetCreature: PCreature ends up holding the creature; the temporary
// intelligent pointer is built, assigned from and destroyed exactly once per call.
#include <stdio.h>
class CBaseObject;
class CThingCreatureBase { public: int id; };

static int g_ctor, g_dtor, g_set;
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();
    CBaseObject* PItem;
    CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;
    void SetPItem(CBaseObject* item);
};
CBaseIntelligentPointer::CBaseIntelligentPointer() : PItem(0) { ++g_ctor; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++g_dtor; }
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return PItem; }
void CBaseIntelligentPointer::SetPItem(CBaseObject* item) { PItem = item; ++g_set; }

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

class CCreatureActionBase {
public:
    virtual void v00();
    unsigned char                           _pad04[0x04];
    CIntelligentPointer<CThingCreatureBase> PCreature;   // +0x08
    unsigned char                           _pad10[0xb4 - 0x10];
    CCreatureActionBase() : PCreature(0) { }
protected:
    void SetCreature(CThingCreatureBase& creature);
};
void CCreatureActionBase::v00() { }
class CTestAction : public CCreatureActionBase {
public:
    void Set(CThingCreatureBase& c) { SetCreature(c); }
};
int main()
{
    if (sizeof(CCreatureActionBase) != 0xb4) return 1;
    CTestAction a;
    if ((char*)&a.PCreature - (char*)&a != 8) return 2;
    CThingCreatureBase c1, c2;
    int ctor0 = g_ctor, dtor0 = g_dtor, set0 = g_set;
    a.Set(c1);
    if (a.PCreature.GetPItem() != (CBaseObject*)&c1) return 3;
    if (g_ctor - ctor0 != 1 || g_dtor - dtor0 != 1 || g_set - set0 != 2) return 4;
    a.Set(c2);
    if (a.PCreature.GetPItem() != (CBaseObject*)&c2) return 5;
    puts("006928c0_TEST PASS");
    return 0;
}
