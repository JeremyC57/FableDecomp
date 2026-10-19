#include <cstdio>
#include <cstring>
// Behaviour test: runs the reconstructed operator= on a filled source and checks every member and the out-of-line container/base calls.
// CExpressionDef::operator=(const CExpressionDef&) -- memberwise copy: CParentDefClassBase part, the eight animation names, sprite fields, sound criteria, dummy object, game event, script name and flags, opinion deed, the extend/sheathe/replace flags, text tag and inventory type; returns *this

// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045D63C, length 207 bytes.
// Pragma:  -- the def module is size-optimised (every copy goes through eax/al).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0.
// Identity: the catalog label "CParentDefClassBase::operator=" is propagated. Slot 19 (Copy) of
// CExpressionDef's vtable 0x01233D1C is the 5-byte `jmp 0x0045D63C` right before this body. The class
// holds no containers, so the PDB members +0x40.. sit at retail +0x3c.. (retail base 0x3c) unchanged.
// Shape: memberwise assignment in declaration order; a CDefString is a single string-table position,
// so each one copies as one dword.
#pragma pack(push, 1)
struct CParentDefClassBase {
    virtual void v00() {}
    unsigned char _defClassBase[0x35];      // +0x04 CDefClassBase part, InstantiationName (+0x25), SubDefInfoMap (+0x29)
    CParentDefClassBase& operator=(const CParentDefClassBase& other);   // 0x00431ED0
};
#pragma pack(pop)

struct CDefString { long TablePos; };

struct CExpressionDef : CParentDefClassBase {   // vtable 0x01233D1C
    CDefString Animation;               // +0x3c
    CDefString AnimationInto;           // +0x40
    CDefString AnimationLoop;           // +0x44
    CDefString AnimationOutOf;          // +0x48
    CDefString AnimationToOther;        // +0x4c
    CDefString AnimationToOtherInto;    // +0x50
    CDefString AnimationToOtherLoop;    // +0x54
    CDefString AnimationToOtherOutOf;   // +0x58
    float      SpriteDuration;          // +0x5c
    long       SpriteGraphic;           // +0x60
    CDefString SpriteOverrideText;      // +0x64
    CDefString SoundCriteria;           // +0x68
    CDefString LoopSoundCriteria;       // +0x6c
    long       DummyObject;             // +0x70 CDefIndex
    long       GameEventType;           // +0x74 EGameEventType
    CDefString ScriptName;              // +0x78
    bool       ScriptLoadResources;     // +0x7c
    bool       DeactivateScriptAtEnd;   // +0x7d
    bool       PerformAfterScript;      // +0x7e
    long       OpinionDeedType;         // +0x80 EOpinionDeedType
    bool       Extendable;              // +0x84
    bool       SheatheWeapons;          // +0x85
    bool       ReplaceHeldItemWhenDone; // +0x86
    unsigned long TextTag;              // +0x88
    long       ExpressionType;          // +0x8c EExpressionInventoryType
    CExpressionDef& operator=(const CExpressionDef& other);
};

CExpressionDef& CExpressionDef::operator=(const CExpressionDef& other)
{
    CParentDefClassBase::operator=(other);
    Animation = other.Animation;
    AnimationInto = other.AnimationInto;
    AnimationLoop = other.AnimationLoop;
    AnimationOutOf = other.AnimationOutOf;
    AnimationToOther = other.AnimationToOther;
    AnimationToOtherInto = other.AnimationToOtherInto;
    AnimationToOtherLoop = other.AnimationToOtherLoop;
    AnimationToOtherOutOf = other.AnimationToOtherOutOf;
    SpriteDuration = other.SpriteDuration;
    SpriteGraphic = other.SpriteGraphic;
    SpriteOverrideText = other.SpriteOverrideText;
    SoundCriteria = other.SoundCriteria;
    LoopSoundCriteria = other.LoopSoundCriteria;
    DummyObject = other.DummyObject;
    GameEventType = other.GameEventType;
    ScriptName = other.ScriptName;
    ScriptLoadResources = other.ScriptLoadResources;
    DeactivateScriptAtEnd = other.DeactivateScriptAtEnd;
    PerformAfterScript = other.PerformAfterScript;
    OpinionDeedType = other.OpinionDeedType;
    Extendable = other.Extendable;
    SheatheWeapons = other.SheatheWeapons;
    ReplaceHeldItemWhenDone = other.ReplaceHeldItemWhenDone;
    TextTag = other.TextTag;
    ExpressionType = other.ExpressionType;
    return *this;
}

static int g_baseCalls = 0, g_vecCalls = 0;
CParentDefClassBase& CParentDefClassBase::operator=(const CParentDefClassBase& o)
{ ++g_baseCalls; for (int i = 0; i < 0x35; ++i) _defClassBase[i] = o._defClassBase[i]; return *this; }

int main()
{
    static CExpressionDef a, b;
    std::memset(a._defClassBase, 0, sizeof(a._defClassBase));
    std::memset(b._defClassBase, 0, sizeof(b._defClassBase));
    b._defClassBase[0x21] = 0x5a;
    b.Animation.TablePos = 1; b.AnimationToOtherOutOf.TablePos = 8; b.SpriteDuration = 2.5f; b.SpriteGraphic = 6; b.LoopSoundCriteria.TablePos = 13; b.DummyObject = 14; b.ScriptName.TablePos = 16; b.DeactivateScriptAtEnd = true; b.OpinionDeedType = 4; b.SheatheWeapons = true; b.ReplaceHeldItemWhenDone = true; b.TextTag = 0xdeadbeef; b.ExpressionType = 3;
    CExpressionDef& r = (a = b);
    bool ok = (&r == &a) && g_baseCalls == 1 && a._defClassBase[0x21] == 0x5a;
    ok = ok && a.Animation.TablePos == 1 && a.AnimationToOtherOutOf.TablePos == 8 && a.SpriteDuration == 2.5f && a.SpriteGraphic == 6 && a.LoopSoundCriteria.TablePos == 13 && a.DummyObject == 14 && a.ScriptName.TablePos == 16 && !a.ScriptLoadResources && a.DeactivateScriptAtEnd && a.OpinionDeedType == 4 && !a.Extendable && a.SheatheWeapons && a.ReplaceHeldItemWhenDone && a.TextTag == 0xdeadbeef && a.ExpressionType == 3;
    if (!ok) { std::printf("0045d63c_TEST FAIL\n"); return 1; }
    std::printf("0045d63c_TEST PASS\n");
    return 0;
}
