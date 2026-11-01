# family gcwf (hand): 0x004167C8 is ?GetCurrentWorldFrame@CMainGameComponent@@QBEJXZ (DECISIONS_W13_RESOLVED; Ego_r name set +
# body `jmp CWorld::GetFrame`), called on g_pMainGameComponent 0x013B86A0 (established spelling) -- the stand-in globals at
# 0x013B86A0 (g_pDisplayEngine_013b86a0, g_657280, g_908de0, g_pWorld) are respelled to it. The CTCLook callers reach the
# object through CTCBase::PeekComponent 0x00686D60 (Ego_r / FableWin ?PeekComponent@CTCBase@@IBEABVCMainGameComponent@@XZ,
# body `mov eax,[0x013B8A1C] ; mov eax,[eax+1Ch]`), modelled as the protected const getter of a local CTCBase base.
# The CAIBrain callers keep their +0x14 object and zero-extend the frame into the 64-bit sum: `(unsigned long)` keeps that.
# 0x0049D870 is ?GetFrame@CWorld@@SIJXZ (static __fastcall): the SA spelling (0x0071D940) and the free GetQuakeFrame
# (0x00699560) are respelled. HE_OV=<overlay>
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit, edit_all
MGC = 'class CMainGameComponent { public:\n    long GetCurrentWorldFrame() const;             // 0x004167C8 (Ego_r.pdb ?GetCurrentWorldFrame@CMainGameComponent@@QBEJXZ)\n};\n'
EXT = 'extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0'
GCWF = 'g_pMainGameComponent->GetCurrentWorldFrame()'
# 0x00657240
A = '00657240'
edit(A, 'src', 'class CDisplayEngine { public:\n    long GetFrame();                               // 0x004167C8\n};\nextern CDisplayEngine* g_pDisplayEngine_013b86a0;  // 0x013B86A0', MGC + EXT)
edit(A, 'src', 'g_pDisplayEngine_013b86a0->GetFrame()', GCWF)
edit(A, 'src', '// Callees: the frame counter getter @ 0x004167C8 on the global display engine pointer at 0x013B86A0', '// Callees: CMainGameComponent::GetCurrentWorldFrame @ 0x004167C8 on g_pMainGameComponent 0x013B86A0')
# 0x00657280, 0x00908DE0
for A, g in (('00657280', 'g_657280'), ('00908de0', 'g_908de0')):
    edit(A, 'src', 'struct G { int Helper(); };\nextern G* %s;' % g, MGC + EXT)
    edit(A, 'src', '%s->Helper()' % g, GCWF)
# 0x00908DA0, 0x00909070
edit('00908da0', 'src', 'struct CWorld_Retail {\n    long GetCurrentFrame();                    // 0x004167C8 -> GFGetCurrentFrame 0x0049D870\n};\n\nextern CWorld_Retail* g_pWorld;                // 0x013B86A0', MGC + '\n' + EXT)
edit('00908da0', 'src', 'g_pWorld->GetCurrentFrame()', GCWF)
edit('00908da0', 'src', '// Callees: the frame counter through the world object @ 0x013B86A0', '// Callees: CMainGameComponent::GetCurrentWorldFrame on g_pMainGameComponent @ 0x013B86A0')
edit('00909070', 'src', 'struct CWorld_Retail {\n    long GetCurrentFrame();                                     // 0x004167C8 -> GFGetCurrentFrame 0x0049D870\n};\nextern CWorld_Retail* g_pWorld;                                 // 0x013B86A0', MGC + EXT)
edit('00909070', 'src', 'g_pWorld->GetCurrentFrame()', GCWF)
# 0x008AF1E0 / 0x008AF5F0 / 0x008AF780
for A in ('008af1e0', '008af5f0', '008af780'):
    edit(A, 'src', 'class CWorld { public:\n    long GetFrameNumber();                  // 0x004167C8 (manifest label "GFGetPreMainMemoryUsed" is propagated)\n};\nextern CWorld* g_pWorld;                    // 0x013B86A0', MGC + EXT)
    edit_all(A, 'src', 'g_pWorld->GetFrameNumber()', GCWF)
    if A == '008af5f0': edit_all(A, 'src', '((CWorldPlayers*)g_pWorld)', '((CWorldPlayers*)g_pMainGameComponent)')
    if A != '008af1e0': edit(A, 'src', '// g_pWorld viewed for its player manager (+0x1C)', '// g_pMainGameComponent viewed for its player manager (+0x1C)')
    edit_all(A, 'src', 'GetFrameNumber 0x004167C8 on g_pWorld 0x013B86A0', 'CMainGameComponent::GetCurrentWorldFrame 0x004167C8 on g_pMainGameComponent 0x013B86A0') if A != '008af1e0' else None
edit('008af1e0', 'src', "// Callees: the world's GetFrameNumber 0x004167C8 on g_pWorld 0x013B86A0", '// Callees: CMainGameComponent::GetCurrentWorldFrame 0x004167C8 on g_pMainGameComponent 0x013B86A0')
# 0x0088B810 / 0x0088BAC0 / 0x0088BCB0 (Component +0x14)
edit('0088b810', 'src', '    unsigned long GetFrameNumber();         // 0x004167C8 -> 0x0049D870', '    long GetCurrentWorldFrame() const;      // 0x004167C8 (Ego_r.pdb ?GetCurrentWorldFrame@CMainGameComponent@@QBEJXZ) -> 0x0049D870')
edit('0088b810', 'src', '(offset + Component->GetFrameNumber())', '(offset + (unsigned long)Component->GetCurrentWorldFrame())')
edit('0088b810', 'src', '// Callees: CMainGameComponent::GetFrameNumber @ 0x004167C8', '// Callees: CMainGameComponent::GetCurrentWorldFrame @ 0x004167C8')
edit('0088bac0', 'src', '    unsigned long GetFrameNumber();                      // 0x004167C8', '    long GetCurrentWorldFrame() const;                   // 0x004167C8 (Ego_r.pdb ?GetCurrentWorldFrame@CMainGameComponent@@QBEJXZ)')
edit('0088bac0', 'src', '(offset + Component->GetFrameNumber())', '(offset + (unsigned long)Component->GetCurrentWorldFrame())')
edit('0088bac0', 'src', '// CMainGameComponent::GetFrameNumber @ 0x004167C8', '// CMainGameComponent::GetCurrentWorldFrame @ 0x004167C8')
edit('0088bcb0', 'src', '    unsigned long GetWorldFrame() const;                // 0x004167C8', '    long GetCurrentWorldFrame() const;                  // 0x004167C8 (Ego_r.pdb ?GetCurrentWorldFrame@CMainGameComponent@@QBEJXZ)')
edit('0088bcb0', 'src', '(offset + Component->GetWorldFrame())', '(offset + (unsigned long)Component->GetCurrentWorldFrame())')
# CTCLook 0x00751AD0 / 0x00753AD0 / 0x00753C70: CTCBase::PeekComponent
PC = '    const CMainGameComponent& PeekComponent() const;   // 0x00686D60 (Ego_r.pdb ?PeekComponent@CTCBase@@IBEABVCMainGameComponent@@XZ)\n'
A = '00751ad0'
edit(A, 'src', 'struct CFrameSource {\n    long GetFrameNumber();                         // 0x004167C8\n};\n\nclass CTCLook { public:\n    unsigned char _pad_0x00[0x3c];\n',
     MGC + 'class CThing;\nclass CTCBase { public:\n    void*         __vftable;                       // +0x00\n    CThing*       ParentThing;                     // +0x04\nprotected:\n' + PC + '};\n\n'
     'class CTCLook : public CTCBase { public:\n    unsigned char _pad_0x08[0x34];\n')
edit(A, 'src', '    CFrameSource* GetFrameSource();                // 0x00686D60 (CTCBase accessor)\n', '')
edit(A, 'src', 'GetFrameSource()->GetFrameNumber()', 'PeekComponent().GetCurrentWorldFrame()')
edit(A, 'src', '// Callees: the CTCBase accessor @ 0x00686D60', '// Callees: CTCBase::PeekComponent @ 0x00686D60')
edit(A, 'src', '// the manifest\'s CViewBase::GetDisplayEngine is propagated), the frame-number getter @ 0x004167C8 on that object', '// the manifest\'s CViewBase::GetDisplayEngine is propagated), CMainGameComponent::GetCurrentWorldFrame @ 0x004167C8 on that object')
A = '00753ad0'
edit(A, 'src', 'struct CFrameSource {\n    long GetFrameNumber();                      // 0x004167C8\n};\nclass CTCLook {\npublic:\n    virtual void v00();\n    CThing* ParentThing;                        // +0x04 (CTCBase)\n',
     MGC + 'class CTCBase {\npublic:\n    virtual void v00();\n    CThing* ParentThing;                        // +0x04\nprotected:\n' + PC + '};\nclass CTCLook : public CTCBase {\npublic:\n')
edit(A, 'src', '    CFrameSource* GetFrameSource();             // 0x00686D60 (CTCBase accessor)\n', '')
edit(A, 'src', 'GetFrameSource()->GetFrameNumber()', 'PeekComponent().GetCurrentWorldFrame()')
edit(A, 'src', 'the CTCBase frame-source accessor @ 0x00686D60 (`[0x013B8A1C]+0x1C`) and its GetFrameNumber @ 0x004167C8', 'CTCBase::PeekComponent @ 0x00686D60 (`[0x013B8A1C]+0x1C`) and CMainGameComponent::GetCurrentWorldFrame @ 0x004167C8')
A = '00753c70'
edit(A, 'src', 'struct CFrameSource {\n    long GetFrameNumber();                                 // 0x004167C8\n};\n\nclass CTCLook { public:\n    void*         __vftable;                               // +0x00\n    CThing*       ParentThing;                             // +0x04\n',
     MGC + 'class CTCBase { public:\n    void*         __vftable;                               // +0x00\n    CThing*       ParentThing;                             // +0x04\nprotected:\n' + PC + '};\n\nclass CTCLook : public CTCBase { public:\n')
edit(A, 'src', '    CFrameSource* GetFrameSource();                        // 0x00686D60 (CTCBase accessor)\n', '')
edit(A, 'src', 'GetFrameSource()->GetFrameNumber()', 'PeekComponent().GetCurrentWorldFrame()')
edit(A, 'src', 'the CTCBase\n//   frame-source accessor @ 0x00686D60 and its GetFrameNumber @ 0x004167C8', 'CTCBase::PeekComponent\n//   @ 0x00686D60 and CMainGameComponent::GetCurrentWorldFrame @ 0x004167C8')
# 0x0049D870 callers
edit('0071d940', 'src', '    static long GetFrame();                                 // 0x0049D870 (returns the global @ 0x013B89BC)',
     '    static long __fastcall GetFrame();                      // 0x0049D870 ?GetFrame@CWorld@@SIJXZ (returns the global @ 0x013B89BC)')
edit('00699560', 'src', 'int __fastcall GetQuakeFrame(); //49D870 global13B89BC', 'class CWorld { public: static long __fastcall GetFrame(); }; //49D870 ?GetFrame@CWorld@@SIJXZ global13B89BC')
edit('00699560', 'src', 'quake.Frame=GetQuakeFrame();', 'quake.Frame=CWorld::GetFrame();')
