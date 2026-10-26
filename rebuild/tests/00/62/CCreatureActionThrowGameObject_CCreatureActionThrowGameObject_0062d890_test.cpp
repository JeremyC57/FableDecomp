// CCreatureAction_ThrowGameObject::CCreatureAction_ThrowGameObject(CThingCreatureBase*, const C3DVector&) -- base CCreatureActionBase(owner, 0.05, 1.0, the owner's "STANDARD_CAST" animation, zeroed CSoundPair, 0, 0), own vtable, the throw target
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0062D890, length 129 bytes (+ int3 padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (literal 0x0125915C "STANDARD_CAST"), GetTableAnimation(CThingCreatureBase*,
// const CCharString&) @ 0x006924B0 (register arguments), CCreatureActionBase::CCreatureActionBase(CThingCreatureBase*, float, float, CAnimationEntry*,
// const CSoundPair&, long, long) @ 0x00693B30, CCharString::~CCharString @ 0x0099EAE0. Own vtable 0x01259044 (rtti-vtables.tsv).
// Shape: `ret 8`; the animation name is an IMPLICIT conversion temporary (`GetTableAnimation(owner, "STANDARD_CAST")` through the defaulted
// length, as in CCreatureAction_Read 0x007E7FD0): VC builds it first, loads the owner into edi, then the two trailing zeros and the sound pair
// share eax = 0 next to the lookup (an inline helper returning the string by value hoists the sound stores above the string constructor);
// the string dies after the base call, then the vtable store and the target copied word by word through C3DVector's memberwise copy constructor.
// No engine header layout used (the PDB flattens the base); modelled locally (retail CCreatureActionBase is 0xa8 bytes, Target at +0xa8).
// CCharString is modelled locally because fable_string.h declares its (const char*, long) constructor without the default length.
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                         // 0x0099EAE0
};
class CThingCreatureBase;
class CAnimationEntry;
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }   // memberwise copy constructor: fixed-offset word moves
};
class CSoundPair { public:                         // two sound ids, zeroed by the inline constructor
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
CAnimationEntry* __fastcall GetTableAnimation(CThingCreatureBase* owner, const CCharString& name);   // 0x006924B0

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(CThingCreatureBase* owner, float a, float b, CAnimationEntry* animation, const CSoundPair& sound, long c, long d);   // 0x00693B30
};

class CCreatureAction_ThrowGameObject : public CCreatureActionBase { public:   // vtable 0x01259044
    C3DVector Target;                       // +0xa8
    virtual void v00();
    CCreatureAction_ThrowGameObject(CThingCreatureBase* owner, const C3DVector& target);
};

CCreatureAction_ThrowGameObject::CCreatureAction_ThrowGameObject(CThingCreatureBase* owner, const C3DVector& target)
    : CCreatureActionBase(owner, 0.05f, 1.0f, GetTableAnimation(owner, "STANDARD_CAST"), CSoundPair(), 0, 0)
    , Target(target)
{
}

#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); ++fails; } } while (0)
static char g_name[64];
static int g_strings = 0;
CCharString::CCharString(const char* text, long length) { m_pData = (void*)text; ++g_strings; if (length == -1) strcpy(g_name, text); }
CCharString::~CCharString() { --g_strings; }
static CAnimationEntry* g_anim = (CAnimationEntry*)0x1234;
static CThingCreatureBase* g_lookupOwner = 0;
CAnimationEntry* __fastcall GetTableAnimation(CThingCreatureBase* owner, const CCharString& name) { g_lookupOwner = owner; CHECK(strcmp((const char*)name.m_pData, "STANDARD_CAST") == 0); return g_anim; }
static CThingCreatureBase* b_owner; static float b_a, b_b; static CAnimationEntry* b_anim; static long b_s1 = 7, b_s2 = 7, b_c = 7, b_d = 7; static int b_live = -1;
CCreatureActionBase::CCreatureActionBase(CThingCreatureBase* owner, float a, float b, CAnimationEntry* animation, const CSoundPair& sound, long c, long d)
{ b_owner = owner; b_a = a; b_b = b; b_anim = animation; b_s1 = sound.FirstSound; b_s2 = sound.LastSound; b_c = c; b_d = d; b_live = g_strings; }
void CCreatureActionBase::v00() { }
void CCreatureAction_ThrowGameObject::v00() { }

int main()
{
    CThingCreatureBase* owner = (CThingCreatureBase*)0x5678;
    float xyz[3] = { 1.5f, -2.0f, 3.25f };
    const C3DVector& target = *(const C3DVector*)xyz;
    CCreatureAction_ThrowGameObject action(owner, target);
    CHECK(b_owner == owner && g_lookupOwner == owner);
    CHECK(b_a == 0.05f && b_b == 1.0f && b_anim == g_anim);
    CHECK(b_s1 == 0 && b_s2 == 0 && b_c == 0 && b_d == 0);
    CHECK(b_live == 1 && g_strings == 0);
    CHECK(strcmp(g_name, "STANDARD_CAST") == 0);
    CHECK(action.Target.X == 1.5f && action.Target.Y == -2.0f && action.Target.Z == 3.25f);
    if (fails == 0) printf("0062d890_TEST PASS\n");
    return fails;
}