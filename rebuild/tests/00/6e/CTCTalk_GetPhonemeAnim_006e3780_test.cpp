// Behaviour test for 006e3780: src/006e3780.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTCTalk::GetPhonemeAnim(const CCharString& phoneme) const
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCTalk::GetPhonemeAnim(const CCharString& phoneme) const -- the animation index the talk definition maps a phoneme name to (0 when the phoneme is not in the map)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E3780, length 77 bytes (+ int3 padding).
// Callees: CDefClassBase::GetSubDef<CTalkDef>(CDefPointer<const CTalkDef>&) @ 0x00485815 (manifest template argument
// "CAbilityDef" propagated), the phoneme map's find(const CCharString&) @ 0x0042B467 (manifest "StdMap_FindLE"; returns
// the node pointer), the def's virtual slot 1 (+4) when the count drops to zero.
// Identity: Ego_r.pdb has both `?GetPhonemeAnim@CThingCreatureBase@@MBEKABVCCharString@@@Z` and
// `?GetPhonemeAnim@CTCTalk@@ABEKABVCCharString@@@Z`; this body reads `this+4` (CTCBase::ParentThing) and is called
// from the CTCTalk speech code around 0x006E3CDA, so it is CTCTalk's (the manifest's CThingCreatureBase is propagated).
// Layout (retail, local model): CTCBase's parent thing at +4, the thing's PBaseDef at +0x70 (landed CTCLook::OnCreate
// 0x00753040); the talk definition's STLport map<CCharString, unsigned long> at +0x60 (header node pointer first);
// STLport tree nodes {colour, parent, left, right, key +0x10, value +0x14}; a counted definition's count at +4.
// Shape: the def pointer is zero-initialised in the frame and loaded ONCE into edi after GetSubDef; the map is a named
// pointer local (esi, re-used for `cmp eax,[esi]` after the find); the found value is a ternary into esi; the def is
// released through edi with NO null test (`dec [edi+4] ; jne`), so the release is written by hand on the local.
class CCharString;

class CDefPointeeBase { public:                        // a counted definition object
    virtual void v000();
    virtual void DestroyDef();                  // slot 1 (+4): called when the count drops to zero
    long m_refCount;                            // +4
    void ReleaseDef()
    {
        if (--m_refCount == 0)
            DestroyDef();
    }
};

template<class T> class CDefPointer { public:          // CDefPointer<const T>: one counted pointer to a definition
    T* m_pObject;
    CDefPointer() : m_pObject(0) { }
};

struct SPhonemeNode {                           // STLport _Rb_tree_node<pair<const CCharString, unsigned long> >
    long          m_colour;                     // +0x00
    SPhonemeNode* m_parent;                     // +0x04
    SPhonemeNode* m_left;                       // +0x08
    SPhonemeNode* m_right;                      // +0x0c
    void*         m_key;                        // +0x10 CCharString
    unsigned long m_anim;                       // +0x14
};

struct CPhonemeMap {                            // map<CCharString, unsigned long>
    SPhonemeNode* m_header;                     // +0x00 end()
    unsigned long m_count;                      // +0x04
    SPhonemeNode* Find(const CCharString& key) const;       // 0x0042B467
};

struct CTalkDef : CDefPointeeBase {
    unsigned char _pad08[0x58];
    CPhonemeMap   Phonemes;                     // +0x60
};

class CDefClassBase { public:
    bool GetSubDef(CDefPointer<CTalkDef>& out) const;       // 0x00485815
};
void* FindDef_CDefClassBase_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CTalkDef>& out) const
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)&out = def;
    return true;
}
#pragma auto_inline(on)

class CThing { public:
    unsigned char  _pad00[0x70];
    CDefClassBase* PBaseDef;                    // +0x70
};

class CTCTalk { public:
    void*   __vftable;                          // +0x00
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned long GetPhonemeAnim(const CCharString& phoneme) const;
};

unsigned long CTCTalk::GetPhonemeAnim(const CCharString& phoneme) const
{
    CDefPointer<CTalkDef> def;
    ParentThing->PBaseDef->GetSubDef(def);
    CTalkDef* talk = def.m_pObject;
    const CPhonemeMap* phonemes = &talk->Phonemes;
    SPhonemeNode* it = phonemes->Find(phoneme);
    unsigned long anim = (it != phonemes->m_header) ? it->m_anim : 0;
    talk->ReleaseDef();
    return anim;
}

// ---- stubs and driver ----
static void* g_def = 0; static int g_destroyed = 0; static SPhonemeNode g_header, g_node; static bool g_found = false; static const void* g_key = 0;
void* FindDef_CDefClassBase_GetSubDef_0() { return g_def; }
void CDefPointeeBase::v000() {}
void CDefPointeeBase::DestroyDef() { ++g_destroyed; }
SPhonemeNode* CPhonemeMap::Find(const CCharString& key) const { g_key = &key; return g_found ? &g_node : m_header; }
int main()
{
    CTalkDef def; def.m_refCount = 3; def.Phonemes.m_header = &g_header; def.Phonemes.m_count = 0; g_def = &def;
    CDefClassBase base;
    CThing thing; memset(&thing, 0, sizeof(thing)); thing.PBaseDef = &base;
    CTCTalk talk; talk.__vftable = 0; talk.ParentThing = &thing;
    int key = 0; const CCharString& k = *(const CCharString*)&key;
    g_found = true; g_node.m_anim = 0x77;
    CHECK(talk.GetPhonemeAnim(k) == 0x77); CHECK(g_key == &key); CHECK(def.m_refCount == 2);
    g_found = false; CHECK(talk.GetPhonemeAnim(k) == 0); CHECK(def.m_refCount == 1); CHECK(g_destroyed == 0);
    g_found = true; CHECK(talk.GetPhonemeAnim(k) == 0x77); CHECK(def.m_refCount == 0); CHECK(g_destroyed == 1);
    if (fails == 0) printf("006e3780_TEST PASS\n"); else printf("006e3780_TEST FAIL\n");
    return fails;
}