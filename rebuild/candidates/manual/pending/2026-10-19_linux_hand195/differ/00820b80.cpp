// Grade: DIFFER(194v194) -- only the __false_type tag's home differs: retail overlays it on the still-live `overTime` argument slot (lea ecx,[esp+24h]), ours on the dead `name` slot (lane gap: tag temporary over a live argument slot)
// CMap::ApplyScriptedMapBrush(const CCharString& name, bool overTime) -- queue a scripted map brush as an update area (timer + name) and, when it is applied at once (or its creation time fits in one frame), apply it fully and zero the new area's timer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00820B80, length 194 bytes (`ret 8`).
// Callees: CMap::GetScriptedMapBrush @ 0x0081E960, CScriptedMapBrush::GetGameArea @ 0x0082E280 (result unused) and GetBrushName @ 0x0082E250
// (both labelled CModeBlock::GetAnimFilter), CCharString copy ctor @ 0x0099EC30, ~CCharString @ 0x0099EAE0,
// vector<CScriptedMapBrushUpdateArea>::_M_insert_overflow @ 0x00826450, CMap::ApplyScriptedMapBrushUpdate(float, CScriptedMapBrush*) @ 0x008203A0,
// the frame-rate global @ 0x01375550 (long), the 1.0f pool constant @ 0x0122DED8.
// Layout: retail CMap = Ego_r.pdb - 8 after the 4-byte retail list: ScriptedMapBrushUpdateAreas (STLport vector, 12 bytes) +0x10;
// CScriptedMapBrush = PDB - 0x10: GameArea +0x38, BrushName +0x48, CreationTimeSeconds +0x4C. No engine header for CMap.
// Shape: the creation time is a named float parked in the dead `name` argument slot across the GetBrushName call; the update area is a
// stack temporary pushed with the inline STLport push_back (placement copy, else _M_insert_overflow with the __false_type tag).
struct CCharString {
    void* PStringData;
    CCharString(const CCharString& other);      // 0x0099EC30
    ~CCharString();                             // 0x0099EAE0
};
struct C2DBoxI { long X0, Y0, X1, Y1; };
struct CScriptedMapBrush {
    unsigned char _pad00[0x38];
    C2DBoxI       GameArea;                     // +0x38
    unsigned char _pad48[0x4];
    float         CreationTimeSeconds;          // +0x4c
    const C2DBoxI& GetGameArea() const;         // 0x0082E280
    const CCharString& GetBrushName() const;    // 0x0082E250
};
struct CScriptedMapBrushUpdateArea {            // 8 bytes
    float       Timer;
    CCharString Name;
    CScriptedMapBrushUpdateArea(float timer, const CCharString& name) : Timer(timer), Name(name) { }
    __forceinline CScriptedMapBrushUpdateArea(const CScriptedMapBrushUpdateArea& o) : Timer(o.Timer), Name(o.Name) { }
};
struct __false_type { __false_type() { } };
inline void* operator new(unsigned int, void* where) { return where; }
struct CUpdateAreaVector {                      // STLport vector<CScriptedMapBrushUpdateArea>
    CScriptedMapBrushUpdateArea* _M_start;
    CScriptedMapBrushUpdateArea* _M_finish;
    CScriptedMapBrushUpdateArea* _M_end_of_storage;
    void _M_insert_overflow(CScriptedMapBrushUpdateArea* position, const CScriptedMapBrushUpdateArea& x, const __false_type&,
                            unsigned long fill_len, bool atend);   // 0x00826450
    void push_back(const CScriptedMapBrushUpdateArea& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CScriptedMapBrushUpdateArea(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
    CScriptedMapBrushUpdateArea& back() { return *(_M_finish - 1); }
};
extern long g_FramesPerSecond;                  // 0x01375550
struct CMap {
    void*             __vftable;                // +0x00
    unsigned char     _pad04[0xc];
    CUpdateAreaVector ScriptedMapBrushUpdateAreas;   // +0x10
    CScriptedMapBrush* GetScriptedMapBrush(const CCharString& name);   // 0x0081E960
    void ApplyScriptedMapBrushUpdate(float amount, CScriptedMapBrush* brush);   // 0x008203A0
    void ApplyScriptedMapBrush(const CCharString& name, bool overTime);
};

void CMap::ApplyScriptedMapBrush(const CCharString& name, bool overTime)
{
    CScriptedMapBrush* brush = GetScriptedMapBrush(name);
    brush->GetGameArea();
    float time = brush->CreationTimeSeconds;
    ScriptedMapBrushUpdateAreas.push_back(CScriptedMapBrushUpdateArea(time, brush->GetBrushName()));
    if (!overTime || 1.0f / g_FramesPerSecond >= brush->CreationTimeSeconds) {
        ApplyScriptedMapBrushUpdate(1.0f, brush);
        ScriptedMapBrushUpdateAreas.back().Timer = 0;
    }
}
