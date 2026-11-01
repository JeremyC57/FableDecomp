# family queue (hand): wave 14 queue rows that need a use-site edit besides the declaration.
#  - 0x009F50A0 ?IsKeyboardEventInQueue@CInputManager@@QBE_NW4EInputEventType@@W4EInputKey@@AAVCInputEvent@@@Z (Ego_r: const,
#    CInputEvent&; queue c651): the two pointer callers pass the event by reference (same `lea; push` at the call).
#  - 0x006622A0 ?IsPerformingAnyAction@CThingCreatureBase@@QBE_NXZ (Ego_r const; queue c649): the three rows that define the
#    member inline in their CThingCreatureBase model make it const (the emitted COMDAT is the call target).
# HE_OV=<overlay>
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit
edit('0041e10d', 'src', '    bool IsKeyboardEventInQueue(EInputEventType type, EInputKey key, CInputEvent* event);   // 0x009F50A0',
     '    bool IsKeyboardEventInQueue(EInputEventType type, EInputKey key, CInputEvent& event) const;   // 0x009F50A0 (Ego_r.pdb const, reference)')
edit('0041e10d', 'src', "IsKeyboardEventInQueue(EIET_KEY_DOWN, (EInputKey)m_keyMap[key], &event)", "IsKeyboardEventInQueue(EIET_KEY_DOWN, (EInputKey)m_keyMap[key], event)")
edit('00759d30', 'src', '    bool IsKeyboardEventInQueue(EInputEventType type, EInputKey key, CInputEvent* event);   // 0x009F50A0',
     '    bool IsKeyboardEventInQueue(EInputEventType type, EInputKey key, CInputEvent& event) const;   // 0x009F50A0 (Ego_r.pdb const, reference)')
edit('00759d30', 'src', "IsKeyboardEventInQueue((EInputEventType)2, (EInputKey)0x2e, &event)", "IsKeyboardEventInQueue((EInputEventType)2, (EInputKey)0x2e, event)")
for a in ('006621c0', '006622d0', '006643c0'):
    edit(a, 'src', '    bool IsPerformingAnyAction()\n', '    bool IsPerformingAnyAction() const                   // 0x006622A0 (Ego_r.pdb const)\n')
