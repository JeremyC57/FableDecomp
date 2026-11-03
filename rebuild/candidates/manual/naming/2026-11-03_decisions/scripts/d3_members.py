# d3_members.py <files>: pass 11 descriptive members -> GameCODA SDK members (offsets in engine/CEAXListener.h / CEAXBuffer.h).
import re, sys
M = [("m_activeSlots.guidActiveFXSlots", "m_Source.eax4BPActiveFXSlotID"),
     ("m_effectiveDistanceFactor", "m_Dest.flDistanceFactor"), ("m_distanceFactor", "m_Listener.flDistanceFactor"),
     ("m_primarySlot", "m_ListenerSlotID"), ("m_pSources", "m_lpBufferList"), ("m_slots", "m_Source.eax4FXSlot"),
     ("m_reverb", "m_Source.eax4LPSlot"), ("m_context", "m_Source.eax4Context"), ("m_defaultReverb", "m_Source.eax3LP"),
     ("m_committed", "m_Dest.eax3LP"), ("m_dirty", "m_dwEAX3Changes"), ("m_pEvents", "lpChanges"),
     ("m_lastError", "m_lEAX4ErrorCode"), ("m_pNext", "lpNext"), ("m_pSource", "lpEAXBuffer"),
     ("m_activeReverb", "m_bEAX4Active"), ("m_pContext", "m_lpEAXListener"), ("m_current", "m_Source.eax4BP"),
     ("m_sends", "m_Source.eax4BPSend"), ("m_occlusionSends", "m_Source.eax4BPOcclusionSend"),
     ("m_exclusionSends", "m_Source.eax4BPExclusionSend"), ("m_defaultProperties", "m_Source.eax3BP"),
     ("m_deferred", "m_Dest.eax3BP"), ("m_sourceID", "m_BufferID")]
for p in sys.argv[1:]:
    t0 = t = open(p).read()
    for a, b in M: t = re.sub(r"\b%s\b" % re.escape(a) if "." not in a else re.escape(a), b, t)
    # AddToChangesList(IDENTIFIER,...): the group literals are the SDK enumerators
    t = re.sub(r"QueueEvent_00c5ffb0\(0,", "AddToChangesList(EAX3LISTENER,", t)
    t = re.sub(r"QueueEvent_00c5ffb0\(1,", "AddToChangesList(EAX3BUFFER,", t)
    t = re.sub(r"QueueEvent_00c5ffb0\(4,", "AddToChangesList(LISTENERUPDATE,", t)
    t = re.sub(r"QueueEvent_00c5ffb0\(5,", "AddToChangesList(BUFFERUPDATE,", t)
    if t != t0: open(p, "w").write(t); print("changed", p)
