#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CBaseIntelligentPointer { virtual void v00() { } void* m_pItem; void* GetPItem() const { return m_pItem; } }; struct CTask { unsigned char _pad[8]; CBaseIntelligentPointer m_horn; };
static void* g_destroyed = 0; struct CCountedPointer { CTask* m_pObject; void* m_pCount; ~CCountedPointer() { std::strcat(g_seq, "D"); g_destroyed = this; } };
struct CIsHornTaskWithThisHorn : CBaseIntelligentPointer { bool operator()(CCountedPointer task) { void* horn = GetPItem(); bool same = task.m_pObject->m_horn.GetPItem() == horn; return same; } };

int main() {
    int horn, other; CIsHornTaskWithThisHorn f; f.m_pItem = &horn; CTask t; t.m_horn.m_pItem = &horn; CCountedPointer p; p.m_pObject = &t; p.m_pCount = 0; bool same = f(p); if (!same || std::strchr(g_seq, 'D') == 0 || g_destroyed == 0) { std::printf("%s\n", g_seq); std::printf("0065f116_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; t.m_horn.m_pItem = &other; bool second = f(p); if (second || std::strchr(g_seq, 'D') == 0) { std::printf("0065f116_TEST FAIL\n"); return 1; }
    std::printf("0065f116_TEST PASS\n"); return 0;
}
