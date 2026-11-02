# h25_bcnc8.py: naming wave 25, family bcnc8 -- the eight implicit-dtor rows under ~CBaseClassNonCopyable (DECISIONS_W24_RESOLVED item 8).
#  Their local CBaseClassNonCopyable declared no destructor, so its implicit public virtual one (??1CBaseClassNonCopyable@@UAE@XZ) was
#  generated in the TU and named by the derived classes' implicit / inline destructors. Retail: CBaseClassNonCopyable declares a protected
#  virtual destructor (Ego_r.pdb / FableWin.pdb ??1CBaseClassNonCopyable@@MAE@XZ) that is out of line at 0x0099A300 -- each derived class's
#  vtable slot 0 (its ??_G) and inline destructor calls 0x0099A300 itself (e.g. the inline ~CDiskFileWin32 in 0x0055D070 ends `call
#  0x0099A300`), and the derived classes (CXMVPlayerBuffers, CThingManager's CClassFactory, CDiskFileWin32, CTCBase) do not inline it.
#  Per row the derived class's own destructor stays as the row declares it (explicit where the PDB class declares one: ~CXMVPlayerBuffers,
#  ~CThingManager, ~CDiskFileWin32; the CTCBase implicit dtor otherwise); only the base declaration changes.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
DECL = 'virtual ~CBaseClassNonCopyable();   // 0x0099A300 ?? 1CBaseClassNonCopyable@@MAE@XZ (Ego_r / FableWin protected virtual; naming wave 25)'.replace('?? 1', '??1')
he.edit('0042db40', 'src', '''class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();                                // 0x0099A2F0
};''', '''class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();                                // 0x0099A2F0
    ''' + DECL + '''
};''')
he.edit('0055d070', 'src', '''class CBaseClassNonCopyable : public CBaseClass {
public:
    CBaseClassNonCopyable();                                // 0x0099A2F0
};''', '''class CBaseClassNonCopyable : public CBaseClass {
public:
    CBaseClassNonCopyable();                                // 0x0099A2F0
protected:
    ''' + DECL + '''
};''')
he.edit('00523540', 'src', '''class CBaseClassNonCopyable : public CBaseClass { };''', '''class CBaseClassNonCopyable : public CBaseClass { protected: ''' + DECL + '''
};''')
for a in ('00770e50', '00778ae0', '0077b250', '0077b9b0', '0077be40'):
    he.edit(a, 'src', ''' class CBaseClassNonCopyable:public CBaseClass {};''', ''' class CBaseClassNonCopyable:public CBaseClass { protected: ''' + DECL + '''
};''')
