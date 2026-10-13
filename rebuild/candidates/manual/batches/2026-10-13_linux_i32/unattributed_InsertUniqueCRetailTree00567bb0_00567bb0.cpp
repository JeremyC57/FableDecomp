// CRetailTree_00567bb0::InsertUnique(const SValue&) -- STLport insert_unique over long keys: descend from the root, step back through the out-of-line decrement @ 0x00566210 unless at begin, insert through the hidden-pointer _M_insert @ 0x00567AF0; returns {iterator, inserted} through the hidden pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00567BB0, oracle length 180 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebx ; mov ebx,[value] ; push ebp ; mov ebp,ecx ; mov ecx,[ebp] ; push esi ; mov esi,[ecx+4] ; test esi,esi ; push edi ; mov edi,ecx ; mov al,1 ; je J ; L: (compare key with [node+10h]) ; mov edi,esi ; (left or right) ; test esi,esi ; jne L ; J: test al,al ; (j = y) ; je C ; cmp edi,[head+8] ; jne D ; (insert x,y,v,0 ; result {it,true}) ; ret 8 ; D: lea ecx,[j] ; call iterator::operator-- ; C: (compare [j+10h] with key) ; jae F ; (insert ; result {it,true}) ; ret 8 ; F: (result {j,false}) ; ret 8
// STLport `_Rb_tree::insert_unique(const value_type&)`: `y` starts at the header, `x` at the root
// (+4 of the header), and the descent keeps `comp = key < node key` choosing left (+8) or right
// (+0xC); if the last comparison went left and `y` is not the leftmost node (+8 of the header),
// the iterator steps back through the out-of-line decrement @ 0x00566210; a key that is not
// already present is inserted through _M_insert @ 0x00567AF0 (x, y, value, null; the inserted
// iterator is written through an explicit result pointer -- the address of `j`, whose slot is
// the dead value-argument slot -- and returned) and the {iterator, bool} pair is written
// through the function's own hidden pointer. The node layout is STLport's (colour, parent, left, right,
// value at +0x10). Labels are propagated; names are descriptive. One of 16 bodies.
struct SNode {
    long m_color;                          // +0
    SNode* m_parent;                       // +4
    SNode* m_left;                         // +8
    SNode* m_right;                        // +0xC
    long m_key;                      // +0x10
};

struct SValue {
    long m_key;
};

struct SIterator {
    SNode* m_node;
    SIterator(SNode* node) : m_node(node) { }
    SIterator(const SIterator& other) : m_node(other.m_node) { }
    SIterator& operator--();               // 0x00566210
    bool operator==(const SIterator& other) const { return m_node == other.m_node; }
};

struct SPairIB {
    SIterator first;
    bool second;
    SPairIB(const SIterator& it, bool inserted) : first(it), second(inserted) { }
};


struct CRetailTree_00567bb0 {
    SNode* m_header;                       // +0
    unsigned long m_size;                  // +4
    SIterator* Insert(SIterator* result, SNode* x, SNode* y, const SValue& value, SNode* w);   // 0x00567AF0
    SIterator begin() { return SIterator(m_header->m_left); }
    SPairIB InsertUnique_CRetailTree_00567bb0(const SValue& value);
};

SPairIB CRetailTree_00567bb0::InsertUnique_CRetailTree_00567bb0(const SValue& value)
{
        SNode* y = m_header;
    SNode* x = m_header->m_parent;
    bool less = true;
    while (x != 0) {
        y = x;
        less = value.m_key < x->m_key;
        x = less ? x->m_left : x->m_right;
    }
    SIterator j(y);
    if (less) {
        if (j == begin())
            return SPairIB(*Insert(&j, y, y, value, 0), true);   // the new minimum: inserted to the left of y
        --j;
    }
    if (j.m_node->m_key < value.m_key)
        return SPairIB(*Insert(&j, x, y, value, 0), true);
    return SPairIB(j, false);
}
