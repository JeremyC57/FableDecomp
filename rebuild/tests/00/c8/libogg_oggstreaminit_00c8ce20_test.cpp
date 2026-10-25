// Behaviour test for 0x00C8CE20 ogg_stream_init (libogg): copy of the model and the logic.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifdef _MSC_VER
typedef __int64 ogg_int64_t;
#else
typedef long long ogg_int64_t;
#endif
struct ogg_stream_state {
    unsigned char* body_data; long body_storage; long body_fill; long body_returned;
    int* lacing_vals; ogg_int64_t* granule_vals; long lacing_storage; long lacing_fill; long lacing_packet; long lacing_returned;
    unsigned char header[282]; int header_fill; int e_o_s; int b_o_s; long serialno; long pageno;
    ogg_int64_t packetno; ogg_int64_t granulepos;
};
int ogg_stream_init(ogg_stream_state* os, int serialno)
{
    if (os) {
        memset(os, 0, sizeof(*os));
        os->body_storage = 16 * 1024;
        os->body_data = (unsigned char*)malloc(os->body_storage * sizeof(*os->body_data));
        os->lacing_storage = 1024;
        os->lacing_vals = (int*)malloc(os->lacing_storage * sizeof(*os->lacing_vals));
        os->granule_vals = (ogg_int64_t*)malloc(os->lacing_storage * sizeof(*os->granule_vals));
        os->serialno = serialno;
        return (0);
    }
    return (-1);
}
int main()
{
    ogg_stream_state* os = (ogg_stream_state*)malloc(sizeof(ogg_stream_state));
    memset(os, 0x5a, sizeof(*os));
    int ok = ogg_stream_init(os, 1234) == 0;
    ok &= os->body_storage == 16384 && os->body_data != 0 && os->lacing_storage == 1024;
    ok &= os->lacing_vals != 0 && os->granule_vals != 0 && os->serialno == 1234;
    ok &= os->body_fill == 0 && os->lacing_fill == 0 && os->pageno == 0 && os->packetno == 0 && os->header[100] == 0;
    os->granule_vals[1023] = 5; os->lacing_vals[1023] = 6; os->body_data[16383] = 7;
    ok &= ogg_stream_init(0, 1) == -1;
    free(os->body_data); free(os->lacing_vals); free(os->granule_vals); free(os);
    if (!ok) { std::printf("00c8ce20_TEST FAIL\n"); return 1; }
    std::printf("00c8ce20_TEST PASS\n");
    return 0;
}