/* ws.c -- fast model of c2.dll 13.10.3077's big-frame layout (K&R qsort by density w*1000/size, top-down offsets from
   0x68) + greedy search for slot edits (weight deltas, deletions, insertions) that reproduce retail's offsets.
   input (stdin): n nparams, then n lines "id size w align target" (target 999999 = unknown).
   argv[1] = rounds; argv[2] = mode bits (1 weights, 2 delete, 4 insert). Prints each accepted edit. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct { int id, size, w, align, target; } Slot;
static int n, np;
static Slot base[4096], work[4096];
static int dens(const Slot *s) { return (s->w * 1000) / s->size; }
static void sw(Slot *v, int i, int j) { Slot t = v[i]; v[i] = v[j]; v[j] = t; }
static void qs(Slot *v, int left, int right) {
    while (left < right) {
        int mid = (left + right) / 2, last = left, i;
        sw(v, left, mid);
        for (i = left + 1; i <= right; i++) {
            int ca = dens(&v[left]), cb = dens(&v[i]);
            if (ca != cb && !(ca >= cb)) { last++; sw(v, last, i); }
        }
        sw(v, left, last);
        qs(v, left, last - 1);
        left = last + 1;
    }
}
static int score(Slot *a, int cnt) {
    int i, top = 0x68, sc = 0;
    memcpy(work, a, sizeof(Slot) * cnt);
    qs(work, np, cnt - 1);
    for (i = np; i < cnt; i++) {
        int al = work[i].align < 1 ? 1 : (work[i].align > 4 ? 4 : work[i].align), x = top - work[i].size;
        x -= ((x % al) + al) % al;
        if (work[i].target == x) sc++;
        top = x;
    }
    return sc;
}
int main(int argc, char **argv) {
    int rounds = argc > 1 ? atoi(argv[1]) : 10, mode = argc > 2 ? atoi(argv[2]) : 7, i, r, d;
    if (scanf("%d %d", &n, &np) != 2) return 1;
    for (i = 0; i < n; i++) scanf("%d %d %d %d %d", &base[i].id, &base[i].size, &base[i].w, &base[i].align, &base[i].target);
    int best = score(base, n);
    printf("base %d\n", best); fflush(stdout);
    static const int isz[] = {1, 4, 8, 12, 16}, iw[] = {1, 2, 3, 4};
    for (r = 0; r < rounds; r++) {
        int gb = best, kind = 0, a1 = 0, a2 = 0, a3 = 0;
        if (mode & 1) for (i = np; i < n; i++) for (d = -3; d <= 3; d++) {
            if (!d || base[i].w + d < 0) continue;
            base[i].w += d; int sc = score(base, n); base[i].w -= d;
            if (sc > gb) { gb = sc; kind = 1; a1 = i; a2 = d; }
        }
        if (mode & 2) for (i = np; i < n; i++) {
            Slot t = base[i]; memmove(&base[i], &base[i + 1], sizeof(Slot) * (n - i - 1));
            int sc = score(base, n - 1);
            memmove(&base[i + 1], &base[i], sizeof(Slot) * (n - i - 1)); base[i] = t;
            if (sc > gb) { gb = sc; kind = 2; a1 = i; }
        }
        if (mode & 4) for (i = np; i <= n; i++) for (int s = 0; s < 5; s++) for (int w = 0; w < 4; w++) {
            memmove(&base[i + 1], &base[i], sizeof(Slot) * (n - i));
            base[i].id = -1; base[i].size = isz[s]; base[i].w = iw[w]; base[i].align = isz[s] >= 4 ? 4 : 1; base[i].target = 999999;
            int sc = score(base, n + 1);
            memmove(&base[i], &base[i + 1], sizeof(Slot) * (n - i));
            if (sc > gb) { gb = sc; kind = 3; a1 = i; a2 = isz[s]; a3 = iw[w]; }
        }
        if (!kind) break;
        best = gb;
        if (kind == 1) { printf("round %d: weight slot#%d (pos %d, size %d) %d -> %d : %d\n", r, base[a1].id, a1, base[a1].size, base[a1].w, base[a1].w + a2, best); base[a1].w += a2; }
        if (kind == 2) { printf("round %d: delete slot#%d (pos %d, size %d, w %d) : %d\n", r, base[a1].id, a1, base[a1].size, base[a1].w, best);
            memmove(&base[a1], &base[a1 + 1], sizeof(Slot) * (n - a1 - 1)); n--; }
        if (kind == 3) { printf("round %d: insert size %d w %d before pos %d (slot#%d) : %d\n", r, a2, a3, a1, a1 < n ? base[a1].id : -1, best);
            memmove(&base[a1 + 1], &base[a1], sizeof(Slot) * (n - a1)); base[a1].id = -1; base[a1].size = a2; base[a1].w = a3; base[a1].align = a2 >= 4 ? 4 : 1; base[a1].target = 999999; n++; }
        fflush(stdout);
    }
    return 0;
}
