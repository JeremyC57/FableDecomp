// C3DAnimationInfo::CAnimationData::CanUnload() -- an animation data block may be unloaded when its pending file read (if any) has finished, or, with no read pending, when neither the shared animation nor any per-id animation is referenced from elsewhere
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A25A70, 99 bytes (+13 int3), __fastcall, returns bool.
// Callees: CBankFileAsyncData::IsFinished 0x009D54E0, the map iterator increment 0x00A29E70 (out of line,
// on the iterator local; labelled rbtree_predecessor).
// Layout: as the landed constructor 0x00A2D5F0 -- the VC6-era map Animations {head, size} at +0x1C
// (node: colour, parent, left +8, right, then the pair {id, CCountedPointer<C3DAnimation2>} with the
// counted pointer's count block at node+0x18), SharedAnimation at +0x28 and FileData at +0x30, each a
// {object, count*} counted pointer (Ego_r.pdb has them 4 bytes later: its map is 16 bytes).
// Shape: the no-read-pending case is the `if` body (fall-through) and the IsFinished tail sits last;
// IsFinished returns an `unsigned char` and `? true : false` re-normalises it with `test al,al ; setne al`
// (`!= 0` gives `neg ; sbb ; neg`). "Referenced elsewhere" is an inline `count && *count > 1` test
// (unsigned: `jbe`). The map's end() is taken into a local before the loop, so the head stays in esi
// (the `it != Animations.end()` form re-reads it after every out-of-line increment); the iterator lives
// in the `push ecx` frame word.
class CBankFileAsyncData {
public:
    unsigned char IsFinished();                      // 0x009D54E0
};
class C3DAnimation2;

template <class T>
class CCountedPointer {
public:
    T* m_pObject;
    unsigned long* m_pCount;
    bool IsShared() const { return m_pCount && *m_pCount > 1; }
};

template <class K, class V>
class CRetailMap {                                   // VC6-era Dinkumware map: {head, size}
public:
    struct SNode {
        unsigned char colour;
        SNode* parent;
        SNode* left;
        SNode* right;
        K first;
        V second;
    };
    class iterator {
    public:
        SNode* m_node;
        iterator& operator++();                      // 0x00A29E70
        SNode* operator->() const { return m_node; }
        bool operator!=(const iterator& other) const { return m_node != other.m_node; }
    };
    SNode* m_head;
    unsigned long m_size;
    iterator begin() const { iterator it; it.m_node = m_head->left; return it; }
    iterator end() const { iterator it; it.m_node = m_head; return it; }
};

class CResource {
public:
    virtual void v00();
    unsigned char _body[0x18];                       // +0x04..+0x1B
};

class CAnimationData : public CResource {
public:
    CRetailMap<unsigned long, CCountedPointer<C3DAnimation2> > Animations;   // +0x1C
    unsigned char _pad_0x24[4];                      // +0x24 (map comparator/allocator slot)
    CCountedPointer<C3DAnimation2> SharedAnimation;  // +0x28
    CCountedPointer<CBankFileAsyncData> FileData;    // +0x30
    void* AnimInfo;                                  // +0x38 C3DAnimationInfo*
    bool CanUnload();
};

bool CAnimationData::CanUnload()
{
    if (!FileData.m_pObject) {
        if (SharedAnimation.IsShared())
            return false;
        CRetailMap<unsigned long, CCountedPointer<C3DAnimation2> >::iterator end = Animations.end();
        for (CRetailMap<unsigned long, CCountedPointer<C3DAnimation2> >::iterator it = Animations.begin(); it != end; ++it)
            if (it->second.IsShared())
                return false;
        return true;
    }
    return FileData.m_pObject->IsFinished() ? true : false;
}