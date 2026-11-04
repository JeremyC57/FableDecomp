// Behaviour test for CSymbolMap::DeallocateUnusedMemory 0x009E5350: trimming the symbol map trims its SLONG table (capacity drops to the
// element count) and keeps every symbol.
#include <cstdio>
#include <vector>
#include <string>
#include <utility>
struct CSymbolInfo { std::string GroupName, EntryName; long Val; };
template <class T> struct CArray : std::vector<T> { void DeallocateUnusedMemory() { std::vector<T>(*this).swap(*this); } };
struct CSymbolMap {
    void* VPtr; CArray<std::pair<std::string, CSymbolInfo> > SLONGMap;
    void DeallocateUnusedMemory() { SLONGMap.DeallocateUnusedMemory(); }
};
int main()
{
    CSymbolMap m;
    m.SLONGMap.reserve(64);
    CSymbolInfo info = { "g", "e", 5 };
    m.SLONGMap.push_back(std::make_pair(std::string("SYM_A"), info));
    m.SLONGMap.push_back(std::make_pair(std::string("SYM_B"), info));
    m.DeallocateUnusedMemory();
    if (m.SLONGMap.size() != 2 || m.SLONGMap.capacity() != 2 || m.SLONGMap[1].first != "SYM_B" || m.SLONGMap[0].second.Val != 5) { std::printf("009e5350_TEST FAIL\n"); return 1; }
    std::printf("009e5350_TEST PASS\n");
    return 0;
}