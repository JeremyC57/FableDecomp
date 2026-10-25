// Behaviour test for STLport map<long, CShopItem>::operator[] (0x00778280): lower_bound, insert (k, T()) when
// the key is absent, return a reference to the mapped value; existing keys keep their value, order is by key.
#include <cstdio>
#include <cstring>

typedef long K;
inline bool Less(const K& a, const K& b) { return a < b; }
struct T {                                          // CShopItem defaults
    float Quantity, PriceMultiplier, MaxStock; long RestockPeriod; float RestockPeriodRandomness, SalesPerDay, SalesPerDayRandomness;
    T() : Quantity(0.0f), PriceMultiplier(1.0f), MaxStock(0.0f), RestockPeriod(1), RestockPeriodRandomness(0.0f),
          SalesPerDay(0.0f), SalesPerDayRandomness(0.0f) {}
};

struct Node { Node* next; K first; T second; };

struct Map {
    Node* head;                                     // sorted singly linked list stands in for the tree
    unsigned int count;
    Map() : head(0), count(0) {}
    ~Map() { while (head) { Node* n = head->next; delete head; head = n; } }
    Node** lower_bound(const K& k)
    {
        Node** p = &head;
        while (*p && Less((*p)->first, k)) p = &(*p)->next;
        return p;
    }
    T& operator[](const K& k)                       // STLport shape
    {
        Node** i = lower_bound(k);
        if (*i == 0 || Less(k, (*i)->first)) {
            Node* n = new Node;
            n->first = k; n->second = T(); n->next = *i; *i = n; ++count;
        }
        return (*i)->second;
    }
};

#define CHECK(c) if (!(c)) { std::printf("00778280_TEST FAIL\n"); return 1; }

int main()
{
    Map m;
    T& a = m[42];
    CHECK(a.Quantity == 0.0f && a.PriceMultiplier > 0.999f && a.PriceMultiplier < 1.001f && a.RestockPeriod == 1);
    a.Quantity = 5.0f;
    m[3].MaxStock = 9.0f;
    CHECK(m[42].Quantity > 4.999f && m[42].Quantity < 5.001f && m.count == 2 && m.head->first == 3);
    std::printf("00778280_TEST PASS\n");
    return 0;
}