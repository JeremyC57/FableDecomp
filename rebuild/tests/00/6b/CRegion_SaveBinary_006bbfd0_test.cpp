#include <cstdio>
class CCharString { public: void* PStringData; };
class C2DVector { public: float X, Y; };

class CDataOutputStream { public:
    char kind[64]; long val[64]; const void* ptr[64]; int n;
    void WriteBool(bool value) { kind[n] = 'b'; val[n++] = value ? 1 : 0; }
    void WriteLong(long value) { kind[n] = 'l'; val[n++] = value; }
    void WriteFloat(float value) { kind[n] = 'f'; val[n++] = (long)(value * 100.0f); }
    void Write2DVector(const C2DVector& value) { kind[n] = 'v'; ptr[n] = &value; val[n++] = (long)value.X; }
    void WritePresizedString(const CCharString& s) { kind[n] = 's'; ptr[n] = &s; val[n++] = 0; }
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
    stream->WriteLong(mapCount);
    long visCount = VisMaps.size();
    stream->WriteLong(visCount);
    for (long i = 0; i < mapCount; ++i)
        stream->WriteLong(Maps[i]);
    for (long j = 0; j < visCount; ++j)
        stream->WriteLong(VisMaps[j]);
    stream->WritePresizedString(Name);
    stream->WritePresizedString(DisplayName);
    stream->WritePresizedString(RegionDefName);
    stream->WritePresizedString(MiniMapGraphic);
    stream->WriteBool(AppearOnWorldMap);
    stream->WriteBool(CreatureGenerationEnabled);
    stream->WriteBool(SoundThemesEnabled);
    stream->WriteFloat(MiniMapScale);
    stream->WriteLong((long)MiniMapOffset.X);
    stream->WriteLong((long)MiniMapOffset.Y);
    stream->WriteLong((long)WorldMapOffset.X);
    stream->WriteLong((long)WorldMapOffset.Y);
    stream->WriteLong(MiniMapRegionExitTextOffsets.NodeCount);
    for (_Rb_tree_node_base* it = MiniMapRegionExitTextOffsets.Header->Left;
         it != MiniMapRegionExitTextOffsets.Header;
         it = _Rb_tree_node_base::Increment(it)) {
        stream->WritePresizedString(static_cast<CExitTextOffsetNode*>(it)->Key);
        stream->Write2DVector(static_cast<CExitTextOffsetNode*>(it)->Value);
    }
}

int main()
{
    static CRegion r;
    long maps[3] = { 7, 8, 9 }; long vis[2] = { 4, 5 };
    r.Maps.Start = maps; r.Maps.Finish = maps + 3; r.VisMaps.Start = vis; r.VisMaps.Finish = vis + 2;
    r.MiniMapScale = 1.5f; r.MiniMapOffset.X = 10.7f; r.MiniMapOffset.Y = -3.2f; r.WorldMapOffset.X = 100.0f; r.WorldMapOffset.Y = 200.9f;
    r.AppearOnWorldMap = true; r.CreatureGenerationEnabled = false; r.SoundThemesEnabled = true;
    static CExitTextOffsetNode header, a, b, c;
    header.Parent = &b; header.Left = &a; header.Right = &c;
    b.Parent = &header; b.Left = &a; b.Right = &c;
    a.Parent = &b; a.Left = 0; a.Right = 0; c.Parent = &b; c.Left = 0; c.Right = 0;
    a.Value.X = 1; b.Value.X = 2; c.Value.X = 3;
    r.MiniMapRegionExitTextOffsets.Header = &header; r.MiniMapRegionExitTextOffsets.NodeCount = 3;
    static CDataOutputStream s; s.n = 0;
    r.SaveBinary(&s);
    const char* kinds = "lllllllssssbbbflllllsvsvsv";
    long vals[] = { 3, 2, 7, 8, 9, 4, 5, 0,0,0,0, 1,0,1, 150, 10, -3, 100, 200, 3, 0,1, 0,2, 0,3 };
    bool ok = s.n == 26;
    for (int i = 0; ok && i < 26; ++i) ok = s.kind[i] == kinds[i] && s.val[i] == vals[i];
    ok = ok && s.ptr[7] == &r.Name && s.ptr[8] == &r.DisplayName && s.ptr[9] == &r.RegionDefName && s.ptr[10] == &r.MiniMapGraphic
            && s.ptr[20] == &a.Key && s.ptr[22] == &b.Key && s.ptr[24] == &c.Key;
    if (!ok) { std::printf("006bbfd0_TEST FAIL\n"); return 1; }
    std::printf("006bbfd0_TEST PASS\n");
    return 0;
}