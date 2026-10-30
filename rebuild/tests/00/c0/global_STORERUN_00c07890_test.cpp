// Behaviour test for LZO 1.08 lzo1x_9x.c STORE_RUN (0x00C07890): emit a literal-run length code then copy t literals.
#include <cstdio>
#include <cstring>
typedef unsigned char lzo_byte; typedef unsigned int lzo_uint;
struct ctx { lzo_byte* out; unsigned long lit_bytes, lit1_r, lit2_r, lit3_r; };
static lzo_byte* STORE_RUN(ctx* c, lzo_byte* op, const lzo_byte* ii, lzo_uint t)
{
    c->lit_bytes += t;
    if (op == c->out && t <= 238) *op++ = (lzo_byte)(17 + t);
    else if (t <= 3) { op[-2] |= (lzo_byte)t; c->lit1_r++; }
    else if (t <= 18) { *op++ = (lzo_byte)(t - 3); c->lit2_r++; }
    else { lzo_uint tt = t - 18; *op++ = 0; while (tt > 255) { tt -= 255; *op++ = 0; } *op++ = (lzo_byte)tt; c->lit3_r++; }
    do *op++ = *ii++; while (--t > 0);
    return op;
}
int main()
{
    static lzo_byte out[1024], in[600];
    for (int i = 0; i < 600; ++i) in[i] = (lzo_byte)(i + 1);
    ctx c = { out, 0, 0, 0, 0 };
    lzo_byte* op = STORE_RUN(&c, out, in, 5);
    if (op != out + 6 || out[0] != 22 || out[5] != 5) { std::printf("00c07890_TEST FAIL\n"); return 1; }
    op[0] = 0x40; op[1] = 0; op += 2;
    lzo_byte* op2 = STORE_RUN(&c, op, in, 2);
    if (op2 != op + 2 || out[6] != 0x42 || c.lit1_r != 1) { std::printf("00c07890_TEST FAIL\n"); return 1; }
    lzo_byte* op3 = STORE_RUN(&c, op2, in, 10);
    if (op3 != op2 + 11 || op2[0] != 7 || c.lit2_r != 1) { std::printf("00c07890_TEST FAIL\n"); return 1; }
    lzo_byte* op4 = STORE_RUN(&c, op3, in, 18 + 300);
    if (op4 != op3 + 3 + 318 || op3[0] != 0 || op3[1] != 0 || op3[2] != 45 || c.lit3_r != 1 || c.lit_bytes != 335)
    { std::printf("00c07890_TEST FAIL\n"); return 1; }
    std::printf("00c07890_TEST PASS\n"); return 0;
}