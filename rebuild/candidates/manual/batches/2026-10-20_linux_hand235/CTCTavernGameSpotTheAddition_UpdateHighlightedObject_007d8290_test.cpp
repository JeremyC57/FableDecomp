// behaviour test for 007d8290 CTCTavernGameSpotTheAddition::UpdateHighlightedObject: the nearest knick-knack within twice
// the def radius is selected (relative distance recorded), deselected beyond the radius, and a change plays the sound
#include <cstdio>
#include <cstring>
#include <math.h>
static int g_sounds = 0, g_destroyed = 0;
struct C3DVector { float X, Y, Z; };
struct CCharString {
    char m_text[48];
    CCharString(const char* text, long) { std::strcpy(m_text, text); }
    ~CCharString() {}
};
struct CThing {
    C3DVector Pos;
    const C3DVector& GetPosition() const { return Pos; }
};
struct KnickKnack { C3DVector Pos; CThing* Thing; };
struct CTavernGameSpotTheAdditionDef {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
    long  m_count;
    float KnickKnackRadius;
    void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
};
struct CSpotTheAdditionDefPointer {
    CTavernGameSpotTheAdditionDef* Object;
    CSpotTheAdditionDefPointer() : Object(0) {}
    ~CSpotTheAdditionDefPointer() { if (Object) Object->Release(); }
    CTavernGameSpotTheAdditionDef* operator->() const { return Object; }
};
static CTavernGameSpotTheAdditionDef g_def;
struct CDefClassBase {
    bool GetSubDef(CSpotTheAdditionDefPointer* out) { ++g_def.m_count; out->Object = &g_def; return true; }
};
struct CThingDefOwner { CDefClassBase* DefClass; };
struct CTCTavernGameSpotTheAddition {
    virtual void v00() {}
    CThingDefOwner* Owner;
    CThing*         Pointer;
    float           DistFromObject;
    KnickKnack*     KnickKnacksBegin;
    KnickKnack*     KnickKnacksEnd;
    long            Selected;
    unsigned long PlaySound(const CCharString& name) { if (std::strcmp(name.m_text, "TAVERN_GAME_OBJECT_SELECTION_CHANGE") == 0) ++g_sounds; return 0; }
    void UpdateHighlightedObject();
};
void CTCTavernGameSpotTheAddition::UpdateHighlightedObject()
{
    CSpotTheAdditionDefPointer def;
    Owner->DefClass->GetSubDef(&def);
    float radius = def->KnickKnackRadius;
    float bestDist = radius + radius;
    long selected = -1;
    DistFromObject = -1.0f;
    long index = 0;
    for (KnickKnack* knick = KnickKnacksBegin; knick != KnickKnacksEnd; ++knick, ++index) {
        const C3DVector& pointerPos = Pointer->GetPosition();
        const C3DVector& pos = knick->Thing->GetPosition();
        float dx = pos.X - pointerPos.X;
        float dy = pos.Y - pointerPos.Y;
        float dist = (float)sqrt(dx * dx + dy * dy);
        if (dist < bestDist) {
            bestDist = dist;
            selected = index;
        }
    }
    if (selected != -1)
        DistFromObject = bestDist / radius;
    if (bestDist > radius)
        selected = -1;
    if (selected != Selected) {
        Selected = selected;
        CCharString sound("TAVERN_GAME_OBJECT_SELECTION_CHANGE", -1);
        PlaySound(sound);
    }
}
int main()
{
    g_def.KnickKnackRadius = 2.0f; g_def.m_count = 0;
    CDefClassBase defClass; CThingDefOwner owner; owner.DefClass = &defClass;
    CThing pointer = { { 0.0f, 0.0f, 0.0f } };
    CThing a = { { 3.0f, 0.0f, 5.0f } }, b = { { 0.0f, 1.0f, 9.0f } };
    KnickKnack knicks[2]; knicks[0].Thing = &a; knicks[1].Thing = &b;
    CTCTavernGameSpotTheAddition game; game.Owner = &owner; game.Pointer = &pointer;
    game.KnickKnacksBegin = knicks; game.KnickKnacksEnd = knicks + 2; game.Selected = -1;
    game.UpdateHighlightedObject();
    bool ok = game.Selected == 1 && game.DistFromObject == 0.5f && g_sounds == 1 && g_destroyed == 1;
    game.UpdateHighlightedObject();               // unchanged: no sound
    ok = ok && game.Selected == 1 && g_sounds == 1;
    pointer.Pos.X = -3.0f; pointer.Pos.Y = 0.0f;    // b at distance ~3.16, a at 6: within 2r but beyond r
    game.UpdateHighlightedObject();
    ok = ok && game.Selected == -1 && g_sounds == 2 && game.DistFromObject > 1.5f && g_destroyed == 3;
    if (!ok) { std::printf("007d8290_TEST FAIL\n"); return 1; }
    std::printf("007d8290_TEST PASS\n");
    return 0;
}
