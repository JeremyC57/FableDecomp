# T20b: 0x0055A890 / 0x0055B040 are CDraggable / CClickable::GetDataFromDef (Ego_r 0x005D3C60 / 0x005D44B0, FableWin 0x022B7B90 / 0x022B9290;
# landed binders 0x0055AA20 / 0x0055B4C0 already spell them so).
DRG = "rebuild/src/compiled/00/55/CDraggable_CDraggable_0055a9c0.cpp"
CLK = "rebuild/src/compiled/00/55/CClickable_CClickable_0055b460.cpp"
EDITS = [
    (DRG, "RegisterDragging", "GetDataFromDef", 0),
    (DRG, "InitialiseInputDelay", "GetDataFromDef", 0),
    (CLK, "InitialiseInputDelay", "GetDataFromDef", 0),
]
