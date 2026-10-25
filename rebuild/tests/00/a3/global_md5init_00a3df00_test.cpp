// Behaviour test for md5_init (0x00A3DF00): the RFC 1321 initial state.
#include <stdio.h>
#include <string.h>
typedef struct md5_state_s { unsigned int count[2]; unsigned int abcd[4]; unsigned char buf[64]; } md5_state_t;
extern "C" void __fastcall md5_init(md5_state_t* pms);
int main()
{
    md5_state_t s;
    memset(&s, 0xCC, sizeof(s));
    md5_init(&s);
    int ok = s.count[0] == 0 && s.count[1] == 0 && s.abcd[0] == 0x67452301u && s.abcd[1] == 0xEFCDAB89u &&
             s.abcd[2] == 0x98BADCFEu && s.abcd[3] == 0x10325476u && s.buf[0] == 0xCC;
    printf(ok ? "00a3df00_TEST PASS\n" : "00a3df00_TEST FAIL\n");
    return ok ? 0 : 1;
}
