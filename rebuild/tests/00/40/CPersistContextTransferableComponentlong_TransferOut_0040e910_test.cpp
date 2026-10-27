// behaviour test for 0x0040E910 CPersistContext_TransferableComponent<long>::TransferOut (model: the object's value reaches the writer)
#include <cstdio>
static int g_written = 0;
static void Write(int* v) { g_written = *v; }
struct Component { int* Object; void TransferOut() { int text = (int)*Object; Write(&text); } };
int main()
{
    int value = 1234; Component c; c.Object = &value;
    c.TransferOut();
    int ok = g_written == 1234;
    if (!ok) { std::printf("0040e910_TEST FAIL\n"); return 1; }
    std::printf("0040e910_TEST PASS\n");
    return 0;
}