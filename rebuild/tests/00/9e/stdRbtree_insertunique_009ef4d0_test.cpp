// Behaviour test for 0x009EF4D0 _Rb_tree<char, pair<const char, CCharString> > (CConsole key-binding map; keys compare as signed chars)::insert_unique(iterator hint, const value_type& v):
// the STLport hint logic over a sorted-array stand-in for the tree (an iterator is an index, end() == n).
#include <cstdio>
typedef signed char u64;   // the key type (char, signed compare)
static u64 g_keys[64];
static int g_n = 0;
static int g_fallback = 0, g_hinted = 0;
static int insert_at(int pos, u64 k) { for (int i = g_n; i > pos; --i) g_keys[i] = g_keys[i - 1]; g_keys[pos] = k; ++g_n; return pos; }
static int insert_unique_plain(u64 k)
{
    ++g_fallback;
    int i = 0; while (i < g_n && g_keys[i] < k) ++i;
    if (i < g_n && g_keys[i] == k) return i;
    return insert_at(i, k);
}
static int insert_unique_hint(int pos, u64 v)
{
    if (pos == 0) {                                          // begin()
        if (g_n <= 0) return insert_unique_plain(v);
        if (v < g_keys[pos]) { ++g_hinted; return insert_at(pos, v); }
        bool comp_pos_v = g_keys[pos] < v;
        if (!comp_pos_v) return pos;
        int after = pos + 1;
        if (after == g_n) { ++g_hinted; return insert_at(after, v); }
        if (v < g_keys[after]) { ++g_hinted; return insert_at(after, v); }
        return insert_unique_plain(v);
    }
    else if (pos == g_n) {                                   // end()
        if (g_keys[g_n - 1] < v) { ++g_hinted; return insert_at(g_n, v); }
        return insert_unique_plain(v);
    }
    else {
        int before = pos - 1;
        bool comp_v_pos = v < g_keys[pos];
        if (comp_v_pos && g_keys[before] < v) { ++g_hinted; return insert_at(pos, v); }
        int after = pos + 1;
        bool comp_pos_v = !comp_v_pos;
        if (!comp_v_pos) comp_pos_v = g_keys[pos] < v;
        if (!comp_v_pos && comp_pos_v && (after == g_n || v < g_keys[after])) { ++g_hinted; return insert_at(after, v); }
        if (comp_v_pos == comp_pos_v) return pos;
        return insert_unique_plain(v);
    }
}
int main()
{
    int bad = 0;
    int r = insert_unique_hint(0, (u64)'m');                 // empty: falls back
    if (r != 0 || g_n != 1 || g_fallback != 1) bad = 1;
    r = insert_unique_hint(g_n, (u64)'z');                   // end() hint, larger key
    if (r != 1 || g_hinted != 1) bad = 1;
    r = insert_unique_hint(0, (u64)-5);                      // begin() hint, negative char sorts first (signed)
    if (r != 0 || g_hinted != 2) bad = 1;
    r = insert_unique_hint(1, (u64)'m');                     // equal key at the hint: returns it
    if (r != 1 || g_n != 3) bad = 1;
    r = insert_unique_hint(2, (u64)'q');                     // interior hint brackets v ('m' < 'q' < 'z')
    if (r != 2 || g_hinted != 3) bad = 1;
    r = insert_unique_hint(1, (u64)'~');                     // wrong hint: falls back, goes to the end
    if (r != g_n - 1 || g_fallback != 2) bad = 1;
    for (int i = 1; i < g_n; ++i) if (!(g_keys[i - 1] < g_keys[i])) bad = 1;
    if (g_n != 5) bad = 1;
    if (bad) { std::printf("009ef4d0_TEST FAIL\n"); return 1; }
    std::printf("009ef4d0_TEST PASS\n");
    return 0;
}