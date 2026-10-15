#include <cstdio>
// Behaviour test for CTCInventoryAbilities::GetPrioritisedExpressionDefs (0x005EED8C): same logic over a copy of the model.
struct CLongVector {
    long* first; long* last; long* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    long& operator[](unsigned long n) { return first[n]; }
    void push_back(const long& value) { *last++ = value; }
};
struct CTCInventoryAbilities_Retail {
    CLongVector ReceivedExpressionOrder;
    void GetPrioritisedExpressionDefs(CLongVector* defs)
    {
        for (unsigned long i = 0; i < ReceivedExpressionOrder.size(); ++i)
            defs->push_back(ReceivedExpressionOrder[i]);
    }
};
int main()
{
    long order[3] = { 40, 10, 30 };
    long out[8] = { 99 };
    CTCInventoryAbilities_Retail a;
    a.ReceivedExpressionOrder.first = order; a.ReceivedExpressionOrder.last = order + 3; a.ReceivedExpressionOrder.end_of_storage = order + 3;
    CLongVector v; v.first = out; v.last = out + 1; v.end_of_storage = out + 8;
    a.GetPrioritisedExpressionDefs(&v);
    if (v.size() != 4 || out[0] != 99 || out[1] != 40 || out[2] != 10 || out[3] != 30) { std::printf("005eed8c_TEST FAIL\n"); return 1; }
    std::printf("005eed8c_TEST PASS\n");
    return 0;
}
