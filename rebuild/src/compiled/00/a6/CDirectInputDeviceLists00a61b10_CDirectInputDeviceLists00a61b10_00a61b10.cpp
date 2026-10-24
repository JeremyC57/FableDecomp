// CDirectInputDeviceLists_00a61b10::~CDirectInputDeviceLists_00a61b10() -- release every DirectInput interface held in the two lists, then destroy the lists (second, then first)
// Leaf: ??1CDirectInputDeviceLists_00a61b10
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A61B10, length 24 bytes, `jmp` tail. Caller: CInputManagerDX's destructor
// 0x009C01C0 on its member at +0x2b0 (`lea ecx,[esi+0x2b0] ; call 0xA61B10`). The member class is not named by any
// table (descriptive name); the manifest label (_Dest_val<allocator<pair<CCharString,CSymbolInfo>>>) is propagated.
// Callees: 0x00A61A80 = the release-all body (for each of the two 4-byte STLport lists, while non-empty: COM Release()
// (`push p ; call [vtbl+8]`) on the last element, unlink the node and free it); 0x00A61D20 = the list's out-of-line
// destructor (manifest "CGuiControlTreePane::SortTreeRecursively", propagated).
// Shape: user destructor body = the release-all call; then the members in reverse order, the first list as the tail jmp.
struct CInterfaceList {                                      // STLport list<IUnknown*> (4 bytes: the node pointer)
    void* m_pNode;
    ~CInterfaceList();                                       // 0x00A61D20
};

struct CDirectInputDeviceLists_00a61b10 {
    CInterfaceList m_devices;                                // +0x00
    CInterfaceList m_effects;                                // +0x04
    void ReleaseAll();                                       // 0x00A61A80
    ~CDirectInputDeviceLists_00a61b10();
};

CDirectInputDeviceLists_00a61b10::~CDirectInputDeviceLists_00a61b10()
{
    ReleaseAll();
}