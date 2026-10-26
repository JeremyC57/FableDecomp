#pragma optimize("s", on)
// CTCVillage::RemoveApologiseableCrimes(long) -- walks the set of the player's crimes and removes each one that can be apologised for
// Leaf: RemoveApologiseableCrimes
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065CE56, length 50 bytes (`ret 4`; the parameter is unused). The catalog
// name "IterateCrimesAndRemoveApologisable" is not a PDB name; the PDB has `?RemoveApologiseableCrimes@CTCVillage@@QAEXJ@Z`
// with this signature and a RemoveCrimeIfApologisable(long) callee.
// Callees: STLport _Rb_global<bool>::_M_increment(_Rb_tree_node_base*) @ 0x004292C0 (__cdecl, the successor in eax;
// manifest "CMemoryAllocatorVariableSize::GetNoAllocatedAreas" is propagated), CTCVillage::RemoveCrimeIfApologisable(long)
// @ 0x0065C1FC.
// Size TU (`pop ecx` after the increment call although its result is used): `optimize("s",on)`.
// Shape: a separate `next` iterator copied from `it` before the entry test (`mov ebx,esi`), advanced with the
// pre-increment before the call that may erase the node, then `it = next` at the bottom (`mov esi,ebx`); the value is
// read from the old node (esi+0x10); end() is cached in a local (edi). The `it++` post-increment form keeps `this` in
// ebx and the old node in ebp instead. The set is STLport's rb-tree: header node pointer at +0x64, begin = header->_M_left (+8).
// Layout: CTCVillage.h is the PDB layout (PlayersCrimes +0xDC); with the 4-byte retail lists it is +0x64. Local model.
struct SRbNode {                            // STLport _Rb_tree_node<long>
    long     _M_color;
    SRbNode* _M_parent;                     // +0x04
    SRbNode* _M_left;                       // +0x08
    SRbNode* _M_right;                      // +0x0C
    long     _M_value_field;                // +0x10
};

extern "C" SRbNode* __cdecl RbIncrement(SRbNode* node);   // 0x004292C0 _Rb_global<bool>::_M_increment

struct SCrimeIterator {
    SRbNode* _M_node;
    SCrimeIterator(SRbNode* node) : _M_node(node) { }
    long operator*() const { return _M_node->_M_value_field; }
    SCrimeIterator& operator++() { _M_node = RbIncrement(_M_node); return *this; }
    bool operator!=(const SCrimeIterator& other) const { return _M_node != other._M_node; }
};

struct CCrimeSet {                          // retail STLport set<long>, 12 bytes
    SRbNode*      _M_header;                // +0x00
    unsigned long _M_node_count;            // +0x04
    unsigned long _M_key_compare;           // +0x08 (empty less<long>, padded)
    SCrimeIterator begin() const { return SCrimeIterator(_M_header->_M_left); }
    SCrimeIterator end() const { return SCrimeIterator(_M_header); }
};

class CTCVillage { public:
    void*         __vftable;                // +0x00
    unsigned char _pad_0x04[0x60];
    CCrimeSet     PlayersCrimes;            // +0x64
    void RemoveCrimeIfApologisable(long crime);   // 0x0065C1FC
    void RemoveApologiseableCrimes(long unused);
};

void CTCVillage::RemoveApologiseableCrimes(long unused)
{
    SCrimeIterator last = PlayersCrimes.end();
    SCrimeIterator it = PlayersCrimes.begin();
    SCrimeIterator next = it;
    while (it != last) {
        ++next;
        RemoveCrimeIfApologisable(*it);
        it = next;
    }
}