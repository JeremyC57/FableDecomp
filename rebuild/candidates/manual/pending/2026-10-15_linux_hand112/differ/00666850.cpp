// CThingPlayerCreature::vfunc_71(long) -- fetches the base def's sub-def and returns the value its map holds for `key`, or 0
// Grade: DIFFER(76v74) -- the def pointer is loaded into eax and copied to edi (`mov eax,[esp+8] ; lea esi,[eax+60h] ; mov edi,eax`) where retail loads edi directly; this explicit-release form is the closest of four (the natural CDefPointer destructor re-reads the frame slot after find: 83-88v74)
// Retail: Fable.exe 0x00666850, 74 bytes (CThingPlayerCreature vtable 0x012457FC slot 71).
// Callees: CDefClassBase::GetSubDef<T>(CDefPointer<T>*) 0x00485815 on the thing's PBaseDef (CThing +0x70
// in retail, PDB +0x80; the template argument is a propagated label), the sub-def map's find
// 0x0042B467 (returns the node; end() is the head word at the map's +0), the def release through
// vtable slot 1 (+4).
// Shape: a default-constructed CDefPointer local (its zero store scheduled after the address push),
// the map reached through a pointer local (esi), the found value or 0 selected into esi, then a release
// with NO null test through the def kept in edi across the find call (`dec [edi+4] ; jne ; call [edx+4]`).
struct SSubDefMapNode {
    SSubDefMapNode* m_pLeft;
    SSubDefMapNode* m_pParent;
    SSubDefMapNode* m_pRight;
    long            m_colour;
    long            m_key;                               // +0x10
    void*           m_pValue;                            // +0x14
};

struct CSubDefMap {
    SSubDefMapNode* m_pHead;                             // +0x00 end()
    SSubDefMapNode* find(long key);                      // 0x0042B467
    SSubDefMapNode* end() const { return m_pHead; }
};

struct CSubDef {
    virtual void v000();
    virtual void Destroy();                              // slot 1 (+4)
    long          m_refCount;                            // +0x04
    unsigned char _pad08[0x58];
    CSubDefMap    m_map;                                 // +0x60
};

template <class T> struct CDefPointer {
    T* m_pDef;
    CDefPointer() : m_pDef(0) {}
    ~CDefPointer() { if (m_pDef) { if (--m_pDef->m_refCount == 0) m_pDef->Destroy(); } }
    T* operator->() const { return m_pDef; }
};

struct CDefClassBase {
    bool GetSubDef(CDefPointer<CSubDef>* out);           // 0x00485815
};

struct CThingPlayerCreature {
    unsigned char               _pad00[0x70];
    CDefClassBase*              PBaseDef;                // +0x70
    void* vfunc_71(long key);
};

void* CThingPlayerCreature::vfunc_71(long key)
{
    CDefPointer<CSubDef> subDef;
    PBaseDef->GetSubDef(&subDef);
    CSubDef* def = subDef.m_pDef;
    CSubDefMap* map = &def->m_map;
    SSubDefMapNode* it = map->find(key);
    void* result = it != map->end() ? it->m_pValue : 0;
    if (--def->m_refCount == 0) def->Destroy();
    subDef.m_pDef = 0;
    return result;
}
