// behaviour test for 0x0091C030 CTCNymph::ClearTarget() (model copy of the source; CBaseIntelligentPointer's out-of-line members are stubs)
#include <cstdio>
class CThingCreatureBase;
class CBaseObject;
static int g_ctor = 0, g_dtor = 0, g_set = 0;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer() : Item(0) { ++g_ctor; }
    virtual ~CBaseIntelligentPointer() { ++g_dtor; }
    CBaseObject* GetPItem() const { return Item; }
    void SetPItem(CBaseObject* item) { Item = item; ++g_set; }
    CBaseObject* Item;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CTCNymph {                                         // Ego_r.pdb 116 bytes; retail 4 below the PDB after FlightSpline (12-byte vector)
public:
    void ClearTarget();
    unsigned char _body[0x68];                           // vptr .. TotalFadeTime (not touched here)
    CIntelligentPointer<CThingCreatureBase> PTarget;     // +0x68 (PDB +0x6C)
};

void CTCNymph::ClearTarget()
{
    PTarget = CIntelligentPointer<CThingCreatureBase>(0);
}
int main()
{
    static double storage[(sizeof(CTCNymph) + 7) / 8];   // raw storage: the members are not constructed by the test
    CTCNymph* o = (CTCNymph*)storage;
    int dummy = 0;
    o->PTarget.Item = (CBaseObject*)&dummy;
    g_ctor = g_dtor = g_set = 0;
    o->CTCNymph::ClearTarget();
    int ok = o->PTarget.Item == 0 && g_ctor == 1 && g_dtor == 1 && g_set == 2;
    if (!ok) { std::printf("0091c030_TEST FAIL\n"); return 1; }
    std::printf("0091c030_TEST PASS\n");
    return 0;
}