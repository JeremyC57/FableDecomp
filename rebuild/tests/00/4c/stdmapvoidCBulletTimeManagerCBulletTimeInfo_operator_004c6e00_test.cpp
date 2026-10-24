// behaviour test for 004c6e00: map<void*, CBulletTimeInfo>::operator[] -- an existing key returns its
// entry without inserting; a new key is inserted (through insert_unique, here a plain unbalanced
// insert that ignores the hint) and its entry returned; the header's leftmost/parent links follow.
#include <cstdio>
struct CBulletTimeInfo {
    float Multiplier;
    float SoundPitch;
    CBulletTimeInfo() { }
};
struct SNode {
    long Color;
    SNode* Parent;
    SNode* Left;
    SNode* Right;
    void* Key;
    CBulletTimeInfo Value;
};
struct SIterator {
    SNode* _M_node;
    SIterator(SNode* node) : _M_node(node) { }
    SIterator(const SIterator& other) : _M_node(other._M_node) { }
};
struct SValue {
    void* first;
    CBulletTimeInfo second;
    SValue(void* const& key, const CBulletTimeInfo& info) : first(key), second(info) { }
};
static SNode g_pool[16];
static int g_used = 0;
static int g_inserts = 0;
struct CMap {
    SNode* _M_header;
    unsigned long _M_node_count;
    SIterator insert_unique(SIterator, const SValue& value)
    {
        ++g_inserts;
        SNode* node = &g_pool[g_used++];
        node->Key = value.first;
        node->Value.Multiplier = 0.0f;
        node->Value.SoundPitch = 0.0f;
        node->Left = 0;
        node->Right = 0;
        SNode* parent = _M_header;
        SNode* x = _M_header->Parent;
        bool left = true;
        while (x != 0) {
            parent = x;
            left = (char*)value.first < (char*)x->Key;
            x = left ? x->Left : x->Right;
        }
        node->Parent = parent;
        if (parent == _M_header) _M_header->Parent = node;
        else if (left) parent->Left = node;
        else parent->Right = node;
        ++_M_node_count;
        return SIterator(node);
    }
    SIterator lower_bound(void* const& key)
    {
        SNode* y = _M_header;
        SNode* x = _M_header->Parent;
        while (x != 0) {
            if (!((char*)x->Key < (char*)key)) {
                y = x;
                x = x->Left;
            } else
                x = x->Right;
        }
        return SIterator(y);
    }
    CBulletTimeInfo& operator[](void* const& key)
    {
        SIterator i = lower_bound(key);
        if (i._M_node == _M_header || (char*)key < (char*)i._M_node->Key) {
            SIterator inserted = insert_unique(i, SValue(key, CBulletTimeInfo()));
            i = inserted;
        }
        return i._M_node->Value;
    }
};
int main()
{
    char objects[8];
    SNode header;
    header.Parent = 0;
    header.Left = &header;
    header.Right = &header;
    CMap m;
    m._M_header = &header;
    m._M_node_count = 0;
    int ok = 1;
    void* k3 = &objects[3];
    void* k1 = &objects[1];
    void* k6 = &objects[6];
    m[k3].Multiplier = 0.5f;
    m[k1].Multiplier = 0.25f;
    m[k6].Multiplier = 2.0f;
    if (g_inserts != 3 || m._M_node_count != 3) ok = 0;
    CBulletTimeInfo& again = m[k3];
    if (g_inserts != 3 || again.Multiplier != 0.5f) ok = 0;
    again.SoundPitch = 1.5f;
    if (m[k3].SoundPitch != 1.5f || m[k1].Multiplier != 0.25f || m[k6].Multiplier != 2.0f || g_inserts != 3) ok = 0;
    void* k0 = &objects[0];
    if (m[k0].Multiplier != 0.0f || g_inserts != 4) ok = 0;
    if (!ok) { std::printf("004c6e00_TEST FAIL\n"); return 1; }
    std::printf("004c6e00_TEST PASS\n");
    return 0;
}