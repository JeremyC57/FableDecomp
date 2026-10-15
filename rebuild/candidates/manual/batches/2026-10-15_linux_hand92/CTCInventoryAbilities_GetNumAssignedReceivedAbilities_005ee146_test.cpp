#include <cstdio>
// Behaviour test for CTCInventoryAbilities::GetNumAssignedReceivedAbilities (005ee146): same logic over a copy of the model.
struct CLongVector {
    long* first; long* last; long* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    const long& operator[](unsigned long n) const { return first[n]; }
};
struct C { CLongVector ReceivedAbilityOrder; long GetNumAssignedReceivedAbilities() const; };
long C::GetNumAssignedReceivedAbilities() const
{
    long n = 0;
    for (unsigned long i = 0; i < ReceivedAbilityOrder.size(); ++i) {
        if (ReceivedAbilityOrder[i] != 0)
            ++n;
    }
    return n;
}
int main()
{
    long data[6] = { 0, 3, -2, 0, 7, 1 };
    C c; c.ReceivedAbilityOrder.first = data; c.ReceivedAbilityOrder.last = data + 6; c.ReceivedAbilityOrder.end_of_storage = data + 6;
    C e; e.ReceivedAbilityOrder.first = data; e.ReceivedAbilityOrder.last = data; e.ReceivedAbilityOrder.end_of_storage = data;
    if (c.GetNumAssignedReceivedAbilities() != 4 || e.GetNumAssignedReceivedAbilities() != 0) { std::printf("005ee146_TEST FAIL\n"); return 1; }
    std::printf("005ee146_TEST PASS\n");
    return 0;
}
