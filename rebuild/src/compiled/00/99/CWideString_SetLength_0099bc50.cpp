// CWideString::SetLength(long) -- clear, resize a private copy, or allocate space-filled string storage
// Retail 0x0099BC50, 162 bytes. String/data layouts use fable_string.h; that header omits
// SetLength and keeps storage_ private, so this method view preserves its one-pointer ABI.
// MakeOwnCopyOfData0099B560, resize0099CD60, initialize0099CA70; free00BFEA14/delete00BFE9BC.
#ifndef FABLE_WIDE_STRING_HOST_TYPES
#include "fable_string.h"
#endif
extern "C" void __cdecl FreeWideCharacters(void*); // 0x00BFEA14
struct CWideAllocator { CWideAllocator() {} };
struct CWideStringDataActions : CWideStringData {
    long RemoveReference() { return --owners; }
    void Initialize(long length, wchar_t fill, const CWideAllocator&); // 0x0099CA70
    void Resize(long length, wchar_t fill); // 0x0099CD60
    CWideStringDataActions(long length, wchar_t fill, const CWideAllocator& allocator) {
        Initialize(length, fill, allocator);
        owners = 1;
    }
    ~CWideStringDataActions() { if (text) FreeWideCharacters(text); }
};
struct CWideStringLengthView {
    CWideStringDataActions* storage_;
    void MakeOwnCopyOfData(); // 0x0099B560
    void SetLength(long);
};
void CWideStringLengthView::SetLength(long length)
{
    if (length == 0) {
        if (storage_) {
            long owners = storage_->RemoveReference();
            if (owners <= 0) delete storage_;
            storage_ = 0;
        }
    } else if (storage_) {
        MakeOwnCopyOfData();
        storage_->Resize(length, 0);
    } else {
        storage_ = new CWideStringDataActions(length, L' ', CWideAllocator());
    }
}