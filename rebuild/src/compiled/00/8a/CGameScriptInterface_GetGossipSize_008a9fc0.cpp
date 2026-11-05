// CGameScriptInterface::GetGossipVillagesSize @ 008aa220
// Leaf: ?CGameScriptInterface_GetGossipSize@@YIHPAXVCCharString@@@Z
// __fastcall int GetGossipVillagesSize(CCharString name) ; name by value

class CCharString { public:
    void* p;
    CCharString(const CCharString& o);
    ~CCharString();
};

struct CArrayHdr {
    int* first; // +0
    int* last;  // +4
};

struct GossipMap {
    CArrayHdr& op_index(const CCharString& key);
};

extern GossipMap g_gossipVillagesMap; // at 0x13bae50 (reloc-masked)

int __fastcall CGameScriptInterface_GetGossipSize(void* self, CCharString category)
{
    CCharString key(category);
    CArrayHdr& e = g_gossipVillagesMap.op_index(key);
    int first = (int)e.first;
    int last = (int)e.last;
    return (last - first) >> 2;
}
// Stand-in (owner rule 2026-11-05): CGameScriptInterface_GetGossipSize for CGameScriptInterface::GetGossipSize; RTTI vtable slot ?GetGossipSize@CGameScriptInterface@@UAEHVCCharString@@@Z + PDB name set (egor/fablewin).
