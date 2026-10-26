// FreeBuffer — hand the buffer back to the owner, then release it through the engine
// free helper (0xbfea14). __fastcall this=ecx, no args. The second `this->m20` read is
// faithful: the member call may clear it.
struct T {
    char pad00[0x0c];
    void* mC;
    char pad10[0x0c];
    char* m1C;
    void* m20;
    void FreeBuffer();
    void Release_007b3b50(void* a, void* b);
};
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)
void T::FreeBuffer() {
    if (this->m20) {
        this->Release_007b3b50(this->mC, this->m1C + 4);
        if (this->m20) free(this->m20);
    }
}