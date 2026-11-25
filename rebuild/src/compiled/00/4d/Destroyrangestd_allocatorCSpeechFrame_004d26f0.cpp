// Byte-exact reconstruction of _Destroy_range<allocator<CSpeechFrame> > at 0x004d26f0
// Leaf: ?_Destroy_range_allocator_CSpeechFrame___@@YIXPAVCSpeechFrame@@0@Z
// element size 0x44 = 68 bytes; destructor at 0x4d1db0 (thiscall); epilogue = ret (0)

class CSpeechFrame { public:
    char pad[0x44];
};


// __fastcall: first in ecx, last in edx. No stack cleanup -> ret 0.
// Naming (NF449 gen, 2026-11-25): callee-only respell of this row's destructor stand-in calls to the PDB destructor (T1 tracker;
//   target self rows' Leaf), as a qualified destructor call on a bodiless declaration (no layout; rulings r2/r3):
//   0x004D1DB0 ??1CSubtitledMessage@@QAE@XZ (was @CSpeechFrame_dtor@4)
class CSubtitledMessage { public: ~CSubtitledMessage(); };
void __fastcall _Destroy_range_allocator_CSpeechFrame___(CSpeechFrame* first, CSpeechFrame* last)
{
    for (; first != last; ++first)
    {
        reinterpret_cast<CSubtitledMessage*>(first)->CSubtitledMessage::~CSubtitledMessage();
    }
}