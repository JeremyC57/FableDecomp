// CRegion::GetMiniMapRegionExitTextOffset(const CCharString& exitName, C2DVector& offset) const -- look up the minimap text offset recorded for a region exit; false when the exit has none
// Leaf: ?GetMiniMapRegionExitTextOffset@CRegion@@QBE_NABVCCharString@@AAVC2DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BC350, 45 bytes (+ int3 padding), `ret 8` (thiscall).
// Callers: 0x0064DE65 and 0x0064DF77 (NPlayerGui::CDrawMiniMap::UpdateRegionExitNames).
// Callees: map<CCharString, C2DVector>::_M_find(const CCharString&) const @ 0x006BCF50 (catalog "RBTreeMap_FindNode";
//   node in eax, `ret 4`, compares through CBasicString<char>::operator< 0x00429950).
// Identity: Ego_r.pdb ?GetMiniMapRegionExitTextOffset@CRegion@@QBE_NABVCCharString@@AAVC2DVector@@@Z.
// Layout: Ego_r.pdb CRegion with the retail 12-byte STLport containers (engine/CRegion.h keeps the PDB's 16-byte ones):
//   MiniMapRegionExitTextOffsets at +0x38 (_M_header +0 = end()); its node holds pair<const CCharString, C2DVector> at
//   +0x10 (the key +0x10, the offset +0x14).
// Shape: `const_iterator it = map.find(exitName); if (it != map.end()) { offset = it->second; return true; } return
//   false;` -- the named iterator keeps the header read after the out-of-line `_M_find` call (`cmp eax,[esi]`), the found
//   arm falls through (an early `== end() return false` puts the false exit first); the C2DVector is copied as two dwords.
// Label correction: the catalog `MusicEntry_GetData` is descriptive.
class CCharString {
public:
    void* PStringData;                                  // +0x00
};

class C2DVector {
public:
    float X;                                            // +0x00
    float Y;                                            // +0x04
};

namespace std {
struct _Rb_tree_node_base {
    bool                _M_color;                       // +0x00
    _Rb_tree_node_base* _M_parent;                      // +0x04
    _Rb_tree_node_base* _M_left;                        // +0x08
    _Rb_tree_node_base* _M_right;                       // +0x0C
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                                   // +0x10
};

template <class K, class T> class map {
public:
    typedef pair<const K, T> value_type;
    typedef _Rb_tree_node<value_type> _Node;
    struct const_iterator {
        _Rb_tree_node_base* _M_node;
        const_iterator(_Rb_tree_node_base* n) : _M_node(n) { }
        const value_type* operator->() const { return &static_cast<_Node*>(_M_node)->_M_value_field; }
        bool operator!=(const const_iterator& o) const { return _M_node != o._M_node; }
    };
    _Rb_tree_node_base* _M_header;                      // +0x00
    unsigned int        _M_node_count;                  // +0x04
    unsigned char       _M_key_compare;                 // +0x08 less<K> (empty)
    _Rb_tree_node_base* _M_find(const K& k) const;      // 0x006BCF50
    const_iterator find(const K& k) const { return const_iterator(_M_find(k)); }
    const_iterator end() const { return const_iterator(_M_header); }
};
}

class CCharStringData;
class CDefPointeeBase;

class CRegion {                                         // retail layout (12-byte STLport containers)
public:
    long*            Maps[3];                           // +0x00 vector<long>
    long*            VisMaps[3];                        // +0x0C vector<long>
    CCharStringData* Name;                              // +0x18 CCharString
    CCharStringData* DisplayName;                       // +0x1C CCharString
    CCharStringData* RegionDefName;                     // +0x20 CCharString
    CDefPointeeBase* PDef;                              // +0x24 CDefPointer<const CRegionDef>
    CCharStringData* MiniMapGraphic;                    // +0x28 CCharString
    float            MiniMapScale;                      // +0x2C
    C2DVector        MiniMapOffset;                     // +0x30
    std::map<CCharString, C2DVector> MiniMapRegionExitTextOffsets;   // +0x38
    C2DVector        WorldMapOffset;                    // +0x44
    C2DVector        NameGraphicOffset;                 // +0x4C
    bool             AppearOnWorldMap;                  // +0x54
    bool             CreatureGenerationEnabled;         // +0x55
    bool             SoundThemesEnabled;                // +0x56
    bool GetMiniMapRegionExitTextOffset(const CCharString& exitName, C2DVector& offset) const;
};

bool CRegion::GetMiniMapRegionExitTextOffset(const CCharString& exitName, C2DVector& offset) const
{
    std::map<CCharString, C2DVector>::const_iterator it = MiniMapRegionExitTextOffsets.find(exitName);
    if (it != MiniMapRegionExitTextOffsets.end()) {
        offset = it->second;
        return true;
    }
    return false;
}