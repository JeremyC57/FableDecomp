# T20: NUISystem component constructors (cx12 4326 4335-4340): the bases take `long` (Ego_r/FableWin ??0C...@NUISystem@@QAE@J@Z)
# and live in NUISystem under their retail names; own ctors take `long` too (PDB).
C = "rebuild/src/compiled/"
DRG = C + "00/55/CDraggable_CDraggable_0055a9c0.cpp"
CLK = C + "00/55/CClickable_CClickable_0055b460.cpp"
NAV = C + "00/55/CNavButton_CNavButton_00558b90.cpp"
FEB = C + "00/54/NUISystemCFrontEndButton_CFrontEndButton_0054e0b0.cpp"
SBO = C + "00/55/NUISystemCScrollBarOutside_CScrollBarOutside_00559360.cpp"
SCB = C + "00/55/NUISystemCScrollBar_CScrollBar_00559830.cpp"
KRD = C + "00/55/NUISystemCKeyRedefiner_CKeyRedefiner_00558540.cpp"
RDL = C + "00/55/NUISystemCRedefinerList_CRedefinerList_00556350.cpp"
MSH = C + "00/55/NUISystemCMesh_CMesh_00550190.cpp"
MOV = C + "00/54/NUISystemCMovie_CMovie_005482d0.cpp"
TSL = C + "00/54/NUISystemCTextSlider_CTextSlider_00549f60.cpp"
TXT = C + "00/54/NUISystemCText_CText_0054f5c0.cpp"
SLD = C + "00/54/NUISystemCSlider_CSlider_0054c050.cpp"
MSP = C + "00/55/NUISystemCMorphingSprite_CMorphingSprite_005545d0.cpp"
MCU = C + "00/55/NUISystemCMouseCursor_CMouseCursor_0055c650.cpp"
SVP = C + "00/53/NUISystemCScrollingViewport_CScrollingViewport_0053f120.cpp"
NOTE = "hence the\n// local models (retail names in NUISystem, the bytes' sizes)"
EDITS = []
for f, own in ((DRG, "CDraggable"), (CLK, "CClickable")):
    EDITS += [
        (f, "hence the\n// `_Retail` names)", NOTE),
        (f, "struct CComponentInterface {", "namespace NUISystem {\n\nstruct CComponentInterface {"),
        (f, "CHoverable_Retail", "CHoverable", 0),
        (f, "CClickable_Retail", "CClickable", 0),
    ]
EDITS += [
    (DRG, "class C2DVector { public:", "}\n\nclass C2DVector { public:"),
    (DRG, "    C2DVector() : X(0.0f), Y(0.0f) { }\n};\n", "    C2DVector() : X(0.0f), Y(0.0f) { }\n};\n\nnamespace NUISystem {\n"),
    (DRG, "CDraggable_Retail::CDraggable_Retail(long owner)", "}\n\nNUISystem::CDraggable::CDraggable(long owner)"),
    (DRG, "CDraggable_Retail", "CDraggable", 0),
    (CLK, "CClickable::CClickable(long owner)", "}\n\nNUISystem::CClickable::CClickable(long owner)"),
    (CLK, "CDraggable_Retail", "CDraggable", -1),
    # CNavButton: global stand-ins -> NUISystem (own ctor ??0CNavButton@NUISystem@@QAE@J@Z, Ego_r 0x005D1CC0)
    (NAV, "hence the `_Retail` names", "hence the local models"),
    (NAV, "struct CComponentInterface {", "namespace NUISystem {\n\nstruct CComponentInterface {"),
    (NAV, "CNavButton::CNavButton(long owner)", "}\n\nNUISystem::CNavButton::CNavButton(long owner)"),
    (NAV, "CClickable_Retail", "CClickable", 0),
    (FEB, "CClickable_Retail", "CClickable", 0),
    (SBO, "CClickable_Retail", "CClickable", 0),
    (SCB, "CDraggable_Retail", "CDraggable", 0),
    # CKeyRedefiner / CRedefinerList: the bases and the own ctors take the def index (PDB QAE@J@Z)
    (KRD, "CKeyRedefiner(CComponentManager* manager)", "CKeyRedefiner(long def)", 0),
    (KRD, "CClickable(CComponentManager* manager);     // 0x0055B460", "CClickable(long def);                       // 0x0055B460"),
    (KRD, "    : CClickable(manager)", "    : CClickable(def)"),
    (RDL, "CRedefinerList(CComponent*)", "CRedefinerList(long)"),
    (RDL, "CScrollableList(CComponent*)", "CScrollableList(long)"),
    (RDL, "CScrollableList(CComponent* parent);                    // 0x00555180", "CScrollableList(long def);                              // 0x00555180"),
    (RDL, "CRedefinerList(CComponent* parent)", "CRedefinerList(long def)", 2),
    (RDL, "    : CScrollableList(parent)", "    : CScrollableList(def)"),
    # 0x0052CC50 is NUISystem::CChangingStateComponent::CChangingStateComponent(long) (Ego_r 0x005A77D0, FableWin 0x02283860)
    (MOV, "CDraggableInto", "CChangingStateComponent", 0),
    (TSL, "CDraggableInto", "CChangingStateComponent", 0),
    (TXT, "CDraggableInto", "CChangingStateComponent", 0),
    (MSH, "struct CDraggableInto : ", "namespace NUISystem {\nstruct CChangingStateComponent : "),
    (MSH, "    CDraggableInto(void* def);                     // 0x0052CC50\n};", "    CChangingStateComponent(long def);             // 0x0052CC50\n};\n}"),
    (MSH, "CDraggableInto", "CChangingStateComponent", 0),
    (MSH, "void* def", "long def", 0),
    # CSprite / CViewport: global void* stand-ins -> NUISystem, long
    (SLD, "struct CSprite : ", "namespace NUISystem {\nstruct CSprite : "),
    (SLD, "    CSprite(void* def);                            // 0x0041B800\n};", "    CSprite(long def);                             // 0x0041B800\n};\n}"),
    (SLD, "void* def", "long def", 0),
    (MSP, "struct CSprite : ", "namespace NUISystem {\nstruct CSprite : "),
    (MSP, "    CSprite(void* def);                            // 0x0041B800\n};", "    CSprite(long def);                             // 0x0041B800\n};\n}"),
    (MSP, "CSprite::CSprite(void*)", "CSprite::CSprite(long)"),
    (MSP, "void* def", "long def", 0),
    (MCU, "struct CSprite : ", "namespace NUISystem {\nstruct CSprite : "),
    (MCU, "    CSprite(void* def);                            // 0x0041B800\n    ~CSprite();                                    // 0x0041AB40\n};",
          "    CSprite(long def);                             // 0x0041B800\n    virtual ~CSprite();                            // 0x0041AB40\n};\n}"),
    (MCU, "void* def", "long def", 0),
    (SVP, "struct CViewport : ", "namespace NUISystem {\nstruct CViewport : "),
    (SVP, "    CViewport(void* def);                          // 0x0054EA00\n};", "    CViewport(long def);                           // 0x0054EA00\n};\n}"),
    (SVP, "void* def", "long def", 0),
]
