// Grade: DIFFER(ours v retail) -- 57v59: ours shrink-wraps the esi save to the find loop (push after the empty je, pop before the end test, `pop ecx` after free); retail saves esi at entry and pops it at the end (`add esp,4`)
// CDisplayManager::RemoveDeviceResetCallback(CDeviceResetCallback* callback) -- finds the callback in the DeviceResetCallbacks list and unlinks and frees its node
// Leaf: RemoveDeviceResetCallback
// Retail: Fable.exe (TLC, VC7.1) @ 0x009BF5A0, 57 bytes (+ int3 padding), `ret 4`.
// Identity: NOT LSocketServer::CloseSocket (propagated label). The landed callers CSystemManager::Uninitialise 0x009A60D0
// and CEngine::Uninitialise 0x00B25D80 call it as CDisplayManager::RemoveDeviceResetCallback; Ego_r.pdb CDisplayManager has
// list<CDeviceResetCallback*> DeviceResetCallbacks at +0x264 (612), the offset the body reads.
// Callee: free @ 0x00BFEA14 (the STLport node allocator).
// Layout: no engine header for CDisplayManager; local model with the PDB member at its offset. Retail STLport list is the
// sentinel pointer (node: next +0, prev +4, data +8).
// Shape: STLport find() inlined (entry `first != last` test, compare, advance), the end re-read for the `!= end()` test,
// then erase(pos): next/prev loaded, prev->next = next, next->prev = prev, node freed.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

class CDeviceResetCallback;

struct CCallbackListNode {                         // STLport _List_node<CDeviceResetCallback*>
    CCallbackListNode*    _M_next;
    CCallbackListNode*    _M_prev;
    CDeviceResetCallback* _M_data;
};

struct CCallbackList {                             // STLport list<CDeviceResetCallback*>
    CCallbackListNode* _M_node;
    CCallbackListNode* begin() const { return _M_node->_M_next; }
    CCallbackListNode* end() const { return _M_node; }
    void erase(CCallbackListNode* position)
    {
        CCallbackListNode* next = position->_M_next;
        CCallbackListNode* prev = position->_M_prev;
        prev->_M_next = next;
        next->_M_prev = prev;
        free(position);
    }
};

inline CCallbackListNode* Find(CCallbackListNode* first, CCallbackListNode* last, CDeviceResetCallback* const& value)
{
    while (first != last && !(first->_M_data == value))
        first = first->_M_next;
    return first;
}

struct CDisplayManager {
    unsigned char _pad000[0x264];                  // +0x000 SystemManager .. RenderManagerInitFlags
    CCallbackList DeviceResetCallbacks;            // +0x264
    void RemoveDeviceResetCallback(CDeviceResetCallback* callback);
};

void CDisplayManager::RemoveDeviceResetCallback(CDeviceResetCallback* callback)
{
    CCallbackListNode* it = Find(DeviceResetCallbacks.begin(), DeviceResetCallbacks.end(), callback);
    if (it != DeviceResetCallbacks.end())
        DeviceResetCallbacks.erase(it);
}
