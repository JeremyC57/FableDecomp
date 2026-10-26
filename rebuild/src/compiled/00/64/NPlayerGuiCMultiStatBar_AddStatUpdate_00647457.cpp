// NPlayerGui::CMultiStatBar::AddStatUpdate(EStatUpdateType, long) -- merge a stat change into the pending update of the same type, or queue a new one and seed the bar's colour / graphic / effect for that type when unset; a bar that was idle-full (mode 1) starts updating (mode 2)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00647457, 186 bytes (`ret 8`, unaligned: a size-optimised frame-pointer TU).
// Pragmas: `#pragma optimize("s", on)` + `#pragma optimize("y", off)` (ebp frame closed with `leave`, esi/edi free).
// Callees: list<CStatUpdate>::push_back(const CStatUpdate&) 0x00655F36 (insert before the end node; the manifest's
// "ListPushFrontWrapper" name is wrong: the iterator it passes is the sentinel, i.e. end()).
// Layout (retail, 0x10 below the PDB because the STLport list is the 4-byte sentinel pointer): DisplayMode +0x28 (base-class
// word: 1 = idle, 2 = updating), Updates +0x68, CurrentType +0x6c, CurrentColour +0x70 (CRGBColour B,G,R,A), CurrentGraphic
// +0x74, CurrentEffect +0x78, CurrentTimeLeft +0x7c, WaitingTimeLeft +0x80. The owning GUI (+0x04) holds the colour for new
// updates at +0x520 and per-type colour / graphic / effect vectors at +0x540 / +0x54c / +0x558.
// Shape: the list walk re-reads end() each step; the merge arm jumps straight to the shared mode tail (a `goto`); the new
// update is a 12-byte frame aggregate filled Change, Colour, Type (retail store order) before push_back.
#pragma optimize("s", on)
#pragma optimize("y", off)

class CRGBColour { public:
    unsigned char B, G, R, A;              // D3DCOLOR byte order
};

namespace NPlayerGui {

enum EStatUpdateType { STAT_UPDATE_NONE = 0 };

template <class T> struct CVector {        // STLport vector: 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};

struct CStatBarOwner {                     // the owning player GUI (only the members read here)
    unsigned char       _pad0[0x520];
    CRGBColour          UpdateColour;      // +0x520
    unsigned char       _pad1[0x1c];
    CVector<CRGBColour> TypeColours;       // +0x540
    CVector<long>       TypeGraphics;      // +0x54c
    CVector<long>       TypeEffects;       // +0x558
};

class CMultiStatBar { public:
    class CStatUpdate { public:                   // 12 bytes
        EStatUpdateType Type;
        long            Change;
        CRGBColour      Colour;
    };
    struct CUpdateNode {                   // STLport _List_node<CStatUpdate>
        CUpdateNode* _M_next;
        CUpdateNode* _M_prev;
        CStatUpdate  _M_data;
    };
    struct CUpdateList {                   // STLport list<CStatUpdate>: the sentinel pointer
        CUpdateNode* _M_node;
        void push_back(const CStatUpdate& update);   // 0x00655F36
    };

    void*           __vftable;             // +0x00
    CStatBarOwner*  POwner;                // +0x04
    unsigned char   _pad0[0x20];
    long            DisplayMode;           // +0x28
    unsigned char   _pad1[0x3c];
    CUpdateList     Updates;               // +0x68
    EStatUpdateType CurrentType;           // +0x6c
    CRGBColour      CurrentColour;         // +0x70
    long            CurrentGraphic;        // +0x74
    long            CurrentEffect;         // +0x78
    float           CurrentTimeLeft;       // +0x7c
    float           WaitingTimeLeft;       // +0x80

    void AddStatUpdate(EStatUpdateType type, long change);
};

void CMultiStatBar::AddStatUpdate(EStatUpdateType type, long change)
{
    for (CUpdateNode* it = Updates._M_node->_M_next; it != Updates._M_node; it = it->_M_next) {
        if (it->_M_data.Type == type) {
            it->_M_data.Colour = POwner->UpdateColour;
            it->_M_data.Change += change;
            goto done;
        }
    }
    {
        CStatUpdate update;
        update.Change = change;
        update.Colour = POwner->UpdateColour;
        update.Type = type;
        Updates.push_back(update);
    }
    if (CurrentColour.R == 0 && CurrentColour.G == 0 && CurrentColour.B == 0 && CurrentColour.A == 0xff)
        CurrentColour = POwner->TypeColours._M_start[type];
    if (CurrentGraphic == 0)
        CurrentGraphic = POwner->TypeGraphics._M_start[type];
    if (CurrentEffect == 0)
        CurrentEffect = POwner->TypeEffects._M_start[type];
done:
    if (DisplayMode == 1)
        DisplayMode = 2;
}

}