# NF181 overlay

No shared-header overlay is used in this hand-back: every changed row compiles against the repo `rebuild/include` as it stands
(the vland runs passed `FABLE_HDR_OVERLAY` pointing at this empty directory). No header opt-in was added, so no landed includer
changes and no static assert was pinned. `orig/` is empty for the same reason.
