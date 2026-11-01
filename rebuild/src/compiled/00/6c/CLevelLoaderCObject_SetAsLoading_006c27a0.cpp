// CLevelLoader::CObject::SetAsLoading(const std::vector<CLevelFiles>& files, const std::set<CCharString>& sections, long index) -- assign the region level files (+0x10), the loadable sections (+0), then store the region index (+0x1c)
// Leaf: ?SetAsLoading@CObject@CLevelLoader@@QAEXABV?$vector@VCLevelFiles@CObject@CLevelLoader@@V?$allocator@VCLevelFiles@CObject@CLevelLoader@@@std@@@std@@ABV?$set@VCCharString@@U?$less@VCCharString@@@std@@V?$allocator@VCCharString@@@3@@4@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C27A0, length 39 bytes (48 with padding), `ret 0Ch`.
// Identity (naming wave 20, chunk 688): FableWin.pdb ?SetAsLoading@CObject@CLevelLoader@@QAEXABV?$vector@..CLevelFiles..@@ABV?$set@VCCharString..@@J@Z (12
//   occurrences); CWorldMap::LoadLevelFromFiles 0x00501150 calls it on its new CObject with (files vector, sections set, 0); the body assigns the vector
//   at +0x10 (RegionLevelFiles), the set at +0 (LoadableSections) and the long at +0x1C (RegionIndex) = Ego_r CLevelLoader::CObject (0x20 bytes retail:
//   12-byte STLport set and vector, PLevelLoader +0xc). Was the descriptive SMappedRecords_006c27a0::Set (the chunk label CSpellContainerList::AddChild
//   is propagated).
// Callees: vector<CLevelFiles>::operator= 0x006C2D40 (landed), _Rb_tree<CCharString, ..>::operator= 0x006B9E00 (landed: the set's inline operator=
//   forwards `_M_t = x._M_t`).
// Shape: each argument read from its slot right before its use (no frame), the vector assigned on `lea ecx,[esi+10h]`, the set on `this`, the word
//   stored last: three assignments in source order.
class CAFile;
class CCharString;

namespace std {
template <class _Tp> class allocator { };
template <class _Tp> struct less { };
template <class _Tp> struct _Identity { };

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc> class _Rb_tree {
public:
    void*         _M_header;                          // +0x00
    unsigned long _M_node_count;                      // +0x04
    _Compare      _M_key_compare;                     // +0x08
    _Rb_tree& operator=(const _Rb_tree& __x);         // 0x006B9E00 (stl/_tree.c, out of line)
};
template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> > class set {
public:
    _Rb_tree<_Key, _Key, _Identity<_Key>, _Compare, _Alloc> _M_t;
    set& operator=(const set& __x) { _M_t = __x._M_t; return *this; }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    vector& operator=(const vector& __x);             // 0x006C2D40 (stl/_vector.c, out of line)
};
}

class CLevelLoader {
public:
    class CObject {
    public:
        class CLevelFiles { public: unsigned char Data[0x1C]; };   // Ego_r.pdb, 28 bytes: LevelID + three CCountedPointer<CAFile>
        std::set<CCharString>    LoadableSections;    // +0x00
        CLevelLoader*            PLevelLoader;        // +0x0c
        std::vector<CLevelFiles> RegionLevelFiles;    // +0x10
        long                     RegionIndex;         // +0x1c
        void SetAsLoading(const std::vector<CLevelFiles>& files, const std::set<CCharString>& sections, long index);
    };
};

void CLevelLoader::CObject::SetAsLoading(const std::vector<CLevelFiles>& files, const std::set<CCharString>& sections, long index)
{
    RegionLevelFiles = files;
    LoadableSections = sections;
    RegionIndex = index;
}
