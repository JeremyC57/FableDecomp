// CKeyedSet_0051f010::Contains(unsigned long) -- whether the tree at +0xcc holds the key
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051F010, oracle length 32 bytes.
// Callees: a tree lookup @ 0x00527E00 (__fastcall on the tree, the key by reference).
//
// Retail bytes:  push esi ; lea esi,[ecx+0CCh] ; lea eax,[esp+8] ; push eax ; mov ecx,esi ; call Find ; mov edx,[esi] ; xor ecx,ecx ; cmp eax,edx ; setne cl ; mov al,cl ; pop esi ; ret 8
// The tree sub-object is addressed once and kept in esi (a named pointer to it); the by-value key
// is passed by reference (its argument slot's address) and the result compared with the tree's
// first dword (its end/head node), a `return cond;` bool through a full register.  `ret 8` pops the
// key and one more dword (a second, unused argument).  The manifest's "CBankFileAsyncData::StartRead"
// label is propagated; the classes are named by address.
struct CTree_0051f010 {
    void* m_pHead;                              // +0x00 the end() node
    void* Find(const unsigned long& key);       // 0x00527E00
};

struct CKeyedSet_0051f010 {
    unsigned char _pad[0xcc];
    CTree_0051f010 m_tree;                      // +0xcc
    bool Contains(unsigned long key, unsigned long unused);   // 0x0051F010
};

bool CKeyedSet_0051f010::Contains(unsigned long key, unsigned long unused)
{
    CTree_0051f010* tree = &m_tree;
    return tree->Find(key) != tree->m_pHead;
}