// Constant-return member: returns false. __fastcall this=ecx.
#include "engine/CNavSwitchableLeafNode.h"  // retyped onto the PDB layout; byte parity re-verified
struct CNavSwitchableLeafNode_Methods : CNavSwitchableLeafNode {
    bool Get(int region, int a1);
};
bool CNavSwitchableLeafNode_Methods::Get(int region, int a1) { return false; }