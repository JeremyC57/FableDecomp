// CTCQuickAccessMenu::GlobalUpdate(bool) -- tear the menu down once it is quitting, else arm Quitting when the game leaves the menu's mode
// Leaf: GlobalUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00625ED0, length 569 bytes; vtable 0x01258B74 (CTCQuickAccessMenu) slot 26,
// the CTCBase GlobalUpdate(bool) slot (catalog name vfunc_26).
// Callees: CManager::GetInstance @ 0x0041E5F2 (slots +0x18, +0x74, +0x94, +0xa8, +0xa0), CRT free @ 0x00BFEA14,
// memmove @ 0x00BFEAE6, CTCBase::GetPlayerManager @ 0x00686D40, CPlayerManager::GetMainPlayer @ 0x00449970,
// CPlayer::RemoveMode @ 0x00634190, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020, the STLport
// _Rb_tree::_M_erase @ 0x00592BCE (catalog "LTextTreeWalkThrough::BuildTreeArray", propagated),
// CPlayerGui::SetSpecialAbilityButtonsAsDisplaying(bool, bool) @ 0x00437580.
// Globals: the quick-access node list @ 0x013B8A98 (an STLport list whose node allocator frees with the CRT free),
// the engine singleton @ 0x013B86A0, the player GUI @ 0x013B8790.
// Layout: Ego_r.pdb CTCQuickAccessMenu as in the landed ctor 0x006259E0 (Active +0x0c, the 12-byte Components vector
// +0x10, Quitting +0x1c). The interface-0x11 component carries two STLport sets at +0x1fc / +0x208 ({header, count}).
// Shape: Components.clear() as the STLport erase(begin(), end()) with a trivial pointer copy; the list clear and the two
// set clears are the inlined STLport bodies; the front-end counted result is released with the DecRef/GetRefCount pair.
#include <string.h>
#include <stdlib.h>

class CThing;
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                      // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);                   // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct CThingIface {                                        // CThing: interface bitset +0x20, interface map +0x44
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20
    CInterfaceMap m_interfaces;                             // +0x44
    bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T>
    __forceinline void GetInterface(long type, T** out)
    {
        if (HasTC(type)) {
            ETCInterfaceType key = (ETCInterfaceType)(type);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(key);
            *out = (T*)((it == map->last || type < it->first) ? map->last : it)->second;
        }
    }
};

namespace NUISystem {
class CComponent { public: virtual ~CComponent(); };
}

struct SRbNode { long color; SRbNode* parent; SRbNode* left; SRbNode* right; };
struct CRbTree {                                            // STLport _Rb_tree: header node pointer + count
    SRbNode*      _M_header;
    unsigned long _M_node_count;
    struct SLess { } _M_key_compare;                        // empty less<> (pads the tree to 12 bytes)
    void _M_erase(SRbNode* x);                              // 0x00592BCE
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_header->parent);
            _M_header->left = _M_header;
            _M_header->parent = 0;
            _M_header->right = _M_header;
            _M_node_count = 0;
        }
    }
};

struct CTC11 {                                              // the interface-0x11 component (only its two sets are touched)
    unsigned char _pad0[0x1fc];
    CRbTree       m_setA;                                   // +0x1fc
    CRbTree       m_setB;                                   // +0x208
};

struct CTCFC {                                              // the interface-0xfc component
    unsigned char _pad0[0xc];
    bool          m_flag0c;                                 // +0x0c
};

struct SListNode { SListNode* next; SListNode* prev; };
struct CNodeList {                                          // STLport list: node pointer only
    SListNode* _M_node;
    void clear()
    {
        SListNode* cur = _M_node->next;
        while (cur != _M_node) {
            SListNode* tmp = cur;
            cur = cur->next;
            free(tmp);
        }
        _M_node->next = _M_node;
        _M_node->prev = _M_node;
    }
};
extern CNodeList g_QuickAccessList;                         // 0x013B8A98

struct CRefCounted {
    virtual void v00();
    virtual void Destroy();                                 // slot 1
    long RefCount;                                          // +0x04
    void DecRef() { --RefCount; }
    long GetRefCount() const { return RefCount; }
    void Release()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CCountedRef {
    CRefCounted* Data;
    ~CCountedRef() { if (Data) Data->Release(); }
};
struct CFrontEndData : CRefCounted { };

namespace NUISystem { class CManager { public:
    static CManager* __fastcall GetInstance();      // 0x0041E5F2
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void Refresh();                                 // +0x18
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual void PopScreen();                               // +0x74
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36();
    virtual void ResetInput();                              // +0x94
    virtual void v38(); virtual void v39();
    virtual void SetScreenData(char* data, bool flag);      // +0xa0
    virtual void v41();
    virtual CCountedRef GetScreenData();                    // +0xa8
}; }

class CPlayer { public: void RemoveMode(long mode); };             // 0x00634190
class CPlayerManager { public: CPlayer& GetMainPlayer(); };        // 0x00449970
class CPlayerGui { public: void SetSpecialAbilityButtonsAsDisplaying(bool a, bool b); };   // 0x00437580
extern CPlayerGui* g_pPlayerGui;                            // 0x013B8790

struct CGameState { unsigned char _pad[0x104]; long Mode; };
class CEngine { public: unsigned char _pad[0x24]; CGameState* State; };
extern CEngine* g_pEngine;                                  // 0x013B86A0

struct CRetailVector_Component {                            // retail std::vector, 12 bytes
    NUISystem::CComponent** _M_start;
    NUISystem::CComponent** _M_finish;
    NUISystem::CComponent** _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    static NUISystem::CComponent** CopyPtrs(NUISystem::CComponent** first, NUISystem::CComponent** last,
                                            NUISystem::CComponent** result)
    {
        if (last == first)
            return result;
        int n = (char*)last - (char*)first;
        return (NUISystem::CComponent**)((char*)memmove(result, first, n) + n);
    }
    void erase(NUISystem::CComponent** first, NUISystem::CComponent** last)
    {
        _M_finish = CopyPtrs(last, _M_finish, first);
    }
    void clear() { erase(_M_start, _M_finish); }
};

class CTCBase
{
protected:
    CPlayerManager& GetPlayerManager();   // 0x00686D40
};

class CTCQuickAccessMenu : public CTCBase { public:                                 // vtable 0x01258B74
    void*                   __vftable;                      // +0x00
    CThingIface*            ParentThing;                    // +0x04
    bool                    ToBeRemoved;                    // +0x08
    bool                    IsInGlobalUpdateTCs;            // +0x09
    unsigned char           _pad0a[2];                      // +0x0a
    bool                    Active;                         // +0x0c
    CRetailVector_Component Components;                     // +0x10
    bool                    Quitting;                       // +0x1c
    void GlobalUpdate(bool);
};

void CTCQuickAccessMenu::GlobalUpdate(bool)
{
    if (Quitting) {
        for (unsigned int i = 0; i < Components.size(); ++i) {
            if (Components._M_start[i])
                delete Components._M_start[i];
        }
        Components.clear();
        NUISystem::CManager::GetInstance()->Refresh();
        NUISystem::CManager::GetInstance()->PopScreen();
        g_QuickAccessList.clear();
        NUISystem::CManager::GetInstance()->ResetInput();
        Active = false;
        Quitting = false;
        GetPlayerManager().GetMainPlayer().RemoveMode(0x24);
        CTC11* tc = 0;
        ParentThing->GetInterface(0x11, &tc);
        tc->m_setA.clear();
        tc->m_setB.clear();
        NUISystem::CManager* source = NUISystem::CManager::GetInstance();
        NUISystem::CManager* target = NUISystem::CManager::GetInstance();
        target->SetScreenData((char*)source->GetScreenData().Data + 0x1a8, true);
        g_pPlayerGui->SetSpecialAbilityButtonsAsDisplaying(false, true);
        return;
    }
    if (Active) {
        CGameState* st = g_pEngine->State;
        if (st->Mode != 0 && st->Mode != 9)
            Quitting = true;
        CTCFC* tc;
        ParentThing->GetInterface(0xfc, &tc);
        if (tc && tc->m_flag0c)
            Quitting = true;
    }
}