// Behaviour test for 0x00CBC130 (parked row): the row's logic over a flat model.
#include <cstdio>

#include <vector>
#include <string>
struct Track { unsigned long ID; std::string Name; };
int main() {
    std::vector<Track> v; v.reserve(1);
    Track a = { 1, "one" }, b = { 2, "two" }, c = { 3, "three" };
    v.push_back(a); v.push_back(b); v.insert(v.begin() + 1, 2, c);    // growth through the overflow path (copy head, fill, copy tail)
    bool ok = v.size() == 4 && v[0].Name == "one" && v[1].ID == 3 && v[2].Name == "three" && v[3].Name == "two" && v.capacity() >= 4;
    std::printf(ok ? "00cbc130_TEST PASS\n" : "00cbc130_TEST FAIL\n");
    return ok ? 0 : 1;
}
