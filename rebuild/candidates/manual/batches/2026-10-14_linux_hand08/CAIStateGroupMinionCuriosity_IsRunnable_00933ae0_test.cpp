#include <cstdio>
struct C3DVector { float X, Y, Z; C3DVector() {} C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {} };
struct CThing { unsigned char _pad[0x91]; unsigned char m_flags91; C3DVector pos; const C3DVector& GetPosition() { return pos; } };
struct CThingCreatureBase { unsigned char _pad0[0x20]; unsigned long m_presentBits[9]; CThing* t;
    bool HasInterface(long i) { return (m_presentBits[i >> 5] & (1UL << (i & 31))) != 0; } CThing* GetCuriosityThing() { return t; } };
struct CAIBrain { unsigned char _pad[0x18]; CThingCreatureBase* Creature; };
struct CAIStateGroupBase { virtual ~CAIStateGroupBase() {} CAIBrain* PBrain; unsigned char _pad[0xc]; };
struct CAIStateGroup_MinionCuriosity : CAIStateGroupBase { C3DVector CuriosityPos; bool FindNewCuriosityPos(); virtual bool IsRunnable(); };
bool CAIStateGroup_MinionCuriosity::FindNewCuriosityPos()
{ CThing* thing = PBrain->Creature->GetCuriosityThing(); if (thing && !(thing->m_flags91 & 1)) { CuriosityPos = thing->GetPosition(); return true; } return false; }
bool CAIStateGroup_MinionCuriosity::IsRunnable() { if (PBrain->Creature->HasInterface(0x34)) return FindNewCuriosityPos(); return false; }
int main()
{
    CThing t; t.m_flags91 = 0; t.pos.X = 4; t.pos.Y = 5; t.pos.Z = 6; CThingCreatureBase c; for (int i = 0; i < 9; ++i) c.m_presentBits[i] = 0; c.t = &t;
    CAIBrain b; b.Creature = &c; CAIStateGroup_MinionCuriosity g; g.PBrain = &b; g.CuriosityPos.X = 0;
    if (g.IsRunnable()) { std::printf("00933ae0_TEST FAIL\n"); return 1; }
    c.m_presentBits[1] = 0x100000; if (!g.IsRunnable() || g.CuriosityPos.X != 4) { std::printf("00933ae0_TEST FAIL\n"); return 1; }
    t.m_flags91 = 1; if (g.IsRunnable()) { std::printf("00933ae0_TEST FAIL\n"); return 1; }
    std::printf("00933ae0_TEST PASS\n"); return 0;
}
