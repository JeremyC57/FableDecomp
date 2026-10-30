#include <cstdio>

struct CAudioSound;
struct CVtable_012B5C10 {
    virtual ~CVtable_012B5C10();
    virtual long Initialise(CAudioSound* sound);
};
struct CVtable_012B5C60 : CVtable_012B5C10 {
    char pad_04[0xd8 - 4];
    unsigned int f_d8;
    char pad2[0x1e8 - 0xd8 - 4];
    unsigned int f_1e8;
    unsigned int f_1ec;
    virtual long Initialise(CAudioSound* sound);
};

static int g_base_called = 0;
static CAudioSound* g_base_sound = 0;
CVtable_012B5C10::~CVtable_012B5C10() { }
long CVtable_012B5C10::Initialise(CAudioSound* sound) { g_base_called = 1; g_base_sound = sound; return 0; }

int main()
{
    CVtable_012B5C60 obj;
    obj.f_d8 = 0xDEAD;
    obj.f_1e8 = 0xBEEF;
    obj.f_1ec = 0xCAFE;
    long r = obj.Initialise((CAudioSound*)0x1234);
    if (r == 0 && obj.f_1e8 == 0x1000 && obj.f_1ec == 0 && obj.f_d8 == 0 && g_base_called == 1 && g_base_sound == (CAudioSound*)0x1234)
        printf("STEALTH_ONCREATE_OK\n");
    else
        printf("FAIL %x %x %x %d\n", obj.f_1e8, obj.f_1ec, obj.f_d8, g_base_called);
    return 0;
}