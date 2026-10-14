// CNavigatorQueueManager::IsNavigatorAllowedToCalculateRoute(void* navigator) -- true when the navigator is one of the first two entries (+0xc of the nodes) of the queue list (the sentinel at +0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A15420, length 45 bytes.
// Leaf: IsNavigatorAllowedToCalculateRoute
// Shape: the index in edx checked against 2 at the loop head (aligned with a `nop`), the navigator in esi, `ret 4`.
struct SQueueNode_00a15420 {
    SQueueNode_00a15420* m_pNext;                  // +0x00
    SQueueNode_00a15420* m_pPrev;                  // +0x04
    long                 m_8;
    void*                m_pNavigator;             // +0x0c
};

struct CNavigatorQueueManager {
    SQueueNode_00a15420* m_pQueue;                 // +0x00: the sentinel
    bool IsNavigatorAllowedToCalculateRoute(void* navigator);
};

bool CNavigatorQueueManager::IsNavigatorAllowedToCalculateRoute(void* navigator)
{
    long index = 0;
    for (SQueueNode_00a15420* node = m_pQueue->m_pNext; node != m_pQueue; node = node->m_pNext, ++index) {
        if (index >= 2)
            break;
        if (node->m_pNavigator == navigator)
            return true;
    }
    return false;
}
