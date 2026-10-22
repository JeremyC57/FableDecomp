#include <cstdio>

struct Sub { int called; const char* arg; };
static void Sub_init(Sub* self, const char* name) { self->called = 1; self->arg = name; }

struct CMemoryFile {
    char pad0[8];
    int  Position;
    int  Length;
    char pad10[0x0c];
    char field1C;
    char pad1D[3];
    Sub  sub;
};

static void Open(CMemoryFile* self)
{
    Sub_init(&self->sub, (const char*)0x122d70c);
    self->Position = 0;
    self->Length = 0;
    self->field1C = 1;
}

int main() {
    CMemoryFile f;
    f.Position = 99; f.Length = 99; f.field1C = 0; f.sub.called = 0;
    Open(&f);
    if (f.sub.called == 1 && f.sub.arg == (const char*)0x122d70c &&
        f.Position == 0 && f.Length == 0 && f.field1C == 1) {
        printf("PARITY_OK\n");
    } else {
        printf("FAIL\n");
    }
    return 0;
}