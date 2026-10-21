// CTCCombat::SetBlockPercentageChance(float percentage, EWeaponClass weaponClass) -- clamp the percentage to [0, 100] and store it (truncated) into every block-type combat ability (data type 1) whose weapon class matches
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00734210, 259 bytes (+ int3 padding), `ret 8`.
// Callees: the ability data's virtual slot 3 (+0x0c, its CCombatAbilityData type), __ftol2 @ 0x00BFEA70.
// Globals: 0.0f @ 0x0122DEDC, 100.0f @ 0x01230A08.
// Layout: Ego_r.pdb CTCCombat with the retail 12-byte STLport vectors: CombatAbilities (multimap<ECombatAbilityDataType,
//   pair<CCountedPointer<CCombatAbilityData>, CCombatAbility> >) at +0x70 (PDB +0x7c; the header-node pointer first).
//   STLport node: colour +0x00, parent +0x04, left +0x08, right +0x0c, key +0x10, the counted pointer's data +0x14.
//   The block data (descriptive): chance +0x04, block type +0x2c. engine/CTCCombat.h has the PDB offsets: local model.
// Shape: the multimap range is taken as two separate calls -- `last = upper_bound(BLOCK)` first, then the loop starts at
//   `lower_bound(BLOCK)` -- so each binds its own key temporary and the second walk re-reads the header and root
//   ([ecx+70h], [esi+4]) as retail does (one shared key through an inline equal_range keeps them in registers). The walk is a for loop
//   whose ++ is STLport's `_M_node = _Rb_global::_M_increment(_M_node)` (a static helper over a node copy, written back);
//   the percentage parameter is clamped in place (`fcomp ; test ah,5 ; jp` for `< 0`, `test ah,41h ; jne` for `> 100`)
//   and converted with __ftol2 after re-reading the data pointer through the node.
// Label: the mangled name ?SetBlockPercentageChance@CTCCombat@@QAEXMW4EWeaponClass@@@Z gives (float, EWeaponClass): retail
//   reads the float from the first argument slot and compares the block data's +0x2c against the second (the earlier
//   (long, float) order was wrong). The same name at 0x0074B7A0 is a mid-function reference, not a second body.
enum EWeaponClass { WEAPON_CLASS_0 };
enum ECombatAbilityDataType { COMBAT_ABILITY_DATA_0, COMBAT_ABILITY_DATA_BLOCK };
struct CCombatAbilityData {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual long GetType() const;               // slot 3 (+0x0c)
    long          BlockPercentageChance;        // +0x04
    unsigned char _pad08[0x2c - 0x08];
    EWeaponClass  BlockType;                    // +0x2c
};
struct CCPPointerInfo;
struct CCombatAbilityDataPointer {              // CCountedPointer<CCombatAbilityData>
    CCombatAbilityData* Data;
    CCPPointerInfo*     Info;
    CCombatAbilityData* operator->() const { return Data; }
};
struct SAbilityNode {                           // _Rb_tree_node<pair<const ECombatAbilityDataType, pair<CCountedPointer, CCombatAbility> > >
    long          _M_color;                     // +0x00
    SAbilityNode* _M_parent;                    // +0x04
    SAbilityNode* _M_left;                      // +0x08
    SAbilityNode* _M_right;                     // +0x0c
    ECombatAbilityDataType    first;            // +0x10
    CCombatAbilityDataPointer second;           // +0x14 (pair<CCountedPointer, CCombatAbility>.first)
};
struct CAbilityIterator {
    SAbilityNode* _M_node;
    CAbilityIterator(SAbilityNode* node) : _M_node(node) { }
    SAbilityNode* operator->() const { return _M_node; }
    bool operator!=(const CAbilityIterator& other) const { return _M_node != other._M_node; }
    static SAbilityNode* Increment(SAbilityNode* node)   // STLport _Rb_global<bool>::_M_increment
    {
        if (node->_M_right != 0) {
            node = node->_M_right;
            while (node->_M_left != 0)
                node = node->_M_left;
        } else {
            SAbilityNode* parent = node->_M_parent;
            while (node == parent->_M_right) {
                node = parent;
                parent = parent->_M_parent;
            }
            if (node->_M_right != parent)
                node = parent;
        }
        return node;
    }
    CAbilityIterator& operator++() { _M_node = Increment(_M_node); return *this; }
};
struct CCombatAbilityMap {                      // STLport multimap (12 bytes): the header-node pointer first
    SAbilityNode* _M_header;
    unsigned long _M_node_count;
    unsigned long _M_key_compare;
    __forceinline CAbilityIterator lower_bound(const ECombatAbilityDataType& k) const
    {
        SAbilityNode* y = _M_header;
        SAbilityNode* x = _M_header->_M_parent;
        while (x != 0) {
            if (!(x->first < k)) {
                y = x;
                x = x->_M_left;
            } else
                x = x->_M_right;
        }
        return CAbilityIterator(y);
    }
    __forceinline CAbilityIterator upper_bound(const ECombatAbilityDataType& k) const
    {
        SAbilityNode* y = _M_header;
        SAbilityNode* x = _M_header->_M_parent;
        while (x != 0) {
            if (k < x->first) {
                y = x;
                x = x->_M_left;
            } else
                x = x->_M_right;
        }
        return CAbilityIterator(y);
    }
};
struct CTCCombat {
    virtual void v00();
    unsigned char _pad04[0x70 - 0x04];          // CTCBase .. UnsheathingWeaponType (retail offsets)
    CCombatAbilityMap CombatAbilities;          // +0x70
    void SetBlockPercentageChance(float percentage, EWeaponClass weaponClass);
};

void CTCCombat::SetBlockPercentageChance(float percentage, EWeaponClass weaponClass)
{
    CAbilityIterator last = CombatAbilities.upper_bound(COMBAT_ABILITY_DATA_BLOCK);
    for (CAbilityIterator it = CombatAbilities.lower_bound(COMBAT_ABILITY_DATA_BLOCK); it != last; ++it) {
        CCombatAbilityData* data = it->second.Data;
        if (data->GetType() == 1 && data->BlockType == weaponClass) {
            if (percentage < 0.0f)
                percentage = 0.0f;
            else if (percentage > 100.0f)
                percentage = 100.0f;
            it->second->BlockPercentageChance = (long)percentage;
        }
    }
}