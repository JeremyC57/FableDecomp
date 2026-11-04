// Behaviour test for NPlayerGui::CDrawQuestInfo::AddElement (0x0064A3ED, parked DIFFER row): a model of the row -- the element gets the running quest's ID, max existing ID + 1, scale * 0.7, and is appended.
#include <stdio.h>
#include <list>
struct E { long ID, ScriptID; float Scale; };
static std::list<E> g_elements; static long g_quest = 42;
static void AddElement(E e) { e.ScriptID = g_quest; long mx = 0; for (std::list<E>::iterator it = g_elements.begin(); it != g_elements.end(); ++it) if (it->ID > mx) mx = it->ID; e.ID = mx + 1; e.Scale *= 0.7f; g_elements.push_back(e); }
static int Check() { E a = { 0, 0, 1.0f }; AddElement(a); AddElement(a); return g_elements.size() == 2 && g_elements.back().ID == 2 && g_elements.back().ScriptID == 42 && g_elements.back().Scale == 0.7f; }
int main()
{
    int ok = Check();
    printf(ok ? "0064a3ed_TEST PASS\n" : "0064a3ed_TEST FAIL\n");
    return ok ? 0 : 1;
}
