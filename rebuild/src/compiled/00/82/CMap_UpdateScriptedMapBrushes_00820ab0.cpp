// CMap::UpdateScriptedMapBrushes() -- advance every pending scripted-map-brush update area by one game frame: look up its brush by name, count its time left down by 1/frames-per-second, erase the entry once the time runs out, otherwise apply the brush at the progress (duration - time left) / duration
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00820AB0, length 158 bytes.
// Callees: CMap::GetScriptedMapBrush(const CCharString&) @ 0x0081E960, the STLport `__copy` @ 0x00824790 (`__fastcall`: first/last in
//   ecx/edx, then result, the random-access tag by reference and the null distance pointer; manifest label _Copy_impl<...CSkeletalMorph...>
//   is a byte-twin), CCharString::~CCharString @ 0x0099EAE0, CMap::ApplyScriptedMapBrushUpdate(float, CScriptedMapBrush*) @ 0x008203A0.
// Globals: the game frame rate (int) 0x01375550; floats 0x0122DED8 = 1.0f, 0x0122DEDC = 0.0f.
// Layout: retail CMap ScriptedMapBrushUpdateAreas (STLport vector of 8-byte {time left, brush name}) +0x10; retail CScriptedMapBrush
//   CreationTimeSeconds +0x4c (PDB +0x5c less the debug container words).
// Shape: an outer `!empty()` test around the `for` (the end test is duplicated at the top); the inlined STLport vector::erase copies the
//   tail down only when the erased element is not the last, then destroys the old last element. The empty tag needs a user constructor
//   (`random_access_iterator_tag() {}`) so its temporary in the frame word's high byte is not zero-initialised; `time < 0.0f` is the
//   `fcom ; test ah,5 ; jp` erase test.
// Layout (supersedes any local-model note above): engine/CScriptedMapBrush.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CScriptedMapBrush CScriptedMapBrushDonor
#include "engine/CScriptedMapBrush.h"
#undef CScriptedMapBrush
class CCharString { public:
    void* m_pStringData;
    ~CCharString();                                // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);
};
class CScriptedMapBrush : public CScriptedMapBrushDonor {
 public:
};
struct CScriptedMapBrushUpdateArea {
    float       TimeLeft;
    CCharString BrushName;
};
struct random_access_iterator_tag { random_access_iterator_tag() { } };
CScriptedMapBrushUpdateArea* __fastcall __copy(CScriptedMapBrushUpdateArea* first, CScriptedMapBrushUpdateArea* last, CScriptedMapBrushUpdateArea* result,
                                               const random_access_iterator_tag& tag, int* distance);   // 0x00824790
struct CUpdateAreaVector {
    CScriptedMapBrushUpdateArea* _M_start;
    CScriptedMapBrushUpdateArea* _M_finish;
    CScriptedMapBrushUpdateArea* _M_end_of_storage;
    CScriptedMapBrushUpdateArea* begin() { return _M_start; }
    CScriptedMapBrushUpdateArea* end() { return _M_finish; }
    bool empty() const { return _M_start == _M_finish; }
    CScriptedMapBrushUpdateArea* erase(CScriptedMapBrushUpdateArea* pos)
    {
        if (pos + 1 != _M_finish)
            __copy(pos + 1, _M_finish, pos, random_access_iterator_tag(), (int*)0);
        --_M_finish;
        _M_finish->~CScriptedMapBrushUpdateArea();
        return pos;
    }
};
extern long g_GameFramesPerSecond;                 // 0x01375550
class CMap { public:
    unsigned char      _pad00[0x10];
    CUpdateAreaVector  ScriptedMapBrushUpdateAreas;   // +0x10 (PDB +0x18)
    CScriptedMapBrush* GetScriptedMapBrush(const CCharString& name);         // 0x0081E960
    void ApplyScriptedMapBrushUpdate(float progress, CScriptedMapBrush* brush);   // 0x008203A0
    void UpdateScriptedMapBrushes();
};
void CMap::UpdateScriptedMapBrushes()
{
    if (!ScriptedMapBrushUpdateAreas.empty()) {
        for (CScriptedMapBrushUpdateArea* it = ScriptedMapBrushUpdateAreas.begin(); it != ScriptedMapBrushUpdateAreas.end(); ) {
            CScriptedMapBrush* brush = GetScriptedMapBrush(it->BrushName);
            it->TimeLeft -= 1.0f / (float)g_GameFramesPerSecond;
            if (it->TimeLeft < 0.0f) {
                it = ScriptedMapBrushUpdateAreas.erase(it);
            } else {
                ApplyScriptedMapBrushUpdate((brush->CreationTimeSeconds - it->TimeLeft) / brush->CreationTimeSeconds, brush);
                ++it;
            }
        }
    }
}