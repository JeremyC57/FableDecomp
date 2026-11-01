# h16_cplayer.py: naming wave 16, family w15d -- wave 15 DEFERRED 00634870 (C2011): the CPlayer_Methods stand-in becomes the
# real class CPlayer on the generated layout header through the landed #define-Layout precedent (0x00830150: #define X X_Layout,
# #include "engine/X.h", #undef X, class X : public X_Layout), so the header's flat record and the method-bearing class do not
# collide. AddMode takes the Ego_r EPlayerMode (?AddMode@CPlayer@@QAEXW4EPlayerMode@@_N@Z, already bound at 0x006345C0).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit
edit('00634870', 'src', '''#include "engine/CPlayer.h"

struct CPlayer_Methods : CPlayer {
    void AddMode(long mode, bool exclusive);   // 0x006345C0 (EPlayerMode, bool)
    void ResetModes();
};

void CPlayer_Methods::ResetModes()
{
    if (this->Local)
        AddMode(0, true);                      // EPlayerMode 0 == default/no mode
}''', '''// Naming wave 16: the CPlayer_Methods stand-in respelled to CPlayer on the layout header (#define-Layout precedent 0x00830150);
//   AddMode takes the Ego_r EPlayerMode (name-only: the bytes are unchanged).
#define CPlayer CPlayer_Layout
#include "engine/CPlayer.h"
#undef CPlayer

enum EPlayerMode { PLAYER_MODE_NULL = 0 };    // enumerator name is ours
class CPlayer : public CPlayer_Layout { public:
    void AddMode(EPlayerMode mode, bool exclusive);   // 0x006345C0
    void ResetModes();
};

void CPlayer::ResetModes()
{
    if (this->Local)
        AddMode(PLAYER_MODE_NULL, true);       // EPlayerMode 0 == default/no mode
}''')
edit('00487bd0', 'src', '''#include "engine/CPlayer.h"

typedef long EPlayerMode;
''', '''// Naming wave 16: the CPlayer_Methods stand-in respelled to CPlayer on the layout header (#define-Layout precedent 0x00830150);
//   AddMode takes the Ego_r EPlayerMode (name-only: the bytes are unchanged).
#define CPlayer CPlayer_Layout
#include "engine/CPlayer.h"
#undef CPlayer

enum EPlayerMode { PLAYER_MODE_NULL = 0 };    // enumerator name is ours; mode 3 has no recorded enumerator name
''')
edit('00487bd0', 'src', '''struct CPlayer_Methods : CPlayer {''', '''class CPlayer : public CPlayer_Layout { public:''')
edit('00487bd0', 'src', '''void CPlayer_Methods::UninitCharacter()''', '''void CPlayer::UninitCharacter()''')
edit('00487bd0', 'src', '''        AddMode(3, true);''', '''        AddMode((EPlayerMode)3, true);''')
