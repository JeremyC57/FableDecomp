#include <cstdio>
#include <cstdlib>
struct CScriptBase { unsigned char _body[0x3c]; CScriptBase() { for (int i = 0; i < 0x3c; ++i) _body[i] = 0; } };
struct CQuestScriptObject : CScriptBase {
    void* Game; void* Data;
    CQuestScriptObject(void* data, void* game) : CScriptBase(), Game(game), Data(data) { }
};
struct CBaseObject { void* m_pIntelligentPointer; CBaseObject() : m_pIntelligentPointer(0) { } };
struct CScriptedThing : CBaseObject { void* Data; void* Info; CScriptedThing() : Data(0), Info(0) { } };
struct Vec { void* s; void* f; void* e; Vec() : s(0), f(0), e(0) { } };
struct Node { char color; Node* parent; Node* left; Node* right; };
struct Map {
    Node* header; unsigned long count; unsigned char cmp;
    Map() : header(0), count(0) { header = (Node*)std::malloc(sizeof(Node)); header->color = 0; header->parent = 0; header->left = header; header->right = header; }
};
struct CCS_OakValeRevisitedScript : CQuestScriptObject {
    Vec FirePoint, Fires; Map OakValeFlag; CScriptedThing v[7];
    CCS_OakValeRevisitedScript(void* data, void* game) : CQuestScriptObject(data, game) { }
};
int main()
{
    int d, g;
    CCS_OakValeRevisitedScript s(&d, &g);
    if (s.Data != &d || s.Game != &g || s.FirePoint.s || s.Fires.e || s.OakValeFlag.count) { std::printf("00ee8100_TEST FAIL\n"); return 1; }
    if (s.OakValeFlag.header->left != s.OakValeFlag.header || s.OakValeFlag.header->right != s.OakValeFlag.header || s.OakValeFlag.header->parent) { std::printf("00ee8100_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 7; ++i) if (s.v[i].Data || s.v[i].Info) { std::printf("00ee8100_TEST FAIL\n"); return 1; }
    std::printf("00ee8100_TEST PASS\n");
    return 0;
}
