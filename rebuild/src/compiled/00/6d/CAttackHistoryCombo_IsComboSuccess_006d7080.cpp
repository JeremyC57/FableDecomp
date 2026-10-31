// CAttackHistoryCombo::IsComboSuccess(const list<EDamageAttribute>& history) const -- match the combo pattern against the end of the attack history, newest first: each step requires a specific attribute, the same attribute as the previous attack, or a different one
// Grade: MATCH (retry wave 2 sec2b): the history advances as attack = *attackIt++ (reverse_iterator post-increment returns the copy that is dereferenced)
// Retail: Fable.exe (TLC, VC7.1) @ 0x006D7080, 146 bytes (+14 int3), __fastcall, ret 4, returns bool.
// No callees. Ego_r.pdb CAttackHistoryCombo: AttackCombo, a vector of {EAttackHistoryType, EDamageAttribute}
// pairs (retail 12-byte vector at +0), Multiplier +0x0C. The history is a list whose first word is the
// head node (node: next, prev, value +8).
// Shape: an empty history fails, an empty combo succeeds; the history is walked backwards from the last
// node with the current attack in edx, the combo backwards through a reverse iterator (`[ebx-8]` type,
// `[ebx-4]` attribute). A walked-off history (or a -1 attribute) turns the current attack into -1, which
// fails the next step. The history's begin node is a local homed in the dead argument slot. The
// history is walked with a reverse iterator (base kept in edi, compared against rend() == begin()).
enum EAttackHistoryType {
    ATTACK_HISTORY_SAME_AS_PREVIOUS = 0,
    ATTACK_HISTORY_DIFFERENT_FROM_PREVIOUS = 1,
    ATTACK_HISTORY_SPECIFIC = 2
};
enum EDamageAttribute { DAMAGE_ATTRIBUTE_NONE = -1 };

struct SAttackNode {
    SAttackNode* next;
    SAttackNode* prev;
    EDamageAttribute value;
};

struct CAttackList {                       // the list's head node pointer
    SAttackNode* m_head;
    class const_iterator {
    public:
        const SAttackNode* m_node;
        const_iterator() {}
        const_iterator(const SAttackNode* node) : m_node(node) {}
        EDamageAttribute operator*() const { return m_node->value; }
        const_iterator& operator--() { m_node = m_node->prev; return *this; }
        bool operator==(const const_iterator& o) const { return m_node == o.m_node; }
        bool operator!=(const const_iterator& o) const { return m_node != o.m_node; }
    };
    class const_reverse_iterator {
    public:
        const SAttackNode* m_base;
        const_reverse_iterator(const SAttackNode* base) : m_base(base) {}
        EDamageAttribute operator*() const { const_iterator tmp(m_base); --tmp; return *tmp; }
        const_reverse_iterator& operator++() { m_base = m_base->prev; return *this; }
        const_reverse_iterator operator++(int) { const_reverse_iterator tmp = *this; ++*this; return tmp; }
        bool operator!=(const const_reverse_iterator& o) const { return m_base != o.m_base; }
    };
    const_iterator begin() const { return const_iterator(m_head->next); }
    const_iterator end() const { return const_iterator(m_head); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(m_head); }
    const_reverse_iterator rend() const { return const_reverse_iterator(m_head->next); }
    bool empty() const { return m_head->next == m_head; }
};

struct SAttackComboStep {                  // pair<EAttackHistoryType, EDamageAttribute>
    EAttackHistoryType first;
    EDamageAttribute second;
};

class CAttackHistoryCombo { public:
    SAttackComboStep* m_pFirst;            // +0x00 AttackCombo
    SAttackComboStep* m_pLast;             // +0x04
    SAttackComboStep* m_pEnd;              // +0x08
    float Multiplier;                      // +0x0C
    bool IsComboSuccess(const CAttackList& history) const;
};

bool CAttackHistoryCombo::IsComboSuccess(const CAttackList& history) const
{
    if (history.empty())
        return false;
    CAttackList::const_reverse_iterator attackIt = history.rbegin();
    EDamageAttribute attack = *attackIt++;
    for (const SAttackComboStep* step = m_pLast; step != m_pFirst; --step) {
        if (attack == DAMAGE_ATTRIBUTE_NONE)
            return false;
        EAttackHistoryType type = step[-1].first;
        if (type == ATTACK_HISTORY_SPECIFIC && attack != step[-1].second)
            return false;
        if (attackIt != history.rend()) {
            EDamageAttribute previous = *attackIt;
            if (previous != DAMAGE_ATTRIBUTE_NONE) {
                if (type == ATTACK_HISTORY_DIFFERENT_FROM_PREVIOUS) {
                    if (attack == previous)
                        return false;
                }
                else if (type == ATTACK_HISTORY_SAME_AS_PREVIOUS) {
                    if (attack != previous)
                        return false;
                }
                attack = *attackIt++;
                continue;
            }
        }
        attack = DAMAGE_ATTRIBUTE_NONE;
    }
    return true;
}