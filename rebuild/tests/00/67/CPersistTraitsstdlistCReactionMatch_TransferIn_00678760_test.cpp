// Behaviour test for 0x00678760 CPersistTraits<std::list<CReactionMatch*> >::TransferIn: ".Add(type, Shape(values))".
// Model of the row's decisions over a token stream: a missing ".Add" reports the error; "(" / "," / "(" gate the shape; the shape
// name picks the class and its value count (Sphere: 5 opinions + radius, MRBlob: 7 floats, Pyramid: axis, 3 floats, axis, 3
// floats, Secondary / SecondaryCentred: axis + 6 floats); a missing "," stops without appending; Sphere / Pyramid / Secondary* need
// the closing ")" before they append, MRBlob appends first and checks ")" after; an unknown name reports an error.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Parser { const char** tok; int pos; const char* Next() { const char* t = tok[pos]; if (t) ++pos; return t ? t : ""; } };
struct Match { char kind[20]; int type; float v[9]; int n; };
static Match g_list[8]; static int g_count; static int g_errors;

static bool Sym(Parser& p, const char* s) { return strcmp(p.Next(), s) == 0; }
static float Num(Parser& p) { return (float)atof(p.Next()); }
static void Push(const char* kind, int type, const float* v, int n)
{
    Match& m = g_list[g_count++]; strcpy(m.kind, kind); m.type = type; m.n = n;
    for (int i = 0; i < n; ++i) m.v[i] = v[i];
}

static void TransferIn(Parser& p)
{
    if (!Sym(p, ".Add")) { ++g_errors; return; }
    if (!Sym(p, "(")) return;
    int type = atoi(p.Next());
    if (!Sym(p, ",")) return;
    char name[32]; strcpy(name, p.Next());
    if (!Sym(p, "(")) return;
    float v[9];
    if (strcmp(name, "Sphere") == 0) {
        for (unsigned long i = 0; i < 5; ++i) { v[i] = Num(p); if (!Sym(p, ",")) return; }
        v[5] = Num(p);
        if (!Sym(p, ")")) return;
        Push("Sphere", type, v, 6);
    }
    else if (strcmp(name, "MRBlob") == 0) {
        for (int i = 0; i < 6; ++i) { v[i] = Num(p); if (!Sym(p, ",")) return; }
        v[6] = Num(p);
        Push("MRBlob", type, v, 7);
        if (!Sym(p, ")")) return;
    }
    else if (strcmp(name, "Pyramid") == 0) {
        for (int i = 0; i < 7; ++i) { v[i] = Num(p); if (!Sym(p, ",")) return; }
        v[7] = Num(p);
        if (!Sym(p, ")")) return;
        Push("Pyramid", type, v, 8);
    }
    else if (strcmp(name, "Secondary") == 0 || strcmp(name, "SecondaryCentred") == 0) {
        v[0] = Num(p);
        for (unsigned long i = 1; i < 7; ++i) { if (!Sym(p, ",")) return; v[i] = Num(p); }
        if (!Sym(p, ")")) return;
        Push(name, type, v, 7);
    }
    else { ++g_errors; return; }
    Sym(p, ")");
}

static void Run(const char** t) { Parser p; p.tok = t; p.pos = 0; TransferIn(p); }

int main()
{
    int ok = 1;
    const char* a[] = { ".Add", "(", "3", ",", "Sphere", "(", "1", ",", "2", ",", "3", ",", "4", ",", "5", ",", "9", ")", ")", 0 };
    Run(a); ok &= g_count == 1 && strcmp(g_list[0].kind, "Sphere") == 0 && g_list[0].type == 3 && g_list[0].v[4] == 5.0f && g_list[0].v[5] == 9.0f;
    const char* b[] = { ".Add", "(", "2", ",", "Sphere", "(", "1", ",", "2", ")", 0 };            // short: no append
    Run(b); ok &= g_count == 1 && g_errors == 0;
    const char* c[] = { ".Add", "(", "4", ",", "MRBlob", "(", "1", ",", "2", ",", "3", ",", "4", ",", "5", ",", "6", ",", "7", "]", 0 };
    Run(c); ok &= g_count == 2 && strcmp(g_list[1].kind, "MRBlob") == 0 && g_list[1].v[6] == 7.0f;  // appended before ")" check
    const char* d[] = { ".Add", "(", "5", ",", "Pyramid", "(", "0", ",", "1", ",", "2", ",", "3", ",", "1", ",", "4", ",", "5", ",", "6", "]", 0 };
    Run(d); ok &= g_count == 2;                                                                         // ")" missing: no append
    const char* e[] = { ".Add", "(", "6", ",", "SecondaryCentred", "(", "2", ",", "1", ",", "2", ",", "3", ",", "4", ",", "5", ",", "6", ")", ")", 0 };
    Run(e); ok &= g_count == 3 && strcmp(g_list[2].kind, "SecondaryCentred") == 0 && g_list[2].v[0] == 2.0f && g_list[2].v[6] == 6.0f;
    const char* f[] = { ".Add", "(", "1", ",", "Cube", "(", "1", ")", 0 };
    Run(f); ok &= g_count == 3 && g_errors == 1;
    const char* g[] = { "Add", "(", 0 };
    Run(g); ok &= g_count == 3 && g_errors == 2;
    printf(ok ? "00678760_TEST PASS\n" : "00678760_TEST FAIL\n");
    return ok ? 0 : 1;
}