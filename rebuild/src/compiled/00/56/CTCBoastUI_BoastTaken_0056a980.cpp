// CTCBoastUI::BoastTaken(const CQuestManager::CBoast*) -- a boast the hero has taken leaves the boast-group map, every group menu sharing its group bits re-selects its first type-8 child, a boasting crowd hears of it, and with no boast left the UI closes before the hero plays "ST_BOAST"
// Leaf: BoastTaken
// Retail 0x0056A980, 656 bytes, `ret 4`. Callees: _Rb_tree_rebalance_for_erase 0x0042A34E (cdecl), ~CWideString
// 0x0099B510, ~CCharString 0x0099EAE0, free 0x00BFEA14, CTCBoastingCrowdControl::OnHeroBoast 0x00783C50 and
// DetachAll 0x00783680, SetAutomaticCameraControl 0x00699090, CPlayerGui::Close 0x004374B0, CCharString 0x0099EBF0,
// CCreatureAction_PlayAnimation 0x00843A80, CThingCreatureBase::SetCurrentAction 0x006644F0, ~CCreatureActionBase 0x00693EF0.
// Layout: retail CTCBoastUI (12-byte STLport map at +0xc, Active +0x18, Quitting +0x28, ActivationRequested +0x2a) as
// the landed 0x0056AC10/0x0056A880; CBoast from Ego_r.pdb (ID +0, ScriptName +0xc, Text +0x10, Title +0x14,
// GroupIndex +0x1c); map node value at +0x10; CThing TC bitset +0x20, interface vector +0x44/+0x48; quest
// manager 0x013B89FC with a list at +0x68.
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14

class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CWideString { public:
    void* m_pData;
    ~CWideString();                                     // 0x0099B510
};
struct CBoast {                                         // CQuestManager::CBoast
    long        ID;
    long        Cost;
    long        Reward;
    CCharString ScriptName;
    CWideString Text;
    CWideString Title;
    bool        Evil, Failed, Completed;
    long        GroupIndex;
};
struct CComponent;
struct SChild { CComponent* Component; long Data; };
struct CChildVector {
    SChild* _M_start;
    SChild* _M_finish;
    SChild* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    SChild& operator[](unsigned long i) { return _M_start[i]; }
};
struct CComponent {
    virtual void c00();
    virtual void c01();
    virtual void c02();
    virtual void c03();
    virtual void c04();
    virtual void c05();
    virtual void c06();
    virtual void c07();
    virtual void c08();
    virtual void c09();
    virtual void c0a();
    virtual void c0b();
    virtual void c0c();
    virtual void c0d();
    virtual void c0e();
    virtual void c0f();
    virtual void c10();
    virtual void c11();
    virtual void c12();
    virtual void c13();
    virtual void c14();
    virtual void c15();
    virtual void c16();
    virtual void c17();
    virtual void c18();
    virtual void c19();
    virtual void c1a();
    virtual void c1b();
    virtual void c1c();
    virtual void c1d();
    virtual void c1e();
    virtual void c1f();
    virtual void c20();
    virtual void c21();
    virtual void c22();
    virtual void c23();
    virtual void c24();
    virtual void c25();
    virtual void c26();
    virtual void c27();
    virtual void c28();
    virtual void c29();
    virtual void c2a();
    virtual void c2b();
    virtual void c2c();
    virtual void c2d();
    virtual void c2e();
    virtual void c2f();
    virtual void c30();
    virtual void c31();
    virtual void c32();
    virtual void c33();
    virtual void c34();
    virtual void c35();
    virtual void c36();
    virtual void c37();
    virtual void c38();
    virtual void c39();
    virtual void c3a();
    virtual void c3b();
    virtual void c3c();
    virtual void c3d();
    virtual void c3e();
    virtual void c3f();
    virtual void c40();
    virtual long GetType();                            // +0x104
};
struct CMenuEntry {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v0a();
    virtual void v0b();
    virtual void v0c();
    virtual void v0d();
    virtual void v0e();
    virtual void v0f();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v1a();
    virtual void v1b();
    virtual void v1c();
    virtual void v1d();
    virtual void v1e();
    virtual void v1f();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v2a();
    virtual void v2b();
    virtual void v2c();
    virtual void v2d();
    virtual void v2e();
    virtual void v2f();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual CChildVector& GetChildren();               // +0xd4
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v3a();
    virtual void v3b();
    virtual void v3c();
    virtual void v3d();
    virtual void v3e();
    virtual void v3f();
    virtual void SetSelection(unsigned long index);     // +0x100
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v4a();
    virtual void v4b();
    virtual void v4c();
    virtual void v4d();
    virtual void v4e();
    virtual void v4f();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v5a();
    virtual void v5b();
    virtual void v5c();
    virtual void Refresh();                            // +0x174
};

struct _Rb_tree_node_base {
    char                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
_Rb_tree_node_base* __cdecl _Rebalance_for_erase(_Rb_tree_node_base* z, _Rb_tree_node_base*& root,
    _Rb_tree_node_base*& leftmost, _Rb_tree_node_base*& rightmost);   // 0x0042A34E
struct SBoastNode : _Rb_tree_node_base {
    CMenuEntry* first;
    CBoast      second;
};
struct CBoastIterator {
    _Rb_tree_node_base* _M_node;
    SBoastNode* operator->() const { return (SBoastNode*)_M_node; }
    bool operator!=(const CBoastIterator& o) const { return _M_node != o._M_node; }
    void operator++()
    {
        if (_M_node->_M_right != 0) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != 0)
                _M_node = _M_node->_M_left;
        } else {
            _Rb_tree_node_base* y = _M_node->_M_parent;
            while (_M_node == y->_M_right) {
                _M_node = y;
                y = y->_M_parent;
            }
            if (_M_node->_M_right != y)
                _M_node = y;
        }
    }
};
struct CBoastMap {
    _Rb_tree_node_base* _M_header;
    unsigned long       _M_node_count;
    unsigned char       _M_key_compare;
    CBoastIterator begin() { CBoastIterator it; it._M_node = _M_header->_M_left; return it; }
    CBoastIterator end() { CBoastIterator it; it._M_node = _M_header; return it; }
    void erase(CBoastIterator pos)
    {
        SBoastNode* y = (SBoastNode*)_Rebalance_for_erase(pos._M_node, _M_header->_M_parent,
            _M_header->_M_left, _M_header->_M_right);
        y->second.~CBoast();
        if (y)
            free(y);
        --_M_node_count;
    }
};

class CTCBase;
struct SInterfaceEntry { long type; CTCBase* component; };
class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _body[0xa4];
    ~CCreatureActionBase();                             // 0x00693EF0
};
class CThing;
class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
class CCreatureAction_PlayAnimation : public CCreatureActionBase { public:
    unsigned char _anim[0x0c];
    CCreatureAction_PlayAnimation(CThing* creature, const CCharString& animation, float blend,
        const CSoundPair& sound, bool a, bool b, bool c, long d, long e);   // 0x00843A80
};
class CThing { public:
    unsigned char    _pad0[0x20];
    unsigned long    m_presentBits[9];                  // +0x20
    SInterfaceEntry* m_interfacesFirst;                 // +0x44
    SInterfaceEntry* m_interfacesLast;                  // +0x48
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long type)
    {
        int len = int(m_interfacesLast - m_interfacesFirst);
        SInterfaceEntry* it = m_interfacesFirst;
        while (len > 0) {
            int half = len / 2;
            SInterfaceEntry* middle = it + half;
            if (middle->type < type) {
                it = middle + 1;
                len = len - half - 1;
            } else
                len = half;
        }
        if (it == m_interfacesLast || type < it->type)
            it = m_interfacesLast;
        return it->component;
    }
    void SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};
class CTCBoastingCrowdControl { public:
    void OnHeroBoast(const CBoast* boast);              // 0x00783C50
    void DetachAll();                                   // 0x00783680
};

class CGameCameraManager { public:
    void SetAutomaticCameraControl();                   // 0x00699090
};
struct CGameWorldCalls {
    unsigned char       _pad00[0x34];
    CGameCameraManager* m_pCameraManager;               // +0x34
};
struct CEngineCalls {
    unsigned char    _pad00[0x24];
    CGameWorldCalls* m_pWorld;                          // +0x24
};
extern CEngineCalls* g_pEngine;                         // 0x013B86A0
struct CPlayerGuiCalls {
    void Close();                                       // 0x004374B0
};
extern CPlayerGuiCalls* g_pPlayerGui;                   // 0x013B8790

struct SListNode { SListNode* _M_next; SListNode* _M_prev; };
struct CBoastList {
    SListNode* _M_node;
    bool empty() const { return _M_node->_M_next == _M_node; }
};
class CQuestManager { public:
    unsigned char _pad00[0x68];
    CBoastList    AcceptedBoasts;                       // +0x68
};
extern CQuestManager* g_pQuestManager;                  // 0x013B89FC

class CTCBoastUI { public:
    void*         __vftable;                            // +0x00
    CThing*       ParentThing;                          // +0x04
    unsigned char _pad08[0x4];
    CBoastMap     BoastGroups;                          // +0x0c
    bool          Active;                               // +0x18
    unsigned char _pad19[0x28 - 0x19];
    bool          Quitting;                             // +0x28
    bool          AllowedToOpen;                        // +0x29
    bool          ActivationRequested;                  // +0x2a
    bool          ActivateLater;                        // +0x2b
    void Deactivate()
    {
        if (ActivationRequested) {
            if (Active) {
                g_pEngine->m_pWorld->m_pCameraManager->SetAutomaticCameraControl();
                Quitting = true;
                g_pPlayerGui->Close();
            } else {
                ActivationRequested = false;
            }
        }
    }
    void BoastTaken(const CBoast* boast);
};

void CTCBoastUI::BoastTaken(const CBoast* boast)
{
    for (CBoastIterator it = BoastGroups.begin(); it != BoastGroups.end(); ++it) {
        if (it->second.ID == boast->ID) {
            BoastGroups.erase(it);
            break;
        }
    }
    for (CBoastIterator it = BoastGroups.begin(); it != BoastGroups.end(); ++it) {
        if (it->second.GroupIndex & boast->GroupIndex) {
            it->first->Refresh();
            for (unsigned long i = 0; i != it->first->GetChildren().size(); ++i) {
                if (it->first->GetChildren()[i].Component->GetType() == 8) {
                    it->first->SetSelection(i);
                    break;
                }
            }
        }
    }
    CTCBoastingCrowdControl* crowd = 0;
    if (ParentThing->HasTC(0x73)) {
        crowd = (CTCBoastingCrowdControl*)ParentThing->GetTC(0x73);
        crowd->OnHeroBoast(boast);
    }
    if (g_pQuestManager->AcceptedBoasts.empty()) {
        if (crowd)
            crowd->DetachAll();
        Deactivate();
    }
    CThing* creature = ParentThing;
    CCharString animation("ST_BOAST");
    creature->SetCurrentAction(CCreatureAction_PlayAnimation(creature, animation, 0.0f,
        CSoundPair(), false, true, false, 0, 0));
}