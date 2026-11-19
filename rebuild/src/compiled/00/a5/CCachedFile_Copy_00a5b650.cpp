// CCachedFile::Copy(const CCachedFile& rhs) -- copy another cached file's state: position, open / valid flags, path name, buffer start, a shared clone of its disk file (or release ours), and its buffered bytes
// Leaf: ?Copy@CCachedFile@@AAEXABV1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A5B650, 184 bytes, `ret 4`. Ego_d.pdb: one parameter `rhs`, no locals.
// Callees: CWideString::operator= 0x0099B7D0, the disk file's virtual Clone (slot 14, +0x38), CCountedPointer<CAFile>::Reset 0x0050F550,
//   operator delete 0x00BFE9BC (the inline ReleaseData), memmove 0x00BFEAE6 (DataBuffer.clear), the random-access STLport __copy
//   0x009E9900 (__fastcall, hidden result in ecx, the tag and the (int*)0 distance pushed) for std::copy into back_inserter(DataBuffer).
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CCACHEDFILE_COPY
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_BACK_INSERTER
#define FABLE_RETAIL_STLP_COPY_GENERIC_INLINE
#define FABLE_RETAIL_STLP_ORIGINAL_COPY_DISPATCH_PASS22
#define FABLE_RETAIL_STLP_COPY_NOINLINE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CCachedFile.h"

void CCachedFile::Copy(const CCachedFile& rhs)
{
    Position = rhs.Position;
    Opened = rhs.Opened;
    Pathname = rhs.Pathname;
    BufferStartPosition = rhs.BufferStartPosition;
    BufferValid = rhs.BufferValid;
    if (!rhs.PDiskFile.IsNull())
        PDiskFile.Reset(rhs.PDiskFile->Clone());
    else
        PDiskFile.ReleaseData();
    DataBuffer.clear();
    std::copy(rhs.DataBuffer.begin(), rhs.DataBuffer.end(), std::back_inserter(DataBuffer));
}