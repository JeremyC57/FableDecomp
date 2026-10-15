// CQ_MinionCampScript::GetRandomSpawnPoint(std::list<CScriptThing>& points, CScriptThing& out) -- copy a uniformly chosen spawn point of the list into `out`; nothing happens for an empty list or once the script thread terminates
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DA0E40, length 141 bytes (padded to 144), `ret 8`.
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (x4), the list's out-of-line size() @ 0x00DA0ED0 (catalog
// label "CGuiControlTreePane::SortTreeRecursively" is propagated: the body counts the nodes from the header),
// rand @ 0x00BFEB16, CScriptThing::operator= @ 0x008AB980.
// Layout: STLport list = one header-node pointer; node {next +0, prev +4, CScriptThing data +8}.
// Shape: the index is `rand() % points.size()` (unsigned `div`; VC7.1 calls size() first); the walk counts i from 0 with
// end() re-read every pass; every exit re-tests the thread; the call after the loop keeps no test because both arms
// return.
extern "C" int __cdecl rand();                     // 0x00BFEB16
struct CScriptThing {
    unsigned char _body[12];
    CScriptThing& operator=(const CScriptThing& other);   // 0x008AB980
};
struct CListNode_CScriptThing {
    CListNode_CScriptThing* _M_next;               // +0x00
    CListNode_CScriptThing* _M_prev;               // +0x04
    CScriptThing            _M_data;               // +0x08
};
struct CRetailList_CScriptThing {                  // STLport list<CScriptThing>
    CListNode_CScriptThing* _M_node;               // the header
    bool empty() const { return _M_node->_M_next == _M_node; }
    unsigned int size() const;                     // 0x00DA0ED0 (out of line in this module)
    CListNode_CScriptThing* begin() const { return _M_node->_M_next; }
    CListNode_CScriptThing* end() const { return _M_node; }
};
struct CScriptBase {
    bool IsActiveThreadTerminating();              // 0x00CB7940
};
struct CQ_MinionCampScript : CScriptBase {
    void GetRandomSpawnPoint(CRetailList_CScriptThing& points, CScriptThing& out);
};
void CQ_MinionCampScript::GetRandomSpawnPoint(CRetailList_CScriptThing& points, CScriptThing& out)
{
    if (points.empty())
        return;
    if (IsActiveThreadTerminating())
        return;
    unsigned int index = rand() % points.size();
    unsigned int i = 0;
    for (CListNode_CScriptThing* it = points.begin(); it != points.end(); it = it->_M_next, ++i) {
        if (IsActiveThreadTerminating())
            return;
        if (i == index) {
            if (IsActiveThreadTerminating())
                return;
            out = it->_M_data;
            return;
        }
    }
    if (IsActiveThreadTerminating())
        return;
}
