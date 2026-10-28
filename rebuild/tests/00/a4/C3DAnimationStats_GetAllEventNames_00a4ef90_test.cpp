#include <cstdio>
#include <cstring>
#include <map>
#include <vector>
#include <string>
// Behaviour test for C3DAnimationStats::GetAllEventNames (0x00A4EF90): reserve, then every multimap key in order.
struct Stats {
    std::multimap<std::string, float> TimingEvents;
    void GetAllEventNames(std::vector<std::string>& names) const
    {
        names.reserve(TimingEvents.size());
        for (std::multimap<std::string, float>::const_iterator it = TimingEvents.begin(); it != TimingEvents.end(); ++it)
            names.push_back(it->first);
    }
};
int main()
{
    Stats s;
    s.TimingEvents.insert(std::make_pair(std::string("Step"), 0.5f));
    s.TimingEvents.insert(std::make_pair(std::string("Attack"), 0.2f));
    s.TimingEvents.insert(std::make_pair(std::string("Step"), 0.9f));
    std::vector<std::string> names;
    names.push_back("Old");
    s.GetAllEventNames(names);
    bool ok = names.size() == 4 && names[0] == "Old" && names[1] == "Attack" && names[2] == "Step" && names[3] == "Step";
    std::printf(ok ? "00a4ef90_TEST PASS\n" : "00a4ef90_TEST FAIL\n");
    return ok ? 0 : 1;
}