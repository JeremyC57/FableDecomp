// CVillageDef::Validate(const CValidateDefinitionInitBase& init, const CCharString& name) const -- when the weapon-class list (STLport vector at +0x28) contains class 1, run the name def-string check against ""
// Grade: DIFFER(ours v retail) 63v63 -- scheduling only: retail `lea ecx,[esp+10h] ; push ecx ; mov ecx,[esi+28h]`, ours `mov ecx,[esi+28h] ; lea edx,[esp+10h] ; push edx`
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007FE670, length 63 bytes. `ret 8`.
// Leaf: Validate
// Callees: SGI find(first, last, const EWeaponClass&, const random_access_iterator_tag&) @ 0x0065D9EE (/Gr: first in ecx, last in edx,
// unrolled by four; manifest "std::linear_search<EWeaponClass>"), CDefString::operator==(const char*) @ 0x005B3440 (result unused) with
// the empty string @ 0x0122D70E.
// Shape: the end pointer in edi across the call, the value temporary 1 in its own frame dword and the empty tag in the high byte of
// the other (`lea eax,[esp+0Bh]`) -- both reproduced; only the begin load is scheduled before the value's lea.
// Tried: inline begin()/end() in one expression, a named `last` local (this file), raw member operands (end re-read, tag slot moved),
// optimize("s") (frame pointer). Next: lane gap (scheduling of the fastcall register load), or begin() through an iterator object.
struct CValidateDefinitionInitBase;
struct CCharString;
enum EWeaponClass { WEAPON_CLASS_NONE = 0, WEAPON_CLASS_1 = 1 };

struct random_access_iterator_tag {
    random_access_iterator_tag() { }
};

const EWeaponClass* __fastcall find_weapon_class(const EWeaponClass* first, const EWeaponClass* last, const EWeaponClass& value,
                                                 const random_access_iterator_tag& tag);   // 0x0065D9EE

inline const EWeaponClass* find(const EWeaponClass* first, const EWeaponClass* last, const EWeaponClass& value)
{
    return find_weapon_class(first, last, value, random_access_iterator_tag());
}

struct CDefString {
    long TablePos;
    bool operator==(const char* text) const;    // 0x005B3440
};

struct CWeaponClassVector {                     // STLport vector<EWeaponClass>
    EWeaponClass* _M_start;
    EWeaponClass* _M_finish;
    EWeaponClass* _M_end_of_storage;
    const EWeaponClass* begin() const { return _M_start; }
    const EWeaponClass* end() const { return _M_finish; }
};

struct CVillageDef {
    virtual void v00();
    unsigned char      _pad04[0x24];
    CWeaponClassVector m_weaponClasses;         // +0x28
    CDefString         m_name;                  // +0x34
    virtual void Validate(const CValidateDefinitionInitBase& init, const CCharString& name) const;
};

void CVillageDef::Validate(const CValidateDefinitionInitBase&, const CCharString&) const
{
    const EWeaponClass* last = m_weaponClasses.end();
    if (find(m_weaponClasses.begin(), last, WEAPON_CLASS_1) != last)
        m_name == "";
}
