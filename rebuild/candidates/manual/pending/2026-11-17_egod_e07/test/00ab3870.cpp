// Behaviour test for CVertexShaderInput::GenerateD3DDeclaration (0x00AB3870): the declaration is resized to sum(elements + 1) over the
// streams, then one element per stream element {stream index, running byte offset, D3DTypes[type], DEFAULT, TEXCOORD, Register member} and
// the D3DDECL_END marker {0xFF, 0, UNUSED, 0, 0, 0}; a type with no D3D equivalent (-1) clears the declaration. Model of the row's
// logic (the STLport resize / clear and the GetDataTypeSize switch included).
#include <cstdio>
#include <vector>
struct Ele { unsigned short Stream, Offset; unsigned char Type, Method, Usage, UsageIndex; };
struct Elem { unsigned long Register; int DataType; };
struct Stream { std::vector<Elem> Elements; };
static const long D3DTypes[14] = { 0, 1, 2, 3, 4, 6, 7, 6, -1, -1, -1, -1, -1, -1 };
static unsigned long GetDataTypeSize(int type)
{
    switch (type) {
    case 0: return 4; case 4: return 4; case 5: return 4; case 7: return 4; case 9: return 4; case 12: return 4; case 13: return 4;
    case 1: return 8; case 6: return 8; case 11: return 8; case 2: return 12; case 3: return 16; case 8: return 2; case 10: return 6;
    }
    return 0;
}
static void Generate(const std::vector<Stream>& streams, std::vector<Ele>& decl)
{
    long data_offset = 0;
    unsigned long size = 0;
    for (unsigned long c_stream = 0; c_stream < streams.size(); ++c_stream)
        size += (unsigned long)streams[c_stream].Elements.size() + 1;
    Ele zero = { 0, 0, 0, 0, 0, 0 };
    if (size < decl.size()) decl.erase(decl.begin() + size, decl.end()); else decl.insert(decl.end(), size - decl.size(), zero);
    unsigned long dest_pos = 0;
    for (unsigned long c_stream = 0; c_stream < streams.size(); ++c_stream) {
        unsigned long no_elem = (unsigned long)streams[c_stream].Elements.size();
        for (unsigned long c_elem = 0; c_elem < no_elem; ++c_elem) {
            int type = streams[c_stream].Elements[c_elem].DataType;
            long d3dtype = D3DTypes[type];
            if (d3dtype == -1) { decl.clear(); return; }
            Ele ele = { (unsigned short)c_stream, (unsigned short)data_offset, (unsigned char)d3dtype, 0, 5,
                        (unsigned char)streams[c_stream].Elements[c_elem].Register };
            decl.at(dest_pos++) = ele;                                       // at(): a store after clear() (no early return) throws
            data_offset += GetDataTypeSize(type);
        }
    }
    Ele end = { 0xff, 0, 17, 0, 0, 0 };
    decl.at(dest_pos) = end;
}
static int fail(int n) { std::printf("00ab3870_TEST FAIL %d\n", n); return 1; }
int main()
{
    std::vector<Stream> s(2);
    Elem a = { 0, 2 }, b = { 3, 1 }, c = { 7, 4 };                       // FLOAT3, FLOAT2, D3DCOLOR
    s[0].Elements.push_back(a); s[0].Elements.push_back(b); s[1].Elements.push_back(c);
    std::vector<Ele> d(9);                                               // larger than needed: shrinks to 2+1+1+1 = 5
    Generate(s, d);
    if (d.size() != 5) return fail(1);
    if (d[0].Stream != 0 || d[0].Offset != 0 || d[0].Type != 2 || d[0].Usage != 5 || d[0].UsageIndex != 0) return fail(2);
    for (int k = 0; k < 3; ++k) if (d[k].Method != 0 || d[k].Usage != 5) return fail(40 + k);           // DEFAULT method, TEXCOORD usage
    if (d[1].Stream != 0 || d[1].Offset != 12 || d[1].Type != 1 || d[1].UsageIndex != 3) return fail(3);
    if (d[2].Stream != 1 || d[2].Offset != 20 || d[2].Type != 4 || d[2].UsageIndex != 7) return fail(4);   // offset keeps running
    if (d[3].Stream != 0xff || d[3].Type != 17 || d[3].Offset != 0 || d[3].Method != 0 || d[3].Usage != 0 || d[3].UsageIndex != 0) return fail(5);
    if (d[4].Stream != 0 || d[4].Type != 0) return fail(6);                // spare entry left as filled / kept
    std::vector<Ele> e;                                                  // grows from empty: zero filled
    Generate(s, e); if (e.size() != 5 || e[3].Stream != 0xff || e[4].Stream != 0 || e[4].Usage != 0) return fail(7);
    if (e[4].Offset != 0 || e[4].Type != 0 || e[4].Method != 0 || e[4].UsageIndex != 0) return fail(43);    // value-initialised fill
    Elem bad = { 1, 8 };                                                 // NORMSHORT1 has no D3D type
    s[1].Elements.push_back(bad);
    Generate(s, e); if (!e.empty()) return fail(8);
    for (int t = 0; t < 14; ++t) {                                       // every size of the switch
        static const unsigned long sz[14] = { 4, 8, 12, 16, 4, 4, 8, 4, 2, 4, 6, 8, 4, 4 };
        if (GetDataTypeSize(t) != sz[t]) return fail(9 + t);
    }
    if (GetDataTypeSize(14) != 0) return fail(30);
    static const long want[14] = { 0, 1, 2, 3, 4, 6, 7, 6, -1, -1, -1, -1, -1, -1 };   // retail D3DTypes 0x0129E9CC
    for (int t = 0; t < 14; ++t) {                                       // every type through one single-element stream
        std::vector<Stream> one(1); Elem el = { 9, t }; one[0].Elements.push_back(el);
        std::vector<Ele> o(3);
        Generate(one, o);
        if (want[t] == -1) { if (!o.empty()) return fail(50 + t); }
        else if (o.size() != 2 || o[0].Type != (unsigned char)want[t] || o[0].UsageIndex != 9 || o[1].Stream != 0xff) return fail(70 + t);
    }
    std::printf("00ab3870_TEST PASS\n");
    return 0;
}
