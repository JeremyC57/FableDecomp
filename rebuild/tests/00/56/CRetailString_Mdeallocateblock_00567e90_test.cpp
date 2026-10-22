// Behaviour test for 00567e90 CRetailString::_M_deallocate_block (free the buffer when present)
#include <cstdio>
#include <cstdlib>
struct CRetailString {
    char* _M_start; char* _M_finish; char* _M_end_of_storage;
    void _M_deallocate_block();
};
void CRetailString::_M_deallocate_block()
{
    if (_M_start)
        std::free(_M_start);
}
int main()
{
    CRetailString s;
    s._M_start = (char*)std::malloc(16); s._M_finish = s._M_start; s._M_end_of_storage = s._M_start + 16;
    s._M_deallocate_block();
    s._M_start = 0;
    s._M_deallocate_block();   // null: no free
    std::printf("00567e90_TEST PASS\n");
    return 0;
}