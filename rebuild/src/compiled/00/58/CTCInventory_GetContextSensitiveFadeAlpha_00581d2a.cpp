#pragma optimize("s",on)
// CTCInventory::GetContextSensitiveFadeAlpha(long)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00581D2A, oracle length 36 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  mov eax,[esp+4] ; mov ecx,[ecx+160h] ; imul eax,eax,1Ch ; add ecx,eax ; cmp byte ptr [ecx],0 ; jne C ; fld [__real@1.0] ; jmp R ; C: call CFadeInFadeOutBase::GetAlpha ; R: ret 4
// The 0x1C-byte elements are CFadeInFadeOutBase (engine header: +0x00 Active). Retail keeps the
// ContextSensitiveFadeTimes vector (`first` at +0) at +0x160 (PDB +0x188: ten std containers
// precede it and each is 4 bytes smaller in the shipped build). An inactive fader reads as fully opaque (1.0f, .rdata
// 0x0122DED8); otherwise its current alpha comes from the fader (0x004348D0, __fastcall).
//
// Lane feedback: element access through a retail-vector iterator object materialises the element pointer
// (`add ecx,eax ; cmp byte ptr [ecx],0`) like retail; the shared `ret 4` needs the "s" pragma,
// which the lander sweep applies. RELOCATION_MATCH["s"].
// Retail std::vector (12 bytes: first/last/end, no allocator slot) with an iterator OBJECT.
// VC7.1 does not fold `*(begin() + n)` through the iterator temporary the way it folds
// `first[n]`: the element pointer is materialised and the `first` load is not reused, which
// is exactly the retail instruction order. A plain `first[n]` differs by 2-4 bytes.
template<class T> struct CRetailVector {
    T* first; T* last; T* end_of_storage;
    struct iterator {
        T* p;
        explicit iterator(T* q) : p(q) {}
        iterator operator+(long n) const { return iterator(p + n); }
        T& operator*() const { return *p; }
    };
    iterator begin() { return iterator(first); }
    unsigned long size() const { return (unsigned long)(last - first); }
    T& operator[](unsigned long n) { return *(begin() + n); }
};

// Call-surface view of CFadeInFadeOutBase (0x1C-byte element of the vector).
struct CFadeInFadeOutBaseCalls {
    bool          Active;                // +0x00
    unsigned char _rest[0x1B];
    float GetAlpha();                    // 0x004348D0
};

// Retail CTCInventory view: the vector sits at +0x160 (PDB +0x188; every container member
// ahead of it is 4 bytes smaller in retail -- see the batch README container-size rule).
struct CTCInventory_Retail {
    unsigned char                          _pad_0x000[0x160];
    CRetailVector<CFadeInFadeOutBaseCalls> ContextSensitiveFadeTimes;   // +0x160
    float GetContextSensitiveFadeAlpha(long index);
};

float CTCInventory_Retail::GetContextSensitiveFadeAlpha(long index)
{
    // Inactive fader => fully opaque (1.0f from the .rdata constant pool), else the fader's alpha.
    // Retail shares one `ret 4` between the two arms: that is the size-optimised layout, which
    // the lander's pragma sweep selects (#pragma optimize("s",on)).
    if (!this->ContextSensitiveFadeTimes[index].Active)
        return 1.0f;
    return this->ContextSensitiveFadeTimes[index].GetAlpha();
}