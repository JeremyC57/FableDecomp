# OUTLINE 0x00598A1C  void NUISystem::CFrontEndManager::BuildComponents(bool showMediaPlayerError)

## Frame
* `push ebp; lea ebp,[esp-74h]; sub esp,0C4h; push ebx; push esi; push edi` ... `pop edi; pop esi; pop ebx; add ebp,74h; leave; ret 4`.
* No EH states, no /GS. Registers: edi = -1 whole function (CCharString length), ebx = 0 until 0x00599169, esi = &m_mapScreens from 0x00598B93.
* Frame map (ebp-relative; function-scope objects first):
  - [ebp-50h..-48h] std::vector<CWideString> profileNames (inline zero ctor at entry; dtor 0x00412130 at exit)
  - [ebp-38h..-34h] CCountedPointer<CAction> action (ctor 0x0042AA29; ReleaseData 0x00429160 at exit)
  - [ebp+18h..+20h] CArray<ESurfaceMultisampleType> antialiasingModes (inline zero ctor at 0x00599327; inline `if(Data) free` at exit)
  - [ebp+40h..+48h] std::vector<CCharString> names (inline zero ctor 0x0059974B; ~vector 0x00414EA0 at exit)
  - [ebp+38h] std::list<CComponent*> rates (ctor 0x0042ABAF with allocator temp [ebp+7Fh]; ~list 0x0042ABCA at exit)
  - [ebp+34h] this spill; [ebp+7Ch] param (later the final key temp)
  - [ebp+6Ch] map-key temp / spilled map[] reference / `text` comp; [ebp+70h] CCharString temp (also a CRGBColour local later)
  - media block: [ebp+64h] message, [ebp+4Ch] code text, [ebp+68h]/[ebp+60h]/[ebp+5Ch] operator+ temps
  - resolution: [ebp+2Ch] resolutionSlider, [ebp+3Ch] &displayModes, ebx = mode iterator, [ebp+54h] CCountedPointer<CComponent> entryPointer, [ebp+28h] entryText, temps [ebp+30h],[68h],[60h],[64h],[4Ch],[6Ch], result [ebp+5Ch]
  - anti-aliasing: [ebp+58h] antialiasingSlider, [ebp+60h] level iterator, [ebp+44h] entryPointer, [ebp+50h] entryText, [ebp+68h] levelText; each case its own temp pair (e.g. case 2 [2Ch]/[3Ch] ... case 16 [24h]/[-0Ch]); ebx = L"X" hoisted
  - redefine: [ebp+50h] accept
  - detail sliders: [ebp+64h] name iterator, [ebp+68h] whole, [ebp+60h] state iterator, colours [ebp+5Ch] (left) / [ebp+70h] (right)
  - text sliders: [ebp+68h] name iterator, [ebp+60h] whole, [ebp+64h] state iterator, colours [ebp+70h] (left) / [ebp+5Ch] (right)
  - refresh: [ebp+64h] list iterator, [ebp+70h] rateText, [ebp+5Ch] CCharString symbol, temps [ebp+24h],[50h],[58h]

## Body (retail order)
```cpp
void CFrontEndManager::BuildComponents(bool showMediaPlayerError)
{
    std::vector<CWideString> profileNames;                         // 0x00598A2C
    GetProfileNames(profileNames);                                 // 0x00598A3E call 0x005955AB  (result unused)

    if (showMediaPlayerError) {                                    // 0x00598A46 cmp [ebp+7Ch],0 ; je 0x00598B90
        m_mapScreens[FRONTEND_SCREEN_MEDIA_PLAYER_ERROR] =         // key -1 = edi at [ebp+5Ch]; map[] 0x0059B5D7 BEFORE GetInstance
            NUISystem::CManager::GetInstance()->CreateComponent("UI_FRONTEND_MEDIA_PLAYER_ERROR", false); // 0x00598A50..0x00598A8A
        CComponent* text = m_mapScreens[FRONTEND_SCREEN_MEDIA_PLAYER_ERROR]->FindChild("UI_MEDIA_PLAYER_ERROR_TEXT"); // 0x00598A8F, vtbl +0Ch
        if (text) {                                                // 0x00598AB8 je 0x00598B8D (reloads this into esi)
            CWideString message = CNewFrontendGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_VIDEO_PLAYER_ERROR"); // 0x00598AC3, result [ebp+64h]
            message += L"\n\n" + CNewFrontendGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_VIDEO_PLAYER_ERROR_CODE")
                       + L" " + MediaPlayerErrorString;            // 0x00598AEE..0x00598B6B: 0x0099BFF0, 0x0099BF30, 0x0099BE70, += 0x0099B8D0; dtors 5C,60,68,4C, ~CCharString 70
            static_cast<CText*>(text)->SetValue(message, true, true);   // 0x00598B7D vtbl +240h
        }                                                          // ~message 0x00598B86
    }
    // 0x00598B90 (esi = this+0x54 from here)
    m_mapScreens[FRONTEND_SCREEN_MAIN_MENU] = 0;                   // key 0 = ebx; store at 0x00598BAA interleaved with next ctor
    // 0x00598BA1.. each: CCharString name(lit) ; map[key] FIRST ; CManager::GetInstance()->CreateComponent(name,false) ; store ; ~name
    m_mapScreens[FRONTEND_SCREEN_PRESS_START_MENU]          = CreateComponent("UI_FRONTEND_PRESS_START_MENU");          // 20  0x00598BA1
    m_mapScreens[FRONTEND_SCREEN_PROFILES_MENU]             = CreateComponent("UI_FRONTEND_PROFILES_MENU");             // 7   0x00598BE4
    m_mapScreens[FRONTEND_SCREEN_DELETE_PROFILE_MENU]       = CreateComponent("UI_FRONTEND_DELETE_PROFILE_MENU");       // 10  0x00598C25
    m_mapScreens[FRONTEND_SCREEN_PROFILE_SAVED_GAMES_MENU]  = CreateComponent("UI_FRONTEND_PROFILE_SAVED_GAMES_MENU");  // 8   0x00598C66
    m_mapScreens[FRONTEND_SCREEN_OPTIONS_MENU]              = CreateComponent("UI_FRONTEND_OPTIONS_MENU");              // 1   0x00598CA7
    m_mapScreens[FRONTEND_SCREEN_AUDIO_OPTIONS_MENU]        = CreateComponent("UI_FRONTEND_AUDIO_OPTIONS_MENU");        // 4   0x00598CE8
    m_mapScreens[FRONTEND_SCREEN_LIVE_AWARE_MENU]           = 0;                                                        // 2   0x00598D29 (store 0x00598D44 after the next push)
    m_mapScreens[FRONTEND_SCREEN_EXTRAS_MENU]               = CreateComponent("UI_FRONTEND_EXTRAS_MENU");               // 3   0x00598D3B
    m_mapScreens[FRONTEND_SCREEN_CREDITS_MENU]              = CreateComponent("UI_FRONTEND_CREDITS_MENU");              // 9   0x00598D7E
    m_mapScreens[FRONTEND_SCREEN_PROFILE_ALREADY_EXISTS_MENU]= CreateComponent("UI_FRONTEND_PROFILE_ALREADY_EXISTS_MENU");// 19 0x00598DBF
    m_mapScreens[FRONTEND_SCREEN_NO_PROFILES_MENU]          = CreateComponent("UI_FRONTEND_NO_PROFILES_MENU");          // 12  0x00598E00
    m_mapScreens[FRONTEND_SCREEN_INVALID_PROFILE_MENU]      = CreateComponent("UI_FRONTEND_INVALID_PROFILE_MENU");      // 15  0x00598E41
    m_mapScreens[FRONTEND_SCREEN_INVALID_SAVE_MENU]         = CreateComponent("UI_FRONTEND_INVALID_SAVE_MENU");         // 16  0x00598E82
    // 0x00598EC3
    CCountedPointer<CAction> action(new CAction);                  // new 0x10 (0x00BFEA1A), null-check, CAction() 0x0042BE50, ctor 0x0042AA29 into [ebp-38h]
    action->ActionType = UI_ACTION_TYPE_GO_TO_MAIN_MENU_FROM_START; // 229, 0x00598EE6
    m_mapScreens[FRONTEND_SCREEN_PRESS_START_MENU]->AddActionOnLeftClicked(action); // 0x00598EEC, vtbl +11Ch
    m_mapScreens[FRONTEND_SCREEN_PROFILES_FOR_DELETE_PC]    = CreateComponent("UI_FRONTEND_SCREEN_PROFILES_FOR_DELETE_PC"); // 21 0x00598F0C
    m_mapScreens[FRONTEND_SCREEN_REDEFINE_KEYS_MENU]        = CreateComponent("UI_FRONTEND_SCREEN_REDEFINE_KEYS_PC");   // 22  0x00598F4D
    m_mapScreens[FRONTEND_SCREEN_VIDEO_OPTIONS_MENU]        = CreateComponent("UI_FRONTEND_SCREEN_VIDEO_OPTIONS_PC");   // 5   0x00598F8E
    m_mapScreens[FRONTEND_SCREEN_NEW_PROFILE_PC]            = CreateComponent("UI_FRONTEND_NEW_PROFILE_SCREEN");        // 23  0x00598FCF
    m_mapScreens[FRONTEND_SCREEN_OPTIONS_SUB_MENU]          = CreateComponent("UI_FRONTEND_OPTIONS_SUB_MENU");          // 24  0x00599010
    m_mapScreens[FRONTEND_SCREEN_OPTIONS_SCOREBOARD]        = CreateComponent("UI_FRONTEND_OPTIONS_SCOREBOARD");        // 25  0x00599051
    m_mapScreens[FRONTEND_SCREEN_PROFILE_ERROR_PC]          = CreateComponent("UI_FRONTEND_PROFILE_ERROR_PC");          // 27  0x00599092
    m_mapScreens[FRONTEND_SCREEN_QUIT]                      = CreateComponent("UI_FRONTEND_QUIT_PROMPT");               // 26  0x005990D3
    m_mapScreens[FRONTEND_SCREEN_ABOUT]                     = CreateComponent("UI_FRONTEND_ABOUT_MENU");                // 28  0x00599114
    // (CreateComponent(...) above = NUISystem::CManager::GetInstance()->CreateComponent(CCharString(lit), false), named or temp CCharString in [ebp+70h])

    CComponent* about = m_mapScreens[FRONTEND_SCREEN_ABOUT]->FindChild("UI_FRONTEND_ABOUT_MESSAGE"); // 0x00599155
    if (about)                                                     // 0x00599182 je 0x005991AD
        static_cast<CText*>(about)->SetValue(GenerateAboutMessage(), false, true); // push 1, push 0 first; 0x00594FDB into [ebp+4Ch]; +240h; ~ 0x005991A8

    // 0x005991AD resolution slider -- twin lines 601-614 verbatim except names
    CComponent* resolutionSlider = m_mapScreens[FRONTEND_SCREEN_VIDEO_OPTIONS_MENU]->FindChild("UI_OPTIONS_RESOLUTION_TEXT_SLIDER"); // [ebp+2Ch]
    const std::vector<CDisplayModeDesc>& displayModes = CUserProfileManager::Get().GetDisplayModes();  // 0x005991DD/0x005991E4
    for (const_iterator mode = displayModes.begin(); mode != displayModes.end(); ++mode) {   // jmp to test 0x005992EE; body 0x005991F3; ++ 0x005992EB (+10h)
        CCountedPointer<CComponent> entryPointer(NUISystem::CManager::GetInstance()->CreateComponent("UI_OPTIONS_TEXT_SLIDER_ITEM_TEMPLATE", false)); // 0x00599217
        if (!entryPointer.IsNull()) {                              // 0x00599227 je 0x005992E0
            CText* entryText = static_cast<CText*>(entryPointer.Get());   // [ebp+28h]
            CWideString modeText = GFIntToWideString(mode->Dimensions.X) + L"X" + GFIntToWideString(mode->Dimensions.Y) + L"X" + GFIntToWideString(mode->BitsPerPixel);
            // eval: BPP(+8)->[30h], Y(+4)->[68h], X(+0)->[60h]; X+"X"->[64h]; +Y->[4Ch]; +"X"->[6Ch]; +BPP->[5Ch]; dtors 6C,4C,64,60,68,30
            entryText->SetValue(modeText, true, true);             // 0x005992C3
            resolutionSlider->AddChild(entryPointer);              // 0x005992DA +ECh (after ~modeText)
        }
    }                                                              // ReleaseData 0x005992E3

    // 0x005992F7 anti-aliasing slider -- twin lines 616-648
    CComponent* antialiasingSlider = m_mapScreens[FRONTEND_SCREEN_VIDEO_OPTIONS_MENU]->FindChild("UI_OPTIONS_ANTIALIASING_TEXT_SLIDER"); // key temp [ebp+3Ch]; result [ebp+58h]
    CArray<ESurfaceMultisampleType> antialiasingModes;             // 0x00599327 inline zero
    GFGetSystemManager()->GetDisplayManager()->EnumerateAntiAliasingModes(antialiasingModes); // 0x00599332 / [eax+60h] / 0x0059933E
    for (level = begin; level != end; ++level) {                   // test top 0x00599346 je 0x005996BF; body 0x00599352 (hoists ebx=L"X"); ++ 0x005996AF
        CCountedPointer<CComponent> entryPointer(...CreateComponent("UI_OPTIONS_TEXT_SLIDER_ITEM_TEMPLATE", false)); // [ebp+44h]
        if (!entryPointer.IsNull()) {                              // je 0x005996A7
            CText* entryText = ...;                                // [ebp+50h]
            CWideString levelText;                                 // 0x00599399 call 0x0099AED0
            switch (*level) {                                      // cmp 10h; ja 0x00599670; jmp [eax*4+0x00599D18]
            case 0 (SURFACE_MULTISAMPLE_NONE): levelText = L"TEXT_GUI_MENU_OFF"; break; // table[0]=table[1]=0x00599670 (= default)
            case 2: levelText = GFIntToWideString(2) + L"X"; break;    // 0x005993B3  (temps [2Ch],[3Ch]; shared ~ tail 0x00599669, jmp 0x0059967D)
            case 3: 0x005993E2 ; 4: 0x00599411 ; 5: 0x00599440 ; 6: 0x0059946F ; 7: 0x0059949E ; 8: 0x005994CD ; 9: 0x005994FC
            case 10: 0x0059952B ; 11: 0x0059955A ; 12: 0x00599589 ; 13: 0x005995B8 ; 14: 0x005995E7 ; 15: 0x00599613 ; 16: 0x0059963F
            default: levelText = L"TEXT_GUI_MENU_OFF"; break;          // 0x00599670 operator=(const wchar_t*) 0x0099B800
            }                                                      // case 1 has no label (falls to default via table)
            entryText->SetValue(levelText, true, true);            // 0x0059967D
        }   // ~levelText 0x00599693 ;  antialiasingSlider->AddChild(entryPointer) 0x005996A1 (inside the if, after ~levelText)
    }       // ReleaseData 0x005996AA

    // 0x005996BF redefine keys
    CComponent* accept = m_mapScreens[FRONTEND_SCREEN_REDEFINE_KEYS_MENU]->FindChild("UI_ACCEPT");   // [ebp+50h], evaluated FIRST
    static_cast<CRedefinerList*>(m_mapScreens[FRONTEND_SCREEN_REDEFINE_KEYS_MENU]->FindChild("UI_FRONTEND_LIST_REDEFINE_KEYS_MENU"))->SetAcceptButton(accept); // 0x00599721

    // 0x00599726 initialise every screen (begin inline: [esi] header, [hdr+8] leftmost; ++ = 0x004292C0)
    for (std::map<FRONTEND_SCREEN,CComponent*>::iterator it = m_mapScreens.begin(); it != m_mapScreens.end(); ++it)
        if (it->second) it->second->Initialise();                  // node+14h, vtbl +ACh

    // 0x0059974B detail sliders -- twin lines 676-710 (names: UI_SLIDER_TEXTURE_DETAIL / MESH / EFFECTS / SHADOW; arrows "UI_OPTIONS_LEFT_ARROW" / "UI_OPTIONS_RIGHT_ARROW")
    std::vector<CCharString> names;  names.push_back(...) x4 (CCharString temp [ebp+70h], push_back 0x0044BFF0)
    for (name ...) {                                               // top test 0x005997DE je 0x005999A3; body 0x005997ED; ++ 0x00599993
        CSlider* slider = static_cast<CSlider*>(m_mapScreens[FRONTEND_SCREEN_VIDEO_OPTIONS_MENU]->FindChild(*name)); // copy ctor 0x0099EC30
        if (slider && slider->Min.X == slider->Max.X && slider->GetParent() && slider->GetParent()->GetParent()) { // fucompp, jp 0x00599993
            CComponent* whole = slider->GetParent()->GetParent();  // [ebp+68h]
            CComponent* arrow = whole->FindChild("UI_OPTIONS_LEFT_ARROW");     // 0x00599882
            if (arrow) { arrow->SetPreventObserving(true); for (state ...GetStates()) state->second.Colour.A /= 2;
                         CRGBColour colour = arrow->GetColour(); colour.A /= 2; arrow->SetColour(colour); }  // colour [ebp+5Ch]
            arrow = whole->FindChild("UI_OPTIONS_RIGHT_ARROW");                // 0x0059990D
            if (arrow) { ...same...  colour at [ebp+70h] }         // 0x00599923..0x00599975
            CComponent* text = whole->FindFirstChildOfTypeNonRecursive(NUISystem::UI_TYPE_TEXT); // 0x0059997F
            if (text) text->MakeUnavailableDynamically();          // 0x0059998D
        }
    }
    names.clear();                                                 // 0x005999AC = erase(begin,end) 0x0043336A
    names.push_back("UI_OPTIONS_RESOLUTION_TEXT_SLIDER"); ("UI_OPTIONS_REFRESH_RATE_TEXT_SLIDER"); ("UI_OPTIONS_ANTIALIASING_TEXT_SLIDER");
    for (name ...) {                                               // 0x00599A17 je 0x00599BAA; body 0x00599A26; ++ 0x00599B9A
        CTextSlider* slider = static_cast<CTextSlider*>(m_mapScreens[FRONTEND_SCREEN_VIDEO_OPTIONS_MENU]->FindChild(*name));
        if (slider && slider->MaxChild == 0 && slider->GetParent()) {   // 0x00599A5A cmp [ebx+19Ch],0
            CComponent* whole = slider->GetParent();               // [ebp+60h]
            arrow = whole->FindChild("UI_OPTIONS_LEFT_ARROW_TEXT") ... colour [ebp+70h]; "UI_OPTIONS_RIGHT_ARROW_TEXT" ... colour [ebp+5Ch];
            text = whole->FindFirstChildOfTypeNonRecursive(UI_TYPE_TEXT); if (text) text->MakeUnavailableDynamically();
        }
    }

    // 0x00599BAA refresh rate -- twin lines 745-752
    CComponent* refreshSlider = m_mapScreens[FRONTEND_SCREEN_VIDEO_OPTIONS_MENU]->FindChild("UI_OPTIONS_REFRESH_RATE_TEXT_SLIDER");
    std::list<CComponent*> rates;                                  // 0x00599BE0 out-of-line ctor
    refreshSlider->FindAllChildrenOfType(NUISystem::UI_TYPE_TEXT, rates);   // +18h
    for (std::list<CComponent*>::iterator rate = rates.begin(); rate != rates.end(); ++rate) { // jmp to test 0x00599C88; body 0x00599BFE; ++ inline [node]
        CText* rateText = static_cast<CText*>(*rate);              // [ebp+70h]
        if (rateText) {
            CCharString symbol("TEXT_GUI_MENU_REFRESH_RATE_HZ");   // NAMED, [ebp+5Ch], built before the push 1/push 1
            rateText->SetValue(rateText->GetValue() + L" " + CNewFrontendGameComponent::Get()->GetTextBank().GetTextBySymbol(symbol), true, true);
            // GetTextBySymbol first ([24h]), then GetValue +238h, + L" " [50h], + [58h]; SetValue +240h; dtors 58,50,24, ~symbol
        }
    }

    // 0x00599C91 first screen
    if (m_bQuitFromGame) {                                         // cmp [ebx+0C0h],0 ; jne 0x00599CB7 (this branch placed LAST)
        LoadProfile(CUserProfileManager::Get().GetProfileName());  // 0x00599CB7: Get, add eax,8, 0x0059899A
        InitScreenStack(m_mapScreens[FRONTEND_SCREEN_MAIN_MENU]);  // key 0 via `and [ebp+7Ch],0`
    } else
        InitScreenStack(m_mapScreens[showMediaPlayerError ? FRONTEND_SCREEN_MEDIA_PLAYER_ERROR : FRONTEND_SCREEN_PRESS_START_MENU]);
        // key written into the dead param slot [ebp+7Ch]; all three arms cross-jump into one `call map[] ; push [eax] ; call 0x0059672A` tail at 0x00599CD1
    // dtors: rates 0x00599CE2, names 0x00599CEA, antialiasingModes (inline free 0x00599CF8), action 0x00599D01, profileNames 0x00599D09
}
```

## Notes for the author
* Where the else-arm is laid out first (0x00599C9D) and the `m_bQuitFromGame` arm is placed last, try `if (!m_bQuitFromGame) ... else ...`
  and the ternary-in-subscript vs. two separate calls; FABLE_RETAIL_STLP_MAP is what lets the key live in the param slot.
* Scheduling of `mov [ebp+6Ch],key` before/after `push lit` varies per screen (e.g. 0x00598FD0 / 0x005990D4 push first) -- that is
  c2 scheduling, not source order; keep one uniform statement form.
* 0x00598D29 (LIVE_AWARE = 0) and 0x00598B90 (MAIN_MENU = 0) are plain `m_mapScreens[k] = 0;`.
