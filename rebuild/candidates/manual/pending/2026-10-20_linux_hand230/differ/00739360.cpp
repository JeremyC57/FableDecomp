// CTCCombat::ResetBlockPercentageChance<CCombatAbilityBlockHeavyWeaponAttackDef>() -- when the combat type def has the heavy-weapon-attack block sub-def, restore that def's default block percentage on every block-type combat ability (data type 1) whose block type matches the def's
// Leaf: ResetBlockPercentageChance
// Grade: DIFFER(259v262) -- the shared equal_range key temporary: ours stores 1 into it before the inline upper_bound walk and compares through edx (and swaps the key/block-type frame words), retail compares with the immediate and stores the key right before the out-of-line lower_bound call (reusing the out-pointer word); block layout and the range loop otherwise match. Tried: stand-in GetSubDef (kept), separate upper/lower statements (worse), static _M_increment (kept)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00739360, 262 bytes, plain `ret`.
// Callees: CDefClassBase::GetSubDef<CCombatAbilityBlockHeavyWeaponAttackDef>(CDefPointer*) @ 0x00738370 (on the combat type
//   def), the CombatAbilities multimap's lower_bound(const ECombatAbilityDataType&) @ 0x00738D10 (out of line, iterator by
//   hidden pointer; upper_bound is inlined), the ability data's virtual slot 3 (+0x0c, its CCombatAbilityData type), the
//   defs' virtual slot 1 (+0x04, delete at the last reference).
// Layout: Ego_r.pdb CTCCombat with the retail 12-byte STLport vectors: PCombatTypeDef +0x28, CombatAbilities (header-node
//   pointer first) +0x70 (as SetBlockPercentageChance 0x00734210); STLport node: left +8, right +0xc, key +0x10, the counted
//   pointer's data +0x14; the block data (descriptive): chance +0x04, block type +0x2c; the block def (descriptive):
//   default chance +0x2c, block type +0x54.
// Shape: the combat type def through the inline by-value accessor (null-tested add-ref, released with a null test right
//   after GetSubDef, the bool parked in bl); the sub-def taken into a holder right after the call (released at the end);
//   `equal_range(1)` = `pair(lower_bound, upper_bound)` evaluated right to left: the inline upper_bound walk (key folded to
//   an immediate, result in ebx), then the out-of-line lower_bound; the def's block type is read into a frame local first.
enum ECombatAbilityDataType { COMBAT_ABILITY_DATA_0, COMBAT_ABILITY_DATA_BLOCK };
struct CCombatAbilityData {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual long GetType() const;               // slot 3 (+0x0c)
    long          BlockPercentageChance;        // +0x04
    unsigned char _pad08[0x2c - 0x08];
    long          BlockType;                    // +0x2c
};
struct CCPPointerInfo;
struct CCombatAbilityDataPointer {              // CCountedPointer<CCombatAbilityData>
    CCombatAbilityData* Data;
    CCPPointerInfo*     Info;
};
struct SAbilityNode {
    long          _M_color;                     // +0x00
    SAbilityNode* _M_parent;                    // +0x04
    SAbilityNode* _M_left;                      // +0x08
    SAbilityNode* _M_right;                     // +0x0c
    ECombatAbilityDataType    first;            // +0x10
    CCombatAbilityDataPointer second;           // +0x14
};
struct CAbilityIterator {
    SAbilityNode* _M_node;
    CAbilityIterator(SAbilityNode* node) : _M_node(node) { }
    SAbilityNode* operator->() const { return _M_node; }
    bool operator!=(const CAbilityIterator& other) const { return _M_node != other._M_node; }
    static SAbilityNode* _M_increment(SAbilityNode* x)   // STLport _Rb_global<bool>::_M_increment, inlined
    {
        if (x->_M_right != 0) {
            x = x->_M_right;
            while (x->_M_left != 0)
                x = x->_M_left;
        } else {
            SAbilityNode* y = x->_M_parent;
            while (x == y->_M_right) {
                x = y;
                y = y->_M_parent;
            }
            if (x->_M_right != y)
                x = y;
        }
        return x;
    }
    CAbilityIterator& operator++() { _M_node = _M_increment(_M_node); return *this; }
};
struct CAbilityRange {
    CAbilityIterator first;
    CAbilityIterator second;
    CAbilityRange(const CAbilityIterator& a, const CAbilityIterator& b) : first(a), second(b) { }
};
struct CCombatAbilityMap {                      // STLport multimap (12 bytes): the header-node pointer first
    SAbilityNode* _M_header;
    unsigned long _M_node_count;
    unsigned long _M_key_compare;
    CAbilityIterator lower_bound(const ECombatAbilityDataType& k);   // 0x00738D10
    __forceinline CAbilityIterator upper_bound(const ECombatAbilityDataType& k)
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
    __forceinline CAbilityRange equal_range(const ECombatAbilityDataType& k)
    {
        return CAbilityRange(lower_bound(k), upper_bound(k));
    }
};
struct CDefPointeeBase {
    virtual void Destroy();                     // slot 0
    virtual void Delete();                      // slot 1 (+0x04)
    long m_refCount;                            // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};
template <class T> struct CDefHolder {          // a counted def reference released at scope end
    T* Object;
    CDefHolder(T* object) : Object(object) { }
    ~CDefHolder()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Delete();
    }
    T* operator->() const { return Object; }
};
template <class T> struct CDefCopy {            // CDefPointer copy: AddRef on construction, release on destruction
    T* Object;
    CDefCopy(T* object) : Object(object) { if (Object) ++Object->m_refCount; }
    ~CDefCopy()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Delete();
    }
    T* operator->() const { return Object; }
};
struct CCombatAbilityBlockHeavyWeaponAttackDef : CDefPointeeBase {
    unsigned char _pad08[0x2c - 0x08];
    long DefaultBlockPercentageChance;          // +0x2c
    unsigned char _pad30[0x54 - 0x30];
    long BlockType;                             // +0x54
};
struct CCombatTypeDef : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CCombatAbilityBlockHeavyWeaponAttackDef>* out);   // 0x00738370 (GetSubDef<T> instantiation)
};
void* FindDef_CCombatTypeDef_GetSubDef();      // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 / 0x00744ED0 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CCombatTypeDef::GetSubDef(CDefPointer<CCombatAbilityBlockHeavyWeaponAttackDef>* out)
{
    void* def = FindDef_CCombatTypeDef_GetSubDef();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
struct CTCCombat {
    virtual void v00();
    unsigned char _pad04[0x28 - 0x04];          // CTCBase .. ParentCreature
    CCombatTypeDef* PCombatTypeDef;             // +0x28 CDefPointer<CCombatTypeDef const>::Object
    unsigned char _pad2c[0x70 - 0x2c];          // AbilityList .. UnsheathingWeaponType (retail offsets)
    CCombatAbilityMap CombatAbilities;          // +0x70
    CDefCopy<CCombatTypeDef> GetCombatTypeDef() const { return CDefCopy<CCombatTypeDef>(PCombatTypeDef); }
    template <class TDef> void ResetBlockPercentageChance();
};

template <class TDef> void CTCCombat::ResetBlockPercentageChance()
{
    CDefPointer<TDef> out;
    bool found = GetCombatTypeDef()->GetSubDef(&out);
    CDefHolder<TDef> def(out.Object);
    if (found) {
        long blockType = def->BlockType;
        CAbilityRange range = CombatAbilities.equal_range(COMBAT_ABILITY_DATA_BLOCK);
        for (CAbilityIterator it = range.first; it != range.second; ++it) {
            CCombatAbilityData* data = it->second.Data;
            if (data->GetType() == 1 && data->BlockType == blockType)
                data->BlockPercentageChance = def->DefaultBlockPercentageChance;
        }
    }
}
template void CTCCombat::ResetBlockPercentageChance<CCombatAbilityBlockHeavyWeaponAttackDef>();
