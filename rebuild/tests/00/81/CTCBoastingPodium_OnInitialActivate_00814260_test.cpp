// Behaviour test for CTCBoastingPodium::OnInitialActivate (0x00814260): a standalone model of the dummy scan -- names with the
// boast-position prefix get a running per-name dummy index, their numeric suffix becomes the position number, a created object that
// is a boasting position is told its podium and filed by number (map sorted at the end), anything else is killed.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

struct Created { int id; bool is_position; bool killed; std::string podium; };

struct Podium {
    std::vector<std::pair<long, int> > positions;     // (number, created id)
    bool dirty;
    std::vector<Created> things;
    std::vector<long> dummy_indices;                   // the index passed to GetDummyObject per match
    int next_non_position;                             // every n-th create is not a boasting position (0: never)

    void OnInitialActivate(const std::vector<std::string>& names, const std::string& prefix)
    {
        std::string last_name("");
        long index = 0;
        for (unsigned long i = 0; i < names.size(); ++i) {
            if (std::strncmp(names[i].c_str(), prefix.c_str(), prefix.size()) == 0) {
                if (names[i] == last_name)
                    ++index;
                else {
                    last_name = names[i];
                    index = 0;
                }
                dummy_indices.push_back(index);
                std::string number = names[i].substr(prefix.size());
                if (number.size() > 0) {
                    long position_number = std::atol(number.c_str());
                    Created c;
                    c.id = (int)things.size();
                    c.is_position = !(next_non_position && (c.id + 1) % next_non_position == 0);
                    c.killed = false;
                    if (c.is_position) {
                        c.podium = "podium";
                        positions.push_back(std::make_pair(position_number, c.id));
                        dirty = true;
                    } else
                        c.killed = true;
                    things.push_back(c);
                }
            }
        }
        if (dirty) {
            std::sort(positions.begin(), positions.end());
            dirty = false;
        }
    }
};

int main()
{
    std::vector<std::string> names;
    names.push_back("BOAST_POS_3");
    names.push_back("BOAST_POS_3");
    names.push_back("OTHER_DUMMY");
    names.push_back("BOAST_POS_1");
    names.push_back("BOAST_POS_");                     // no number: skipped after the dummy lookup
    names.push_back("BOAST_POS_2");
    Podium p;
    p.dirty = false;
    p.next_non_position = 3;                           // the third creation is not a boasting position
    p.OnInitialActivate(names, "BOAST_POS_");
    // dummy indices: 3 -> 0, 3 again -> 1, 1 -> 0, "" -> 0, 2 -> 0
    long want_idx[] = { 0, 1, 0, 0, 0 };
    if (p.dummy_indices.size() != 5) { std::printf("00814260_TEST FAIL\n"); return 1; }
    for (int k = 0; k < 5; ++k)
        if (p.dummy_indices[k] != want_idx[k]) { std::printf("00814260_TEST FAIL\n"); return 1; }
    if (p.things.size() != 4 || !p.things[2].killed || p.things[0].killed || p.things[0].podium != "podium") {
        std::printf("00814260_TEST FAIL\n"); return 1;
    }
    // filed: 3 (id 0), 3 (id 1), 2 (id 3) -> sorted 2, 3, 3; map not dirty afterwards
    if (p.positions.size() != 3 || p.positions[0].first != 2 || p.positions[1].first != 3 || p.positions[2].first != 3 || p.dirty) {
        std::printf("00814260_TEST FAIL\n"); return 1;
    }
    std::printf("00814260_TEST PASS\n");
    return 0;
}