// CSubtitledMessage::AddCameraTagFromIndex(long index, float time, CDefPointer<const CGlobalDef> def) -- look the camera-tag index up in the global def's index->text map and add that camera tag at the given time (false when the index is unknown); releases the by-value def pointer
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CE4E0, 98 bytes; `ret 0Ch`. Callees: the STLport map<long, CCharString>::find @ 0x004D0980
//          (catalog "BinaryTree_FindNodeLE"), CSubtitledMessage::AddCameraTagFromText(float, const CCharString&) @ 0x004CDC40.
// Shape: the map sits at def+0xf4; find returns its iterator through a hidden pointer (VC homes it in the def's dead
//        argument slot); end() is the header node pointer, the mapped string sits at node+0x14 (STLport _Rb_tree_node:
//        colour, parent, left, right, then the pair). The by-value CDefPointer's inline destructor runs on both paths:
//        `dec [p+4] ; jne`, then virtual slot 1 (the def's self-release).
// Leaf: AddCameraTagFromIndex
class CCharString {
public:
    void* m_pStringData;
};
struct CTagNode {                                  // STLport _Rb_tree_node<pair<const long, CCharString> >
    long        Colour;
    CTagNode*   Parent;
    CTagNode*   Left;
    CTagNode*   Right;
    long        Key;                               // +0x10
    CCharString Value;                             // +0x14
};
struct CTagIterator {
    CTagNode* Node;
    bool operator!=(const CTagIterator& o) const { return Node != o.Node; }
    CTagNode* operator->() const { return Node; }
};
struct CIndexTextMap {                             // STLport map<long, CCharString>: the header node pointer first
    CTagNode*     Header;
    unsigned long Count;
    CTagIterator find(const long& key) const;      // 0x004D0980
    CTagIterator end() const { CTagIterator it; it.Node = Header; return it; }
};
struct CGlobalDef {
    virtual void v00();
    virtual void Release();                        // slot 1: frees the def when the last reference goes
    long          RefCount;                        // +0x04
    unsigned char _pad08[0xec];
    CIndexTextMap CameraTagTexts;                  // +0xf4
};
template <class T> struct CDefPointer {
    T* Ptr;
    T* operator->() const { return Ptr; }
    ~CDefPointer()
    {
        if (--((CGlobalDef*)Ptr)->RefCount == 0)
            ((CGlobalDef*)Ptr)->Release();
    }
};
class CSubtitledMessage {
public:
    bool AddCameraTagFromText(float time, const CCharString& text);   // 0x004CDC40
    bool AddCameraTagFromIndex(long index, float time, CDefPointer<const CGlobalDef> def);
};

bool CSubtitledMessage::AddCameraTagFromIndex(long index, float time, CDefPointer<const CGlobalDef> def)
{
    CTagIterator it = def->CameraTagTexts.find(index);
    if (it.Node != def->CameraTagTexts.Header)
        return AddCameraTagFromText(time, it->Value);
    return false;
}