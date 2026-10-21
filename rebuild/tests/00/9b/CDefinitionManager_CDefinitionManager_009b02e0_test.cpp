// behaviour test for 009b02e0 CDefinitionManager::~CDefinitionManager (host g++/VC): member teardown order and counts
#include <stdio.h>
#include <stdlib.h>
static int g_freed = 0, g_wide = 0, g_char = 0, g_erased = 0, g_destroyed = 0, g_cleared = 0, g_base = 0, g_range = 0;
static void xfree(void* p) { if (p) ++g_freed; }
struct __false_type {};
struct CCharString { void* PStringData; ~CCharString() { ++g_char; } };
struct CWideString { void* PStringData; ~CWideString() { ++g_wide; } };
struct CDefClassInfo { unsigned char _body[0x18]; };
static void DestroyRange(CDefClassInfo* f, CDefClassInfo* l, const __false_type&) { g_range += (int)(l - f); }
template <class T> struct CRetailVector { T* s; T* f; T* e; ~CRetailVector() { T* last = f; for (T* p = s; p != last; ++p) p->~T(); xfree(s); } };
template <> struct CRetailVector<unsigned long> { unsigned long* s; unsigned long* f; unsigned long* e; ~CRetailVector() { xfree(s); } };
template <> struct CRetailVector<CDefClassInfo> { CDefClassInfo* s; CDefClassInfo* f; CDefClassInfo* e; ~CRetailVector() { __false_type tag; DestroyRange(s, f, tag); xfree(s); } };
template <class T> struct CVectorMap { CRetailVector<T> Entries; bool Locked; bool Sorted; };
template <class T> struct CArray { T* Data; long Count; long Capacity; ~CArray() { xfree(Data); } };
struct Node { unsigned char c; Node* parent; Node* left; Node* right; };
struct CRetailMap { Node* head; unsigned long count; void Erase(Node*) { ++g_erased; } ~CRetailMap() { if (count) { Erase(head->parent); head->left = head; head->parent = 0; head->right = head; count = 0; } xfree(head); } };
struct SCountBlock { long refs; void (*destroy)(void*); void* obj; };
static void DestroyObj(void*) { ++g_destroyed; }
template <class T> struct CCountedPointer { T* obj; SCountBlock* cnt; ~CCountedPointer() { if (cnt) { --cnt->refs; if (cnt->refs == 0) { cnt->destroy(cnt->obj); delete cnt; } } obj = 0; cnt = 0; } };
struct CResourceList { virtual void v00() {} ~CResourceList() { ++g_base; } };
struct CSymbolMap; struct CInstantiatedDefInfo;
class CDefinitionManager : public CResourceList {
public:
    CRetailVector<CWideString> SymbolPathList; CRetailVector<CWideString> CompilePathList;
    CCountedPointer<CSymbolMap> PSymbolMap; CCountedPointer<CSymbolMap> PDefinitionSymbolMap;
    CVectorMap<unsigned long> NameCRCToIndexMap; CVectorMap<CDefClassInfo> DefClassInfo;
    CArray<unsigned char> CompressedDefData; CVectorMap<unsigned long> CompressedOffsets; CArray<CInstantiatedDefInfo> InstantiatedDefs;
    void* ErrorCallback; CCharString CurrentFilename; CWideString CompiledFileName; unsigned long CRC; bool Safe; CRetailMap RecompiledDefs;
    void ClearInstantiatedDefs() { ++g_cleared; }
    ~CDefinitionManager() { ClearInstantiatedDefs(); }
};
int main()
{
    CWideString paths[3]; CDefClassInfo infos[2]; unsigned long pods[4]; Node head; Node root;
    SCountBlock* b1 = new SCountBlock; b1->refs = 1; b1->destroy = DestroyObj; b1->obj = 0;
    SCountBlock* b2 = new SCountBlock; b2->refs = 2; b2->destroy = DestroyObj; b2->obj = 0;
    {
        CDefinitionManager m;
        m.SymbolPathList.s = paths; m.SymbolPathList.f = paths + 2; m.SymbolPathList.e = paths + 3;
        m.CompilePathList.s = paths; m.CompilePathList.f = paths + 1; m.CompilePathList.e = paths + 1;
        m.PSymbolMap.obj = 0; m.PSymbolMap.cnt = b1; m.PDefinitionSymbolMap.obj = 0; m.PDefinitionSymbolMap.cnt = b2;
        m.NameCRCToIndexMap.Entries.s = pods; m.NameCRCToIndexMap.Entries.f = pods + 4; m.NameCRCToIndexMap.Entries.e = pods + 4;
        m.DefClassInfo.Entries.s = infos; m.DefClassInfo.Entries.f = infos + 2; m.DefClassInfo.Entries.e = infos + 2;
        m.CompressedDefData.Data = 0; m.CompressedOffsets.Entries.s = 0; m.InstantiatedDefs.Data = (CInstantiatedDefInfo*)pods;
        head.parent = &root; m.RecompiledDefs.head = &head; m.RecompiledDefs.count = 5;
    }
    bool ok = g_cleared == 1 && g_erased == 1 && head.left == &head && head.right == &head && head.parent == 0
           && g_wide == 2 + 1 + 1 && g_char == 1 && g_range == 2 && g_destroyed == 1 && b2->refs == 1
           && g_freed == 6 && g_base == 1;   // head, InstantiatedDefs, NameCRC, DefClassInfo, both path vectors, (CompressedDefData/Offsets null)
    delete b2;
    printf(ok ? "009b02e0_TEST PASS\n" : "009b02e0_TEST FAIL freed=%d wide=%d\n", g_freed, g_wide);
    return ok ? 0 : 1;
}