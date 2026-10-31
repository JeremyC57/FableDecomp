// Grade: DIFFER(710v698) (sec1b 2026-10-31) -- first compiled body (the parked row had none): retail_asm ROUND for the two fistp roundings, UI_MISC_THINGS_DEF / TEXT_GUI_MENU_DAMAGE literals and 10.0f read from the binary, members from Ego_r.pdb (CUIMiscThingsDef SpaceSeparator/OpenBracket/CloseBracket/Positive, CWeaponDef Class/Damage/Shot, CObjectAugmentationsDef NumberOfSlots/InitialAugmentations, CThingShotDef HitThingDamage); 103/242 difflines. Left: frame layout (this at [ebp-0Ch] v [ebp-8], def slots), the weapon def release without a null check, augmentation-slot holder (descriptive name CWeaponAugmentationSlots / GetDamage 0x007661C0: name_review) and the second parameter's type (a def global index, not CThing*)
// CTCInventoryWeapons::ConstructWeaponDescriptionDamage(CWideString& description, long weaponDefIndex, float compareDamage) -- appends the weapon's damage line (and the difference against the wielded weapon) to the description
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C7452, 698 bytes, ebp frame (size TU), `ret 0Ch`.
// Decode (for Fable 5.1; no compiled source kept):
//   GetHero 0x005C30D3; "..." 0x01230018 -> CGameDefinitionManager::Get 0x0044C6B0 ->GetDefGlobalIndexFromName-style lookup
//   0x009AD390 (with 1) and the def fetch 0x0042B312 into a CDefPointer at [ebp-10h];
//   the weapon's def through CTCBase GetDefinitionManager 0x006869D0 -> 0x009AD9E0 (CDefPointer out [ebp-4]);
//   a CCountedPointer at [ebp+0Ch] (0x0042B1A6), a 0x14-byte STLport vector<long> at [ebp-3Ch] built from the def's
//   index list (+0x40/+0x44) element by element; then the weapon def +0x2C test 0x005DA510 picks the damage either from
//   the def +0x44 or from a second def lookup (+0xA0 index -> def +0x54);
//   damage = GetDamage 0x007661C0 * value * k(0x01237F0C) rounded with fistp; compare = arg * k rounded with fistp;
//   difference = damage - compare; the line is built from "..." 0x01254584 through the text-formatting helpers
//   0x009C95E0 / 0x00405F00 / 0x0099BE70 and appended with CWideString::operator+= 0x0099B8D0 (prefix +0x3C,
//   suffix +0x4C, "+" +0x54 when difference > 0, value +0x50); vector storage released with free() 0x00BFEA14.
// Retail-asm: 0x005C75C4..0x005C75DC `fstp [ebp-8] ; fld [ebp-8] ; fistp dword ptr [ebp-0Ch]` and `fstp [ebp+10h] ; fld [ebp+10h] ; fistp dword ptr [ebp-8]` -- the engine's inline-assembly ROUND (a bare dword fistp, as the landed CSprite::GetHeight 0x00B1B5E0); VC7.1 emits no bare dword fistp from C++.
#pragma optimize("y", off)
#pragma optimize("s", on)
__forceinline long ROUND(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
class CWideString {
public:
    void* PStringData;
    ~CWideString();                                         // 0x0099B510
    CWideString& operator+=(const CWideString& other);      // 0x0099B8D0
};
CWideString __fastcall operator+(const CWideString& a, const CWideString& b);   // 0x0099BE70
CWideString __fastcall IntToWideString(long value);         // 0x00405F00
namespace NGameText {
class CDataBank {
public:
    CWideString GetTextBySymbol(const CCharString& symbol) const;   // 0x009C95E0
};
}
struct CDefPointeeBase {
    virtual void v00();
    virtual void Delete();                                  // slot 1 (+0x04)
    long RefCount;                                          // +0x04
};
struct CDefClassBase;
template <class T> struct CDefPointer {
    T* Object;
    __forceinline CDefPointer() { }
    __forceinline CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
    T* operator->() const { return Object; }
};
struct CThingShotDef : CDefPointeeBase {
    unsigned char _pad08[0x54 - 0x08];
    float HitThingDamage;                                   // +0x54
};
enum EWeaponClass { WEAPON_CLASS_NONE = 0 };
struct CWeaponDef : CDefPointeeBase {
    unsigned char _pad08[0x2c - 0x08];
    EWeaponClass  Class;                                    // +0x2c
    unsigned char _pad30[0x44 - 0x30];
    float         Damage;                                   // +0x44
    unsigned char _pad48[0xa0 - 0x48];
    long          Shot;                                     // +0xa0 (CThingShotDef global index)
};
struct CRetailLongVector {                                  // STLport vector<long>
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    long& operator[](unsigned long i) { return _M_start[i]; }
    void resize(unsigned long count, const long& value);    // 0x004428F0
};
struct CObjectAugmentationsDef : CDefPointeeBase {
    unsigned char _pad08[0x28 - 0x08];
    long              NumberOfSlots;                        // +0x28
    unsigned char     _pad2c[0x40 - 0x2c];
    CRetailLongVector InitialAugmentations;                 // +0x40
};
struct CWeaponAugmentationSlots {                           // 0x20 bytes: the augmentation slots the damage is computed from
    CRetailLongVector Slots;                                // +0x00
    long  Reserved0, Reserved1, Reserved2;                  // +0x0c
    bool  Enabled;                                          // +0x18
    long  Index;                                            // +0x1c
    CWeaponAugmentationSlots() : Reserved0(0), Reserved1(0), Reserved2(0), Enabled(true), Index(-1)
    {
        Slots._M_start = 0; Slots._M_finish = 0; Slots._M_end_of_storage = 0;
    }
    ~CWeaponAugmentationSlots() { if (Slots._M_start) free(Slots._M_start); }
    float GetDamage() const;                                // 0x007661C0
};
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CWeaponDef>* out) const;                 // 0x0042B1A6
    bool GetSubDef(CDefPointer<CObjectAugmentationsDef>* out) const;    // 0x0042AF9E
};
struct CUIMiscThingsDef : CDefPointeeBase {
    unsigned char _pad08[0x3c - 0x08];
    CWideString   SpaceSeparator;                           // +0x3c
    unsigned char _pad40[0x4c - 0x40];
    CWideString   OpenBracket;                              // +0x4c
    CWideString   CloseBracket;                             // +0x50
    CWideString   Positive;                                 // +0x54
};
struct CDefinitionManager {
    long GetGlobalIndexFromClassIndex(const CCharString& className, long classIndex) const;   // 0x009AD390
    bool GetDef(long index, CDefPointer<CUIMiscThingsDef>* out) const;                       // 0x0042B312
    CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long index) const;                     // 0x009AD9E0
};
struct CGameDefinitionManager {
    static CDefinitionManager* __fastcall Get();            // 0x0044C6B0
};
struct CMainGameComponent {
    unsigned char _pad00[0x14];
    NGameText::CDataBank* TextBank;                         // +0x14
};
extern CMainGameComponent* g_pMainGameComponent;            // 0x013B86A0
bool __fastcall IsMeleeWeaponClass(EWeaponClass weaponClass);   // 0x005DA510

struct CTCInventoryWeapons {
    void RefreshActiveWeaponData() const;                   // 0x005C30D3
    CDefinitionManager* GetDefinitions();                   // 0x006869D0 (CTCBase)
    void ConstructWeaponDescriptionDamage(CWideString& description, long weaponDefIndex, float compareDamage);
};

void CTCInventoryWeapons::ConstructWeaponDescriptionDamage(CWideString& description, long weaponDefIndex, float compareDamage)
{
    RefreshActiveWeaponData();
    CDefPointer<CUIMiscThingsDef> uiDef;
    uiDef.Object = 0;
    long uiIndex;
    {
        CCharString className("UI_MISC_THINGS_DEF");
        uiIndex = CGameDefinitionManager::Get()->GetGlobalIndexFromClassIndex(className, 1);
    }
    CGameDefinitionManager::Get()->GetDef(uiIndex, &uiDef);
    CDefPointer<CDefClassBase> def = GetDefinitions()->GetPDefFromGlobalIndex(weaponDefIndex);
    CDefPointer<CWeaponDef> weaponDef;
    weaponDef.Object = 0;
    def->GetSubDef(&weaponDef);
    CWeaponDef* weapon;
    {
    CWeaponAugmentationSlots slots;
    CDefPointer<CObjectAugmentationsDef> augDef;
    augDef.Object = 0;
    bool found = def->GetSubDef(&augDef);
    CObjectAugmentationsDef* aug = augDef.Object;
    if (found) {
        slots.Slots.resize(aug->NumberOfSlots, 0);
        for (unsigned long i = 0; i < aug->InitialAugmentations.size(); ++i) {
            if (i >= slots.Slots.size())
                break;
            slots.Slots[i] = aug->InitialAugmentations[i];
        }
    }
    if (aug && --aug->RefCount == 0)
        aug->Delete();
    weapon = weaponDef.Object;
    float baseDamage;
    if (IsMeleeWeaponClass(weapon->Class))
        baseDamage = weapon->Damage;
    else {
        CDefPointer<CDefClassBase> shotTemp = GetDefinitions()->GetPDefFromGlobalIndex(weapon->Shot);
        CThingShotDef* shot = (CThingShotDef*)shotTemp.Object;
        if (shot)
            ++shot->RefCount;
        CDefClassBase* temp = shotTemp.Object;
        if (temp) {
            if (--temp->RefCount == 0)
                temp->Delete();
            shotTemp.Object = 0;
        }
        baseDamage = shot->HitThingDamage;
        if (--shot->RefCount == 0)
            shot->Delete();
    }
    long damage = ROUND(slots.GetDamage() * baseDamage * 10.0f);
    long compare = ROUND(compareDamage * 10.0f);
    long difference = damage - compare;
    NGameText::CDataBank* bank = g_pMainGameComponent->TextBank;
    CUIMiscThingsDef* ui = uiDef.Object;
    description += bank->GetTextBySymbol(CCharString("TEXT_GUI_MENU_DAMAGE")) + ui->SpaceSeparator;
    description += IntToWideString(damage) + ui->SpaceSeparator;
    description += ui->OpenBracket;
    if (difference > 0)
        description += ui->Positive;
    description += IntToWideString(difference) + ui->CloseBracket;
    }
    if (--weapon->RefCount == 0)
        weapon->Delete();
    CDefClassBase* owner = def.Object;
    if (owner) {
        if (--owner->RefCount == 0)
            owner->Delete();
        def.Object = 0;
    }
    CUIMiscThingsDef* ui = uiDef.Object;
    if (ui && --ui->RefCount == 0)
        ui->Delete();
}
