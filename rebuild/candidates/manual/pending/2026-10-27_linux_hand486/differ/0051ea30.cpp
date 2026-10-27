// CThingManager::FillRenderListsWithAllEntities() -- marks the graphic appearance of every allocated thing of each used thing type (types 1..15) as changed, so the render lists pick them all up
// Grade: DIFFER(86v88) -- register allocation only: retail keeps `this` in ecx through the outer loop (restored from its spill slot after the inner walk) and re-reads the ClassTypeInfo base [this+4] for GetTypeList, testing Used through dl; VC here keeps the base in ecx and reloads `this` from the spill at the loop top. Tried a direct unsigned-char index, the IsClassTypeRegistered/GetTypeList(unsigned char) inline split (this shape: same instructions bar the allocation) and a const operator[] (3 grades).
// Leaf: ?FillRenderListsWithAllEntities@CThingManager@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0051EA30, 88 bytes, thiscall, plain `ret`.
// Identity (probable): the only caller is CThingManager::SetUpdateAndRenderAllFlag(bool) 0x0051EA90 (it stores
//   UpdateAndRenderAllFlag +0x99 and then calls this row). The walk is over the CClassFactory base's
//   ClassTypeInfo vector (+0x04; retail CClassTypeInfo is 0x14 bytes: list<CClass*> TypeList +0, NoAllocated +4,
//   bool Used +8, bool Local +9, AllocFunc +0xC, CCharString Name +0x10) for thing types 1..15, calling the CThing
//   appearance forwarder 0x004C7590 (ecx = node data, `push 0Fh`; it forwards to
//   NGraphicAppearance::CTCGraphicAppearance::SetAsChanged(unsigned long) 0x004BF9E0 when the thing has one) on each
//   listed thing. Ego_r.pdb gives CThingManager three private no-argument methods (CheckOwnerLevelIDs,
//   ClearRenderList, FillRenderListsWithAllEntities); the one that touches every entity for the render-all flag is
//   FillRenderListsWithAllEntities. The forwarder's name is not in either PDB (landed rows bind it as
//   SetGraphicAppearanceChanged / UpdateGraphicAppearance); this source spells it SetGraphicAppearanceChanged.
// Shape: a long type index; CClassFactory::IsClassTypeRegistered(type) tests Used, and CThingManager::GetTypeList(unsigned
//   char) (Ego_r.pdb `E` argument, `movzx eax,bl`) returns the list, so the entry address is recomputed from the reloaded base,
//   the list end reloaded every pass, `this` spilled across the inner loop.
class CCharString {
public:
    void* PStringData;
};

class CClass {
public:
    void* _vfptr;
};

class CThing : public CClass {
public:
    void SetGraphicAppearanceChanged(unsigned long flags);     // 0x004C7590
};

namespace std {
template <class T> class allocator { };
struct _List_node_base {
    _List_node_base* _M_next;                                 // +0
    _List_node_base* _M_prev;                                 // +4
};
template <class T> struct _List_node : public _List_node_base {
    T _M_data;                                                // +8
};
template <class T> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* n) : _M_node(n) { }
    T& operator*() const { return ((_List_node<T>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& x) const { return _M_node != x._M_node; }
};
template <class T, class A = allocator<T> > class list {
public:
    _List_node_base* _M_node;                                 // the header node
    _List_iterator<T> begin() { return _List_iterator<T>(_M_node->_M_next); }
    _List_iterator<T> end() { return _List_iterator<T>(_M_node); }
};
template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T& operator[](unsigned int n) { return _M_start[n]; }
};
}

class CClassTypeInfo {                                        // 0x14 bytes (retail)
public:
    std::list<CClass*> TypeList;                              // +0x00
    unsigned long      NoAllocated;                           // +0x04
    bool               Used;                                  // +0x08
    bool               Local;                                 // +0x09
    CClass* (__cdecl*  AllocFunc)();                          // +0x0C
    CCharString        Name;                                  // +0x10
};

class CClassFactory {
public:
    void*                        _vfptr;                      // +0x00 (CBaseClassNonCopyable)
    std::vector<CClassTypeInfo>  ClassTypeInfo;               // +0x04
protected:
    bool IsClassTypeRegistered(unsigned long type) const { return ClassTypeInfo._M_start[type].Used; }
    std::list<CClass*>& GetTypeList(unsigned long type) { return ClassTypeInfo[type].TypeList; }
};

class CThingManager : public CClassFactory {
public:
    std::list<CClass*>& GetTypeList(unsigned char type) { return CClassFactory::GetTypeList(type); }
private:
    void FillRenderListsWithAllEntities();
};

void CThingManager::FillRenderListsWithAllEntities()
{
    for (long type = 1; type < 16; ++type)
    {
        if (IsClassTypeRegistered(type))
        {
            std::list<CClass*>& things = GetTypeList((unsigned char)type);
            for (std::_List_iterator<CClass*> it = things.begin(); it != things.end(); ++it)
                static_cast<CThing*>(*it)->SetGraphicAppearanceChanged(0xF);
        }
    }
}
