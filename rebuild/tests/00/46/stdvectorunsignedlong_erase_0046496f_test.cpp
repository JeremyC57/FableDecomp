// Behaviour test for 0x0046496F std::vector<unsigned long>::erase(first, last)
#include <cstdio>
#include <cstring>
struct ULongVector {
    unsigned int* start; unsigned int* finish; unsigned int* eos;
    unsigned int* erase(unsigned int* first, unsigned int* last)
    {
        unsigned int* i = (last == finish) ? first
            : (unsigned int*)((char*)std::memmove(first, last, (char*)finish - (char*)last) + ((char*)finish - (char*)last));
        finish = i;
        return first;
    }
};
int main()
{
    unsigned int data[6] = { 1, 2, 3, 4, 5, 6 };
    ULongVector v; v.start = data; v.finish = data + 6; v.eos = data + 6;
    unsigned int* r = v.erase(data + 1, data + 3);           // remove 2, 3
    if (r != data + 1 || v.finish != data + 4 || data[0] != 1 || data[1] != 4 || data[2] != 5 || data[3] != 6) { std::printf("0046496f_TEST FAIL\n"); return 1; }
    r = v.erase(data + 2, v.finish);                         // erase to the end: no move
    if (r != data + 2 || v.finish != data + 2) { std::printf("0046496f_TEST FAIL\n"); return 1; }
    r = v.erase(v.start, v.finish);                          // clear
    if (v.finish != v.start) { std::printf("0046496f_TEST FAIL\n"); return 1; }
    std::printf("0046496f_TEST PASS\n");
    return 0;
}