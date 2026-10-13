// CTCStockItem::FrameUpdate()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F5070, oracle length 37 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  push esi ; mov esi,ecx ; call <stock holder> ; test eax,eax ; je R ; mov al,[esi+0Ch] ; test al,al ; je R ; mov eax,[esi+18h] ; test eax,eax ; jne R ; mov ecx,[esi+4] ; pop esi ; jmp CThing::SetAsFrameUpdated ; R: pop esi ; ret
// engine/CTCStockItem.h (PDB): +0x0C bool ForSale, +0x14 CEnginePrimitiveHandle PrimHandle (8
// bytes; its second dword at +0x18 is the live handle id). +0x04 is the owning CThing (component
// base). A for-sale item that has a stock holder (0x007F4FC0, __fastcall, returns pointer) but no
// price-tag primitive yet asks its thing for a frame update (0x004C7C40, tail call) so the
// primitive gets created.
#include "engine/CTCStockItem.h"

struct CThingCalls {
    void SetAsFrameUpdated();            // 0x004C7C40
};

struct CTCStockItem_Methods : CTCStockItem {
    void* GetStockHolder();              // 0x007F4FC0
    void FrameUpdate();
};

void CTCStockItem_Methods::FrameUpdate()
{
    if (GetStockHolder() && this->ForSale && *(long*)&this->PrimHandle[4] == 0)
        reinterpret_cast<CThingCalls*>(*(void**)&this->_pad_0x04[0])->SetAsFrameUpdated();
}