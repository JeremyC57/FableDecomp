// Behaviour test for CTCVillage::HasCrimeBeenReported (0x0065914B): find(crime) != end() over a stand-in set.
#include <stdio.h>
struct Node { long key; };
struct Tree {
    Node* header; unsigned int count; Node* nodes; int n;
    Node* find_node(const long& k) const { for (int i = 0; i < n; ++i) if (nodes[i].key == k) return &nodes[i]; return header; }
};
class CTCVillage {
public:
    unsigned char _pad_0x00[0x64];
    Tree PlayersCrimes;
    bool HasCrimeBeenReported(long crime) { return PlayersCrimes.find_node(crime) != PlayersCrimes.header; }
};
int main()
{
    static Node header = { 0 };
    static Node crimes[3] = { { 4 }, { 9 }, { 15 } };
    static CTCVillage v;
    v.PlayersCrimes.header = &header; v.PlayersCrimes.count = 3; v.PlayersCrimes.nodes = crimes; v.PlayersCrimes.n = 3;
    int ok = v.HasCrimeBeenReported(9) && v.HasCrimeBeenReported(4) && !v.HasCrimeBeenReported(5) && !v.HasCrimeBeenReported(0);
    printf(ok ? "0065914b_TEST PASS\n" : "0065914b_TEST FAIL\n");
    return ok ? 0 : 1;
}
