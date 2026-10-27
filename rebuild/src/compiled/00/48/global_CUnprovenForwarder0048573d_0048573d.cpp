// CUnprovenForwarder_0048573d() -- forward to the global object's member
// Leaf: ?CUnprovenForwarder_0048573d@@YAXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0048573D, 10 bytes: `mov ecx, offset 0x013B8A98 ; jmp 0x004855A3`.
// Shape (family K004, representative 0x0048573D): a forwarder whose only work is a member call on one global object, which
//   VC7.1 emits as the `this` load and a tail jump.
// Identity (descriptive): its jump target 0x004855A3 clears an STLport list (frees every node, relinks the sentinel) and its global 0x013B8A98 carries five conflicting stand-in names (g_ListRegistry / g_UIList / g_frontEndNodeList / ...); its only caller is CTCQuestCompletionUI::GlobalUpdate 0x005E78F0.
class CUnprovenList_013b8a98 {                           // the global list at 0x013B8A98 (owner unnamed)
public:
    void clear();                                        // 0x004855A3
};
extern CUnprovenList_013b8a98 g_CUnprovenList_013b8a98;  // 0x013B8A98
void CUnprovenForwarder_0048573d()
{
    g_CUnprovenList_013b8a98.clear();
}