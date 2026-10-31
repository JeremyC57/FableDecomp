// Test: CDisplayManager::EnumerateAntiAliasingModes -- every multisample type the device accepts for any back-buffer
// format, collected into a sorted unique set (std::set), then copied in order into the output array (cleared first).
#include <cstdio>
#include <set>
#include <vector>
struct FakeD3D {
    // accepts (format, type) when type divides format or type is 0
    long CheckDeviceMultiSampleType(unsigned int, long, long format, long, long type, long) const
    {
        return (type == 0 || format % type == 0) ? 0 : -1;
    }
};
static void Enumerate(const FakeD3D& d3d, const std::vector<long>& formats, const std::vector<long>& types,
                      std::vector<long>& out)
{
    std::set<long> found;
    for (unsigned int i = 0; i < formats.size(); ++i)
        for (unsigned int j = 0; j < types.size(); ++j) {
            long type = types[j];
            if (d3d.CheckDeviceMultiSampleType(0, 1, formats[i], 0, type, 0) == 0)
                found.insert(type);
        }
    out.clear();
    out.reserve(found.size());
    for (std::set<long>::iterator it = found.begin(); it != found.end(); ++it)
        out.push_back(*it);
}
int main()
{
    FakeD3D d3d;
    std::vector<long> formats, types, out;
    formats.push_back(6); formats.push_back(8);
    types.push_back(4); types.push_back(0); types.push_back(3); types.push_back(5); types.push_back(2); types.push_back(4);
    out.push_back(99);
    Enumerate(d3d, formats, types, out);
    // 6: 0,3,2  8: 4,0,2,4  -> {0,2,3,4}
    bool ok = out.size() == 4 && out[0] == 0 && out[1] == 2 && out[2] == 3 && out[3] == 4;
    std::vector<long> none;
    Enumerate(d3d, none, types, out);
    ok = ok && out.empty();
    std::printf(ok ? "009c05c0_TEST PASS\n" : "009c05c0_TEST FAIL\n");
    return ok ? 0 : 1;
}