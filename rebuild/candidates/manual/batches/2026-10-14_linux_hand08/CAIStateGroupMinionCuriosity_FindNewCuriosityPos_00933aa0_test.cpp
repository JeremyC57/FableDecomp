#include <cstdio>
struct C3DVector { float X, Y, Z; C3DVector() {} C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {} };
struct CThing { unsigned char _pad[0x91]; unsigned char m_flags91; C3DVector pos; const C3DVector& GetPosition() { return pos; } };
struct CThingCreatureBase { CThing* t; CThing* GetCuriosityThing() { return t; } };
struct CAIBrain { unsigned char _pad[0x18]; CThingCreatureBase* Creature; };
struct CAIStateGroupBase { virtual ~CAIStateGroupBase() {} CAIBrain* PBrain; unsigned char _pad[0xc]; };
struct CAIStateGroup_MinionCuriosity : CAIStateGroupBase { C3DVector CuriosityPos; bool FindNewCuriosityPos(); };
bool CAIStateGroup_MinionCuriosity::FindNewCuriosityPos()
{ CThing* thing = PBrain->Creature->GetCuriosityThing(); if (thing && !(thing->m_flags91 & 1)) { CuriosityPos = thing->GetPosition(); return true; } return false; }
int main()
{
    CThing t; t.m_flags91 = 0; t.pos.X = 4; t.pos.Y = 5; t.pos.Z = 6; CThingCreatureBase c = { 0 }; CAIBrain b; b.Creature = &c;
    CAIStateGroup_MinionCuriosity g; g.PBrain = &b; g.CuriosityPos.X = g.CuriosityPos.Y = g.CuriosityPos.Z = 0;
    if (g.FindNewCuriosityPos()) { std::printf("00933aa0_TEST FAIL\n"); return 1; }
    c.t = &t; t.m_flags91 = 1; if (g.FindNewCuriosityPos() || g.CuriosityPos.X != 0) { std::printf("00933aa0_TEST FAIL\n"); return 1; }
    t.m_flags91 = 2; if (!g.FindNewCuriosityPos() || g.CuriosityPos.X != 4 || g.CuriosityPos.Y != 5 || g.CuriosityPos.Z != 6) { std::printf("00933aa0_TEST FAIL\n"); return 1; }
    std::printf("00933aa0_TEST PASS\n"); return 0;
}
