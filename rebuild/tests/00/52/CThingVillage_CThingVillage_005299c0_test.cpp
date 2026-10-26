#include <cstdio>
#include <new>
// Behaviour test for CThingVillage::CThingVillage (0x005299C0): type 4, CurrentGraphic zeroed.
struct CIntelligentPointer { void* m_pItem; CIntelligentPointer() : m_pItem(0) { } };
class CThing { public: long m_type; CThing(long type) : m_type(type) { } };
class CThingGameObject : public CThing { public: CIntelligentPointer m_owner; CThingGameObject(long type) : CThing(type), m_owner() { } };
class CEngineGraphic { public:
    unsigned long BankIndex; float AnimStep; float RenderSizeX; bool AdditiveAlpha; unsigned char Type;
    CEngineGraphic() : BankIndex(0), AnimStep(0), RenderSizeX(0), AdditiveAlpha(false), Type(0) { }
};
class CThingVillage : public CThingGameObject { public:
    CEngineGraphic CurrentGraphic;
    CThingVillage() : CThingGameObject(4), CurrentGraphic() { }
};
int main()
{
    unsigned char raw[sizeof(CThingVillage)];
    for (unsigned i = 0; i < sizeof(raw); ++i) raw[i] = 0xCD;
    CThingVillage* v = new (raw) CThingVillage;
    int fails = 0;
    if (v->m_type != 4 || v->m_owner.m_pItem) ++fails;
    if (v->CurrentGraphic.BankIndex || v->CurrentGraphic.AnimStep != 0.0f || v->CurrentGraphic.RenderSizeX != 0.0f || v->CurrentGraphic.AdditiveAlpha || v->CurrentGraphic.Type) ++fails;
    if (fails) { std::printf("005299c0_TEST FAIL\n"); return 1; }
    std::printf("005299c0_TEST PASS\n");
    return 0;
}