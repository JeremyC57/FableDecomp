// PKCAutoDeleteArray::PKCAutoDeleteArray() -- constructs an empty owned heap array: the element pointer (+0) and the element count (+4) zeroed
// Leaf: ??0?$PKCAutoDeleteArray@G@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C167B0, 16 bytes, __thiscall `ret`, returns `this`:
//   mov eax,ecx ; mov dword ptr [eax],0 ; mov dword ptr [eax+4],0 ; ret
// Naming (sweep1, 2026-10-25): was landed as CBaseIntelligentPointer::CBaseIntelligentPointer (a propagated manifest label
//   over a pack(1) `T::Init` stand-in); FINDINGS chunk 439 ("0xC167B0 CBaseIntelligentPointer is an owned heap block").
//   The {data, count} block's bounds-checked accessor in the same TU, 0x00C16B10, warns
//   "d:\dev\audio\ver_fable_1.5_b\libpk\PKCAutoDeleteArray.h(77) : Index <n> is out of range (...)", hence the template
//   name; its Resize 0x00C169D0 frees through scalar operator delete 0x00BFE9BC (as does the sibling 0x00C167C0). The
//   element type is inferred from the same-TU Assign 0x00C16820, which sizes from the wcslen import (0x01440084), copies
//   2n bytes and stores a 0 word terminator: wchar_t (unsigned short under VC7.1 without /Zc:wchar_t). Callers (9):
//   0x00C2D1AC, 0x00C48336 .. 0x00C48596, 0x00C48DA1, 0x00C48F43 / 0x00C48F67 (two arrays in SDecoderMessage
//   0x00C48F30). One out-of-line copy for every caller, so one instantiation. No PDB covers libpk.
template<class T> class PKCAutoDeleteArray {    // 8 bytes (libpk PKCAutoDeleteArray.h)
public:
    T*            m_pData;                      // +0x00
    unsigned long m_count;                      // +0x04
    PKCAutoDeleteArray();
};

template<class T> PKCAutoDeleteArray<T>::PKCAutoDeleteArray()
    : m_pData(0), m_count(0)
{
}

template PKCAutoDeleteArray<unsigned short>::PKCAutoDeleteArray();
