#include <cstdio>
struct CThing { int m_id; };
struct CTCBase {
    CThing* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs;
    CTCBase(CThing* parent) : ParentThing(parent), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { }
};
struct CDefPointer { void* Object; CDefPointer() : Object(0) { } };
struct CList { void* head; CList() : head(&head) { } };
struct CTCAISummon : CTCBase {
    CDefPointer PSummonDef; CList MySummonedCreatures; long LastSummonTick;
    CTCAISummon(CThing* parent) : CTCBase(parent), PSummonDef(), MySummonedCreatures(), LastSummonTick(0) { }
};
int main()
{
    CThing t; t.m_id = 4;
    CTCAISummon s(&t);
    if (s.ParentThing != &t || s.PSummonDef.Object != 0 || s.MySummonedCreatures.head != &s.MySummonedCreatures.head || s.LastSummonTick != 0 || s.ToBeRemoved) { std::printf("004df2d4_TEST FAIL\n"); return 1; }
    std::printf("004df2d4_TEST PASS\n");
    return 0;
}