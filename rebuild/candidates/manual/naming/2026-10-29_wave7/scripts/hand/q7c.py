# wave 7 hand edits, part c: const cascades that the CTCBase base injection (fam_tcb7) exposes; run BEFORE fam_tcb7.
#   0x00686A80 is the const ?PeekWorld@CTCBase@@IBEABVCWorld@@XZ, so what retail calls on its result is const:
#   CWorld slot 13 PeekWorldMap (FableWin.pdb ?PeekWorldMap@CWorld@@UBEABVCWorldMap@@XZ), the inline GetAllThings wrapper, and
#   CWorld 0x0049D810 (first binding ?GetPRandomSeed@CWorld@@QBEPAKXZ, by 00662040).
#   005bc5c6: ?GetRightStickPosition@CTCInventoryBase@NInventory@@QAEXAAVC2DVector@@@Z is NON-const (Ego_r + FableWin), and the
#   CTCBase base is global (fam_tcb7 would inject it inside namespace NInventory), so this row is done here, not by the family.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit

for F in ('005fc620', '005fd1d0'):
    edit(F, 'src', 'CTCBase::GetWorld() const 0x00686A80, CWorld slot 13 (+0x34)', 'CTCBase::PeekWorld() const 0x00686A80, CWorld slot 13 (+0x34)')
    edit(F, 'src', '    virtual CWorldMap* PeekWorldMap();                // slot 13 (+0x34)\n',
         '    virtual const CWorldMap* PeekWorldMap() const;    // slot 13 (+0x34) (FableWin.pdb: const)\n')
    edit(F, 'src', '    void GetAllThings(const CFilter_Bind2& filter, list<CThing*>& out) { PThingSearchTools',
         '    void GetAllThings(const CFilter_Bind2& filter, list<CThing*>& out) const { PThingSearchTools')
    edit(F, 'src', 'CWorldMap* worldMap = GetWorld()', 'const CWorldMap* worldMap = GetWorld()')

F = '0078bfc0'
edit(F, 'src', '    unsigned long* GetPRandomSeed();                        // 0x0049D810\n',
     '    unsigned long* GetPRandomSeed() const;                  // 0x0049D810 (bound const, 00662040)\n')

F = '005bc5c6'
edit(F, 'src', '// CTCInventoryBase::GetRightStickPosition(C2DVector&) const -- stores',
     '// NInventory::CTCInventoryBase::GetRightStickPosition(C2DVector&) -- stores')
edit(F, 'src', '\nnamespace NInventory {\n\nclass CTCInventoryBase { public:\n',
     '\nclass CTCBase { protected:\n    CPlayerManager& GetPlayerManager();   // 0x00686D40 (Ego_r.pdb CTCBase, protected)\n};\n\n'
     'namespace NInventory {\n\nclass CTCInventoryBase : public ::CTCBase { public:\n')
edit(F, 'src', '    CPlayerManager* GetPlayerManager() const;        // 0x00686D40 (CTCBase)\n', '')
edit(F, 'src', '    void GetRightStickPosition(C2DVector& out) const;\n',
     '    void GetRightStickPosition(C2DVector& out);      // Ego_r.pdb / FableWin.pdb: non-const\n')
edit(F, 'src', 'void CTCInventoryBase::GetRightStickPosition(C2DVector& out) const\n', 'void CTCInventoryBase::GetRightStickPosition(C2DVector& out)\n')
edit(F, 'src', 'GetPlayerManager()->GetMainPlayer()', 'GetPlayerManager().GetMainPlayer()')
