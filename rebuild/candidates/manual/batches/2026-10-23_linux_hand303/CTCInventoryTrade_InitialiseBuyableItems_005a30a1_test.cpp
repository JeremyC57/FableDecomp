// Behaviour test for CTCInventoryTrade::InitialiseBuyableItems @ 0x005A30A1: stocked marks for hero specials, sold-out list, buyable totals
#include <cstdio>
#include <map>
#include <vector>
#include <utility>
struct Shop { int tattoo; int barber; int ability; };
static int IsTattoo(int d) { return d >= 100 && d < 200; }
static int IsHair(int d) { return d >= 200 && d < 300; }
static int IsAbility(int d) { return d >= 300 && d < 400; }
static int Initialise(const Shop& shop, std::map<int, int>& stock, const std::vector<std::pair<int, int> >& hero,
                      std::map<int, int>& buyable, std::vector<int>& notInStock, std::vector<std::pair<int, int> >& items)
{
    int total = 0;
    buyable.clear();
    for (unsigned int i = 0; i < hero.size(); ++i) {
        int key = hero[i].first;
        bool special = IsTattoo(key) && shop.tattoo;
        if (!special)
            special = IsHair(key) && shop.barber;
        if (special || (IsAbility(key) && shop.ability)) {
            stock[key] = 1;
            if (buyable.find(key) == buyable.end()) buyable[key] = hero[i].second;
            else buyable[key] += hero[i].second;
        }
    }
    notInStock.clear();
    for (std::map<int, int>::iterator it = stock.begin(); it != stock.end(); ++it) {
        int count = it->second;
        int key = it->first;
        if (count == 0) {
            if (!IsTattoo(key) && !IsHair(key)) notInStock.push_back(key);
        } else {
            total += count;
            items.push_back(std::make_pair(key, count));
        }
    }
    return total;
}
int main()
{
    int ok = 1;
    Shop shop = { 1, 0, 1 };
    std::map<int, int> stock;
    stock[5] = 3; stock[6] = 0; stock[150] = 0;
    std::vector<std::pair<int, int> > hero;
    hero.push_back(std::make_pair(150, 2));
    hero.push_back(std::make_pair(250, 1));
    hero.push_back(std::make_pair(350, 4));
    std::map<int, int> buyable;
    std::vector<int> notInStock;
    std::vector<std::pair<int, int> > items;
    int total = Initialise(shop, stock, hero, buyable, notInStock, items);
    if (buyable.size() != 2 || buyable[150] != 2 || buyable[350] != 4) ok = 0;
    if (stock[150] != 1 || stock.count(250) != 0) ok = 0;
    if (notInStock.size() != 1 || notInStock[0] != 6) ok = 0;
    if (total != 5 || items.size() != 3) ok = 0;
    if (!ok) { std::printf("005a30a1_TEST FAIL\n"); return 1; }
    std::printf("005a30a1_TEST PASS\n");
    return 0;
}
