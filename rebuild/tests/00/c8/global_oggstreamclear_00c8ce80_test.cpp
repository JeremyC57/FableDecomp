// Behaviour test for 0x00C8CE80 ogg_stream_clear (libogg): copy of the model and the logic.
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
int ogg_stream_clear(ogg_stream_state* os)
{
    if (os) {
        if (os->body_data) free(os->body_data);
        if (os->lacing_vals) free(os->lacing_vals);
        if (os->granule_vals) free(os->granule_vals);
        memset(os, 0, sizeof(*os));
    }
    return (0);
}
int main()
{
    ogg_stream_state os; memset(&os, 0x11, sizeof(os));
    os.body_data = (unsigned char*)malloc(16); os.lacing_vals = 0; os.granule_vals = (ogg_int64_t*)malloc(16);
    int ok = ogg_stream_clear(&os) == 0;
    ok &= os.body_data == 0 && os.granule_vals == 0 && os.serialno == 0 && os.header[281] == 0 && os.granulepos == 0;
    ok &= ogg_stream_clear(0) == 0;
    if (!ok) { std::printf("00c8ce80_TEST FAIL\n"); return 1; }
    std::printf("00c8ce80_TEST PASS\n");
    return 0;
}