// NUISystem::CIME::Get() -- returns the IME singleton object at 0x013B7CD8
// Leaf: ?Get@CIME@NUISystem@@SIAAV12@XZ
// Identity (naming wave 11): Ego_r.pdb 0x00404100 + FableWin.pdb 0x018BD220 ?Get@CIME@NUISystem@@SIAAV12@XZ (first CIME member in both retail and Ego_r); the object is 0x54 bytes = Ego_r sizeof CIME.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00404A80, 6 bytes. Callees: none.
//
// `mov eax, offset <object>; ret`: GFUninitialise 0x00401B80 calls CIME::Shutdown 0x004051C0 on the result, the front end
// IsIMEActive / FinaliseString / SetEditBoxCaretPos / ShouldEditBoxShowCaret. The AddChildPrimitive label was propagated.
//
// Matching: returns the address of a global (absolute relocation), not its contents.
namespace NUISystem {
class CIME {
public:
    unsigned char _body[0x54];                            // Ego_r.pdb sizeof CIME = 0x54
    static CIME& __fastcall Get();
};
}
extern unsigned char g_primitiveVTableOrData[0x54];   // the CIME object at 0x013B7CD8 (bound data spelling kept)

NUISystem::CIME& NUISystem::CIME::Get()
{
    return *(CIME*)g_primitiveVTableOrData;
}
