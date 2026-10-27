// behaviour test for 0x00935A80 CAIStateGroup_MinionPutOutFires::Cleanup() (model copy of the source; CBaseIntelligentPointer's out-of-line members are stubs)
#include <cstdio>
class CThingBuilding;
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
class CAIBrain;
class CAIStateGroupBase {                                // Ego_r.pdb, 0x14 bytes (retail = PDB: the CCharString is 4 bytes)
public:
    virtual ~CAIStateGroupBase();
    virtual void Cleanup();                              // vtable slot 9 in retail
    CAIBrain* PBrain;                                    // +0x04
    long ScriptAIPriority;                               // +0x08 EScriptAIPriority
    void* Name;                                          // +0x0C CCharString (one data pointer)
    bool Terminated;                                     // +0x10
    unsigned char Priority;                              // +0x11
    bool Enabled;                                        // +0x12
};
class CAIStateGroup_MinionPutOutFires : public CAIStateGroupBase {   // Ego_r.pdb, 0x1C bytes
public:
    virtual void Cleanup();
    CIntelligentPointer<CThingBuilding> PBuildingOnFire; // +0x14
};

CAIStateGroupBase::~CAIStateGroupBase() { }
void CAIStateGroupBase::Cleanup() { }
void CAIStateGroup_MinionPutOutFires::Cleanup()
{
    PBuildingOnFire = CIntelligentPointer<CThingBuilding>(0);
}
int main()
{
    static double storage[(sizeof(CAIStateGroup_MinionPutOutFires) + 7) / 8];   // raw storage: the members are not constructed by the test
    CAIStateGroup_MinionPutOutFires* o = (CAIStateGroup_MinionPutOutFires*)storage;
    int dummy = 0;
    o->PBuildingOnFire.Item = (CBaseObject*)&dummy;
    g_ctor = g_dtor = g_set = 0;
    o->CAIStateGroup_MinionPutOutFires::Cleanup();
    int ok = o->PBuildingOnFire.Item == 0 && g_ctor == 1 && g_dtor == 1 && g_set == 2;
    if (!ok) { std::printf("00935a80_TEST FAIL\n"); return 1; }
    std::printf("00935a80_TEST PASS\n");
    return 0;
}