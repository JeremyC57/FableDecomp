## 0x00492973

engine/CSubtitleRenderer.h: FABLE_RETAIL_CSUBTITLERENDERER_SETTEXTINFO -- nested CSetTextInfo (ctor 0x0048DDF6 / dtor 0x0043E3D0) + GetTextFromTextTag / ProcessEmbeddedGraphicTags / ProcessEmbeddedHighlightTags / InitPlayerMode (private) + SetButton1OverrideText decls (row 0x00492973).
engine/CMessageEventManager.h: FABLE_RETAIL_CMESSAGEEVENTMANAGER_FINDALL -- const member template FindAllMessages(const TFilter&, list<const CMessageEvent*>&) returning const long, decl only (0x00493EAB).
engine/CMessageEventFilters.h: FABLE_RETAIL_CMESSAGEEVENTFILTERS_ISOFTYPE_CTOR -- the inline CMessageEventFilter_IsOfType(EMessageEventType) ctor alone.
engine/CTCHeroReceiveItems.h: FABLE_RETAIL_CTCHERORECEIVEITEMS_DISPLAYINGITEMS (inside _PICKEDUPITEMS) -- PCurrentItem +0x1C, PersistCurrentItem +0x24, ReceivingItems +0x25, DisplayingItems +0x26, ForceClear +0x27.
stlport/fable_deque.h: FABLE_RETAIL_STLP_DEQUE_PUSH_BACK_OUTOFLINE -- deque::push_back declared only (0x004941C0); FABLE_RETAIL_STLP_QUEUE -- STLport queue with inline push().
Repair 3: no header changes (overlay unchanged); RemoveMessage defined inline+noinline in the row against the existing FABLE_RETAIL_CMESSAGEEVENTMANAGER_REMOVE declaration.
Repair 4: no overlay change; the row also defines the repo header's existing FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE (stlport/fable_containers.h, list::size() via STLport distance/__distance) so FindAllMessages byte-matches 0x00493EAB.

