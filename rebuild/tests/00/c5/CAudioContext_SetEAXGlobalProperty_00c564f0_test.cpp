// Behaviour test for CAudioContext::SetEAXGlobalProperty (0x00C564F0): model and body verbatim, GetProcAddress and the
// resolved EAXSet entry stubbed; exercises the lazy resolution and every translated error code.
#include <stdio.h>

typedef void* (__cdecl* PFN_alGetProcAddress)(const char* name);
typedef long (__cdecl* PFN_EAXSet)(const void* propertySetGuid, unsigned long property, unsigned long source, void* value, unsigned long size);

enum {
    AL_NO_ERROR = 0,
    AL_INVALID_NAME = 0xA001,
    AL_INVALID_ENUM = 0xA002,
    AL_INVALID_VALUE = 0xA003,
    AL_INVALID_OPERATION = 0xA004,
    AL_OUT_OF_MEMORY = 0xA005
};

enum EAudioResult {
    AUDIO_OK = 0,
    AUDIO_INVALID_ARGUMENT = -3,
    AUDIO_OUT_OF_MEMORY = -5,
    AUDIO_FAILED = -9
};

static inline long ConvertALError(long error)
{
    if (error != AL_NO_ERROR)
        return error == AL_OUT_OF_MEMORY ? AUDIO_OUT_OF_MEMORY : AUDIO_FAILED;
    return AUDIO_OK;
}

struct CAudioContext {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual long SetEAXGlobalProperty(const void* propertySetGuid, unsigned long property, void* value, unsigned long size);   // slot 8
    void*              Module;                           // +0x04
    unsigned char      _pad1[0xe0];
    PFN_alGetProcAddress pfnGetProcAddress;              // +0xe8
    unsigned char      _pad2[4];
    PFN_EAXSet         pfnEAXSet;                        // +0xf0
    void* GetProcAddress(const char* name);              // 0x00C4D740
    long EAXSetGlobal(const void* propertySetGuid, unsigned long property, void* value, unsigned long size)   // 0x00C56250
    {
        if (!pfnEAXSet)
            pfnEAXSet = (PFN_EAXSet)GetProcAddress("EAXSet");
        return pfnEAXSet(propertySetGuid, property, 0, value, size);
    }
};

long CAudioContext::SetEAXGlobalProperty(const void* propertySetGuid, unsigned long property, void* value, unsigned long size)
{
    long error = EAXSetGlobal(propertySetGuid, property, value, size);
    switch (error) {
    case AL_NO_ERROR:
        return AUDIO_OK;
    case AL_INVALID_NAME:
    case AL_INVALID_VALUE:
        return AUDIO_INVALID_ARGUMENT;
    case AL_INVALID_OPERATION:
        return AUDIO_INVALID_ARGUMENT;
    default:
        return ConvertALError(error);
    }
}

static int g_resolveCalls = 0;
static long g_error = 0;
static const void* g_guid = 0;
static unsigned long g_property = 0, g_source = 1, g_size = 0;
static void* g_value = 0;

static long __cdecl StubEAXSet(const void* propertySetGuid, unsigned long property, unsigned long source, void* value, unsigned long size)
{
    g_guid = propertySetGuid; g_property = property; g_source = source; g_value = value; g_size = size;
    return g_error;
}

void* CAudioContext::GetProcAddress(const char* name)
{
    ++g_resolveCalls;
    return (void*)&StubEAXSet;
}

void CAudioContext::v00() {} void CAudioContext::v01() {} void CAudioContext::v02() {} void CAudioContext::v03() {}
void CAudioContext::v04() {} void CAudioContext::v05() {} void CAudioContext::v06() {} void CAudioContext::v07() {}
struct CTestContext : CAudioContext { };

static int fail(const char* why) { printf("FAIL: %s\n", why); return 1; }

int main()
{
    CTestContext ctx;
    ctx.pfnEAXSet = 0;
    int guid = 7;
    int value = 9;

    g_error = AL_NO_ERROR;
    if (ctx.SetEAXGlobalProperty(&guid, 3, &value, 4) != AUDIO_OK) return fail("no error");
    if (g_resolveCalls != 1 || ctx.pfnEAXSet != &StubEAXSet) return fail("lazy resolution");
    if (g_guid != &guid || g_property != 3 || g_source != 0 || g_value != &value || g_size != 4) return fail("arguments");

    g_error = AL_INVALID_NAME;
    if (ctx.SetEAXGlobalProperty(&guid, 3, &value, 4) != AUDIO_INVALID_ARGUMENT) return fail("invalid name");
    if (g_resolveCalls != 1) return fail("resolved again");
    g_error = AL_INVALID_VALUE;
    if (ctx.SetEAXGlobalProperty(&guid, 3, &value, 4) != AUDIO_INVALID_ARGUMENT) return fail("invalid value");
    g_error = AL_INVALID_OPERATION;
    if (ctx.SetEAXGlobalProperty(&guid, 3, &value, 4) != AUDIO_INVALID_ARGUMENT) return fail("invalid operation");
    g_error = AL_OUT_OF_MEMORY;
    if (ctx.SetEAXGlobalProperty(&guid, 3, &value, 4) != AUDIO_OUT_OF_MEMORY) return fail("out of memory");
    g_error = AL_INVALID_ENUM;
    if (ctx.SetEAXGlobalProperty(&guid, 3, &value, 4) != AUDIO_FAILED) return fail("invalid enum -> failed");
    g_error = 0x1234;
    if (ctx.SetEAXGlobalProperty(&guid, 3, &value, 4) != AUDIO_FAILED) return fail("unknown -> failed");

    printf("00c564f0_TEST PASS\n");
    return 0;
}