// PKCFrallocStatic::Get(unsigned long index) -- EH-framed bounds-checked element access of the libpk static pool, here PKCFrallocStatic<CPlaybackSlot> (0x2c-byte elements): an index at or past the count at +4 logs `PKCFrallocStatic.h(31) : Error: Fralloc overflow` through the library's warning block and breaks; the element of the block at +0x10 is returned regardless
// Leaf: ?Get@?$PKCFrallocStatic@UCPlaybackSlot@@@@QAEPAUCPlaybackSlot@@K@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C13700, oracle length 202 bytes. Built under /EHsc.
// Retail bytes:  ... mov esi,ecx ; mov eax,[esi+4] ; push edi ; mov edi,[esp+1Ch] ; cmp edi,eax ; jb ok ; mov eax,[_afxPchNil] ; mov [esp+8],eax ; mov edx,offset "d:\dev\audio\ver_fable_1.5_b\libpk\PKCFrallocStatic.h" ; lea ecx,[esp+8] ; mov dword ptr [esp+14h],0 ; call operator<< ; ... operator<<(1Fh) ... ; mov eax,[g_pfnAssertHook] ; test eax,eax ; je skip ; mov ecx,[esp+8] ; call eax ; skip: lea ecx,[esp+8] ; mov dword ptr [esp+14h],-1 ; call ~CString ; lea ecx,[esp+1Ch] ; call CDebugBreak::CDebugBreak ; ok: mov ecx,[esi+10h] ; mov eax,edi ; imul eax,eax,2Ch ; add eax,ecx ; ... ret 4
// Identity (sweep1, 2026-10-25): the class is the header the warning names (libpk PKCFrallocStatic.h line 31), a
//   template: 0x00C13620 is the same accessor over the CSoundEventManager's 0xb4-byte CSoundEvents. This instantiation
//   is the manager's playback pool at +0x224 (count +0x228; TracePlaybacks 0x00C127C0 walks it, call 0x00C1280A); the
//   other call sites are 0x00C11F95, 0x00C1201A and 0x00C1C517. The 0x2c-byte element is named descriptively
//   (CPlaybackSlot); no PDB covers libpk.
// Callees: CString::operator<<(const char*) 0x00C0DEC0 / operator<<(long) 0x00C0DF00 (`__fastcall`), ~CString 0x00C0BA40,
//   the optional assert hook @ 0x0143BB2C, CDebugBreak 0x00C16750 (an empty temporary homed in the dead argument slot).
// Label correction: the landed row named it CFrallocStatic_00c13700::GetSlot over CSlot_00c13700 with an address-named
//   string class; the catalog's "TextureIndex_GetByProperties" is propagated.
extern char* _afxPchNil;                        // 0x0139C900
typedef void (__fastcall* PFNASSERTHOOK)(const char* message);
extern PFNASSERTHOOK g_pfnAssertHook;           // 0x0143BB2C

class CString {                                 // the libpk MFC-style string: one word
public:
    char* m_pchData;
    CString() : m_pchData(_afxPchNil) { }
    ~CString();                                 // 0x00C0BA40
    CString& __fastcall operator<<(const char* text);   // 0x00C0DEC0
    CString& __fastcall operator<<(long value);         // 0x00C0DF00
};

struct CDebugBreak {                            // empty temporary whose constructor breaks into the debugger
    CDebugBreak();                              // 0x00C16750
};

struct CPlaybackSlot {                          // 0x2c-byte playback-pool element
    unsigned char _body[0x2c];
};

template<class T> class PKCFrallocStatic {
public:
    long m_0;
    unsigned long m_count;                      // +4
    long m_8;
    long m_c;
    T* m_pData;                                 // +0x10
    T* Get(unsigned long index);
};

template<class T> T* PKCFrallocStatic<T>::Get(unsigned long index)
{
    if (index >= m_count) {
        {
            CString line;
            line << "d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCFrallocStatic.h" << "(" << (long)31 << ") : " << "Error: Fralloc overflow" << "\n";
            if (g_pfnAssertHook)
                g_pfnAssertHook(line.m_pchData);
        }
        CDebugBreak();
    }
    return &m_pData[index];
}

template CPlaybackSlot* PKCFrallocStatic<CPlaybackSlot>::Get(unsigned long);
