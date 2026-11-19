// Behaviour test for 0x006CA8C0 CEngineTextureMorphSet::AddEngineMorph: the row's defines, includes and body verbatim, on a set image whose
// Morphs vector is set up by hand. In-place arm: with spare capacity (the buffer pre-filled with 0xAB garbage) each call constructs
// {original_texture, 0, empty Blends} at the old finish, advances finish by one record, keeps the buffer, and returns that record
// (= &back()). Overflow arm: exactly at finish == end_of_storage the vector grows (old size + max(old size, 1)), the old records are
// copied (their Blends deep-copied) and the new one appended last. An empty vector grows to one record.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define protected public   // test access to the vector words and the protected member only
#define private public
#define FABLE_RETAIL_CARRAY_PUSHBACK
#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#define FABLE_STLP_OUTOFLINE_UNINIT_FILL_N
#define FABLE_RETAIL_CENGINETEXTUREMORPH_CTORS
#define FABLE_RETAIL_CENGINETEXTUREMORPHSET
#define FABLE_RETAIL_CENGINETEXTUREMORPHSET_ADDENGINEMORPH
#include "engine/CEngineTextureMorph.h"

// Test-only stand-in for the out-of-line CCompositeBlendDesc copy constructor (retail 0x006CABA0; the shared header declares it).
CCompositeBlendDesc::CCompositeBlendDesc(const CCompositeBlendDesc& o)
    : TextureIndex(o.TextureIndex), BlendType(o.BlendType), DiffuseColour(o.DiffuseColour), SpecularColour(o.SpecularColour) { }

CEngineTextureMorph* CEngineTextureMorphSet::AddEngineMorph(long original_texture)
{
    Morphs.push_back(CEngineTextureMorph(original_texture, 0));
    return &Morphs.back();
}


static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
static double g_set[2];   // 12-byte CEngineTextureMorphSet image, never destroyed (its ~vector is out of line)
static int Fresh(const CEngineTextureMorph* m, long index)
{
    return m->OriginalTextureIndex == index && m->MorphID == 0 && m->Blends._M_start == 0 && m->Blends._M_finish == 0
        && m->Blends._M_end_of_storage._M_data == 0;
}
int main()
{
    memset(g_set, 0, sizeof(g_set));
    CEngineTextureMorphSet* set = reinterpret_cast<CEngineTextureMorphSet*>(g_set);
    std::vector<CEngineTextureMorph>& v = set->Morphs;

    // Empty vector: overflow to one record.
    CEngineTextureMorph* r = set->AddEngineMorph(7);
    check(v._M_start != 0 && v.size() == 1 && r == v._M_start && r == &v.back(), "empty -> one record, &back()");
    check(Fresh(r, 7), "empty: record {7, 0, no blends}");
    check(v._M_end_of_storage._M_data == v._M_start + 1, "empty: capacity 1");

    // In-place arm on a hand-made buffer of 3 garbage records.
    CEngineTextureMorph* buf = (CEngineTextureMorph*)malloc(3 * sizeof(CEngineTextureMorph));
    memset(buf, 0xAB, 3 * sizeof(CEngineTextureMorph));
    v._M_start = buf; v._M_finish = buf; v._M_end_of_storage._M_data = buf + 3;
    r = set->AddEngineMorph(-5);
    check(r == buf && v._M_finish == buf + 1 && v._M_start == buf && v._M_end_of_storage._M_data == buf + 3, "in place 1: no growth");
    check(Fresh(r, -5), "in place 1: record {-5, 0, no blends}");
    // Give the first record real blends and a morph id so the overflow copy is visible.
    r->MorphID = 3;
    CCompositeBlendDesc* blends = (CCompositeBlendDesc*)malloc(2 * sizeof(CCompositeBlendDesc));
    memset(blends, 0, 2 * sizeof(CCompositeBlendDesc));
    blends[0].TextureIndex = 11; blends[1].TextureIndex = 22;
    r->Blends._M_start = blends; r->Blends._M_finish = blends + 2; r->Blends._M_end_of_storage._M_data = blends + 2;
    r = set->AddEngineMorph(100);
    check(r == buf + 1 && v._M_finish == buf + 2 && Fresh(r, 100), "in place 2");
    r = set->AddEngineMorph(200);
    check(r == buf + 2 && v._M_finish == buf + 3 && v._M_start == buf && Fresh(r, 200), "in place 3: fills the last slot");

    // Exactly at finish == end_of_storage: overflow to 3 + 3 = 6 records.
    r = set->AddEngineMorph(300);
    check(v._M_start != buf && v.size() == 4 && v._M_end_of_storage._M_data == v._M_start + 6, "boundary: grows to 6");
    check(r == v._M_start + 3 && r == &v.back() && Fresh(r, 300), "boundary: new record last, {300, 0, no blends}");
    CEngineTextureMorph* s = v._M_start;
    check(s[0].OriginalTextureIndex == -5 && s[0].MorphID == 3 && s[0].Blends.size() == 2
          && s[0].Blends._M_start != blends && s[0].Blends._M_start[0].TextureIndex == 11 && s[0].Blends._M_start[1].TextureIndex == 22,
          "boundary: old record copied with its blends");
    check(Fresh(&s[1], 100) && Fresh(&s[2], 200), "boundary: other old records copied");

    // Back to the in-place arm after growth.
    r = set->AddEngineMorph(400);
    check(r == s + 4 && v._M_start == s && v.size() == 5 && Fresh(r, 400), "after growth: in place again");

    if (!g_fail) printf("006ca8c0_TEST PASS\n");
    return g_fail;
}