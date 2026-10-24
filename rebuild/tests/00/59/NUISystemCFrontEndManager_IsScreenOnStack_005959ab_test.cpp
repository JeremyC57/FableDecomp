// Behaviour test for 0x005959AB NUISystem::CFrontEndManager::IsScreenOnStack (model copy; the stack is copied, not consumed)
#include <cstdio>
struct Component { int id; };
struct Stack {
    Component* items[8]; int count;
    bool empty() const { return count == 0; }
    Component*& top() { return items[count - 1]; }
    void pop() { --count; }
};
struct FrontEndManager {
    Stack m_stackScreens;
    bool IsScreenOnStack(Component* screen) const
    {
        Stack screens = m_stackScreens;
        while (!screens.empty()) {
            if (screens.top() == screen) return true;
            screens.pop();
        }
        return false;
    }
};
int main()
{
    Component a, b, c, d; a.id = 1; b.id = 2; c.id = 3; d.id = 4;
    FrontEndManager m; m.m_stackScreens.count = 0;
    m.m_stackScreens.items[m.m_stackScreens.count++] = &a;
    m.m_stackScreens.items[m.m_stackScreens.count++] = &b;
    m.m_stackScreens.items[m.m_stackScreens.count++] = &c;
    if (!m.IsScreenOnStack(&a) || !m.IsScreenOnStack(&c) || m.IsScreenOnStack(&d)) { std::printf("005959ab_TEST FAIL\n"); return 1; }
    if (m.m_stackScreens.count != 3) { std::printf("005959ab_TEST FAIL\n"); return 1; }
    m.m_stackScreens.count = 0;
    if (m.IsScreenOnStack(&a)) { std::printf("005959ab_TEST FAIL\n"); return 1; }
    std::printf("005959ab_TEST PASS\n");
    return 0;
}