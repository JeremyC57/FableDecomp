#include <cstdio>
// Behaviour test for CTCInventoryAbilities::GetNumAssignedReceivedExpressions (005ee24b): same logic over a copy of the model.
struct CLongVector {
    long* first; long* last; long* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    const long& operator[](unsigned long n) const { return first[n]; }
};
struct C { CLongVector ReceivedExpressionOrder; long GetNumAssignedReceivedExpressions() const; };
long C::GetNumAssignedReceivedExpressions() const
{
    long n = 0;
    for (unsigned long i = 0; i < ReceivedExpressionOrder.size(); ++i) {
        if (ReceivedExpressionOrder[i] > 0)
            ++n;
    }
    return n;
}
int main()
{
    long data[6] = { 0, 3, -2, 0, 7, 1 };
    C c; c.ReceivedExpressionOrder.first = data; c.ReceivedExpressionOrder.last = data + 6; c.ReceivedExpressionOrder.end_of_storage = data + 6;
    C e; e.ReceivedExpressionOrder.first = data; e.ReceivedExpressionOrder.last = data; e.ReceivedExpressionOrder.end_of_storage = data;
    if (c.GetNumAssignedReceivedExpressions() != 3 || e.GetNumAssignedReceivedExpressions() != 0) { std::printf("005ee24b_TEST FAIL\n"); return 1; }
    std::printf("005ee24b_TEST PASS\n");
    return 0;
}
