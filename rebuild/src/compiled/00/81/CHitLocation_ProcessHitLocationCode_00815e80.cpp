// CHitLocation::ProcessHitLocationCode(long a, long b) -- look the owner's code word (+4 -> +0x78) up in the global hit-location code tree (0x013BAF2C; find @ 0x00817740) and, when found, call the node's `__fastcall` handler (+0x14) with the two arguments
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00815E80, length 49 bytes.
// Shape: a 4-byte frame for the iterator, `mov ecx,13BAF2Ch` for the tree, `cmp eax,[13BAF2Ch]` against the head, `ret 8`.
struct CRetailTree_longkey {                      // the landed 0x00817740 tree (Dinkumware, long keys)
    struct node { unsigned char flag; unsigned char _pad[3]; node* parent; node* left; node* right; long key; void (__fastcall* value)(long a, long b); };
    struct iterator { node* current; iterator(node* n) : current(n) { } };
    node* head;
    unsigned long size;
    iterator find_00817740(const long& key);                // 0x00817740
};
extern CRetailTree_longkey g_hitLocationCodes_013baf2c;   // 0x013BAF2C
struct CHitOwner_00815e80 {
    unsigned char _pad[0x78];
    long          m_code;                          // +0x78
};
struct CHitLocation {
    long                 POwnerThing;
    CHitOwner_00815e80*  m_pOwner;                 // +0x04
    void ProcessHitLocationCode(long a, long b);
};

void CHitLocation::ProcessHitLocationCode(long a, long b)
{
    CRetailTree_longkey::iterator it = g_hitLocationCodes_013baf2c.find_00817740(m_pOwner->m_code);
    if (it.current != g_hitLocationCodes_013baf2c.head)
        it.current->value(a, b);
}