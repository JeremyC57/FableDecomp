// CEngineInternalPrimitiveAnimatedMesh::GetBoneScalings(C3DMesh2* mesh) -- the cached per-bone scaling array for `mesh` (initialised on the first request), or null while the primitive has no animated skeleton
// Grade: RELOCATION_MATCH (retry helper C) -- STLport layering: the inline map::find forwards to the out-of-line _Rb_tree::find 0x00B83520, given its same-TU noinline body (landed 0x006699C0 shape). Both results then land in the iterator's one frame word and are re-read from it, as retail (was DIFFER(167v155)). The Tried/Next notes below are from the earlier extern-callee attempts.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B83750, length 155 bytes (`ret 4`).
// Callees: the cache map's find(const unsigned long&) @ 0x00B83520 (thiscall on +0xD4, iterator by hidden pointer; manifest
// "RBTree_LowerBound"), InitialiseBoneScalings(C3DMesh2*) @ 0x00BD2A90.
// Layout: the animation state pointer +0x78 (inside the PDB C3DAnimationInterpolator Anim at +0x70; +0x18 tested non-null), the
// CCache map at +0xD4 (header node pointer first; node key +0x10, value vector<C3DVector> +0x14); the key is the mesh's +0xE8 word,
// copied into the dead `mesh` argument slot for the by-reference find. Return: `size() ? start : (C3DVector*)&vector` (signed /12).
// Tried: (a) key local + reassigned iterator [this file]; (b) two scoped lookups with two returns (no tail merge, 194 bytes);
// (c) user copy/assignment on the iterator (same as a); (d) a long key converted at the call (same temporaries).
// Next: lane gap candidate (RTM coalescing of the call-result temporary with the dead iterator); try a `CBoneScalingArray*` result
// helper or a `goto`-shaped single lookup if Fable 5.1 has another idea.
class C3DVector { public: float X, Y, Z; };

class C3DMesh2 { public:
    unsigned char _pad[0xe8];
    unsigned long m_meshID;                        // +0xe8 (the bone-scaling cache key)
};

struct CBoneScalingArray {                         // STLport vector<C3DVector>: start, finish, end of storage
    C3DVector* m_pStart;
    C3DVector* m_pFinish;
    C3DVector* m_pEndOfStorage;
    long size() const { return m_pFinish - m_pStart; }
    C3DVector* GetData() { return size() ? m_pStart : (C3DVector*)this; }
};

struct SCacheNode {                                // map node: left/parent/right/colour words, key +0x10, value +0x14
    long              m_colour;                    // +0x00
    SCacheNode*       m_pParent;                   // +0x04
    SCacheNode*       m_pLeft;                     // +0x08
    SCacheNode*       m_pRight;                    // +0x0c
    unsigned long     m_key;                       // +0x10
    CBoneScalingArray m_value;                     // +0x14
};

struct CCache {                                    // map<unsigned long, vector<C3DVector> > (header node pointer first)
    SCacheNode* m_pHeader;
    struct iterator {
        SCacheNode* m_pNode;
        iterator() { }
        iterator(SCacheNode* n) : m_pNode(n) { }
        bool operator==(const iterator& o) const { return m_pNode == o.m_pNode; }
        SCacheNode* operator->() const { return m_pNode; }
    };
    iterator TreeFind(const unsigned long& key);   // 0x00B83520 (_Rb_tree::find; manifest "RBTree_LowerBound")
    iterator find(const unsigned long& key) { return TreeFind(key); }
    SCacheNode* end() const { return m_pHeader; }
};

#pragma auto_inline(off)
// Same-TU body of the map<unsigned long, vector<C3DVector> >::find instantiation 0x00B83520 (as the landed 0x006699C0):
// lower-bound walk from the root, then end() unless the key matches.
__declspec(noinline) CCache::iterator CCache::TreeFind(const unsigned long& key)
{
    SCacheNode* result = m_pHeader;
    SCacheNode* n = m_pHeader->m_pParent;
    while (n) {
        if (!(n->m_key < key)) {
            result = n;
            n = n->m_pLeft;
        }
        else
            n = n->m_pRight;
    }
    SCacheNode* found = result;
    if (found == m_pHeader || key < found->m_key)
        found = m_pHeader;
    return iterator(found);
}
#pragma auto_inline(on)

struct SAnimationState {
    unsigned char _pad[0x18];
    void*         m_pSkeleton;                     // +0x18
};

class CEngineInternalPrimitiveAnimatedMesh {
public:
    virtual void v000();
    unsigned char    _pad04[0x74];
    SAnimationState* m_pAnimationState;            // +0x78 (inside the PDB C3DAnimationInterpolator Anim at +0x70)
    unsigned char    _pad7c[0x58];
    CCache           Cache;                        // +0xd4
    void InitialiseBoneScalings(C3DMesh2* mesh);   // 0x00BD2A90
    C3DVector* GetBoneScalings(C3DMesh2* mesh);
};

C3DVector* CEngineInternalPrimitiveAnimatedMesh::GetBoneScalings(C3DMesh2* mesh)
{
    if (m_pAnimationState == 0 || m_pAnimationState->m_pSkeleton == 0)
        return 0;
    unsigned long key = mesh->m_meshID;
    CCache::iterator it = Cache.find(key);
    if (it.m_pNode == Cache.end()) {
        InitialiseBoneScalings(mesh);
        key = mesh->m_meshID;
        it = Cache.find(key);
    }
    return it->m_value.GetData();
}