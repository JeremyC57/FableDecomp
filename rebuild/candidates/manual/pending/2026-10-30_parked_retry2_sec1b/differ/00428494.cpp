// Grade: DIFFER(155v1062) (sec1b 2026-10-31) -- partial reconstruction: signature from the landed caller 0x005A45F6 (const CWideString&, float, float, const CCharString&, float, CCharString, float, CCharString button; ret 20h) and the button-template cache block (find / operator[] / CreateComponent / the out-of-line CCountedPointer ctor, ShareData through a __forceinline operator=, dtor, Reset) matching retail's shape; left: frame 0x24 + ebx zero register, and the entry build (the d-string empty test, state-map positions and alpha, text/icon children: about 900 bytes) is not written
// NUISystem::CManager::CreateMenuEntryComponent(const CWideString& text, float a, float b, const CCharString& definition, float c, CCharString d, float e, CCharString button) -- build a menu entry: make sure the button template is cached in Templates, create the entry component and its children, return it
// Leaf: CreateMenuEntryComponent
// Retail: Fable.exe (TLC, VC7.1) @ 0x00428494, 1062 bytes, frame-pointer /Os TU, `ret 0x20` (signature from the landed caller 0x005A45F6).
// Callees: Templates (+0x88) find 0x0042B52B / operator[] 0x0042D45D; CreateComponent 0x0041DB1D; CCountedPointer<CComponent>
//   (T*) 0x00429C15, ShareData 0x0042920A, ~ 0x004291DE, Reset 0x00429C6F (out-of-line COMDATs in this TU).
#pragma optimize("s", on)
#pragma optimize("y", off)
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
};
struct CWideString { void* PStringData; };
struct CCPPointerInfo;
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(T* data);                          // 0x00429C15
    ~CCountedPointer();                                // 0x004291DE
    void ShareData(T* data, CCPPointerInfo* info);     // 0x0042920A
    void Reset(T* data);                               // 0x00429C6F
    __forceinline CCountedPointer& operator=(const CCountedPointer& other) { ShareData(other.Data, other.Info); return *this; }
};
namespace NUISystem {
struct CComponent;
struct CTemplateMap {                                  // map<CCharString, CCountedPointer<CComponent> >
    void* _M_header;
    unsigned long _M_node_count;
    long _M_key_compare;
    void* find(const CCharString& key);                // 0x0042B52B (returns the node)
    CCountedPointer<CComponent>& operator[](const CCharString& key);   // 0x0042D45D
};
class CManager {
public:
    unsigned char _pad00[0x88];
    CTemplateMap  Templates;                           // +0x88
    CComponent* CreateComponent(const CCharString& definition, bool b);   // 0x0041DB1D
    CComponent* CreateMenuEntryComponent(const CWideString& text, float a, float b, const CCharString& definition,
                                         float c, CCharString d, float e, CCharString button);
};
CComponent* CManager::CreateMenuEntryComponent(const CWideString& text, float a, float b, const CCharString& definition,
                                               float c, CCharString d, float e, CCharString button)
{
    CTemplateMap& templates = Templates;
    if (templates.find(button) == templates._M_header) {
        CCountedPointer<CComponent> created(CreateComponent(button, false));
        templates[button] = created;
    }
    else if (templates[button].Data == 0)
        templates[button].Reset(CreateComponent(button, false));
    return 0;
}
}
