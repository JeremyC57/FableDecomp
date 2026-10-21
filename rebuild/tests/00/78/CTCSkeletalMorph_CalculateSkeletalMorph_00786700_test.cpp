#include <cstdio>
#include <cstring>
// Behaviour test for CTCSkeletalMorph::CalculateSkeletalMorph (0x00786700): the body of src/ over stand-ins -- the morph's
// parameters are cleared, the resource manager is created on first use, one counted parameter per named morph is built from
// its target, added and given the morph's value, then the combined morph is reset.
struct CCharString { const char* text; };
struct SCountBlock {
    long  references;
    void (*destroy)(void*);
    void* object;
    SCountBlock(long refs, void (*fn)(void*), void* data) : references(refs), destroy(fn), object(data) {}
};
template <class T> struct CCountedPointer {
    T*           object;
    SCountBlock* count;
    CCountedPointer() : object(0), count(0) {}
    CCountedPointer(const CCountedPointer& other) : object(other.object), count(other.count)
    {
        if (count)
            ++count->references;
    }
    explicit CCountedPointer(T* value);
    ~CCountedPointer()
    {
        if (count) {
            if (--count->references == 0) {
                count->destroy(count->object);
                delete count;
            }
        }
        object = 0;
        count = 0;
    }
    T* operator->() const { return object; }
    void Reset(T* value) { object = value; count = 0; }
};
struct C3DSkeletalMorphTarget { char name[16]; };
static int g_paramsAlive = 0;
struct C3DSkeletalMorphParameter {
    C3DSkeletalMorphTarget* target;
    float value;
    C3DSkeletalMorphParameter(C3DSkeletalMorphTarget* t) : target(t), value(0) { ++g_paramsAlive; }
    ~C3DSkeletalMorphParameter() { --g_paramsAlive; }
    void SetCurrentValue(float v) { value = v; }
};
static void DeleteData(void* data) { delete (C3DSkeletalMorphParameter*)data; }
template <> inline CCountedPointer<C3DSkeletalMorphParameter>::CCountedPointer(C3DSkeletalMorphParameter* value)
    : object(value)
{
    if (value)
        count = new SCountBlock(1, DeleteData, object);
    else
        count = 0;
}
static C3DSkeletalMorphTarget g_targets[2];
static int g_managers = 0;
class CSkeletalMorphResourceManager {
public:
    CCountedPointer<C3DSkeletalMorphTarget> cached;
    CSkeletalMorphResourceManager() { ++g_managers; }
    const CCountedPointer<C3DSkeletalMorphTarget>& GetMorphTarget(const CCharString& name)
    {
        cached.object = strcmp(name.text, "fat") == 0 ? &g_targets[0] : &g_targets[1];
        return cached;
    }
};
static CCountedPointer<CSkeletalMorphResourceManager> g_SkeletalMorphResourceManager;
static char g_log[128];
struct C3DSkeletalMorph {
    CCountedPointer<C3DSkeletalMorphParameter> params[4];
    int n;
    void ClearMorphParameters() { strcat(g_log, "clear,"); n = 0; }
    void AddMorphParameter(const CCountedPointer<C3DSkeletalMorphParameter>& p)
    {
        strcat(g_log, "add,");
        params[n].object = p.object; params[n].count = p.count; ++p.count->references; ++n;
    }
    void ResetCombinedMorph() { strcat(g_log, "reset,"); }
};
struct CSkeletalMorphEntry { long _00; CCharString Name; float Value; };
class CTCSkeletalMorph {
public:
    CSkeletalMorphEntry* m_morphsStart;
    CSkeletalMorphEntry* m_morphsFinish;
    C3DSkeletalMorph*    PSkeletalMorph;
    void CalculateSkeletalMorph();
};

void CTCSkeletalMorph::CalculateSkeletalMorph()
{
    PSkeletalMorph->ClearMorphParameters();
    for (CSkeletalMorphEntry* morph = m_morphsStart; morph != m_morphsFinish; ++morph) {
        if (!g_SkeletalMorphResourceManager.object)
            g_SkeletalMorphResourceManager.Reset(new CSkeletalMorphResourceManager);
        CCountedPointer<C3DSkeletalMorphTarget> target = g_SkeletalMorphResourceManager->GetMorphTarget(morph->Name);
        CCountedPointer<C3DSkeletalMorphParameter> parameter(new C3DSkeletalMorphParameter(target.object));
        PSkeletalMorph->AddMorphParameter(parameter);
        parameter->SetCurrentValue(morph->Value);
    }
    PSkeletalMorph->ResetCombinedMorph();
}

static int fail(int line) { printf("00786700_TEST FAIL (line %d)\n", line); return 1; }
#define CHECK(c) do { if (!(c)) return fail(__LINE__); } while (0)

int main()
{
    CSkeletalMorphEntry morphs[2] = { { 0, { "fat" }, 0.75f }, { 0, { "thin" }, 0.25f } };
    C3DSkeletalMorph skel; skel.n = 0;
    CTCSkeletalMorph tc; tc.m_morphsStart = morphs; tc.m_morphsFinish = morphs + 2; tc.PSkeletalMorph = &skel;
    tc.CalculateSkeletalMorph();
    CHECK(strcmp(g_log, "clear,add,add,reset,") == 0 && g_managers == 1);
    CHECK(skel.n == 2 && skel.params[0].object->target == &g_targets[0] && skel.params[1].object->target == &g_targets[1]);
    CHECK(skel.params[0].object->value == 0.75f && skel.params[1].object->value == 0.25f);
    CHECK(skel.params[0].count->references == 1 && g_paramsAlive == 2);
    // the morph's references are the last: releasing them frees the parameters
    skel.params[0].~CCountedPointer();
    skel.params[1].~CCountedPointer();
    CHECK(g_paramsAlive == 0);
    // second run reuses the manager; an empty morph list only clears and resets
    g_log[0] = 0; tc.m_morphsFinish = morphs;
    tc.CalculateSkeletalMorph();
    CHECK(strcmp(g_log, "clear,reset,") == 0 && g_managers == 1);
    printf("00786700_TEST PASS\n");
    return 0;
}