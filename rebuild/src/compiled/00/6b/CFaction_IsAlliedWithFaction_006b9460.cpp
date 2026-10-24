// CFaction::IsAlliedWithFaction(const CFaction& faction) const -- a faction is allied with itself (same name) and with every faction named in its AlliedFactions set
// Leaf: ?IsAlliedWithFaction@CFaction@@QBE_NABV1@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B9460, 80 bytes (+ int3 padding), `ret 4` (thiscall).
// Callers: CTCEnemy::IsAlliedWithFaction 0x0076C7F0 (tail jump) and CTCEnemy::IsAllyOf 0x0076D460 (0x0076D4D1).
// Callees: Compare(const char*, const char*) @ 0x00411570 (`__fastcall`, the inlined CCharString equality, landed body
//   pasted under auto_inline(off)), set<CCharString>::_M_find(const CCharString&) const @ 0x004B9370 (node in eax, `ret 4`).
// Identity: Ego_r.pdb ?IsAlliedWithFaction@CFaction@@QBE_NABV1@@Z (its sibling IsEnemyOfFaction tests EnemyFactions).
// Layout: CFaction (Ego_r.pdb, no engine header) Name CCharString +0, AlliedFactions set<CCharString> +4, EnemyFactions
//   +0x10 -- the retail STLport set is 12 bytes (_M_header +0, _M_node_count +4, the empty less<> +8), so `end()` is the
//   header word at this+4.
// Shape: `return faction.Name == Name || AlliedFactions.count(faction.Name) != 0;` -- the `||` value in a bool function
//   gives retail's full-width `mov eax,1` / `xor eax,eax` exits (two `if (..) return true;` give `mov al,1` + `setne`).
//   The inline CCharString operator== takes references (same block -> equal; both blocks non-null with equal lengths ->
//   `Compare(..) ? false : true` = `neg ; sbb al,al ; inc al`); STLport `set::count` is `_M_find(k) == end ? 0 : 1`, which
//   reads the header word AFTER the out-of-line `_M_find` call (`find(..) != end()` evaluates end() first and hoists it).
// Label correction: the catalog `FactionMap_FindAndCompare` is descriptive.
struct CCharStringData {                                // the shared string block
    const char*   m_pData;                              // +0x00
    unsigned long m_length;                             // +0x04
};

#pragma auto_inline(off)
long __fastcall Compare_00411570(const char* a, const char* b)   // 0x00411570 (the landed body)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)

class CCharString {
public:
    CCharStringData* PStringData;                       // +0x00
    bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData && PStringData->m_length == other.PStringData->m_length)
            return Compare_00411570(PStringData->m_pData, other.PStringData->m_pData) ? false : true;
        return false;
    }
};

struct _Rb_tree_node_base;

struct CCharStringSet {                                 // STLport set<CCharString> (12 bytes)
    _Rb_tree_node_base* _M_header;                      // +0x00 (end())
    unsigned int        _M_node_count;                  // +0x04
    unsigned char       _M_key_compare;                 // +0x08 less<CCharString> (empty)
    _Rb_tree_node_base* _M_find(const CCharString& k) const;    // 0x004B9370
    unsigned int count(const CCharString& k) const { return _M_find(k) == _M_header ? 0 : 1; }
};

class CFaction {
public:
    CCharString    Name;                                // +0x00
    CCharStringSet AlliedFactions;                      // +0x04
    CCharStringSet EnemyFactions;                       // +0x10
    bool IsAlliedWithFaction(const CFaction& faction) const;
};

bool CFaction::IsAlliedWithFaction(const CFaction& faction) const
{
    return faction.Name == Name || AlliedFactions.count(faction.Name) != 0;
}