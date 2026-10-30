// Behaviour test for libogg framing.c _os_body_expand (0x00C8CEF0): grow body storage by needed + 1024 when full.
#include <cstdio>
#include <cstdlib>
struct os_t { unsigned char* body_data; long body_storage, body_fill; };
static void _os_body_expand(os_t* os, int needed)
{
    if (os->body_storage <= os->body_fill + needed) {
        os->body_storage += (needed + 1024);
        os->body_data = (unsigned char*)std::realloc(os->body_data, os->body_storage * sizeof(*os->body_data));
    }
}
int main()
{
    os_t os = { (unsigned char*)std::malloc(16), 16, 10 };
    _os_body_expand(&os, 4);
    if (os.body_storage != 16) { std::printf("00c8cef0_TEST FAIL\n"); return 1; }
    _os_body_expand(&os, 6);
    if (os.body_storage != 16 + 6 + 1024 || !os.body_data) { std::printf("00c8cef0_TEST FAIL\n"); return 1; }
    os.body_data[1045] = 1; std::free(os.body_data);
    std::printf("00c8cef0_TEST PASS\n"); return 0;
}