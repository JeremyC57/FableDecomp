// Behaviour test for 0x00411b00 (NF232, q12631) on the shared headers: std::vector<CDateAndTime>::erase(first, last), the row's explicit
// instantiation over stlport/fable_containers.h (FABLE_RETAIL_STLP_COPY_NOINLINE: the random-access __copy body, out of line) and
// engine/CDateAndTime.h. The vector is a never-copied test subclass of the shared std::vector that adopts a static 28-byte element array
// (and drops it before destruction). Cases: a middle range (tail copied down, finish shrunk, first returned), an empty range (no change),
// the tail range (no copy). Replaces the HEAD host model of the old vector_push_back_copy stand-in; prints the row's PassPattern.
#include <stdio.h>
#include <string.h>
#define FABLE_RETAIL_STLP_COPY_NOINLINE
#include "stlport/fable_containers.h"
#include "engine/CDateAndTime.h"
template std::vector<CDateAndTime>::iterator std::vector<CDateAndTime>::erase(std::vector<CDateAndTime>::iterator, std::vector<CDateAndTime>::iterator);

struct Dates : std::vector<CDateAndTime> {
    void Adopt(CDateAndTime* first, CDateAndTime* last) { this->_M_start = first; this->_M_finish = last; this->_M_end_of_storage._M_data = last; }
    void Drop() { this->_M_start = this->_M_finish = this->_M_end_of_storage._M_data = 0; }
    CDateAndTime* Start() const { return this->_M_start; }
    CDateAndTime* Finish() const { return this->_M_finish; }
};
static CDateAndTime g_items[5];
static void Fill() { for (int i = 0; i < 5; ++i) { long* w = (long*)&g_items[i]; for (int k = 0; k < 7; ++k) w[k] = (i + 1) * 100 + k; } }
static long Tag(int i) { return ((long*)&g_items[i])[0]; }
static bool Whole(int i, int from) { const long* w = (const long*)&g_items[i]; for (int k = 0; k < 7; ++k) if (w[k] != (from + 1) * 100 + k) return false; return true; }
static int g_fails = 0;
static void Check(bool ok, const char* what) { if (!ok) { printf("FAIL: %s\n", what); ++g_fails; } }

int main()
{
    Check(sizeof(CDateAndTime) == 28, "28-byte element");
    Dates v;
    Fill(); v.Adopt(g_items, g_items + 5);
    CDateAndTime* r = v.erase(g_items + 1, g_items + 3);
    Check(r == g_items + 1 && v.Finish() == g_items + 3 && v.Start() == g_items, "middle: first returned, finish shrunk by 2");
    Check(Whole(0, 0) && Whole(1, 3) && Whole(2, 4) && Tag(3) == 400 && Tag(4) == 500, "middle: [3,5) copied down whole, the old tail left");
    Fill(); v.Adopt(g_items, g_items + 5);
    r = v.erase(g_items + 2, g_items + 2);
    Check(r == g_items + 2 && v.Finish() == g_items + 5 && Whole(2, 2) && Whole(4, 4), "empty range: no change");
    Fill(); v.Adopt(g_items, g_items + 5);
    r = v.erase(g_items + 2, g_items + 5);
    Check(r == g_items + 2 && v.Finish() == g_items + 2 && Whole(2, 2), "tail: finish = first, nothing copied");
    v.Drop();
    printf(g_fails ? "00411b00 FAIL\n" : "VECTOR_PUSH_COPY_PASS\n");
    return g_fails ? 1 : 0;
}
