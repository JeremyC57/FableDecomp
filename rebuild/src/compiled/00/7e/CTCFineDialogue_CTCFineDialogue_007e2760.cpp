// CTCFineDialogue::~CTCFineDialogue() -- the fine-dialogue thing component leaves the global TC list, then the CTCBase
// destructor runs (retail stores vtable 0x01270274 = ??_7CTCFineDialogue@@6B@, calls 0x00686DF0, tail-jumps 0x00686830)
// Leaf: ??1CTCFineDialogue@@UAE@XZ
// Retail: Fable.exe @ 0x007E2760, 22 bytes. Family pass 3 had modelled it as a CInputProcessManager destructor
// (g_vtbl_CInputProcessManager bound to 0x01270274); the vtable it stores is CTCFineDialogue's (reloc_bindings
// ??_7CTCFineDialogue@@6B@ -> 0x01270274, catalog name CTCFineDialogue::~CTCFineDialogue).

class CTCBase {
public:
    virtual ~CTCBase();               // 0x00686830
    void RemoveFromGlobalTCs();       // 0x00686DF0
};

class CTCFineDialogue : public CTCBase {
public:
    virtual ~CTCFineDialogue();
};

CTCFineDialogue::~CTCFineDialogue()
{
    RemoveFromGlobalTCs();
}
