// CGameScriptThing::DecrementScriptCounter() -- decrements the script reference counter at +0x14
// Retail: Fable.exe 0x008D4680, 4 bytes: `dec dword ptr [ecx+14h] ; ret`.
// Identity: CGameScriptThing vtable 0x012852CC slots 72/73/74 are 0x008D4670 / 0x008D4680 / 0x008D4690, the
// Increment/Decrement/GetScriptCounter triple CScriptThing declares in that order (Ego_r.pdb
// ?DecrementScriptCounter@CGameScriptThing@@UAEXXZ, public virtual).
// Layout: engine/CGameScriptThing.h flattens the vptr into a `__vftable` data member, so a virtual member cannot be
// defined on it; this local polymorphic model keeps the header's ScriptCounter at +0x14 (Header debt in NOTES.md).
// Leaf: ?DecrementScriptCounter@CGameScriptThing@@UAEXXZ
class CGameScriptThing {
public:
    virtual void DecrementScriptCounter();
    unsigned char _pad_0x04[0x10];          // +0x04 .. +0x13 (PThing at +0x0C)
    long ScriptCounter;                     // +0x14
};

void CGameScriptThing::DecrementScriptCounter()
{
    --ScriptCounter;
}