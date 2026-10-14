#include <cstdio>
struct C3DVector { float x, y, z; };
struct CThing { C3DVector pos; C3DVector* GetPosition() { return &pos; } };
struct CMessageEvent;
struct CMessageEventManager { long type; unsigned long dist; C3DVector* pos; CThing* src; CThing* extra; float a, b; long c; int n;
    CMessageEvent* AddMessage(long t, unsigned long d, C3DVector* p, CThing* s, CThing* e, float fa, float fb, long lc) { type = t; dist = d; pos = p; src = s; extra = e; a = fa; b = fb; c = lc; ++n; return 0; } };
struct CWorld { unsigned char _pad[0x60]; CMessageEventManager* m_pMessageManager; };
#pragma pack(push, 1)
struct CBaseIntelligentPointer { void* __vftable; CThing* m_pItem; CThing* GetPItem() { return m_pItem; } };
struct CAIBrain { unsigned char _pad[0x18]; CThing* Creature; };
struct CAIStateGroupBase { void* __vftable; CAIBrain* PBrain; unsigned char _pad[0xc]; CWorld* w; CWorld* GetPWorld() { return w; } };
struct CAIStateGroup_KidPlayHopscotch : CAIStateGroupBase { bool m_initiated; CBaseIntelligentPointer m_pMarker; unsigned char _pad1d[0x43]; long m_gamesStarted; void OnActivate(); };
#pragma pack(pop)
void CAIStateGroup_KidPlayHopscotch::OnActivate()
{ ++m_gamesStarted; CThing* creature = PBrain->Creature; CThing* marker = m_pMarker.GetPItem(); CMessageEventManager* messages = GetPWorld()->m_pMessageManager; messages->AddMessage(0x18, 9999999, marker->GetPosition(), creature, m_pMarker.GetPItem(), 0.0f, 0.0f, 0); }
int main()
{
    CThing kid, marker; CMessageEventManager m = { 0 }; CWorld w; w.m_pMessageManager = &m; CAIBrain b; b.Creature = &kid;
    CAIStateGroup_KidPlayHopscotch g; g.PBrain = &b; g.w = &w; g.m_pMarker.m_pItem = &marker; g.m_gamesStarted = 2;
    g.OnActivate();
    if (g.m_gamesStarted != 3 || m.n != 1 || m.type != 0x18 || m.dist != 9999999 || m.pos != &marker.pos || m.src != &kid || m.extra != &marker || m.a != 0 || m.b != 0 || m.c != 0) { std::printf("00941550_TEST FAIL\n"); return 1; }
    std::printf("00941550_TEST PASS\n"); return 0;
}