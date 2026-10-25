// PKCFrallocStatic::Get(unsigned long index) -- EH-framed bounds-checked element access of the libpk static pool, here PKCFrallocStatic<CSoundEvent> (0xb4-byte elements): an index at or beyond the count at +4 logs `PKCFrallocStatic.h(31) : Error: Fralloc overflow` through the library's warning block and breaks; the element at the index of the block at +0x10 is returned regardless
// Leaf: ?Get@?$PKCFrallocStatic@UCSoundEvent@@@@QAEPAUCSoundEvent@@K@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C13620, oracle length 205 bytes. Built under /EHsc.
// Retail bytes:  ... mov eax,[esi+4] ; push edi ; mov edi,[esp+1Ch] ; cmp edi,eax ; jb ok ; mov eax,[_afxPchNil] ; mov [esp+8],eax ; mov edx,offset file ; lea ecx,[esp+8] ; mov dword ptr [esp+14h],0 ; call CString::operator<< ; ... operator<<(1Fh) ... ; mov eax,[g_pfnAssertHook] ; test eax,eax ; je rel ; mov ecx,[esp+8] ; call eax ; rel: lea ecx,[esp+8] ; mov dword ptr [esp+14h],-1 ; call ~CString ; lea ecx,[esp+1Ch] ; call CDebugBreak::CDebugBreak ; ok: mov ecx,[esi+10h] ; mov eax,edi ; imul eax,eax,0B4h ; add eax,ecx ; ... ret 4
// Identity (sweep1, 2026-10-25): the class is the header the warning names, "d:\dev\audio\ver_fable_1.5_b\libpk\
//   PKCFrallocStatic.h" (line 31, "Error: Fralloc overflow"), a template: the byte-identical-but-for-the-stride
//   0x00C13700 is the same accessor over 0x2c-byte elements. This instantiation is the CSoundEventManager's +0x5C pool
//   of 0xb4-byte CSoundEvents: its 12 call sites (0x00C11CC5 .. 0x00C128C3) are the manager walks (ReleaseTarget
//   0x00C12140, Shutdown 0x00C123B0,
//   CountWaitingEvents 0x00C125E0, CountPlayingEvents 0x00C12680, TraceEvents 0x00C12720, Update 0x00C12880 ...),
//   which hand each element to CSoundEvent::IsActive 0x00C20840. Descriptive method name (no PDB covers libpk).
// Callees: the library warning block (as in PKCFrallocStatic::Push 0x00C22B40 and CInstanceManager::Assign 0x00C2D5E0):
//   CString::operator<<(const char*) 0x00C0DEC0 / operator<<(long) 0x00C0DF00 (`__fastcall`, returning the string),
//   ~CString 0x00C0BA40, the optional assert hook @ 0x0143BB2C, CDebugBreak 0x00C16750 (an empty temporary homed in
//   the dead argument slot).
// Label correction: the landed row named the class CEntityArray_00c13620 over CEntity_00c13620; the catalog's
//   "VectorBoundsCheckOrThrow" is descriptive. Callers in other landed rows model it as CSoundEventPool::Get /
//   GetAt / operator[] (the same retail target).
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

struct CSoundEvent {                            // 0xb4 bytes
    unsigned char _body[0xb4];
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

template CSoundEvent* PKCFrallocStatic<CSoundEvent>::Get(unsigned long);
