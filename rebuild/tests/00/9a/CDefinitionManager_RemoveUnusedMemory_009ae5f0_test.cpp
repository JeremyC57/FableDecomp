// Behaviour test for CDefinitionManager::RemoveUnusedMemory (0x009AE5F0): every owned array is shrunk exactly once;
// defs without a sub-def table are skipped.
#include <cstdio>
static int g_shrinks = 0, g_symbolShrinks = 0;
template <class T> struct CArray {
    T* First; T* Last; T* End; int shrunk;
    unsigned long size() const { return (unsigned long)(Last - First); }
    T& operator[](unsigned long i) { return First[i]; }
    void DeallocateUnusedMemory() { ++shrunk; ++g_shrinks; End = Last; }
};
struct CSymbolMap { void RemoveUnusedMemory() { ++g_symbolShrinks; } };
struct SSubDefEntry { unsigned long Key; unsigned long a, b; };
struct CDefClassBase {
    CArray<SSubDefEntry>* sub;
    virtual CArray<SSubDefEntry>* GetSubDefInfo() { return sub; }
};
struct CInstantiatedDefInfo { long ClassName; long InstantiationName; unsigned long ClassIndex; CDefClassBase* PDef; };
struct CDefClassInfo { void* Name; void* AllocFunc; CArray<unsigned long> GlobalIndices; };
struct SDefClassEntry { unsigned long Key; CDefClassInfo Info; };
struct SDefClassVector {
    SDefClassEntry* First; SDefClassEntry* Last;
    unsigned long size() const { return (unsigned long)(Last - First); }
    SDefClassEntry& operator[](unsigned long i) { return First[i]; }
};
class CDefinitionManager {
public:
    CSymbolMap* PDefinitionSymbolMap; SDefClassVector DefClassInfo; CArray<CInstantiatedDefInfo*> InstantiatedDefs;
    void RemoveUnusedMemory();
};
void CDefinitionManager::RemoveUnusedMemory()
{
    InstantiatedDefs.DeallocateUnusedMemory();
    PDefinitionSymbolMap->RemoveUnusedMemory();
    for (unsigned long i = 0; i < DefClassInfo.size(); ++i)
        DefClassInfo[i].Info.GlobalIndices.DeallocateUnusedMemory();
    for (unsigned long j = 0; j < InstantiatedDefs.size(); ++j)
        if (InstantiatedDefs[j]->PDef->GetSubDefInfo())
            InstantiatedDefs[j]->PDef->GetSubDefInfo()->DeallocateUnusedMemory();
}
int main()
{
    CSymbolMap symbols;
    SDefClassEntry classes[3];
    for (int c = 0; c < 3; ++c) { classes[c].Info.GlobalIndices.First = classes[c].Info.GlobalIndices.Last = classes[c].Info.GlobalIndices.End = 0; classes[c].Info.GlobalIndices.shrunk = 0; }
    CArray<SSubDefEntry> subA; subA.First = subA.Last = subA.End = 0; subA.shrunk = 0;
    CDefClassBase withSub, without; withSub.sub = &subA; without.sub = 0;
    CInstantiatedDefInfo i0 = { 0, 0, 0, &withSub }, i1 = { 0, 0, 0, &without };
    CInstantiatedDefInfo* defs[2] = { &i0, &i1 };
    CDefinitionManager m; m.PDefinitionSymbolMap = &symbols;
    m.DefClassInfo.First = classes; m.DefClassInfo.Last = classes + 3;
    m.InstantiatedDefs.First = defs; m.InstantiatedDefs.Last = defs + 2; m.InstantiatedDefs.End = defs + 2; m.InstantiatedDefs.shrunk = 0;
    m.RemoveUnusedMemory();
    int bad = 0;
    if (m.InstantiatedDefs.shrunk != 1 || g_symbolShrinks != 1) bad = 1;
    for (int c = 0; c < 3; ++c) if (classes[c].Info.GlobalIndices.shrunk != 1) bad = 2;
    if (subA.shrunk != 1 || g_shrinks != 1 + 3 + 1) bad = 3;
    if (bad) { std::printf("009ae5f0_TEST FAIL\n"); return 1; }
    std::printf("009ae5f0_TEST PASS\n");
    return 0;
}