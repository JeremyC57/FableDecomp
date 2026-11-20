// CFileInstaller::ProcessActiveFileForDecompression(CActiveFile& active_file) -- feed the file's zlib stream: decompress into the destination chunk and queue it for writing, or hand the stream the next completed read
// Leaf: ?ProcessActiveFileForDecompression@CFileInstaller@@IAEXAAVCActiveFile@1@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x009C3C20, 909 bytes, `ret 4`, no frame pointer (`sub esp, 1Ch`), no EH frame, six exits.
// Identity: Ego_r.pdb / Ego_d.pdb ?ProcessActiveFileForDecompression@CFileInstaller@@IAEXAAVCActiveFile@1@@Z (the shared header's
//   declaration; lib_file_installer, next to the landed ProcessActiveFileForRead 0x009C3AE0). Parameter and locals exactly as
//   Ego_d.pdb (debug length 1527): active_file; { cs }; { finished, file_finished, dst, dst_size { out_size, decompress_size }
//   { cs } { size, cs } }; { cs, it { cs } { pread { compressed_size, compressed_src { output_size } } } }.
// Shape: Ego_d.pdb's C13 line table (ref/009c3c20/egod_lines.tsv, source lines 621..726): 71 code lines there, 71 here, and the
//   VC7.1 /Od size of every line equals Ego_d's except where the VC10 debug build differs by rule (`this` at [ebp-0ACh]: +3 per use
//   and compiler temporaries at disp32; +7 per EH state store on the lock lines; checked list iterators on 689 / 690 / 714 and the
//   two returns that destroy `it`; long jumps). Line by line: 622 the stream test (22 = ours); 624 the missing-chunk test (12);
//   626 / 659 / 669 / 681 / 687 the five locks (27 = our 17 + this + EH); 627 Get().AllocateBuffer(false) (23 = ours); 628 (13);
//   631 the chunk test (16); 633 dst (23 = ours); 634 dst_size (24 = our 18 + this + a disp32 member); 635 finished (4); 636 the
//   three-term `while` (44 = ours); 638 the chunk size as a conditional expression (39 = our 30 + three disp32 temporaries; a
//   GFMin call would be about 30); 639 out_size (26 = ours); 640 the error accessor, a call in the debug build (18 = ours);
//   642 / 702 SetDirtyDiscError(true) (7); 643 return (5); 645..648 the three updates and Sleep(1) (21 / 9 / 9 / 8 = ours);
//   651 `bool file_finished = finished && active_file.DecompressFinish;` (53 = ours); 652 / 654 its test and the finish call
//   (8 / 11); 657 the second stream test (18); 660 / 682 Get().FreeBuffer (22); 661 / 683 (13); 665 the queue test (35 = our
//   29 + two this); 667 size (12); 670 push_back of a CPendingWrite temporary (47 = ours); 671 / 672 (13 / 21); 673 SetEvent
//   (19 = our 16 + this); 677 the else jump (5); 679 (12); 692 pread (13); 693 the position test (21); 695 / 696 the two locals
//   (11 / 9); 697 the first-block test (13 = our 9 + long jump); 699 the magic test, a memcmp CALL in the debug build (23 bytes
//   = push 4, the literal, the pointer, call, add esp, test, jump; retail has the intrinsic's four-byte compare through the
//   literal's address, 0x009C3EE9 `mov eax,[ebx] ; mov edx,offset "biz!" ; cmp eax,[edx]` -- a dereferenced literal folds to
//   `cmp dword ptr [ebx], imm32`, 906v909); 703 / 721 delete pread; 704 / 722 return; 706..708 output_size and the two eight-byte
//   steps (9 / 9 / 9 = ours); 710 StartWritingFile (19 = our 16 + this); 713 (24); 715 (15); 716 SetStreamInput (19); 717 the
//   last-block test (20); 719 (10).
//   The nested-if form (no early return after the allocation), the conditional expression and the file_finished statement are what
//   put retail's `finished` in the dead argument slot [esp+30h] and file_finished in [esp+13h]; the October draft (early returns,
//   explicit flag stores) was 877v909.
// Inline helpers expanded by retail: CFileInstaller::SetDirtyDiscError (out-of-line copy 0x009C3530; its FableWin text is defined
//   below as in every sibling row; two sites, 0x009C3DFC and 0x009C3EF4), CCompressorZlib::GetErrorFlag (0x009C3CE2 `mov cl,
//   [esi+8Eh]`; NEW overlay opt-in FABLE_RETAIL_CCOMPRESSORZLIB_STREAM_INPUT), the CPendingWrite constructor (NEW overlay opt-in
//   FABLE_RETAIL_CFILEINSTALLER_CPENDINGWRITE_CTOR), STLport list begin / end / iterator ++ / erase (0x009C3F50..0x009C3F5C: unlink,
//   free the node).
// Retail-volatile: 0x009C3E1C / 0x009C3F14 `mov dl / cl, byte ptr [eax+7Ah]` -- the discarded read of ExitThread in the inline
//   SetDirtyDiscError(true) (the shared header's volatile member, as in the landed siblings).
// Callees: 0x00A3A7E0 CCompressorZlib::HasStreamInput (three sites), 0x00A62920 / 0x00A629A0 CCriticalSectionLock constructor /
//   destructor, 0x00404440 CTBaseSingleton<CFileInstaller>::Get, 0x009C3A60 CFileInstaller::AllocateBuffer, 0x00A3A840
//   CCompressorZlib::DecompressStream, KERNEL32 Sleep (IAT 0x0143FE1C), 0x00A3ABB0 CCompressorZlib::FinishDecompressStream,
//   0x009C3330 CFileInstaller::FreeBuffer (two sites), 0x009C6AF0 list<CPendingWrite>::push_back, KERNEL32 SetEvent (IAT
//   0x0143FDE0), 0x009C3660 CFileInstaller::StartWritingFile, 0x00BFEA14 free (the list node), 0x00A3AB90
//   CCompressorZlib::SetStreamInput, 0x00BFE9BC operator delete (the read record, shared by both exits at 0x009C3F91).
// Globals: 0x013CA814 CFileInstaller::DirtyDisc; 0x0129B418 the literal "biz!".
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCOMPRESSORZLIB
#define FABLE_RETAIL_CCOMPRESSORZLIB_STREAM
#define FABLE_RETAIL_CCOMPRESSORZLIB_STREAM_INPUT
#define FABLE_RETAIL_CACOMPRESSOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CCRITICALSECTIONLOCK
#define FABLE_RETAIL_CTBASESINGLETON_GET_DECL
#define FABLE_RETAIL_CFILEINSTALLER_CPENDINGWRITE_CTOR
#include <string.h>
#include "engine/CFileInstaller.h"
#include "engine/CCriticalSectionLock.h"

extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long milliseconds);   // IAT 0x0143FE1C
extern "C" __declspec(dllimport) int __stdcall SetEvent(void* event);               // IAT 0x0143FDE0

inline void FABLE_FASTCALL CFileInstaller::SetDirtyDiscError(bool on)
{
    CCriticalSectionLock cs(CTBaseSingleton<CFileInstaller>::Get().CriticalSection, false);
    DirtyDisc = on;
    CTBaseSingleton<CFileInstaller>::Get().ExitThread = CTBaseSingleton<CFileInstaller>::Get().ExitThread || on;
}

void CFileInstaller::ProcessActiveFileForDecompression(CActiveFile& active_file)
{
    if (active_file.Decompressor.HasStreamInput())
    {
        if (active_file.PDecompressDstBuffer == 0)
        {
            CCriticalSectionLock cs(CriticalSection, false);
            active_file.PDecompressDstBuffer = Get().AllocateBuffer(false);
            active_file.DecompressDstUsed = 0;
        }

        if (active_file.PDecompressDstBuffer != 0)
        {
            unsigned char* dst = active_file.PDecompressDstBuffer->Mem + active_file.DecompressDstUsed;
            unsigned long dst_size = BufferChunkSize - active_file.DecompressDstUsed;
            bool finished = false;
            while (dst_size > 0 && !finished && active_file.Decompressor.HasStreamInput())
            {
                unsigned long decompress_size = dst_size < 0x40000 ? dst_size : 0x40000;
                unsigned long out_size = active_file.Decompressor.DecompressStream(dst, decompress_size, finished);
                if (active_file.Decompressor.GetErrorFlag())
                {
                    SetDirtyDiscError(true);
                    return;
                }
                active_file.DecompressDstUsed += out_size;
                dst += out_size;
                dst_size -= out_size;
                Sleep(1);
            }

            bool file_finished = finished && active_file.DecompressFinish;
            if (file_finished)
            {
                active_file.Decompressor.FinishDecompressStream();
            }

            if (!active_file.Decompressor.HasStreamInput())
            {
                CCriticalSectionLock cs(CriticalSection, false);
                Get().FreeBuffer(active_file.PDecompressSrcBuffer);
                active_file.PDecompressSrcBuffer = 0;
            }

            if (active_file.DecompressDstUsed == BufferChunkSize || file_finished)
            {
                unsigned long size = active_file.DecompressDstUsed;

                CCriticalSectionLock cs(CriticalSection, false);
                active_file.PendingWrites.push_back(CPendingWrite(active_file.DecompressDstPos, size, active_file.PDecompressDstBuffer));
                active_file.PDecompressDstBuffer = 0;
                active_file.DecompressDstPos += size;
                SetEvent(Event);
            }
        }
    }
    else
    {
        if (active_file.PDecompressSrcBuffer != 0)
        {
            CCriticalSectionLock cs(CriticalSection, false);
            Get().FreeBuffer(active_file.PDecompressSrcBuffer);
            active_file.PDecompressSrcBuffer = 0;
        }

        CCriticalSectionLock cs(CriticalSection, false);

        std::list<CActiveRead*>::iterator it;
        for (it = active_file.PendingDecompression.begin(); it != active_file.PendingDecompression.end(); ++it)
        {
            CActiveRead* pread = *it;
            if (pread->FilePos == active_file.DecompressSrcPos)
            {
                const unsigned char* compressed_src = pread->PBufferChunk->Mem;
                unsigned long compressed_size = pread->TransferSize;
                if (pread->FilePos == 0)
                {
                    if (memcmp(compressed_src, "biz!", 4) != 0)
                    {
                        SetDirtyDiscError(true);
                        delete pread;
                        return;
                    }
                    unsigned long output_size = *(const unsigned long*)(compressed_src + 4);
                    compressed_src += 8;
                    compressed_size -= 8;
                    StartWritingFile(active_file, output_size);
                }
                active_file.DecompressSrcPos += pread->TransferSize;
                active_file.PendingDecompression.erase(it);
                active_file.PDecompressSrcBuffer = pread->PBufferChunk;
                active_file.Decompressor.SetStreamInput(compressed_src, compressed_size);
                if (pread->FilePos + pread->TransferSize >= active_file.InputSize)
                {
                    active_file.DecompressFinish = true;
                }
                delete pread;
                return;
            }
        }
    }
}