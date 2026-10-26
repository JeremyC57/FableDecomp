#include <cstdio>

struct Sub { int calls; int lastArg; int lastFlag; };

static void notify(Sub* s, int arg, unsigned char flag) {
    s->calls++;
    s->lastArg = arg;
    s->lastFlag = flag;
}

class CTCVillage { public:
    Sub*          field0;
    int           ParentThing;
    char          pad8[4];
    unsigned char byte_0c;
    unsigned char byte_0d;
};

static void OnInitialActivate(CTCVillage* self) {
    if (self->byte_0d) {
        notify(self->field0, self->ParentThing, self->byte_0c);
        self->byte_0d = 0;
    }
}

int main() {
    Sub sink = {0,0,0};
    CTCVillage v;
    v.field0 = &sink;
    v.ParentThing = 42;
    v.byte_0c = 7;

    // case active
    v.byte_0d = 1;
    OnInitialActivate(&v);
    bool ok = (sink.calls == 1 && sink.lastArg == 42 && sink.lastFlag == 7 && v.byte_0d == 0);

    // case inactive: no call, byte stays 0
    OnInitialActivate(&v);
    ok = ok && (sink.calls == 1 && v.byte_0d == 0);

    // re-arm and fire again
    v.byte_0d = 1;
    v.byte_0c = 255;
    OnInitialActivate(&v);
    ok = ok && (sink.calls == 2 && sink.lastFlag == 255 && v.byte_0d == 0);

    if (ok) printf("BEHAVIOR_PASS_cbe010\n");
    else    printf("BEHAVIOR_FAIL\n");
    return 0;
}