// NScript::CQ_MinionCampScript::FindFurthestSpawnPoint(const list<CScriptThing>& spawnPoints, const CScriptThing*& furthest, float& furthestDistance) -- keep the spawn point furthest from the hero (a -1 distance means none yet); false when none was taken or the thread is terminating
// Leaf: ?FindFurthestSpawnPoint@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DA0D90, 163 bytes, `__fastcall`, three stack arguments (`ret 0Ch`), bool
//   in al. FableWin symbol ?FindFurthestSpawnPoint@CQ_MinionCampScript@NScript@@QAE_NABV?$list... (truncated in the
//   dump). Callers: 0x00DA0CC1 / 0x00DA0CD9 (LeftSpawnPoints +0x90 then RightSpawnPoints +0x94, sharing one
//   `furthest = 0` / `distance = -1.0f` pair).
// Label correction: the catalog "CScriptBase_RunActiveSpawnPoints" is descriptive.
// Callees: CScriptBase::IsActiveThreadTerminating() @ 0x00CB7940, the script's Game (+0x40) virtual slot 70 (+0x118)
//   GetHero() (returns CScriptThing&), GetDistanceBetweenThings(const CScriptThing&, const CScriptThing&) @ 0x00CBE583
//   (__fastcall, float in st0; catalogued as CScriptBase::Activate).
// Shape: speed TU (no pragma). The script translator's form: a terminate guard at the top of each iteration and before
//   the update; the found flag lives in the `push ecx` slot, the distance is spilled to the dead furthestDistance-
//   pointer argument slot, `distance > furthestDistance` is `fcomp ; test ah,41h ; je`, the `== -1.0f` sentinel test
//   is VC's integer compare against 0BF800000h, and the closing `if (terminating) return false; return found;` folds
//   to `neg ; sbb ; not ; and`. The list end is re-read through the reference each iteration.
class CScriptThing {                                    // 12 bytes, vtable 0x01238C8C
public:
    virtual ~CScriptThing();
    void* PImp_Data;                                    // +0x04
    void* PImp_Info;                                    // +0x08
};
float __fastcall GetDistanceBetweenThings(const CScriptThing& a, const CScriptThing& b);   // 0x00CBE583
class CGameScriptInterfaceBase {
public:
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
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
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
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65();
    virtual void v66();
    virtual void v67();
    virtual void v68();
    virtual void v69();
    virtual CScriptThing& GetHero();                    // slot 70 (+0x118)
};

namespace _STL {
struct _List_node_base {
    _List_node_base* _M_next;                           // +0
    _List_node_base* _M_prev;                           // +4
};
template <class T> struct _List_node : public _List_node_base {
    T _M_data;                                          // +8
};
template <class T> struct _List_const_iterator {
    _List_node_base* _M_node;
    _List_const_iterator(_List_node_base* n) : _M_node(n) { }
    const T& operator*() const { return ((_List_node<T>*)_M_node)->_M_data; }
    _List_const_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_const_iterator& other) const { return _M_node != other._M_node; }
};
template <class T> class list {
public:
    typedef _List_const_iterator<T> const_iterator;
    _List_node_base* _M_node;                           // the header node (alloc proxy)
    const_iterator begin() const { return const_iterator(_M_node->_M_next); }
    const_iterator end() const { return const_iterator(_M_node); }
};
}

namespace NScript {
class CScriptBase {                                     // retail 0x40 bytes
public:
    virtual ~CScriptBase();
    unsigned char _body[0x3c];
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};
class CQ_MinionCampScript : public CScriptBase {
public:
    CGameScriptInterfaceBase* Game;                     // +0x40
    void*                     MasterScriptData;         // +0x44
    bool FindFurthestSpawnPoint(const _STL::list<CScriptThing>& spawnPoints, const CScriptThing*& furthest, float& furthestDistance);
};
bool CQ_MinionCampScript::FindFurthestSpawnPoint(const _STL::list<CScriptThing>& spawnPoints, const CScriptThing*& furthest, float& furthestDistance)
{
    bool found = false;
    for (_STL::list<CScriptThing>::const_iterator it = spawnPoints.begin(); it != spawnPoints.end(); ++it) {
        if (IsActiveThreadTerminating())
            return false;
        const CScriptThing& spawnPoint = *it;
        float distance = GetDistanceBetweenThings(spawnPoint, Game->GetHero());
        if (distance > furthestDistance || furthestDistance == -1.0f) {
            if (IsActiveThreadTerminating())
                return false;
            furthestDistance = distance;
            furthest = &spawnPoint;
            found = true;
        }
    }
    if (IsActiveThreadTerminating())
        return false;
    return found;
}
}