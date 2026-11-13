## 0x00AD9F10

LESSON: a callee's address-taken out-param (InsertBlock(index, added&)) only gets load-hoisted past an unrelated store when the callee's real body is visible in the TU (auto_inline(off)); a register-colour residue around such a local is callee visibility

