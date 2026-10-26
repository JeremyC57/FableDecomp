// CConsole::ScrollToHistoryBeginning() -- show the oldest history lines: the visible window starts at begin() and spans min(size, lines onscreen)
// Retail: Fable.exe (TLC, VC7.1) @ 0x009EAC80, 143 bytes. PDB: ?ScrollToHistoryBeginning@CConsole@@AAEXXZ (the CConsole
//   method whose behaviour fits: both visible-window iterators reset to begin(); no xref proof). Catalog label
//   "LinkedList_RotateToIndex" is wrong.
// Shape: STLport list (sentinel pointer only); size() is distance(begin(), end()) and is evaluated twice (once per
//   branch); advance() is the bidirectional __advance (if n > 0 forward, else backward) with the member iterator as
//   the loop variable (re-read / re-stored each step). The long line count against the unsigned size compares unsigned
//   (jbe); the second advance takes the unsigned size, so its backward loop survives behind a je.
// Layout: retail CConsole offsets as in the landed CConsole::SetHistoryMode 0x009EAB80: CommandLineHistory +0x4c,
//   VisibleHistoryStart +0x50, VisibleHistoryFinish +0x54, NumberOfHistoryLinesOnscreen +0x58.
// Leaf: ?ScrollToHistoryBeginning@CConsole_Retail@@QAEXXZ
namespace std {

struct input_iterator_tag { input_iterator_tag() {} };
struct bidirectional_iterator_tag : input_iterator_tag { bidirectional_iterator_tag() {} };

struct _List_node_base
{
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp> struct _List_node : _List_node_base
{
    _Tp _M_data;
};

template <class _Tp> struct _List_iterator
{
    _List_node_base* _M_node;
    _List_iterator() {}
    _List_iterator(_List_node_base* __x) : _M_node(__x) {}
    _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    _List_iterator& operator--() { _M_node = _M_node->_M_prev; return *this; }
    bool operator==(const _List_iterator& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator& __y) const { return _M_node != __y._M_node; }
};

template <class _InputIterator, class _Distance>
inline void __distance(const _InputIterator& __first, const _InputIterator& __last, _Distance& __n, const input_iterator_tag&)
{
    _InputIterator __it(__first);
    while (__it != __last) { ++__it; ++__n; }
}

template <class _BidirectionalIterator, class _Distance>
inline void __advance(_BidirectionalIterator& __i, _Distance __n, const bidirectional_iterator_tag&)
{
    if (__n > 0)
        while (__n--) ++__i;
    else
        while (__n++) --__i;
}

template <class _InputIterator, class _Distance>
inline void advance(_InputIterator& __i, _Distance __n)
{
    __advance(__i, __n, bidirectional_iterator_tag());
}

template <class _Tp> class list
{
public:
    typedef _List_iterator<_Tp> iterator;
    _List_node_base* _M_node;
    iterator begin() const { return iterator(_M_node->_M_next); }
    iterator end() const { return iterator(_M_node); }
    unsigned int size() const
    {
        unsigned int __result = 0;
        __distance(begin(), end(), __result, input_iterator_tag());
        return __result;
    }
};

} // namespace std

class CCharString { public: void* PStringData; };

class CConsoleCommandLine                                      // vtable 0x0129C400 (16 bytes)
{ public:
    virtual void v00();
    CCharString CurrentEntry;                                   // +0x04
    bool        UserEntry;                                      // +0x08
    long        CursorPosition;                                 // +0x0c
};

struct CConsole_Retail                                          // retail offsets (see the landed CConsole::SetHistoryMode 0x009EAB80)
{
    virtual void v00();
    unsigned char       _maps[0x18];                            // +0x04 KeyBindingMap, +0x10 StringBindingMap (12 bytes each)
    CConsoleCommandLine FrontCommandLine;                       // +0x1c
    CConsoleCommandLine BackCommandLine;                        // +0x2c
    CCharString         HelpBarString;                          // +0x3c
    unsigned char       _registered[0x0c];                      // +0x40 RegisteredConsoleInputs
    std::list<CConsoleCommandLine>           CommandLineHistory;    // +0x4c
    std::list<CConsoleCommandLine>::iterator VisibleHistoryStart;   // +0x50
    std::list<CConsoleCommandLine>::iterator VisibleHistoryFinish;  // +0x54
    long                NumberOfHistoryLinesOnscreen;           // +0x58

    void ScrollToHistoryBeginning();
};

void CConsole_Retail::ScrollToHistoryBeginning()
{
    VisibleHistoryStart = VisibleHistoryFinish = CommandLineHistory.begin();
    if (CommandLineHistory.size() > NumberOfHistoryLinesOnscreen)
        std::advance(VisibleHistoryFinish, NumberOfHistoryLinesOnscreen);
    else
        std::advance(VisibleHistoryFinish, CommandLineHistory.size());
}