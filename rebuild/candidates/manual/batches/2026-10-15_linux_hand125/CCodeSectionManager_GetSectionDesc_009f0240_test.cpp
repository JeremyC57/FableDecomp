// Behaviour test for 0x009F0240 CCodeSectionManager::GetSectionDesc: found entry, else the shared empty description (re-cleared each call).
#include <cstdio>
struct CSectionDesc { const char* ShortName; const char* LongName; bool Loaded; long Handle; };
struct SNode { unsigned long Key; CSectionDesc Value; };
struct CSectionMap {
    SNode* nodes; int count; SNode header;
    SNode* find(unsigned long key) { for (int i = 0; i < count; ++i) if (nodes[i].Key == key) return &nodes[i]; return &header; }
};
struct CCodeSectionManager {
    CSectionMap SectionDesc;
    const CSectionDesc& GetSectionDesc(unsigned long handle)
    {
        SNode* it = SectionDesc.find(handle);
        if (it == &SectionDesc.header) {
            static CSectionDesc s_empty = { "", "", false, 0 };
            s_empty.Loaded = false;
            s_empty.Handle = 0;
            return s_empty;
        }
        return it->Value;
    }
};
int main()
{
    SNode nodes[2] = { { 5, { "a", "alpha", true, 5 } }, { 9, { "b", "beta", false, 9 } } };
    CCodeSectionManager m; m.SectionDesc.nodes = nodes; m.SectionDesc.count = 2;
    if (&m.GetSectionDesc(9) != &nodes[1].Value || m.GetSectionDesc(5).Handle != 5) { std::printf("009f0240_TEST FAIL\n"); return 1; }
    CSectionDesc& e = const_cast<CSectionDesc&>(m.GetSectionDesc(7));
    e.Loaded = true; e.Handle = 42;
    const CSectionDesc& e2 = m.GetSectionDesc(8);
    if (&e2 != &e || e2.Loaded || e2.Handle != 0) { std::printf("009f0240_TEST FAIL\n"); return 1; }
    std::printf("009f0240_TEST PASS\n");
    return 0;
}
