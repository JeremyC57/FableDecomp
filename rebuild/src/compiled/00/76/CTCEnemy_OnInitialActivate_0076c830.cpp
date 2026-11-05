// CTCEnemy::OnInitialActivate @ 0076c830
// Leaf: ?CTCEnemy_OnInitialActivate@@YIXPAVCTCEnemy@@@Z
// Naming wave 18: the global stand-in g_pPlayerGui (0x013B8790) is the private static CCountedPointer<CPlayerGui>
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
class CTCEnemy;

// the player GUI at 0x013B8790; CPlayerGui::UpdateMiniMapMarkers(bool) 0x004378C0 (Ego_r.pdb / FableWin.pdb, chunk 563)
class CCPPointerInfo;
template <class T> class CCountedPointer { public:                    // engine/CCountedPointer.h shape: Data +0, Info +4
    T* Data;
    CCPPointerInfo* Info;
    T& operator*() const { return *Data; }
};
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CPlayerGui
    static CCountedPointer<T> PClass;                                // 0x013B8790 ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ (inline)
};
class CPlayerGui { public:
    void UpdateMiniMapMarkers(bool force);   // 0x004378C0
};

// base OnInitialActivate (tail jmp 0x686870), __fastcall(this)
void __fastcall CTCEnemy_base_OnInitialActivate(CTCEnemy* enemy);

void __fastcall CTCEnemy_OnInitialActivate(CTCEnemy* enemy)
{
    CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().UpdateMiniMapMarkers(false);
    CTCEnemy_base_OnInitialActivate(enemy);
}