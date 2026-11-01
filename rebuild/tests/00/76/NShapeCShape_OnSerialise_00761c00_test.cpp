// Behaviour test for NShape::CShape::OnSerialise (0x00761C00): a copy of the persisted-name logic over a recording
// context, checking the "Shape[<i>]" names, the type-name gate (types 0 and 1 only) and the offset-relative X/Y values.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

struct Vec { float X, Y, Z; };
struct Coord { long X, Y; };
struct Record { std::string name; std::string text; float value; long count; };
struct Context {
    std::vector<Record> log;
    void Transfer(const std::string& n, std::string& v) { Record r; r.name = n; r.text = v; r.value = 0; r.count = 0; log.push_back(r); }
    void Transfer(const std::string& n, long& v) { Record r; r.name = n; r.value = 0; r.count = v; log.push_back(r); }
    void Transfer(const std::string& n, float& v) { Record r; r.name = n; r.value = v; r.count = 0; log.push_back(r); }
};

static std::string IntStr(long v) { char b[16]; std::sprintf(b, "%ld", v); return b; }

static void OnSerialise(int type, const std::vector<Vec>& points, Context& context, long index, const Coord& offset)
{
    std::string name = std::string("Shape[") + IntStr(index) + "]";
    std::string typeName;
    switch (type) {
    case 0: typeName = "SHAPE_TYPE_CLOSED"; break;
    case 1: typeName = "SHAPE_TYPE_LINE"; break;
    default: return;
    }
    context.Transfer(name + ".Type", typeName);
    long count = (long)points.size();
    context.Transfer(name + ".size()", count);
    long i = 0;
    for (std::vector<Vec>::const_iterator it = points.begin(); it != points.end(); ++it) {
        std::string pointName = name + ".pos[" + IntStr(i++) + "]";
        float value = it->X - (float)offset.X;
        context.Transfer(pointName + ".X", value);
        value = it->Y - (float)offset.Y;
        context.Transfer(pointName + ".Y", value);
        value = it->Z;
        context.Transfer(pointName + ".Z", value);
    }
}

int main()
{
    int fails = 0;
    std::vector<Vec> pts;
    Vec a = { 10.0f, 20.0f, 3.0f }; pts.push_back(a);
    Vec b = { 4.5f, -1.0f, 7.0f }; pts.push_back(b);
    Coord off = { 2, 5 };

    Context c0; OnSerialise(0, pts, c0, 7, off);
    if (c0.log.size() != 8) ++fails;
    else {
        if (c0.log[0].name != "Shape[7].Type" || c0.log[0].text != "SHAPE_TYPE_CLOSED") ++fails;
        if (c0.log[1].name != "Shape[7].size()" || c0.log[1].count != 2) ++fails;
        if (c0.log[2].name != "Shape[7].pos[0].X" || c0.log[2].value != 8.0f) ++fails;
        if (c0.log[3].name != "Shape[7].pos[0].Y" || c0.log[3].value != 15.0f) ++fails;
        if (c0.log[4].name != "Shape[7].pos[0].Z" || c0.log[4].value != 3.0f) ++fails;
        if (c0.log[5].name != "Shape[7].pos[1].X" || c0.log[5].value != 2.5f) ++fails;
        if (c0.log[6].name != "Shape[7].pos[1].Y" || c0.log[6].value != -6.0f) ++fails;
        if (c0.log[7].name != "Shape[7].pos[1].Z" || c0.log[7].value != 7.0f) ++fails;
    }
    Context c1; OnSerialise(1, std::vector<Vec>(), c1, 0, off);
    if (c1.log.size() != 2 || c1.log[0].text != "SHAPE_TYPE_LINE" || c1.log[1].count != 0) ++fails;
    Context c2; OnSerialise(2, pts, c2, 3, off);
    if (!c2.log.empty()) ++fails;

    if (fails == 0) std::printf("00761c00_TEST PASS\n");
    else std::printf("00761c00_TEST FAIL %d\n", fails);
    return fails;
}