// CTCInGameMenu::PeekGuiDef() const -- the "PLAYER_GUI" player-GUI definition, fetched through a temporary counted def pointer
// Leaf: ?PeekGuiDef@CTCInGameMenu@@ABEABVCPlayerGuiDef@@XZ
// Retail: Fable.exe @ 0x0047D5A3, 79 bytes, plain `ret` (`this` unused); no direct callers in the xref index; Ego_r.pdb
// ?PeekGuiDef@CTCInGameMenu@@ABEABVCPlayerGuiDef@@XZ (79-byte body fingerprint; the 63-byte sibling
// ?PeekGuiDef@CPlayerGui@@SIABVCPlayerGuiDef@@XZ is the static one).
// Body: CDefPointer<CPlayerGuiDef> def (zeroed); CCharString name("PLAYER_GUI") (0x01235F20, ctor 0x0099EBF0);
// CGameDefinitionManager::Get() (0x0044C6B0, static __fastcall) ->GetDef(name, 1, def) (0x004858B7);
// ~CCharString (0x0099EAE0); the def pointer's inline release (`dec [esi+4]`, virtual slot 1 at zero); return *def.
// Layout: a counted definition holds its count at +4 behind the vptr (the landed 0x00835B90 model).
// Shape: the def pointer declared before the name, which lives in an inner block (constructed before the argument pushes,
// destroyed before the def is read back); the release tail (`dec [esi+4]`) needs VC7.1 to see that the def slot does not
// escape GetDef, hence the in-TU stand-in body (landed 0x00835B90 idiom).
// Opt: size TU with frame pointers (`push ebp / mov ebp,esp`, `leave`, unaligned entry).
#pragma optimize("s", on)
#pragma optimize("y", off)
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
struct CDefVirtual {
    virtual void v000();
    virtual void DestroyDef();                           // slot 1 (+4): called when the count drops to zero
};
struct CRefCounted {
    long m_refCount;                                     // +4
};
class CDefPointeeBase : public CDefVirtual, public CRefCounted { public:
    void ReleaseDef()
    {
        if (--m_refCount == 0)
            DestroyDef();
    }
};
template<class T> class CDefPointer { public:
    T* m_pObject;
    CDefPointer() : m_pObject(0) { }
    ~CDefPointer()
    {
        if (m_pObject)
            m_pObject->ReleaseDef();
    }
    T& operator*() const { return *m_pObject; }
};
class CPlayerGuiDef : public CDefPointeeBase {
};
class CGameDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();     // 0x0044C6B0
    bool GetDef(const CCharString& type_name, long index, CDefPointer<CPlayerGuiDef>& out);   // 0x004858B7
};
void* FindDef_PlayerGui();                                // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00835B90 / 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CGameDefinitionManager::GetDef(const CCharString& type_name, long index, CDefPointer<CPlayerGuiDef>& out)
{
    void* def = FindDef_PlayerGui();
    if (!def)
        return false;
    *(void**)&out = def;
    return true;
}
#pragma auto_inline(on)
class CTCInGameMenu {
private:
    const CPlayerGuiDef& PeekGuiDef() const;
    friend struct CTCInGameMenu_Test;                    // behaviour-test hook
};
const CPlayerGuiDef& CTCInGameMenu::PeekGuiDef() const
{
    CDefPointer<CPlayerGuiDef> def;
    {
        const CCharString name("PLAYER_GUI");
        CGameDefinitionManager::Get().GetDef(name, 1, def);
    }
    return *def;
}