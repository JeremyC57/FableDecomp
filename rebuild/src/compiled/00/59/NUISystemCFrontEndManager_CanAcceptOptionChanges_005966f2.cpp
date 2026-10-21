// NUISystem::CFrontEndManager::CanAcceptOptionChanges() -- on the key-redefinition screen ask the redefiner list, otherwise always true
// Leaf: ?CanAcceptOptionChanges@CFrontEndManager@NUISystem@@QAE_NXZ
// Retail: Fable.exe @ 0x005966F2, 56 bytes, plain `ret`, no direct reference; Ego_r.pdb
// ?CanAcceptOptionChanges@CFrontEndManager@NUISystem@@QAE_NXZ. Body: m_mapScreens[22] (map::operator[] 0x0059B5D7 on
// +0x54, the key a stack temporary) and m_stackScreens.top() (deque::back 0x0059B039 on +0x20); when the top screen is
// that screen, return the static NUISystem::CRedefinerList::CanAcceptChanges() (0x00556540), else true. The
// FRONTEND_SCREEN enumerator name for 22 is not recovered.
// Layout: retail CFrontEndManager -- m_stackScreens +0x20 (40-byte STLport deque), m_vComponents +0x48 (12-byte vector),
// m_mapScreens +0x54 (Ego_r.pdb +0x2c / +0x44 / +0x54 with VC7 containers); no engine/ header.
// Opt: size TU with frame pointers (`push ebp / mov ebp,esp`, unaligned entry, `leave`).
#pragma optimize("s", on)
#pragma optimize("y", off)
enum FRONTEND_SCREEN { FRONTEND_SCREEN_22 = 22 };
namespace NUISystem {
class CComponent;
class CRedefinerList {
public:
    static bool __fastcall CanAcceptChanges();           // 0x00556540 (Ego_r.pdb SI: static __fastcall)
};
}
namespace std {
template <class T> class deque {
public:
    unsigned char _body[0x28];
    T& back();                                           // 0x0059B039 for NUISystem::CComponent*
};
template <class T, class Seq = deque<T> > class stack {
public:
    Seq c;
    T& top() { return c.back(); }
};
template <class K, class V> class map {
public:
    unsigned char _body[0x10];
    V& operator[](const K& key);                         // 0x0059B5D7 for FRONTEND_SCREEN -> CComponent*
};
}
namespace NUISystem {
class CFrontEndManager {
public:
    unsigned char                                   _pad00[0x20];
    std::stack<CComponent*>                         m_stackScreens;     // +0x20
    unsigned char                                   m_vComponents[0xc]; // +0x48 vector<CComponent*>
    std::map<FRONTEND_SCREEN, CComponent*>          m_mapScreens;       // +0x54
    bool CanAcceptOptionChanges();
};
bool CFrontEndManager::CanAcceptOptionChanges()
{
    if (m_stackScreens.top() == m_mapScreens[FRONTEND_SCREEN_22])
        return CRedefinerList::CanAcceptChanges();
    return true;
}
}