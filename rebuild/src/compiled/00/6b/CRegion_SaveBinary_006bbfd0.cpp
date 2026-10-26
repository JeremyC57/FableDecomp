// CRegion::SaveBinary(CDataOutputStream*) -- write the region's map lists, names, flags, minimap/world offsets and exit-text offsets
// Retail 0x006BBFD0, 400 bytes, ret 4. Callees: CDataOutputStream::WriteSLONG 0x00993EB0 (catalog "WriteCBYTE"),
// WriteFloat 0x00993EE0, WriteBool 0x00993E30, WritePresizedString 0x00995A80, Write2DVector 0x00993F90; __ftol2 0x00BFEA70.
// Layout: retail CRegion is 0x58 bytes (GOTCHAS: "CRegion 0x58 bytes with Name +0x18"): its STLport vectors and map are
// 12 bytes where engine/CRegion.h (PDB) has 16, so every member after Maps sits below the header's offset:
// VisMaps +0x0C, Name +0x18, DisplayName +0x1C, RegionDefName +0x20, PDef +0x24, MiniMapGraphic +0x28,
// MiniMapScale +0x2C, MiniMapOffset +0x30, exit-text map {header, count, compare} +0x38, WorldMapOffset +0x44,
// NameGraphicOffset +0x4C, the three bools +0x54..+0x56. Hence the local model instead of the shared header.
// Shape: the two sizes are named locals written before either loop (the first loop's index lives in the dead
// stream-argument slot); offsets go out truncated to long; the map walk inlines STLport's static _M_increment
// (right-then-leftmost, else climb while we are the parent's right child) with end() re-read each pass.
class CCharString { public: void* PStringData; };
class C2DVector { public: float X, Y; };

class CDataOutputStream { public:
    void WriteFloat(float value);                       // 0x00993EE0
    void Write2DVector(const C2DVector& value);         // 0x00993F90
    void WritePresizedString(const CCharString& s);     // 0x00995A80
public:
    void WriteEBOOL(bool);   // 0x00993E30
    void WriteSLONG(long);   // 0x00993EB0
};

struct _Rb_tree_node_base {
    long                Color;    // +0x00
    _Rb_tree_node_base* Parent;   // +0x04
    _Rb_tree_node_base* Left;     // +0x08
    _Rb_tree_node_base* Right;    // +0x0c
    static _Rb_tree_node_base* Increment(_Rb_tree_node_base* node)
    {
        if (node->Right != 0) {
            node = node->Right;
            while (node->Left != 0)
                node = node->Left;
        } else {
            _Rb_tree_node_base* y = node->Parent;
            while (node == y->Right) {
                node = y;
                y = y->Parent;
            }
            if (node->Right != y)
                node = y;
        }
        return node;
    }
};
struct CExitTextOffsetNode : _Rb_tree_node_base {
    CCharString Key;              // +0x10
    C2DVector   Value;            // +0x14
};
struct CExitTextOffsetMap {       // STLport map<CCharString, C2DVector>, 12 bytes
    _Rb_tree_node_base* Header;   // +0x00
    unsigned long       NodeCount;// +0x04
    unsigned long       Compare;  // +0x08
};

template <class T> struct CStlVector {
    T* Start; T* Finish; T* EndOfStorage;
    long size() const { return Finish - Start; }
    T& operator[](long i) { return Start[i]; }
};

class CRegion { public:
    CStlVector<long>   Maps;                    // +0x00
    CStlVector<long>   VisMaps;                 // +0x0c
    CCharString        Name;                    // +0x18
    CCharString        DisplayName;             // +0x1c
    CCharString        RegionDefName;           // +0x20
    void*              PDef;                    // +0x24
    CCharString        MiniMapGraphic;          // +0x28
    float              MiniMapScale;            // +0x2c
    C2DVector          MiniMapOffset;           // +0x30
    CExitTextOffsetMap MiniMapRegionExitTextOffsets;   // +0x38
    C2DVector          WorldMapOffset;          // +0x44
    C2DVector          NameGraphicOffset;       // +0x4c
    bool               AppearOnWorldMap;        // +0x54
    bool               CreatureGenerationEnabled;   // +0x55
    bool               SoundThemesEnabled;      // +0x56
    void SaveBinary(CDataOutputStream* stream);
};

void CRegion::SaveBinary(CDataOutputStream* stream)
{
    long mapCount = Maps.size();
    stream->WriteSLONG(mapCount);
    long visCount = VisMaps.size();
    stream->WriteSLONG(visCount);
    for (long i = 0; i < mapCount; ++i)
        stream->WriteSLONG(Maps[i]);
    for (long j = 0; j < visCount; ++j)
        stream->WriteSLONG(VisMaps[j]);
    stream->WritePresizedString(Name);
    stream->WritePresizedString(DisplayName);
    stream->WritePresizedString(RegionDefName);
    stream->WritePresizedString(MiniMapGraphic);
    stream->WriteEBOOL(AppearOnWorldMap);
    stream->WriteEBOOL(CreatureGenerationEnabled);
    stream->WriteEBOOL(SoundThemesEnabled);
    stream->WriteFloat(MiniMapScale);
    stream->WriteSLONG((long)MiniMapOffset.X);
    stream->WriteSLONG((long)MiniMapOffset.Y);
    stream->WriteSLONG((long)WorldMapOffset.X);
    stream->WriteSLONG((long)WorldMapOffset.Y);
    stream->WriteSLONG(MiniMapRegionExitTextOffsets.NodeCount);
    for (_Rb_tree_node_base* it = MiniMapRegionExitTextOffsets.Header->Left;
         it != MiniMapRegionExitTextOffsets.Header;
         it = _Rb_tree_node_base::Increment(it)) {
        stream->WritePresizedString(static_cast<CExitTextOffsetNode*>(it)->Key);
        stream->Write2DVector(static_cast<CExitTextOffsetNode*>(it)->Value);
    }
}