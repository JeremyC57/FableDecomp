#pragma optimize("s",on)
class CWideString;
struct Tail;

// 0x5d97b0: thiscall on CWideString* (ecx), returns int
extern int __fastcall callee_5d97b0(CWideString* self);
// 0x5c08c0: fastcall ecx=m20, edx=m24, one stack arg=int, returns Tail*
extern Tail* __fastcall callee_5c08c0(void* ecx, void* edx, int arg);

struct Tail {
    void tail_5bf19c(CWideString* text, long p3, bool p4);
};

struct CTCInventoryBase {
    char pad[0x20];
    void* m20; // +0x20
    void* m24; // +0x24
    void GetItemDescription(CWideString* text, long p3, bool p4);
};

void CTCInventoryBase::GetItemDescription(CWideString* text, long p3, bool p4)
{
    int r = callee_5d97b0(text);
    Tail* t = (Tail*)callee_5c08c0(this->m20, this->m24, r);
    t->tail_5bf19c(text, p3, p4);
}