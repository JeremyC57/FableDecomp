// Grade: DIFFER(373v373) -- base/index operand order of `buffer[size] = 0` (`[eax+esi]` vs ours `[esi+eax]`), same gap as 009bb0d0 (advisor-confirmed); also tried GetSize(), a p-local alias, size[buffer], unsigned buffer
// CSymbolMap::LoadFromFile(const CWideString& filename) -- reset the symbol map, read the whole file into a zero-terminated buffer, strip // and /* */ comments, parse it, sort the symbol table and drop spare capacity
// Leaf: LoadFromFile
// Retail 0x009E5C90, 373 bytes, `ret 4`.
// Callees: the symbol vector-map's clear 0x009E79E0, set<CCharString>::_M_erase(node) 0x004ABA70 (inline clear), the current
// file-name CWideString 0x013CAA34 operator=(const CWideString&) 0x0099B7D0 / operator=(const wchar_t*) 0x0099B800 (L""
// 0x0122D70C), CBase::CBase 0x0099A2F0 / ~CBase 0x0099A300, CWideString() 0x0099AED0 / ~CWideString 0x0099B510,
// CDiskFileWin32::Open(const CWideString&, FileAccessType, ulong) 0x0099A6A0 / Read(void*, long, CASuspendableProcess*)
// 0x0099A960 / Close 0x0099A920 (vtable 0x0122D06C), malloc 0x00BFEA0E / free 0x00BFEA14, GFRemoveComments(char*, const
// char*, const char*) 0x00A60410 (`__fastcall`; "//" "\n", "//" "\r", "/*" "*/"), CSymbolMap::ParseBuffer 0x009E5AD0, the
// vector-map's introsort 0x009E7E40 (`__fastcall`, comparator byte by value), DeallocateUnusedMemory 0x009E7C70.
// Layout: Ego_r.pdb CSymbolMap with retail 12-byte containers: SLONGMap +4 (first +0, last +4, comparator +0xC, unsorted
// flag +0xD), NamesUsed set +0x14 (header pointer, count +4); CDiskFileWin32 0x1C bytes (size +0x0C, open flag +0x14, name
// +0x18).
// Shape: the file object sets its open flag in the constructor BODY (an init-list store is scheduled before the Open
// pushes); the result flag is a frame byte set to `true` only on a good parse; the set clear is STLport's inline
// `_Rb_tree::clear`; the buffer is `n ? malloc(n) : 0` plus an inline memset; the file object's user destructor closes it.
extern "C" void* __cdecl malloc(unsigned int size);       // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
extern "C" void* __cdecl memset(void* dst, int value, unsigned int size);
#pragma intrinsic(memset)
struct CWideString {
    void* PStringData;
    CWideString();                                         // 0x0099AED0
    ~CWideString();                                        // 0x0099B510
    CWideString& operator=(const CWideString& other);      // 0x0099B7D0
    CWideString& operator=(const unsigned short* text);    // 0x0099B800
};
extern CWideString g_CurrentSymbolFile;                    // 0x013CAA34
void __fastcall GFRemoveComments(char* buffer, const char* start, const char* end);   // 0x00A60410
struct CBase {
    virtual ~CBase();                                      // 0x0099A300
    CBase();                                               // 0x0099A2F0
};
struct CASuspendableProcess;
struct CDiskFileWin32 : CBase {                            // vtable 0x0122D06C
    long          m_handle;                                // +0x04
    long          m_position;                              // +0x08
    long          m_size;                                  // +0x0c
    long          m_mode;                                  // +0x10
    bool          m_open;                                  // +0x14
    CWideString   m_name;                                  // +0x18
    CDiskFileWin32() { m_open = false; }
    ~CDiskFileWin32() { Close(); }
    virtual void Open(const CWideString& name, long access, unsigned long flags);   // 0x0099A6A0
    virtual void Read(void* buffer, long size, CASuspendableProcess* process);      // 0x0099A960
    virtual void Close();                                  // 0x0099A920
};
struct SSymbolEntry;
struct CSymbolCompare { unsigned char m_byte; };
void __fastcall SortSymbols(SSymbolEntry* first, SSymbolEntry* last, CSymbolCompare compare);   // 0x009E7E40
struct CSymbolVectorMap {
    SSymbolEntry*  m_first;                                // +0x00
    SSymbolEntry*  m_last;                                 // +0x04
    SSymbolEntry*  m_end;                                  // +0x08
    CSymbolCompare m_compare;                              // +0x0c
    bool           m_unsorted;                             // +0x0d
    void clear();                                          // 0x009E79E0
    void DeallocateUnusedMemory();                         // 0x009E7C70
    void Sort()
    {
        if (m_unsorted) {
            SortSymbols(m_first, m_last, m_compare);
            m_unsorted = false;
        }
    }
};
struct SNameNode {
    long       Color;
    SNameNode* Parent;                                     // +0x04
    SNameNode* Left;                                       // +0x08
    SNameNode* Right;                                      // +0x0c
};
struct CNameSet {                                          // STLport set<CCharString>
    SNameNode*    _M_header;                               // +0x00
    unsigned long _M_node_count;                           // +0x04
    unsigned long _M_compare;                              // +0x08
    void _M_erase(SNameNode* node);                        // 0x004ABA70
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_header->Parent);
            _M_header->Left = _M_header;
            _M_header->Parent = 0;
            _M_header->Right = _M_header;
            _M_node_count = 0;
        }
    }
};
struct CSymbolMap {
    void*            __vftable;                            // +0x00
    CSymbolVectorMap SLONGMap;                             // +0x04
    CNameSet         NamesUsed;                            // +0x14
    bool ParseBuffer(char* buffer);                        // 0x009E5AD0
    bool LoadFromFile(const CWideString& filename);
};

bool CSymbolMap::LoadFromFile(const CWideString& filename)
{
    bool ok = false;
    SLONGMap.clear();
    NamesUsed.clear();
    g_CurrentSymbolFile = filename;
    CDiskFileWin32 file;
    file.Open(filename, 1, 2);
    unsigned long length = file.m_size + 1;
    char* buffer = length ? (char*)malloc(length) : 0;
    memset(buffer, 0, length);
    *(file.m_size + buffer) = 0;
    file.Read(buffer, file.m_size, 0);
    GFRemoveComments(buffer, "//", "\n");
    GFRemoveComments(buffer, "//", "\r");
    GFRemoveComments(buffer, "/*", "*/");
    if (ParseBuffer(buffer))
        ok = true;
    SLONGMap.Sort();
    SLONGMap.DeallocateUnusedMemory();
    g_CurrentSymbolFile = (const unsigned short*)L"";
    if (buffer)
        free(buffer);
    return ok;
}
