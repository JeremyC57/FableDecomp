// Behaviour test for CTCShop::StockDisplay (0x00774C60), g++ model: one sale item per free place, as far as both lists go.
#include <cstdio>
#include <vector>
#include <utility>
static std::vector<std::pair<long, int> > StockDisplay(const std::vector<int>& places, const std::vector<long>& items)
{
    std::vector<std::pair<long, int> > added;
    unsigned long i = 0, numItems = items.size(), numPlaces = places.size();
    for (std::vector<int>::const_iterator place = places.begin(); i < (numPlaces < numItems ? numPlaces : numItems); ++place) {
        added.push_back(std::make_pair(items[i], *place));
        ++i;
    }
    return added;
}
int main()
{
    std::vector<int> places; places.push_back(7); places.push_back(9);
    std::vector<long> items; items.push_back(100); items.push_back(200); items.push_back(300);
    std::vector<std::pair<long, int> > r = StockDisplay(places, items);
    int ok = r.size() == 2 && r[0].first == 100 && r[0].second == 7 && r[1].first == 200 && r[1].second == 9;
    ok = ok && StockDisplay(places, std::vector<long>()).empty() && StockDisplay(std::vector<int>(), items).empty();
    std::vector<long> one(1, 5); r = StockDisplay(places, one); ok = ok && r.size() == 1 && r[0].second == 7;
    std::printf(ok ? "00774c60_TEST PASS\n" : "00774c60_TEST FAIL\n");
    return ok ? 0 : 1;
}
