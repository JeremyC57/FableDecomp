# queue c651 0x009F4ED0 (wave-14 DEFERRED, KEEP ?EventScanStart@CInputManager@@QAE...): Ego_r spells EventScanStart const
# (?EventScanStart@CInputManager@@QBE_NAAVCInputEventScanInfo@@AAVCInputEvent@@@Z; both landed callers 0x0042E3EE / 0x00446330
# already declare it const). The body increments NoEventScansRunning (+0x20), so the counter is modelled mutable; the
# EventStore read goes through the const begin().
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit
a = '009f4ed0'
edit(a, 'src', '    iterator begin() { return iterator(first); }', '    iterator begin() const { return iterator(first); }')
edit(a, 'src', '    long          NoEventScansRunning;              // +0x20\n    bool EventScanStart(CInputEventScanInfo& scan, CInputEvent& event);',
     '    mutable long  NoEventScansRunning;              // +0x20 (mutable: the const EventScanStart counts the scan)\n    bool EventScanStart(CInputEventScanInfo& scan, CInputEvent& event) const;   // Ego_r QBE')
edit(a, 'src', 'bool CInputManager::EventScanStart(CInputEventScanInfo& scan, CInputEvent& event)\n{', 'bool CInputManager::EventScanStart(CInputEventScanInfo& scan, CInputEvent& event) const\n{')
edit(a, 'src', '// CInputManager::EventScanStart(CInputEventScanInfo& scan, CInputEvent& event) --', '// CInputManager::EventScanStart(CInputEventScanInfo& scan, CInputEvent& event) const --')
