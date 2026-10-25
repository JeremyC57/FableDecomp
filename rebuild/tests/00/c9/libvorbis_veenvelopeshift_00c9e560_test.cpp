// Behaviour test for 0x00C9E560 libvorbis _ve_envelope_shift (copy of the model + logic).
#include <cstdio>
#include <cstring>
struct envelope_part { int searchstep; int* mark; long storage; long current; long curmark; long cursor; };
static void envelope_shift(envelope_part* e, long shift)
{
    int smallsize = e->current / e->searchstep + 2;
    int smallshift = shift / e->searchstep;
    std::memmove(e->mark, e->mark + smallshift, (smallsize - smallshift) * sizeof(*e->mark));
    e->current -= shift;
    if (e->curmark >= 0) e->curmark -= shift;
    e->cursor -= shift;
}
int main()
{
    int marks[16], i;
    for (i = 0; i < 16; i++) marks[i] = 100 + i;
    envelope_part e = { 64, marks, 16, 640, 600, 700 };     /* smallsize 12, shift 256 -> smallshift 4 */
    envelope_shift(&e, 256);
    int ok = marks[0] == 104 && marks[7] == 111 && marks[8] == 108 &&   /* 8 words moved, the rest untouched */
             e.current == 384 && e.curmark == 344 && e.cursor == 444;
    e.curmark = -1;
    envelope_shift(&e, 0);
    if (e.curmark != -1 || e.current != 384) ok = 0;         /* a negative curmark is left alone */
    if (!ok) { std::printf("00c9e560_TEST FAIL\n"); return 1; }
    std::printf("00c9e560_TEST PASS\n");
    return 0;
}