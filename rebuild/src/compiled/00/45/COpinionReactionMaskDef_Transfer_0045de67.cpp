// COpinionReactionMaskDef::Transfer(CPersistContext& context) -- persist the default flag and the per-reaction enable map, then drop every map entry whose reaction type is below 18
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045DE67, length 83 bytes (`ret 4`; a virtual slot: referenced from the
// vtable at 0x01233DCC). Callees: CPersistContext::Transfer @ 0x0043314A (the bool) and @ 0x0045DEE9 (the map), the
// STLport tree increment @ 0x004292C0 (__cdecl, `pop ecx`; manifest "CMemoryAllocatorVariableSize::GetNoAllocatedAreas"
// is propagated), the map's erase(iterator) @ 0x0045DEBA (manifest "Map_EraseNode").
// Layout: the PDB places ReactionEnabledDefault at +0x3D and ReactionEnabled (map<EOpinionReactionType, bool>) at +0x40;
// retail is 4 bytes lower (0x3C-byte def base: +0x39 / +0x3C). STLport nodes: colour, parent, left, right,
// key at +0x10; the header's left link is begin().
// Shape: a size-optimised module (#pragma optimize("s", on)); the context is re-read from its argument slot for the
// second call; end() is a named iterator cached in ebx, `next` a copy of `it` advanced first in each pass (ebp), the
// key test `< 18` signed (`jge`), and erase takes the iterator BY VALUE -- a 4-byte class with a user copy
// constructor built in its argument slot (`push ecx ; mov eax,esp ; mov [eax],esi`).
// Leaf: ?Transfer@COpinionReactionMaskDef
#pragma optimize("s", on)
enum EOpinionReactionType { OPINION_REACTION_NONE = 0 };

struct SReactionNode {                             // STLport red-black node: colour, parent, left, right, value
    long                 m_color;                  // +0x00
    SReactionNode*       m_pParent;                // +0x04
    SReactionNode*       m_pLeft;                  // +0x08
    SReactionNode*       m_pRight;                 // +0x0C
    EOpinionReactionType first;                    // +0x10
    bool                 second;                   // +0x14
};

struct SRbGlobal {
    static SReactionNode* __cdecl Increment(SReactionNode* node);   // 0x004292C0 (_Rb_global<bool>::_M_increment)
};

class CReactionIterator {
public:
    SReactionNode* m_pNode;
    CReactionIterator(SReactionNode* node) : m_pNode(node) {}
    CReactionIterator(const CReactionIterator& other) : m_pNode(other.m_pNode) {}
    SReactionNode* operator->() const { return m_pNode; }
    CReactionIterator& operator++() { m_pNode = SRbGlobal::Increment(m_pNode); return *this; }
    bool operator!=(const CReactionIterator& other) const { return m_pNode != other.m_pNode; }
};

class CReactionEnabledMap {                        // map<EOpinionReactionType, bool>
public:
    SReactionNode* m_pHeader;
    unsigned long  m_reserved[2];
    CReactionIterator begin() const { return CReactionIterator(m_pHeader->m_pLeft); }
    CReactionIterator end() const { return CReactionIterator(m_pHeader); }
    void erase(CReactionIterator position);        // 0x0045DEBA
};

class CPersistContext {
public:
    void Transfer(bool& value);                    // 0x0043314A
    void Transfer(CReactionEnabledMap& value);     // 0x0045DEE9
};

class COpinionReactionMaskDef {
public:
    virtual void Transfer(CPersistContext& context);
    unsigned char       _pad04[0x35];
    bool                ReactionEnabledDefault;    // +0x39 (PDB +0x3D)
    unsigned char       _pad3a[0x02];
    CReactionEnabledMap ReactionEnabled;           // +0x3C (PDB +0x40)
};

void COpinionReactionMaskDef::Transfer(CPersistContext& context)
{
    context.Transfer(ReactionEnabledDefault);
    context.Transfer(ReactionEnabled);
    CReactionIterator end = ReactionEnabled.end();
    CReactionIterator it = ReactionEnabled.begin();
    CReactionIterator next = it;
    while (it != end) {
        ++next;
        if (it->first < 18)
            ReactionEnabled.erase(it);
        it = next;
    }
}