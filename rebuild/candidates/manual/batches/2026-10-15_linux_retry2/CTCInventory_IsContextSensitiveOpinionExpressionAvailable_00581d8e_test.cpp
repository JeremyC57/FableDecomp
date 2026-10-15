#include <cstdio>
// Behaviour of CTCInventory::IsContextSensitiveOpinionExpressionAvailable (0x00581D8E): true when any slot holding an item (> 0)
// maps to the opinion-expression type (6); empty slots are not looked up.
enum EContextSensitiveType { CONTEXT_SENSITIVE_TYPE_OPINION_EXPRESSION = 6 };
static int g_lookups = 0;
static EContextSensitiveType GetContextSensitiveType(long itemType) { ++g_lookups; return itemType == 42 ? CONTEXT_SENSITIVE_TYPE_OPINION_EXPRESSION : (EContextSensitiveType)1; }
struct CLongVector {
    long* first; long* last; long* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    long* begin() const { return first; }
    long& operator[](unsigned long n) { return *(begin() + n); }
};
struct CTCInventory {
    CLongVector ContextSensitiveItems;
    bool IsContextSensitiveOpinionExpressionAvailable()
    {
        for (unsigned long i = 0; i < ContextSensitiveItems.size(); ++i) {
            if (ContextSensitiveItems[i] > 0 &&
                GetContextSensitiveType(ContextSensitiveItems[i]) == CONTEXT_SENSITIVE_TYPE_OPINION_EXPRESSION)
                return true;
        }
        return false;
    }
};
int main()
{
    long a[3] = { 0, 5, -2 }; long b[3] = { 0, 5, 42 };
    CTCInventory inv; inv.ContextSensitiveItems.first = a; inv.ContextSensitiveItems.last = a + 3; inv.ContextSensitiveItems.end_of_storage = a + 3;
    if (inv.IsContextSensitiveOpinionExpressionAvailable() || g_lookups != 1) { std::printf("00581d8e_TEST FAIL\n"); return 1; }
    inv.ContextSensitiveItems.first = b; inv.ContextSensitiveItems.last = b + 3;
    if (!inv.IsContextSensitiveOpinionExpressionAvailable() || g_lookups != 3) { std::printf("00581d8e_TEST FAIL\n"); return 1; }
    inv.ContextSensitiveItems.last = b;
    if (inv.IsContextSensitiveOpinionExpressionAvailable()) { std::printf("00581d8e_TEST FAIL\n"); return 1; }
    std::printf("00581d8e_TEST PASS\n"); return 0;
}
