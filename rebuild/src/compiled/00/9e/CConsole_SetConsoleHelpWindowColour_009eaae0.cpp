// CConsole::SetConsoleHelpWindowColour(CConsole&, const std::vector<CGenericVar>&) -- console command: sets the help-window colour from three numbers
// Retail: Fable.exe 0x009EAAE0, 39 bytes: reads args[0..2].ValULONG (element stride 0x1C, value at +0x10) and writes
// HelpWindowColour at +0x88 byte-wise (B +0x88 = args[2], G +0x89 = args[1], R +0x8A = args[0], A +0x8B = 0xFF).
// Identity: CConsole::Initialise 0x009ED190 registers each command as a CConsoleCommand<CConsole> (vtable 0x0129C4E0)
// whose handler pointer is stored at +0x18; the name built just before `mov [edi+18h],9EAAE0h` is the string
// "SetConsoleHelpWindowColour" (0x0129C65C). Ego_r.pdb: ?SetConsoleHelpWindowColour@CConsole@@SIXAAV1@ABV?$vector@
// VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z (public static __fastcall). The sibling handlers
// 0x009EAA50 / 0x009EAA80 / 0x009EAAB0 / 0x009EAB10 (Text / History / Client / ScrollBar) write +0x90 / +0x80 /
// +0x84 / +0x8C, the retail (STLport) offsets of the PDB's colour members.
// Layout: CConsole has no engine header; the Ego_r.pdb offsets are shifted by the Dinkumware container sizes, so the
// colour block is placed at the retail offsets from these handlers (descriptive padding before it).
// Leaf: ?SetConsoleHelpWindowColour@CConsole@@SIXAAV1@ABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z
class CCharString { public: void* PStringData; };

class CGenericVar {                              // Ego_r.pdb, 0x1C bytes, polymorphic
public:
    virtual ~CGenericVar();
    long          Type;                          // +0x04 EGenericVarType
    CCharString   ValString;                     // +0x08
    long          ValSLONG;                      // +0x0C
    unsigned long ValULONG;                      // +0x10
    bool          ValBool;                       // +0x14
    float         ValFloat;                      // +0x18
};

namespace std {
    template <class _Tp> class allocator { public: };
    template <class _Value, class _Tp, class _MaybeReboundAlloc>
    class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };
    template <class _Tp, class _Alloc> class _Vector_base {
    public:
        _Tp* _M_start;
        _Tp* _M_finish;
        _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    };
    template <class _Tp, class _Alloc = allocator<_Tp> >
    class vector : protected _Vector_base<_Tp, _Alloc> {
    public:
        typedef const _Tp& const_reference;
        typedef unsigned int size_type;
        const _Tp* begin() const { return this->_M_start; }
        const_reference operator[](size_type __n) const { return *(begin() + __n); }
    };
}

class CRGBColour {                               // D3DCOLOR byte order (Ego_r.pdb union: B, G, R, A)
public:
    unsigned char B, G, R, A;
    // Model helper (inline, no symbol): the channels arrive as full dwords and are narrowed at the store -- retail
    // loads args[1]/args[2] as dwords; the PDB's uchar Initialise/ctor narrows the loads to bytes instead.
    void SetChannels(long r, long g, long b, long a) { R = (unsigned char)r; G = (unsigned char)g; B = (unsigned char)b; A = (unsigned char)a; }
};

class CConsole {
public:
    static void __fastcall SetConsoleHelpWindowColour(CConsole& console, const std::vector<CGenericVar>& args);
    unsigned char _pad_0x00[0x80];               // vptr, binding maps, command lines, history, font ... (retail offsets)
    CRGBColour HistoryWindowColour;              // +0x80
    CRGBColour ClientWindowColour;               // +0x84
    CRGBColour HelpWindowColour;                 // +0x88
    CRGBColour ScrollBarColour;                  // +0x8C
    CRGBColour TextColour;                       // +0x90
};

void __fastcall CConsole::SetConsoleHelpWindowColour(CConsole& console, const std::vector<CGenericVar>& args)
{
    console.HelpWindowColour.SetChannels(args[0].ValULONG, args[1].ValULONG, args[2].ValULONG, 255);
}