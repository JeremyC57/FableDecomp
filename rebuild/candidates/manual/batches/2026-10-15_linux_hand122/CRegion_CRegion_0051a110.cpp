// CRegion::~CRegion() -- destroy a world-map region record (two long vectors, names, def pointer, exit-text map)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051A110, length 158 bytes.
// Callees: _Rb_tree<CCharString, pair<const CCharString, C2DVector>, ...>::_M_erase(node*) @ 0x005149B0 (the
// recursive node eraser: right subtree first, then the node's CCharString key, then the node, then left),
// operator delete @ 0x00BFEA14, CCharString::~CCharString @ 0x0099EAE0, the def object's virtual slot 1.
// Label: the manifest's "~CAIStateGroup_SummonerCharge" is wrong (that state group is 0x24 bytes with an
// intelligent pointer at +0x14). The body is the destructor of CRegion: its 39 callers sit in CWorldMap
// (0x005066E0..) and the region vector code, and the member types match the CRegion PDB layout -- Maps and
// VisMaps vector<long>, Name / DisplayName / RegionDefName strings, PDef (CDefPointer<CRegionDef>),
// MiniMapGraphic string, MiniMapScale, MiniMapOffset, MiniMapRegionExitTextOffsets map.
//
// Shape: a NON-polymorphic class (no vtable store) whose members are destroyed in reverse order through
// their inline destructors: the STLport map (`clear()`: count test, _M_erase(root), leftmost/root/rightmost
// reset through the re-read header pointer, count = 0; then the header node freed with a null test), the
// strings out of line, the def pointer as a local copy (`dec [p+4] ; jne ; call [vt+4]`) with the member
// cleared inside the null test, and the two vectors' buffers freed with a null test (member +0 last, read
// into edi). The engine header CRegion.h carries the PDB layout with 16-byte vectors; retail vectors are 12
// bytes (everything after them sits 8 bytes lower, sizeof 0x58 = the `add edi,58h` stride of the CWorldMap
// region loop), so the class is modelled locally with retail offsets.
// Leaf: ??1CRegion

struct CCharString {
    void* m_pData;
    ~CCharString();                             // 0x0099EAE0
};

struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long RefCount;                              // +0x04
};

struct CDefPointer {                            // CDefPointer<CRegionDef const>
    CDefPointeeBase* Object;
    ~CDefPointer()
    {
        CDefPointeeBase* object = Object;
        if (object != 0) {
            if (--object->RefCount == 0)
                object->Destroy();
            Object = 0;
        }
    }
};

struct CLongVector {                            // STLport vector<long>: {start, finish, end of storage}
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    ~CLongVector()
    {
        if (_M_start != 0)
            operator delete(_M_start);
    }
};

struct C2DVector {
    float X, Y;
};

struct SExitTextNode {                          // _Rb_tree_node<pair<const CCharString, C2DVector> >
    long _M_color;
    SExitTextNode* _M_parent;                   // +0x04
    SExitTextNode* _M_left;                     // +0x08
    SExitTextNode* _M_right;                    // +0x0c
};

struct CExitTextOffsetMap {                     // STLport map<CCharString, C2DVector>: {header*, count, less}
    SExitTextNode* _M_header;
    unsigned long _M_node_count;
    void _M_erase(SExitTextNode* x);            // 0x005149B0
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_header->_M_parent);
            _M_header->_M_left = _M_header;
            _M_header->_M_parent = 0;
            _M_header->_M_right = _M_header;
            _M_node_count = 0;
        }
    }
    ~CExitTextOffsetMap()
    {
        clear();
        if (_M_header != 0)
            operator delete(_M_header);
    }
};

class CRegion {
public:
    CLongVector Maps;                           // +0x00
    CLongVector VisMaps;                        // +0x0c
    CCharString Name;                           // +0x18
    CCharString DisplayName;                    // +0x1c
    CCharString RegionDefName;                  // +0x20
    CDefPointer PDef;                           // +0x24
    CCharString MiniMapGraphic;                 // +0x28
    float MiniMapScale;                         // +0x2c
    C2DVector MiniMapOffset;                    // +0x30
    CExitTextOffsetMap MiniMapRegionExitTextOffsets;   // +0x38
    long _M_key_compare_pad;                    // +0x40 (the map's empty less<> comparator, padded)
    C2DVector WorldMapOffset;                   // +0x44
    C2DVector NameGraphicOffset;                // +0x4c
    bool AppearOnWorldMap;                      // +0x54
    bool CreatureGenerationEnabled;             // +0x55
    bool SoundThemesEnabled;                    // +0x56
    ~CRegion();
};

CRegion::~CRegion()
{
}
