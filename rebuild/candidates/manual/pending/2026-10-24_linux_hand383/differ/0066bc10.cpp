// Grade: DIFFER(199v232) -- the first push_back is not inlined (`push eax ; mov ecx,esi ; call`; retail inlines both, with the null-tested copy ctor 0x0099EC30 and the _M_insert_overflow 0x00433530 call, tag in the dead argument slot), and the out slot is zeroed with an immediate (retail shares `xor edi,edi` with the holder); the frame, the def/slot order and the holder-in-edi release match
// CTCSoundPlayer::GetPotentialCriteriaStrings(const CCharString& criteria, vector<CCharString>& names) -- the candidate sound-event names for a criteria: "<entity sound identifier>;CS_<criteria>" when the thing's def has an entity sound def, then "CS_<criteria>"
// Leaf: ?GetPotentialCriteriaStrings@CTCSoundPlayer
// Retail: Fable.exe (TLC, VC7.1) @ 0x0066BC10, 232 bytes (+ int3 padding to 240), thiscall `ret 8`.
//   Name: Ego_r.pdb `?GetPotentialCriteriaStrings@CTCSoundPlayer@@AAEXABVCCharString@@AAV?$vector@VCCharString@@...@Z`
//   (the landed PlayAttachedCriteria 0x0066BD00 already calls this address by that name). Callers: 0x0066BD00, 0x0066BE37
//   (PlayUnattachedCriteria 0x0066BDF0).
// Callees: CDefClassBase::GetSubDef_0065f604(CDefOut&) @ 0x0065F604 (the CEntitySoundDef instantiation, as the landed
//   0x0066B1D0), the `__fastcall` CCharString operator+ (const CCharString&, const char*) @ 0x0099F600 / (const CCharString&,
//   const CCharString&) @ 0x0099F570 / (const char*, const CCharString&) @ 0x0099F690 (hidden result in ecx), CCharString
//   copy ctor @ 0x0099EC30 / ~CCharString @ 0x0099EAE0, vector<CCharString>::_M_insert_overflow @ 0x00433530; literals
//   ";CS_" @ 0x0125B238 and "CS_" @ 0x0125B234; the def's virtual slot 1 on the last release.
// Label correction: the catalog "Sound_AppendAbilityNameToDescription" is a propagated name.
// Layout: CTCBase ParentThing +4; retail CThing PDef +0x70; CEntitySoundDef SoundIdentifier +0x48 (the landed 0x0066B1D0
//   model); STLport vector<CCharString> {_M_start, _M_finish, _M_end_of_storage}.
// Shape: the thing def read before the out slot is zeroed; the sub def read once from the out slot into a register-held
//   holder (0 when the thing has no def) released at the end; push_back inline (null-tested placement copy then `finish += 1` through memory, else the
//   overflow with the empty __false_type tag), each name a temporary destroyed after its push. No pragma.
inline void* operator new(unsigned int, void* where) { return where; }
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
CCharString __fastcall operator+(const char* left, const CCharString& right);          // 0x0099F690

namespace std {
struct __false_type { };
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;                              // +0x00
    _Tp* _M_finish;                             // +0x04
    _Tp* _M_end_of_storage;                     // +0x08
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);   // 0x00433530
    void push_back(const _Tp& __x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) _Tp(__x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, __x, __false_type(), 1UL, true);
    }
};
}

class CDefBase {
public:
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> class CDefOut {                      // the out slot filled by GetSubDef
public:
    T* Object;
    CDefOut() : Object(0) { }
};
template <class T> class CDefHolder {                   // the def read once from the out slot, released at scope end
public:
    T* Object;
    CDefHolder(T* object) : Object(object) { }
    ~CDefHolder() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
class CEntitySoundDef : public CDefBase {
public:
    unsigned char _pad08[0x48 - 0x08];
    CCharString SoundIdentifier;                        // +0x48
};
class CDefClassBase : public CDefBase {
public:
    bool GetSubDef_0065f604(CDefOut<CEntitySoundDef>& out);   // 0x0065F604
};
class CThing {
public:
    unsigned char _pad00[0x70];
    CDefClassBase* PDef;                                // +0x70
};
class CTCBase {
public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04
};
class CTCSoundPlayer : public CTCBase {
public:
    void GetPotentialCriteriaStrings(const CCharString& criteria, std::vector<CCharString>& names);
};

void CTCSoundPlayer::GetPotentialCriteriaStrings(const CCharString& criteria, std::vector<CCharString>& names)
{
    CDefClassBase* thingDef = ParentThing->PDef;
    CDefOut<CEntitySoundDef> soundDefOut;
    CDefHolder<CEntitySoundDef> soundDef(0);
    if (thingDef) {
        bool found = thingDef->GetSubDef_0065f604(soundDefOut);
        soundDef.Object = soundDefOut.Object;
        if (found)
            names.push_back(soundDef->SoundIdentifier + ";CS_" + criteria);
    }
    names.push_back("CS_" + criteria);
}
