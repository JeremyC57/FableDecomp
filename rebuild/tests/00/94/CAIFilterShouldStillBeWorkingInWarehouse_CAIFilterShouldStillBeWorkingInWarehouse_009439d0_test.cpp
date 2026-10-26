// Behaviour test for 009439d0 (CAIFilter_ShouldStillBeWorkingInWarehouse destructor): the members are destroyed in reverse order -- PCreature, then PTask -- through the intelligent-pointer base destructor.
#include <cstdio>
static const void* g_destroyed[4];
static int g_count = 0;
class CThing { public: int id; };
class CTCVillage { public: int id; };
class CVillageTaskCarryCrate;
class CThingAICreature;
struct CBaseIntelligentPointer {
    virtual void v00() { }
    CThing* PItem;
    CBaseIntelligentPointer() : PItem(0) { }
    ~CBaseIntelligentPointer() { g_destroyed[g_count++] = this; PItem = 0; }   // stands in for 0x00A01C10
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    virtual void v00() { }
};
class CAIStateGroupBase { public:
    class CAIFilterBase { public:
        virtual bool operator()(CThing* thing) const { return thing != 0; }
        ~CAIFilterBase() { }
    };
};
class CAIFilter_ShouldStillBeWorkingInWarehouse : public CAIStateGroupBase::CAIFilterBase {
public:
    CTCVillage*                                  PTCVillage;
    CIntelligentPointer<CVillageTaskCarryCrate>  PTask;
    CIntelligentPointer<CThingAICreature>        PCreature;
    CAIFilter_ShouldStillBeWorkingInWarehouse() : PTCVillage(0) { }
    virtual bool operator()(CThing* thing) const { return thing != 0 && PTCVillage != 0; }
    ~CAIFilter_ShouldStillBeWorkingInWarehouse();
};
CAIFilter_ShouldStillBeWorkingInWarehouse::~CAIFilter_ShouldStillBeWorkingInWarehouse()
{
}
int main()
{
    const void* task = 0;
    const void* creature = 0;
    {
        CAIFilter_ShouldStillBeWorkingInWarehouse filter;
        CTCVillage village;
        filter.PTCVillage = &village;
        task = &filter.PTask;
        creature = &filter.PCreature;
        if (g_count != 0) { std::printf("009439d0_TEST FAIL\n"); return 1; }
    }
    if (g_count != 2 || g_destroyed[0] != creature || g_destroyed[1] != task) { std::printf("009439d0_TEST FAIL\n"); return 1; }
    std::printf("009439d0_TEST PASS\n");
    return 0;
}