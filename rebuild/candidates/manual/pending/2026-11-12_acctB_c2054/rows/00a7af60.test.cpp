// Behaviour test for 0x00A7AF60 CNavQuadTree::LoadFromFile: a standalone model of the navigation map loader checked on four
// files (versions 7, 6, 5 and a refused version 4). The model follows the row statement for statement: header (file size, version,
// map size), the region list (ids (map << 16) ^ (index & 0xFFFF), indices from 1), the map-relative action points, the index table
// seeded with 0 -> the shared blocked node, the layer tables sized (width / 32) * (height / 32), the node records (switchable flag
// from version 6, top-level and blocked flags, the shared blocked node for a plain blocked record, otherwise leaf flag, level, layer,
// position, index and the node's own data, with the preferability byte from version 7), the end marker, the index -> pointer pass
// over the top-level nodes, the sorted switchable table opened entry by entry, and the breath checks in between. The payload each
// node constructor reads is the model's own (the constructors are other rows). Fourteen single-statement mutations must each
// change the result signature.
#include <cstdio>
#include <cstring>

#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif

struct File {
    unsigned char data[2048];
    int len, pos;
    bool overrun;
    File() : len(0), pos(0), overrun(false) { std::memset(data, 0, sizeof(data)); }
    void Put(const void* p, int n) { std::memcpy(data + len, p, n); len += n; }
    void P1(unsigned char v) { Put(&v, 1); }
    void P4(int v) { Put(&v, 4); }
    void PF(float v) { Put(&v, 4); }
    void P8(u64 v) { Put(&v, 8); }
    void Read(void* p, int n)
    {
        if (pos + n > len) { overrun = true; std::memset(p, 0, n); pos = len; return; }
        std::memcpy(p, data + pos, n); pos += n;
    }
};

static double g_clock;
static int g_time_calls;
static double GetTime() { ++g_time_calls; g_clock -= 0.003; return g_clock; }   // a clock that runs down: every second check breathes

enum { BLOCKED, QUAD, LEAF, SWITCHABLE };
struct Node {
    int kind;
    float x, y;
    unsigned char level, layer;
    int index;
    int child_index[4];
    Node* child[4];
    int region_index;
    unsigned char preferability;
    u64 ids[4];
    int no_ids;
    int converted;
    int opened, closed;
    unsigned char nav_layer;                         // the blocked node's NavLayer
};
struct ActionPoint { float x, y; int layer; };
struct IndexEntry { int index; Node* node; };
struct SwitchEntry { u64 id; Node* node; };

struct Tree {
    bool Initialised;
    float MapPosX, MapPosY, MapWidth, MapHeight;
    int MapIndex;
    int Regions[16]; int NoRegions;
    ActionPoint ActionPoints[16]; int NoActionPoints;
    Node* Layers[4][16]; int NoLayers, NodesPerLayer;
    SwitchEntry Switchable[16]; int NoSwitchable; bool Dirty;
    Node Pool[32]; int NoPool;
    Node Blocked;
    IndexEntry Index[40]; int NoIndex;
    int suspends;
    bool bad;
    u64 open_log[16]; int no_open_log;

    Tree() { std::memset(this, 0, sizeof(*this)); Blocked.kind = BLOCKED; Blocked.nav_layer = 0xEE; }

    void Breath(double& last, int m)
    {
        double now = GetTime();
        double d = m == 10 ? now - last : last - now;
        if (d >= 0.005) { ++suspends; last = now; }
    }
    void IndexInsert(int index, Node* node)          // insert_unique: the first entry of a key stays
    {
        for (int i = 0; i < NoIndex; ++i) if (Index[i].index == index) return;
        if (NoIndex >= 40) { bad = true; return; }
        Index[NoIndex].index = index; Index[NoIndex].node = node; ++NoIndex;
    }
    Node* IndexFind(int index) const
    {
        for (int i = 0; i < NoIndex; ++i) if (Index[i].index == index) return Index[i].node;
        return 0;
    }
    Node* New(int kind, float x, float y, unsigned char level, unsigned char layer)
    {
        if (NoPool >= 32) { bad = true; return 0; }
        Node* n = &Pool[NoPool++];
        n->kind = kind; n->x = x; n->y = y; n->level = level; n->layer = layer;
        return n;
    }
    void ReadLeaf(Node* n, File& f, bool read_preferability)
    {
        f.Read(&n->region_index, 4);
        if (read_preferability) f.Read(&n->preferability, 1);
    }
    void Convert(Node* n)
    {
        ++n->converted;
        if (n->kind == QUAD)
            for (int k = 0; k < 4; ++k) n->child[k] = IndexFind(n->child_index[k]);
    }
    void Update(int m)
    {
        if (!Dirty) return;
        if (m != 13)
            for (int i = 1; i < NoSwitchable; ++i) {
                SwitchEntry e = Switchable[i]; int j = i;
                for (; j > 0 && e.id < Switchable[j - 1].id; --j) Switchable[j] = Switchable[j - 1];
                Switchable[j] = e;
            }
        Dirty = false;
    }

    bool LoadFromFile(File& file, int mapIndex, int tlx, int tly, int m)
    {
        MapIndex = mapIndex;
        MapPosX = (float)tlx; MapPosY = (float)tly;
        int file_size; file.Read(&file_size, 4);
        int version; file.Read(&version, 4);
        if (version < (m == 1 ? 4 : 5))
            return false;
        double last_breath_time = GetTime();
        file.Read(&MapWidth, 4);
        file.Read(&MapHeight, 4);
        unsigned int no_regions; file.Read(&no_regions, 4);
        for (unsigned int i = 0; i < no_regions && i < 16; ++i)
            Regions[NoRegions++] = (MapIndex << 16) ^ ((m == 2 ? i : i + 1) & 0xFFFF);
        unsigned int no_action_points; file.Read(&no_action_points, 4);
        for (unsigned int i = 0; i < no_action_points && i < 16; ++i) {
            Breath(last_breath_time, m);
            ActionPoint action_point; file.Read(&action_point, 12);
            if (m != 3) { action_point.x += MapPosX; action_point.y += MapPosY; }
            ActionPoints[NoActionPoints++] = action_point;
        }
        if (m != 11) IndexInsert(0, &Blocked);
        unsigned int no_layers; file.Read(&no_layers, 4);
        if (no_layers > 4) { bad = true; return true; }
        NoLayers = (int)no_layers;
        NodesPerLayer = (int)((MapWidth / (m == 8 ? 16.0f : 32.0f)) * (MapHeight / 32.0f));
        if (NodesPerLayer > 16) { bad = true; return true; }
        for (unsigned int i = 0; i < no_layers; ++i)
            Breath(last_breath_time, m);
        int no_nodes; file.Read(&no_nodes, 4);
        int layer = 0, node_in_layer = 0;
        for (int i = 0; i < no_nodes && !file.overrun && !bad; ++i) {
            Breath(last_breath_time, m);
            unsigned char flag = 0;
            if (version >= 6 || m == 5) file.Read(&flag, 1);
            bool switchable = flag != 0;
            file.Read(&flag, 1); bool top_level = flag != 0;
            file.Read(&flag, 1); bool blocked = flag != 0;
            Node* node;
            if ((m == 4 || !switchable) && blocked) {
                if (m != 14) Blocked.nav_layer = (unsigned char)layer;
                node = &Blocked;
            } else {
                file.Read(&flag, 1); bool leaf_node = flag != 0;
                unsigned char quad_tree_level; file.Read(&quad_tree_level, 1);
                unsigned char nav_layer; file.Read(&nav_layer, 1);
                float position[2]; file.Read(position, 8);
                float x = position[0] + MapPosX, y = position[1] + MapPosY;
                int index; file.Read(&index, 4);
                if (leaf_node) {
                    bool read_preferability = version >= (m == 6 ? 6 : 7);
                    if (switchable) {
                        node = New(SWITCHABLE, x, y, quad_tree_level, nav_layer);
                        if (!node) return true;
                        ReadLeaf(node, file, read_preferability);
                        file.Read(&node->no_ids, 4);
                        if (node->no_ids < 0 || node->no_ids > 4) { bad = true; return true; }
                        for (int k = 0; k < node->no_ids; ++k) file.Read(&node->ids[k], 8);
                        for (int j = 0; j < node->no_ids; ++j) {
                            if (NoSwitchable >= 16) { bad = true; return true; }
                            Switchable[NoSwitchable].id = node->ids[j]; Switchable[NoSwitchable].node = node; ++NoSwitchable;
                            Dirty = true;
                        }
                    } else {
                        node = New(LEAF, x, y, quad_tree_level, nav_layer);
                        if (!node) return true;
                        ReadLeaf(node, file, read_preferability);
                    }
                } else {
                    node = New(QUAD, x, y, quad_tree_level, nav_layer);
                    if (!node) return true;
                    file.Read(node->child_index, 16);
                }
                node->index = index;
                IndexInsert(index, node);
            }
            if (top_level) {
                if (layer >= NoLayers || node_in_layer >= NodesPerLayer) { bad = true; return true; }
                Layers[layer][node_in_layer] = node;
                if (m == 7 ? ++node_in_layer > NodesPerLayer : ++node_in_layer >= NodesPerLayer) {
                    ++layer;
                    node_in_layer = 0;
                }
            }
        }
        int end_marker; file.Read(&end_marker, 4);
        for (int l = 0; l < NoLayers; ++l) {
            Breath(last_breath_time, m);
            for (int k = 0; k < NodesPerLayer; ++k)
                if (Layers[l][k]) Convert(Layers[l][k]); else bad = true;
        }
        Breath(last_breath_time, m);
        Update(m);
        for (int i = 0; i < NoSwitchable; ++i) {
            Node* n = Switchable[i].node;
            if (m == 9) ++n->closed; else ++n->opened;
            open_log[no_open_log++] = Switchable[i].id;
        }
        Initialised = m != 12;
        return true;
    }
};

static unsigned long g_sig;
static void Mix(unsigned long v) { g_sig = g_sig * 16777619UL ^ (v & 0xFFFFFFFFUL); }
static void MixF(float f) { unsigned int u; std::memcpy(&u, &f, 4); Mix(u); }
static int Ident(const Tree& t, const Node* n)       // a pointer-free identity of a node
{
    if (n == 0) return -1;
    if (n == &t.Blocked) return 1000;
    return 2000 + n->index * 4 + n->kind;
}
static void MixTree(const Tree& t, bool result, const File& f)
{
    Mix(result); Mix(t.Initialised); Mix(t.bad); Mix(f.overrun); Mix(f.pos); Mix(t.MapIndex);
    MixF(t.MapPosX); MixF(t.MapPosY); MixF(t.MapWidth); MixF(t.MapHeight);
    Mix(t.NoRegions); for (int i = 0; i < t.NoRegions; ++i) Mix(t.Regions[i]);
    Mix(t.NoActionPoints);
    for (int i = 0; i < t.NoActionPoints; ++i) { MixF(t.ActionPoints[i].x); MixF(t.ActionPoints[i].y); Mix(t.ActionPoints[i].layer); }
    Mix(t.NoLayers); Mix(t.NodesPerLayer);
    for (int l = 0; l < t.NoLayers; ++l) for (int k = 0; k < t.NodesPerLayer; ++k) Mix(Ident(t, t.Layers[l][k]));
    Mix(t.Blocked.nav_layer); Mix(t.Blocked.converted);
    Mix(t.NoPool);
    for (int i = 0; i < t.NoPool; ++i) {
        const Node& n = t.Pool[i];
        Mix(n.kind); MixF(n.x); MixF(n.y); Mix(n.level); Mix(n.layer); Mix(n.index); Mix(n.region_index); Mix(n.preferability);
        Mix(n.converted); Mix(n.opened); Mix(n.closed);
        for (int k = 0; k < 4; ++k) Mix(Ident(t, n.child[k]));
    }
    Mix(t.NoSwitchable); Mix(t.Dirty);
    for (int i = 0; i < t.no_open_log; ++i) { Mix((unsigned long)(t.open_log[i] & 0xFFFFFFFFUL)); Mix((unsigned long)(t.open_log[i] >> 32)); }
    Mix(t.suspends); Mix(g_time_calls);
}

// record writers (the test's file format mirrors the model's reads)
static void Header(File& f, int version, float w, float h, int regions)
{
    f.P4(1234); f.P4(version); f.PF(w); f.PF(h); f.P4(regions);
}
static void Flags(File& f, int version, bool sw, bool top, bool blocked)
{
    if (version >= 6) f.P1(sw);
    f.P1(top); f.P1(blocked);
}
static void Common(File& f, bool leaf, unsigned char level, unsigned char layer, float x, float y, int index)
{
    f.P1(leaf); f.P1(level); f.P1(layer); f.PF(x); f.PF(y); f.P4(index);
}
static void LeafData(File& f, int version, int region, unsigned char pref)
{
    f.P4(region); if (version >= 7) f.P1(pref);
}
static void Build(File& f, int version)
{
    Header(f, version, 64.0f, 64.0f, 3);
    f.P4(2);                                               // action points
    f.PF(1.0f); f.PF(2.0f); f.P4(0);
    f.PF(3.5f); f.PF(4.5f); f.P4(1);
    f.P4(2);                                               // layers (4 nodes each)
    f.P4(10);                                              // node records
    Flags(f, version, false, true, true);                                              // layer 0 slot 0: blocked
    Flags(f, version, false, true, false); Common(f, false, 1, 0, 10.0f, 20.0f, 5);    // layer 0 slot 1: quad 5
    f.P4(0); f.P4(7); f.P4(8); f.P4(0);
    Flags(f, version, false, false, false); Common(f, true, 0, 0, 11.0f, 21.0f, 7);    // child leaf 7
    LeafData(f, version, 1, 9);
    Flags(f, version, true, false, version >= 6); Common(f, true, 0, 0, 12.0f, 22.0f, 8);   // child 8: switchable AND blocked (version 6+)
    LeafData(f, version, 2, 3);
    if (version >= 6) { f.P4(2); f.P8(((u64)5 << 32) | 2); f.P8(((u64)1 << 32) | 1); }
    Flags(f, version, true, true, false); Common(f, true, 0, 0, 13.0f, 23.0f, 9);      // layer 0 slot 2: switchable 9 (a leaf before 6)
    LeafData(f, version, 3, 4);
    if (version >= 6) { f.P4(1); f.P8(((u64)3 << 32) | 3); }
    Flags(f, version, false, true, true);                                              // layer 0 slot 3: blocked -> wraps
    Flags(f, version, false, true, true);                                              // layer 1 slot 0: blocked (NavLayer 1)
    Flags(f, version, false, true, false); Common(f, true, 2, 1, 14.0f, 24.0f, 11);    // layer 1 slot 1: leaf 11
    LeafData(f, version, 1, 6);
    Flags(f, version, false, true, false); Common(f, true, 2, 1, 15.0f, 25.0f, 5);     // layer 1 slot 2: leaf with the taken index 5
    LeafData(f, version, 2, 7);
    Flags(f, version, false, true, false); Common(f, false, 3, 1, 16.0f, 26.0f, 12);   // layer 1 slot 3: quad 12
    f.P4(11); f.P4(9); f.P4(0); f.P4(5);
    f.P4(-1);                                              // end marker
}

static unsigned long Run(int m, bool check, bool& ok)
{
    g_sig = 2166136261UL;
    static const int versions[4] = { 7, 6, 5, 4 };
    for (int c = 0; c < 4; ++c) {
        int version = versions[c];
        File f; Build(f, version);
        static Tree t;
        t = Tree();
        g_clock = 100.0; g_time_calls = 0;
        bool result = t.LoadFromFile(f, 3, 100, 200, m);
        MixTree(t, result, f);
        if (!check) continue;
        if (version == 4) {
            if (result || t.Initialised || f.pos != 8 || g_time_calls != 0 || t.NoRegions != 0) ok = false;
            if (t.MapIndex != 3 || t.MapPosX != 100.0f || t.MapPosY != 200.0f) ok = false;   // stored before the version test
            continue;
        }
        if (!result || !t.Initialised || t.bad || f.overrun || f.pos != f.len) ok = false;
        if (t.NoRegions != 3 || t.Regions[0] != 0x30001 || t.Regions[2] != 0x30003) ok = false;
        if (t.NoActionPoints != 2 || t.ActionPoints[0].x != 101.0f || t.ActionPoints[0].y != 202.0f || t.ActionPoints[1].x != 103.5f ||
            t.ActionPoints[1].y != 204.5f || t.ActionPoints[1].layer != 1) ok = false;
        if (t.NoLayers != 2 || t.NodesPerLayer != 4) ok = false;
        if (t.Layers[0][0] != &t.Blocked || t.Layers[0][3] != &t.Blocked || t.Layers[1][0] != &t.Blocked) ok = false;
        if (t.Blocked.nav_layer != 1 || t.Blocked.converted != 3) ok = false;
        Node* quad5 = t.Layers[0][1]; Node* quad12 = t.Layers[1][3];
        if (!quad5 || quad5->kind != QUAD || quad5->index != 5 || quad5->x != 110.0f || quad5->y != 220.0f || quad5->converted != 1) ok = false;
        if (!quad12 || quad12->kind != QUAD || quad12->index != 12) ok = false;
        if (ok) {
            // index 0 resolves to the shared blocked node; the second record with index 5 does not replace the first
            if (quad5->child[0] != &t.Blocked || quad5->child[3] != &t.Blocked || quad12->child[2] != &t.Blocked) ok = false;
            if (!quad5->child[1] || quad5->child[1]->index != 7 || quad5->child[1]->kind != LEAF || quad5->child[1]->converted != 0) ok = false;
            if (quad12->child[3] != quad5 || quad12->child[0] != t.Layers[1][1] || quad12->child[1] != t.Layers[0][2]) ok = false;
            if (!quad5->child[2] || quad5->child[2]->kind != (version >= 6 ? SWITCHABLE : LEAF)) ok = false;
            if (quad5->child[1]->preferability != (version >= 7 ? 9 : 0)) ok = false;
        }
        // breath checks: 2 action points + 2 layers + 10 records + 2 layers + 1, after the initial time read; every second one breathes
        if (g_time_calls != 18 || t.suspends != 8) ok = false;
        if (version >= 6) {
            if (t.NoSwitchable != 3 || t.Dirty) ok = false;
            if (t.open_log[0] != (((u64)1 << 32) | 1) || t.open_log[1] != (((u64)3 << 32) | 3) || t.open_log[2] != (((u64)5 << 32) | 2)) ok = false;
            if (ok && (quad5->child[2]->opened != 2 || quad5->child[2]->closed != 0 || t.Layers[0][2]->opened != 1)) ok = false;
        } else if (t.NoSwitchable != 0 || t.Layers[0][2]->kind != LEAF) ok = false;
    }
    return g_sig;
}

int main()
{
    bool ok = true;
    unsigned long base = Run(0, true, ok);
    if (!ok) { std::printf("00a7af60_TEST FAIL\n"); return 1; }
    bool dummy = true;
    if (Run(0, false, dummy) != base) { std::printf("00a7af60_TEST FAIL\n"); return 1; }   // the signature is deterministic
    for (int m = 1; m <= 14; ++m)
        if (Run(m, false, dummy) == base) { std::printf("00a7af60_TEST FAIL\n"); return 1; }
    std::printf("00a7af60_TEST PASS\n");
    return 0;
}
