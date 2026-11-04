// Behaviour test for CResourceBank::RemoveResource 0x009FC3C0: removing a resource from the bank removes it from the bank's resource
// list only (the loading queue keeps its own entries).
#include <cstdio>
#include <list>
#include <algorithm>
struct CResource { int id; };
struct CResourceList {
    std::list<CResource*> Items;
    void AddResource(CResource& r) { Items.push_back(&r); }
    void RemoveResource(CResource& r) { Items.remove(&r); }
    bool Has(CResource& r) const { return std::find(Items.begin(), Items.end(), &r) != Items.end(); }
};
struct CResourceBank {
    void* VPtr; CResourceList ResourceList; CResourceList LoadingQueue;
    void RemoveResource(CResource& r) { ResourceList.RemoveResource(r); }
};
int main()
{
    CResource a = { 1 }, b = { 2 };
    CResourceBank bank;
    bank.ResourceList.AddResource(a); bank.ResourceList.AddResource(b); bank.LoadingQueue.AddResource(a);
    bank.RemoveResource(a);
    if (bank.ResourceList.Has(a) || !bank.ResourceList.Has(b) || !bank.LoadingQueue.Has(a)) { std::printf("009fc3c0_TEST FAIL\n"); return 1; }
    std::printf("009fc3c0_TEST PASS\n");
    return 0;
}