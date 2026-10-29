// CPlayer::InitModeClickPastText(EPlayerMode mode, CProcessedInput* input) -- entering the mode hands the mode value to the yes/no question input process, attaches it and the right-stick look-around process to the player's interface, and cancels the live player character's current mutex special ability
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006359C0, length 142 bytes.
// Callees: each input process's virtual AddToInterface(CGamePlayerInterface*) (slot 1), CBaseIntelligentPointer::GetPItem
// @ 0x00A01B50 (three times), the component map's LowerBound @ 0x0040F020 (key 0xA7 = the special-abilities component),
// CTCSpecialAbilities::CancelCurrentMutexAbility @ 0x0072B420.
// Label note: the process at +0x144 is engine/CPlayer.h InputProcessYesNoQuestion_ptr (+0x140 is ClickPastText); the
// row is referenced only from the mode-table initialiser at 0x0121B160, so the catalog name is kept.
// Layout: engine/CPlayer.h (PlayerInterface +0x0C, PlayerCharacter +0x34, InputProcessYesNoQuestion_ptr +0x144,
// InputProcessRightStickLookAround_ptr +0x170); the thing's component mask at +0x20 (word 5 bit 7 = byte +0x34 sign)
// and its sorted component map at +0x44, dead bit +0x91 (the chunk-141 model).
// Shape: the character is fetched three times (null test, dead test, then the component lookup); the inlined map
// `find` takes the key by value, so its copy lands in the dead mode-argument slot.
#include "engine/CPlayer.h"

typedef long EPlayerMode;
class CProcessedInput;

struct CInputProcessVirtuals {
    virtual void v00();
    virtual bool AddToInterface(CGamePlayerInterface* playerInterface);   // slot 1 (+0x04)
};

struct CInputProcessYesNoQuestionRetail : CInputProcessVirtuals {
    unsigned char _pad04[0x28 - 0x04];
    EPlayerMode   Mode;                                     // +0x28 (descriptive)
};

struct CTCBase_Retail;
class CTCSpecialAbilities { public:
public:
    void CancelCurrentMutexAbility();   // 0x0072B420
};
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> STCEntry_Retail;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const ETCInterfaceType& kind);         // 0x0040F020
    __forceinline CTCBase_Retail* Find(long kind)
    {
        STCEntry_Retail* it = LowerBound((ETCInterfaceType)kind);
        if (it == m_pLast || kind < it->first)
            it = m_pLast;
        return ((CTCBase_Retail*)it->second);
    }
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CTCMap_Retail;
struct CThing_Retail {
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                             // +0x20: which component kinds the thing holds
    unsigned char  _pad40[4];
    CTCMap_Retail  m_tcs;                                   // +0x44
    unsigned char  _pad50[0x41];
    unsigned char  m_dead : 1;                              // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};

struct CPlayer_Methods : CPlayer {
    CThing_Retail* GetPlayerCharacter() { return (CThing_Retail*)reinterpret_cast<CBaseIntelligentPointer*>(this->PlayerCharacter)->GetPItem(); }
    void InitModeClickPastText(EPlayerMode mode, CProcessedInput* input);
};

void CPlayer_Methods::InitModeClickPastText(EPlayerMode mode, CProcessedInput* input)
{
    reinterpret_cast<CInputProcessYesNoQuestionRetail*>(this->InputProcessYesNoQuestion_ptr)->Mode = mode;
    reinterpret_cast<CInputProcessVirtuals*>(this->InputProcessYesNoQuestion_ptr)->AddToInterface(this->PlayerInterface);
    reinterpret_cast<CInputProcessVirtuals*>(this->InputProcessRightStickLookAround_ptr)->AddToInterface(this->PlayerInterface);
    if (GetPlayerCharacter() && !GetPlayerCharacter()->m_dead) {
        CThing_Retail* character = GetPlayerCharacter();
        if (character->HasTC(0xa7))
            ((CTCSpecialAbilities*)character->m_tcs.Find(0xa7))->CancelCurrentMutexAbility();
    }
}