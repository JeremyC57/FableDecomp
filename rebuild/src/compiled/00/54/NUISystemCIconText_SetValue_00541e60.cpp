// NUISystem::CIconText::SetValue(const CCharString&) -- show the text-bank string for this symbol (game bank first, else the system bank), or the symbol itself widened when missing
// Retail 0x00541E60, 187 bytes (vtable 0x012470CC slot 145, `ret 4`). Callees: CDataBank::GetTextBySymbol 0x009C95E0 (CWideString
// by hidden pointer, built in the dead argument slot), CWideString::operator!= 0x0099B0F0 against the empty string 0x013BCA24,
// CCharString::ToWideString 0x0099E480, CWideString::operator= 0x0099B7D0, ~CWideString 0x0099B510, UpdateText (slot 150, +0x258).
// Name: Ego_r.pdb ?SetValue@CIconText@NUISystem@@UAEXABVCCharString@@@Z (source order SetValue(CCharString), SetValue(CWideString)).
// Leaf: ?SetValue@CIconText@NUISystem@@UAEXABVCCharString@@@Z
// Local models: CWideString / CCharString as retail calls them (fable_string.h lacks the CWideString compare and the
// CWideString -> CCharString conversion); Game/System text banks at +0x14 / +0x60 as the landed SetValue(ulong) 0x00542140.
class CWideString;
class CCharString {
public:
 void* PStringData;
 ~CCharString();                                     // 0x0099EAE0
 CWideString ToWideString() const;                   // 0x0099E480
};
class CWideString {
public:
 void* PStringData;
 CWideString(const CWideString&);
 ~CWideString();                                     // 0x0099B510
 const CWideString& operator=(const CWideString&);         // 0x0099B7D0
 bool operator!=(const CWideString&) const;          // 0x0099B0F0
 CCharString ConvertToCharString() const;            // 0x0099B2C0
};
namespace NGameText {
class CDataBank { public: const CWideString GetTextBySymbol(const CCharString& symbol) const; };
}
using NGameText::CDataBank;   // 0x009C95E0
struct Game { unsigned char Pad[0x14]; CDataBank* TextBank; CDataBank* GetTextDataBank() const { return TextBank; } };
struct System { unsigned char Pad[0x60]; CDataBank* TextBank; CDataBank* GetTextDataBank() const { return TextBank; } };
extern Game* Game_013b86a0;                          // 0x013B86A0
extern System* System_013b871c;                      // 0x013B871C
extern CWideString Empty_013bca24;                   // 0x013BCA24
class C2DBoxF { public: float Left, Top, Right, Bottom; };
namespace NUISystem {
class CIconText { public:
 virtual void Slot000()=0; virtual void Slot001()=0; virtual void Slot002()=0; virtual void Slot003()=0; virtual void Slot004()=0; virtual void Slot005()=0;
 virtual void Slot006()=0; virtual void Slot007()=0; virtual void Slot008()=0; virtual void Slot009()=0; virtual void Slot010()=0; virtual void Slot011()=0;
 virtual void Slot012()=0; virtual void Slot013()=0; virtual void Slot014()=0; virtual void Slot015()=0; virtual void Slot016()=0; virtual void Slot017()=0;
 virtual void Slot018()=0; virtual void Slot019()=0; virtual void Slot020()=0; virtual void Slot021()=0; virtual void Slot022()=0; virtual void Slot023()=0;
 virtual void Slot024()=0; virtual void Slot025()=0; virtual void Slot026()=0; virtual void Slot027()=0; virtual void Slot028()=0; virtual void Slot029()=0;
 virtual void Slot030()=0; virtual void Slot031()=0; virtual void Slot032()=0; virtual void Slot033()=0; virtual void Slot034()=0; virtual void Slot035()=0;
 virtual void Slot036()=0; virtual void Slot037()=0; virtual void Slot038()=0; virtual void Slot039()=0; virtual void Slot040()=0; virtual void Slot041()=0;
 virtual void Slot042()=0; virtual void Slot043()=0; virtual void Slot044()=0; virtual void Slot045()=0; virtual void Slot046()=0; virtual void Slot047()=0;
 virtual void Slot048()=0; virtual void Slot049()=0; virtual void Slot050()=0; virtual void Slot051()=0; virtual void Slot052()=0; virtual void Slot053()=0;
 virtual void Slot054()=0; virtual void Slot055()=0; virtual void Slot056()=0; virtual void Slot057()=0; virtual void Slot058()=0; virtual void Slot059()=0;
 virtual void Slot060()=0; virtual void Slot061()=0; virtual void Slot062()=0; virtual void Slot063()=0; virtual void Slot064()=0; virtual void Slot065()=0;
 virtual void Slot066()=0; virtual void Slot067()=0; virtual void Slot068()=0; virtual void Slot069()=0; virtual void Slot070()=0; virtual void Slot071()=0;
 virtual void Slot072()=0; virtual void Slot073()=0; virtual void Slot074()=0; virtual void Slot075()=0; virtual void Slot076()=0; virtual void Slot077()=0;
 virtual void Slot078()=0; virtual void Slot079()=0; virtual void Slot080()=0; virtual void Slot081()=0; virtual void Slot082()=0; virtual void Slot083()=0;
 virtual void Slot084()=0; virtual void Slot085()=0; virtual void Slot086()=0; virtual void Slot087()=0; virtual void Slot088()=0; virtual void Slot089()=0;
 virtual void Slot090()=0; virtual void Slot091()=0; virtual void Slot092()=0; virtual void Slot093()=0; virtual void Slot094()=0; virtual void Slot095()=0;
 virtual void Slot096()=0; virtual void Slot097()=0; virtual void Slot098()=0; virtual void Slot099()=0; virtual void Slot100()=0; virtual void Slot101()=0;
 virtual void Slot102()=0; virtual void Slot103()=0; virtual void Slot104()=0; virtual void Slot105()=0; virtual void Slot106()=0; virtual void Slot107()=0;
 virtual void Slot108()=0; virtual void Slot109()=0; virtual void Slot110()=0; virtual void Slot111()=0; virtual void Slot112()=0; virtual void Slot113()=0;
 virtual void Slot114()=0; virtual void Slot115()=0; virtual void Slot116()=0; virtual void Slot117()=0; virtual void Slot118()=0; virtual void Slot119()=0;
 virtual void Slot120()=0; virtual void Slot121()=0; virtual void Slot122()=0; virtual void Slot123()=0; virtual void Slot124()=0; virtual void Slot125()=0;
 virtual void Slot126()=0; virtual void Slot127()=0; virtual void Slot128()=0; virtual void Slot129()=0; virtual void Slot130()=0; virtual void Slot131()=0;
 virtual void Slot132()=0; virtual void Slot133()=0; virtual void Slot134()=0; virtual void Slot135()=0; virtual void Slot136()=0; virtual void Slot137()=0;
 virtual void Slot138()=0; virtual void Slot139()=0; virtual void Slot140()=0; virtual void Slot141()=0; virtual void Slot142()=0; virtual void Slot143()=0;
 virtual void SetValue(const CCharString&); virtual void SetValue(const CWideString&); // overloads, reversed by MSVC: (CWideString) 144, (CCharString) 145 // slot 144 (+0x240)
 virtual void Slot146()=0;
 virtual void SetTextWindow(C2DBoxF&); // slot 147 (+0x24c)
 virtual void Slot148()=0; virtual void Slot149()=0;
 virtual void UpdateText(); // slot 150 (+0x258)
 virtual long GetTextAlignment(); // slot 151 (+0x25c)
 unsigned char _pad04[0x12e - 4];
 unsigned char Flags12e;                  // +0x12e bit 4: centred, bit 5: right-aligned
 unsigned char _pad12f[0x15c - 0x12f];
 CWideString Text;                        // +0x15c
 unsigned char _pad160[0x198 - 0x160];
 C2DBoxF TextWindow;                      // +0x198
};
void CIconText::SetValue(const CCharString& symbol)
{
 if (Game_013b86a0) {
  CWideString text = Game_013b86a0->GetTextDataBank()->GetTextBySymbol(symbol);
  if (text != Empty_013bca24)
   Text = text;
  else
   Text = symbol.ToWideString();
 } else if (System_013b871c) {
  CWideString text = System_013b871c->GetTextDataBank()->GetTextBySymbol(symbol);
  if (text != Empty_013bca24)
   Text = text;
  else
   Text = symbol.ToWideString();
 } else {
  Text = symbol.ToWideString();
 }
 UpdateText();
}
}