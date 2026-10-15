// CNavSwitchableLeafNode::SaveToFile(CAFile&, const map<const CANavQuadTreeNode*, long>&, bool, const C2DVector&) const -- save the navigable-leaf part, then the count and the 64-bit keys of the switchable IDs this node is open for
// Leaf: ?SaveToFile@CNavSwitchableLeafNode
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A77C30, 99 bytes (+ int3 padding; `ret 10h`). Vtable 0x0129DE04 slot 21.
// Callees: CNavNavigableLeafNode::SaveToFile @ 0x00A77A10 (qualified base call), CAFile::Write(const void*, long, long) (virtual slot 4, +0x10).
// Layout: UniqueIDsOpen at +0x20 (CVectorMap<unsigned __int64, bool>: its STLport vector of 16-byte pairs first, start +0x20,
//   finish +0x24).
// Shape: the entry count is a long local VC homes in the dead C2DVector& argument slot; only the 8-byte key of each pair is written.
struct CAFile {
    virtual void Slot0();
    virtual void Slot1();
    virtual void Slot2();
    virtual void Slot3();
    virtual void Write(const void* data, long size, long flags);   // slot 4 (+0x10)
};
struct C2DVector { float X, Y; };
struct CANavQuadTreeNode;
namespace std {
template <class T> struct allocator { };
template <class T> struct less { };
template <class T1, class T2> struct pair { T1 first; T2 second; };
template <class K, class V, class C = less<K>, class A = allocator<pair<const K, V> > > struct map;
template <class T, class A = allocator<T> > struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    T* end() const { return _M_finish; }
    unsigned int size() const { return end() - begin(); }
};
}
template <class K, class V> struct CKeyPairCompareLess { };
template <class K, class V, class C> struct CVectorMap {
    typedef std::pair<K, V> value_type;
    std::vector<value_type> m_data;
    unsigned char           _pad10[4];
    const value_type* begin() const { return m_data.begin(); }
    const value_type* end() const { return m_data.end(); }
    unsigned int size() const { return m_data.size(); }
};
typedef std::map<const CANavQuadTreeNode*, long> CNodeIndexTable;
struct CNavNavigableLeafNode {
    void*         __vftable;                       // +0x00
    unsigned char _pad04[0x1c];
    void SaveToFile(CAFile& file, const CNodeIndexTable& table, bool saveNeighbours, const C2DVector& origin) const;   // 0x00A77A10
};
struct CNavSwitchableLeafNode : CNavNavigableLeafNode {
    CVectorMap<unsigned __int64, bool, CKeyPairCompareLess<unsigned __int64, bool> > UniqueIDsOpen;   // +0x20
    void SaveToFile(CAFile& file, const CNodeIndexTable& table, bool saveNeighbours, const C2DVector& origin) const;
};

void CNavSwitchableLeafNode::SaveToFile(CAFile& file, const CNodeIndexTable& table, bool saveNeighbours, const C2DVector& origin) const
{
    CNavNavigableLeafNode::SaveToFile(file, table, saveNeighbours, origin);
    long count = UniqueIDsOpen.size();
    file.Write(&count, sizeof(count), 0);
    for (const std::pair<unsigned __int64, bool>* it = UniqueIDsOpen.begin(); it != UniqueIDsOpen.end(); ++it)
        file.Write(&it->first, sizeof(it->first), 0);
}
