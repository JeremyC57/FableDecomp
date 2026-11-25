#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x004E769F now binds its PDB spelling ??1CTCLightningNode@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCLightningNode_Destruct@@YIXPAX@Z.
class CTCLightningNode { public: virtual ~CTCLightningNode(); };
void* __fastcall CTCLightningNode_ScalarDeletingDestructor(void* node, int, unsigned int flags)
{
    reinterpret_cast<CTCLightningNode*>(node)->CTCLightningNode::~CTCLightningNode();
    if (flags & 1)
        operator delete(node);
    return node;
}