// Test for 0x00485AA7 std::vector<NUISystem::CComponent*>::erase(iterator, iterator) on stlport/fable_containers.h.
#include "rebuild_abi.h"
#define FABLE_RETAIL_STLP_COPY_TRIVIAL_OUTOFLINE
#include "stlport/fable_containers.h"
#include <stdio.h>
#include <string.h>
namespace NUISystem { class CComponent; }
namespace std {
void* FABLE_FASTCALL __copy_trivial(const void* first, const void* last, void* result)
{
    size_t n = (const char*)last - (const char*)first;
    memmove(result, first, n);
    return (char*)result + n;
}
}

int main()
{
    std::vector<NUISystem::CComponent*> v;
    for (int i = 1; i <= 5; ++i)
        v.push_back((NUISystem::CComponent*)(size_t)i);
    std::vector<NUISystem::CComponent*>::iterator r = v.erase(v.begin() + 1, v.begin() + 3);
    bool ok = r == v.begin() + 1 && v.size() == 3 && (size_t)v[0] == 1 && (size_t)v[1] == 4 && (size_t)v[2] == 5;
    if (ok)
        printf("LISTBOX_SELECT_OK 00485aa7 erase PASS\n");   // PassPattern kept from the July test (build_candidates.ps1)
    else
        printf("00485aa7_TEST FAIL size=%u\n", (unsigned)v.size());
    return ok ? 0 : 1;
}
