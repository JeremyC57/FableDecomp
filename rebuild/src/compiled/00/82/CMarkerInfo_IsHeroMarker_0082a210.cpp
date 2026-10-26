// CMarkerInfo::IsHeroMarker  @ 0082a210
// bool __fastcall CMarkerInfo::IsHeroMarker(CMarkerInfo* this)

class CPlayerGuiDef { public:
    char pad[0x958];
    int* pHeroId;   // at +0x958
};

// call 0x438570 -> returns CPlayerGuiDef*
class CPlayerGui {
public:
    static const CPlayerGuiDef& __fastcall PeekGuiDef();   // 0x00438570
};   // 0x00438570

struct CMarkerInfo {
    int field0;   // +0
    int field4;   // +4
    int markerId; // +8
    bool IsHeroMarker();
};

bool CMarkerInfo::IsHeroMarker()
{
    int* p = CPlayerGui::PeekGuiDef().pHeroId;
    return this->markerId == *p;
}