// CMapNodeAllocator::Release(PKCTRRedblackNode** node) -- hand the node to the allocator's virtual Free (slot 0) and clear the caller's pointer
// Leaf: ?Release@CMapNodeAllocator@@QAEXPAPAUPKCTRRedblackNode@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1B1E0, 22 bytes (`mov eax,[ecx] ; push esi ; mov esi,[esp+8] ; mov edx,[esi]
//   ; push edx ; call [eax] ; mov dword ptr [esi],0 ; pop esi ; ret 4`), thiscall.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 433 "0xC1B1E0 CTexture::GetNoLevels = node allocator Release"): the only
//   caller is PKCTRRedblackMap::Remove 0x00C1B7D0 (agent433), which loads the map's allocator (`mov ecx,[edi]`, the map's
//   +0 PAllocator) and passes the address of the unlinked node (`lea edx,[esp+0Ch] ; push edx ; call 0x00C1B1E0`); slot
//   0 of the allocator vtable 0x012B30EC is Free 0x00C1B320 (operator delete). Descriptive names (no PDB covers libpk).
// Label correction: the landed row was a free `long __fastcall GetNoLevels(CTexture*, int, void*)` with a dummy edx
//   (catalog "CTexture::GetNoLevels", propagated).
struct PKCTRRedblackNode;

class CMapNodeAllocator {                       // vtable 0x012B30EC
public:
    virtual void Free(void* node);              // slot 0 (0x00C1B320)
    virtual void* Allocate();                   // slot 1 (0x00C1B330)
    void Release(PKCTRRedblackNode** node);
};

void CMapNodeAllocator::Release(PKCTRRedblackNode** node)
{
    Free(*node);
    *node = 0;
}
