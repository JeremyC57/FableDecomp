# T30: 0x0055A890 is NUISystem::CDraggable::GetDataFromDef (Ego_r 0x005D3C60 / FableWin 0x022B7B90): the CDraggable ctors
# 0x0055A9C0 / 0x0055AA20 (vtables 0x0124BA94.. = .?AVCDraggable@NUISystem@@) call it, and its two actions fill the two
# CDraggable lists (PActionsOnDropped +0x1a0 / PActionsOnDroppedNowhere +0x1a4). CClickable's is 0x0055B040 (ctor 0x0055B460).
F = "rebuild/src/compiled/00/55/NUISystemCClickable_GetDataFromDef_0055a890.cpp"
EDITS = [
    (F, "// NUISystem::CClickable::GetDataFromDef() -- add the def's two configured actions (AddActionOnLeftClicked / AddActionOnLeftHeld)",
        "// NUISystem::CDraggable::GetDataFromDef() -- add the def's two configured actions (AddActionOnDropped / AddActionOnDroppedNowhere)"),
    (F, "// Leaf: ?GetDataFromDef@CClickable@NUISystem@@QAEXXZ", "// Leaf: ?GetDataFromDef@CDraggable@NUISystem@@QAEXXZ"),
    (F, "AddActionOnLeftClicked", "AddActionOnDropped", 0),
    (F, "AddActionOnLeftHeld", "AddActionOnDroppedNowhere", 0),
    (F, "class CClickable {", "class CDraggable {"),
    (F, "void CClickable::GetDataFromDef()", "void CDraggable::GetDataFromDef()"),
]
