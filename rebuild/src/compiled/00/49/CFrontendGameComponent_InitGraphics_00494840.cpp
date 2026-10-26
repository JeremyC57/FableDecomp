// CFrontendGameComponent::InitGraphics() -- debug front end: take the ENG_ARIAL_16 font from the global font bank into MainFont; always succeeds
// Leaf: ?InitGraphics@CFrontendGameComponent@@AAE_NXZ (Ego_r.pdb, private; modelled public)
// Retail: Fable.exe @ 0x00494840, 187 bytes, plain `ret`.
// Body: MainFont = g_pFontBank->GetFont("ENG_ARIAL_16") (implicit conversion: the temporary is addressed with lea) -- CCharString(const char*, long) 0x0099EBF0
// (literal 0x01231168), CFontBank::GetFont(const CCharString&) 0x009E2C80 (bank pointer 0x013B838C, CCountedPointer by
// hidden pointer); inline CCountedPointer operator= (source words into ebx/edi first, count-block compare, release
// re-reading the member, add-ref) and the temporary's inline release (destroy + operator delete 0x00BFE9BC, then both
// words cleared); ~CCharString 0x0099EAE0. Returns true.
// Layout: CFrontendGameComponent MainFont +0x1E04 (Data) / +0x1E08 (Info), as Ego_r.pdb (no container precedes it).
// Shape: the counted-pointer model of the landed CText::GetDataFromDef 0x0054ED90.
// Opt: /O2 (16-aligned entry).
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CCPPointerInfo { public:                                 // CCountedPointer count block
    long References;                                    // +0
    void (__fastcall* Destroy)(void* object);           // +4
    void* Object;                                       // +8
};
template <class T> class CCountedPointer { public:
    T*              Data;                               // +0
    CCPPointerInfo* Info;                               // +4
    void Release()
    {
        if (Info) {
            --Info->References;
            if (Info->References == 0) {
                Info->Destroy(Info->Object);
                operator delete(Info);
            }
        }
    }
    ~CCountedPointer() { Release(); Data = 0; Info = 0; }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        T* data = other.Data;
        CCPPointerInfo* info = other.Info;
        if (Info != info) {
            Release();
            Data = data;
            Info = info;
            if (info)
                ++info->References;
        }
        return *this;
    }
};
class CFontBank {
public:
    CCountedPointer<const CFontBank> GetFont(const CCharString& name);   // 0x009E2C80
};
extern CFontBank* g_pFontBank;                          // 0x013B838C

class CFrontendGameComponent {
public:
    unsigned char                    _pad0000[0x1e04];
    CCountedPointer<const CFontBank> MainFont;          // +0x1e04
    bool InitGraphics();
};

bool CFrontendGameComponent::InitGraphics()
{
    MainFont = g_pFontBank->GetFont("ENG_ARIAL_16");
    return true;
}