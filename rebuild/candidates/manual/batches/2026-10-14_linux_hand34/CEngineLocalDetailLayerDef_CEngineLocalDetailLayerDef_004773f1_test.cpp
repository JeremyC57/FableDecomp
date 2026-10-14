#include <cstdio>
static int g_base = 0;
struct CParentDefClassBase { virtual void v00() { } long m_base; CParentDefClassBase() : m_base(42) { } CParentDefClassBase(const CParentDefClassBase& o) : m_base(o.m_base) { ++g_base; } };
struct CRetailLayerVector_00477428 { long m_tag; CRetailLayerVector_00477428() : m_tag(0) { } CRetailLayerVector_00477428(const CRetailLayerVector_00477428& o) : m_tag(o.m_tag) { } };
struct CRetailObjectDefVector_004774d6 { long m_tag; CRetailObjectDefVector_004774d6() : m_tag(0) { } CRetailObjectDefVector_004774d6(const CRetailObjectDefVector_004774d6& o) : m_tag(o.m_tag) { } };
struct CEngineLocalDetailLayerDef : CParentDefClassBase { CRetailLayerVector_00477428 m_layers; CRetailObjectDefVector_004774d6 m_objectDefs; CEngineLocalDetailLayerDef() { }
    CEngineLocalDetailLayerDef(const CEngineLocalDetailLayerDef& o) : CParentDefClassBase(o), m_layers(o.m_layers), m_objectDefs(o.m_objectDefs) { } };
int main() {
    CEngineLocalDetailLayerDef a; a.m_layers.m_tag = 5; a.m_objectDefs.m_tag = 7; CEngineLocalDetailLayerDef b(a);
    if (b.m_layers.m_tag != 5 || b.m_objectDefs.m_tag != 7 || g_base != 1 || b.m_base != 42) { std::printf("004773f1_TEST FAIL\n"); return 1; }
    std::printf("004773f1_TEST PASS\n"); return 0;
}
