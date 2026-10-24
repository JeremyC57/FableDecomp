// Behaviour test for CRenderManager::OnViewportChanged (0x00A0AAC0): the hook is empty,
// so calling it must leave the render manager's state untouched.
#include <stdio.h>
#include <string.h>

class CRenderManager
{
public:
    unsigned char state[16];
    void OnViewportChanged();
};

void CRenderManager::OnViewportChanged()
{
}

int main()
{
    CRenderManager manager;
    unsigned char before[16];
    int i;
    for (i = 0; i < 16; ++i) manager.state[i] = (unsigned char)(i * 7 + 3);
    memcpy(before, manager.state, sizeof(before));
    manager.OnViewportChanged();
    manager.OnViewportChanged();
    if (memcmp(before, manager.state, sizeof(before)) != 0)
    {
        printf("FABLETLC_VIEWPORT_E2_BEHAVIOR FAIL\n");
        return 1;
    }
    printf("FABLETLC_VIEWPORT_E2_BEHAVIOR PASS\n");
    return 0;
}
