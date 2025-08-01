#include "common.h"
#include "filemenu.h"
#include "hud_element.h"
#include "audio/public.h"
#include "fio.h"
#include "game_modes.h"

extern HudScript HES_Spirit1;
extern HudScript HES_Spirit2;
extern HudScript HES_Spirit3;
extern HudScript HES_Spirit4;
extern HudScript HES_Spirit5;
extern HudScript HES_Spirit6;
extern HudScript HES_Spirit7;
extern HudScript HES_Spirit1Missing;
extern HudScript HES_Spirit2Missing;
extern HudScript HES_Spirit3Missing;
extern HudScript HES_Spirit4Missing;
extern HudScript HES_Spirit5Missing;
extern HudScript HES_Spirit6Missing;
extern HudScript HES_Spirit7Missing;
extern HudScript HES_JpFile;
extern HudScript HES_JpFileDisabled;
extern HudScript HES_OptionMonoOn;
extern HudScript HES_OptionMonoOff;
extern HudScript HES_OptionStereoOn;
extern HudScript HES_OptionStereoOff;

#if VERSION_PAL
extern u8   D_filemenu_802508D0[4];
extern u8   D_filemenu_802508D4[4];
extern u8   D_filemenu_802508D8[4];
extern u8   D_filemenu_802508DC[4];
extern u8   D_filemenu_802508E0[4];
extern u8   D_filemenu_802508E4[4];
extern u8   D_filemenu_802508E8[4];
extern u8   D_filemenu_802508EC[4];
extern u8   D_filemenu_802508F0[4];
extern u8   D_filemenu_802508F4[4];
extern u16  D_filemenu_80250900[4];
extern u8   D_filemenu_80250910[16];
extern u8   D_filemenu_80250920[4];
extern u8   D_filemenu_80250938[4];
extern u8   D_filemenu_8025093C[4];
extern u16  D_filemenu_80250940[4];
extern u8   D_filemenu_80250954[4];
#define COPY_OFFSET_X                 (D_filemenu_8025093C[gCurrentLanguage])
#define SAVE_OFFSET_X                 (D_filemenu_80250938[gCurrentLanguage])
#define SELECT_CONFIRM_PROMPT_WIDTH   (D_filemenu_80250900[gCurrentLanguage])
#define DELETE_CONFIRM_PROMPT_WIDTH   (D_filemenu_80250920[gCurrentLanguage])
#define FILE_COPIED_MESSAGE_WIDTH     (D_filemenu_80250954[gCurrentLanguage])
#define COPY_CONFIRM_PROMPT_WIDTH     (D_filemenu_80250940[gCurrentLanguage])
#else
#define SAVE_OFFSET_X                 25
#define COPY_OFFSET_X                 16
#define SELECT_CONFIRM_PROMPT_WIDTH   192
#define DELETE_CONFIRM_PROMPT_WIDTH   118
#define FILE_COPIED_MESSAGE_WIDTH     154
#define COPY_CONFIRM_PROMPT_WIDTH     182
#endif

BSS u8 filemenu_filename[8];

#if VERSION_IQUE
#define OFFSET_WIDTH        5
#define DELETE_OFFSET_X     9
#define CENTER_CANCEL_X     30
#define RIGHT_CANCEL_X      24
#define FILE_X              4
#define FILE_NUMBER_X       36
#define FILE_NAME_X         48
#define NUMBER_OFFSET_Y     1
#else
#define OFFSET_WIDTH        0
#define DELETE_OFFSET_X     8
#define CENTER_CANCEL_X     18
#define RIGHT_CANCEL_X      20
#define FILE_X              5
#define FILE_NUMBER_X       33
#define FILE_NAME_X         46
#define NUMBER_OFFSET_Y     0
#endif

extern HudScript HES_OptionMonoOn_de;
extern HudScript HES_OptionMonoOff_de;
extern HudScript HES_OptionStereoOn_de;
extern HudScript HES_OptionStereoOff_de;
extern HudScript HES_OptionMonoOn_fr;
extern HudScript HES_OptionMonoOff_fr;
extern HudScript HES_OptionStereoOn_fr;
extern HudScript HES_OptionStereoOff_fr;
extern HudScript HES_OptionMonoOn_es;
extern HudScript HES_OptionMonoOff_es;
extern HudScript HES_OptionStereoOn_es;
extern HudScript HES_OptionStereoOff_es;

HudScript* filemenu_main_hudScripts[][20] = {
    [LANGUAGE_DEFAULT] = {
        &HES_Spirit1, &HES_Spirit2, &HES_Spirit3, &HES_Spirit4, &HES_Spirit5, &HES_Spirit6, &HES_Spirit7,
        &HES_Spirit1Missing, &HES_Spirit2Missing, &HES_Spirit3Missing, &HES_Spirit4Missing, &HES_Spirit5Missing,
        &HES_Spirit6Missing, &HES_Spirit7Missing, &HES_JpFile, &HES_JpFileDisabled, &HES_OptionMonoOn,
        &HES_OptionMonoOff,
        &HES_OptionStereoOn, &HES_OptionStereoOff,
    },
#if VERSION_PAL
    [LANGUAGE_DE] = {
        &HES_Spirit1, &HES_Spirit2, &HES_Spirit3, &HES_Spirit4, &HES_Spirit5, &HES_Spirit6, &HES_Spirit7,
        &HES_Spirit1Missing, &HES_Spirit2Missing, &HES_Spirit3Missing, &HES_Spirit4Missing, &HES_Spirit5Missing,
        &HES_Spirit6Missing, &HES_Spirit7Missing, &HES_JpFile, &HES_JpFileDisabled, &HES_OptionMonoOn_de,
        &HES_OptionMonoOff_de,
        &HES_OptionStereoOn_de, &HES_OptionStereoOff_de,
    },
    [LANGUAGE_FR] = {
        &HES_Spirit1, &HES_Spirit2, &HES_Spirit3, &HES_Spirit4, &HES_Spirit5, &HES_Spirit6, &HES_Spirit7,
        &HES_Spirit1Missing, &HES_Spirit2Missing, &HES_Spirit3Missing, &HES_Spirit4Missing, &HES_Spirit5Missing,
        &HES_Spirit6Missing, &HES_Spirit7Missing, &HES_JpFile, &HES_JpFileDisabled, &HES_OptionMonoOn_fr,
        &HES_OptionMonoOff_fr,
        &HES_OptionStereoOn_fr, &HES_OptionStereoOff_fr,
    },
    [LANGUAGE_ES] = {
        &HES_Spirit1, &HES_Spirit2, &HES_Spirit3, &HES_Spirit4, &HES_Spirit5, &HES_Spirit6, &HES_Spirit7,
        &HES_Spirit1Missing, &HES_Spirit2Missing, &HES_Spirit3Missing, &HES_Spirit4Missing, &HES_Spirit5Missing,
        &HES_Spirit6Missing, &HES_Spirit7Missing, &HES_JpFile, &HES_JpFileDisabled, &HES_OptionMonoOn_es,
        &HES_OptionMonoOff_es,
        &HES_OptionStereoOn_es, &HES_OptionStereoOff_es,
    }
#endif
};

u8 filemenu_main_gridData[] = {
    0, 0, 1, 2, 2, 3, 4, 5, 6,
    0, 0, 1, 2, 2, 3, 6, 6, 6,
    0, 0, 1, 2, 2, 3, 6, 6, 6,
    0, 0, 1, 2, 2, 3, 6, 6, 6,
#if !VERSION_PAL
    0, 0, 1, 2, 2, 3, 6, 6, 6,
#endif
};

MenuWindowBP filemenu_main_windowBPs[] = {
    {
        .windowID = WIN_FILES_TITLE,
        .unk_01 = 0,
        .pos = { .x = 0, .y = 0 },
        .width = 0,
        .height = 0,
        .priority = WINDOW_PRIORITY_0,
        .fpDrawContents = &filemenu_draw_contents_title,
        .tab = NULL,
        .parentID = WIN_FILES_MAIN,
        .fpUpdate = { .func = &filemenu_update_show_title },
        .extraFlags = 0,
        .style = { .customStyle = &filemenu_windowStyles[3] }
    },
    {
        .windowID = WIN_FILES_STEREO,
        .unk_01 = 0,
        .pos = { .x = -2, .y = 6 },
        .width = 68,
        .height = 20,
        .priority = WINDOW_PRIORITY_0,
        .fpDrawContents = &filemenu_draw_contents_stereo,
        .tab = NULL,
        .parentID = WIN_FILES_MAIN,
        .fpUpdate = { .func = &filemenu_update_show_options_left },
        .extraFlags = 0,
        .style = { .customStyle = &filemenu_windowStyles[2] }
    },
    {
        .windowID = WIN_FILES_MONO,
        .unk_01 = 0,
        .pos = { .x = 221, .y = 6 },
        .width = 68,
        .height = 20,
        .priority = WINDOW_PRIORITY_0,
        .fpDrawContents = &filemenu_draw_contents_mono,
        .tab = NULL,
        .parentID = WIN_FILES_MAIN,
        .fpUpdate = { .func = &filemenu_update_show_options_right },
        .extraFlags = 0,
        .style = { .customStyle = &filemenu_windowStyles[2] }
    },
    {
        .windowID = WIN_FILES_OPTION_LEFT,
        .unk_01 = 0,
        .pos = { .x = 5, .y = 171 },
        .width = 88,
        .height = 16,
        .priority = WINDOW_PRIORITY_0,
        .fpDrawContents = &filemenu_draw_contents_option_left,
        .tab = NULL,
        .parentID = WIN_FILES_MAIN,
        .fpUpdate = { .func = &filemenu_update_show_options_bottom },
        .extraFlags = 0,
        .style = { .customStyle = &filemenu_windowStyles[4] }
    },
    {
        .windowID = WIN_FILES_OPTION_CENTER,
        .unk_01 = 0,
        .pos = { .x = 0, .y = 171 },
        .width = 88,
        .height = 16,
        .priority = WINDOW_PRIORITY_0,
        .fpDrawContents = &filemenu_draw_contents_option_center,
        .tab = NULL,
        .parentID = WIN_FILES_MAIN,
        .fpUpdate = { .func = &filemenu_update_show_options_bottom },
        .extraFlags = 0,
        .style = { .customStyle = &filemenu_windowStyles[5] }
    },
    {
        .windowID = WIN_FILES_OPTION_RIGHT,
        .unk_01 = 0,
        .pos = { .x = 195, .y = 171 },
        .width = 88,
        .height = 16,
        .priority = WINDOW_PRIORITY_0,
        .fpDrawContents = &filemenu_draw_contents_option_right,
        .tab = NULL,
        .parentID = WIN_FILES_MAIN,
        .fpUpdate = { .func = &filemenu_update_show_options_bottom },
        .extraFlags = 0,
        .style = { .customStyle = &filemenu_windowStyles[6] }
    },
    {
        .windowID = WIN_FILES_SLOT3_BODY,
        .unk_01 = 0,
        .pos = { .x = 9, .y = 110 },
        .width = 130,
        .height = 54,
        .priority = WINDOW_PRIORITY_0,
        .fpDrawContents = &filemenu_draw_contents_file_2_info,
        .tab = NULL,
        .parentID = WIN_FILES_MAIN,
        .fpUpdate = { .func = &filemenu_update_show_options_left },
        .extraFlags = 0,
        .style = { .customStyle = &filemenu_windowStyles[9] }
    },
    {
        .windowID = WIN_FILES_SLOT3_TITLE,
        .unk_01 = 0,
        .pos = { .x = 3, .y = -8 },
        .width = 124 + OFFSET_WIDTH,
        .height = 15,
        .priority = WINDOW_PRIORITY_0,
        .fpDrawContents = &filemenu_draw_contents_file_2_title,
        .tab = NULL,
        .parentID = WIN_FILES_SLOT3_BODY,
        .fpUpdate = { WINDOW_UPDATE_SHOW },
        .extraFlags = 0,
        .style = { .customStyle = &filemenu_windowStyles[13] }
    },
    {
        .windowID = WIN_FILES_SLOT4_BODY,
        .unk_01 = 0,
        .pos = { .x = 149, .y = 110 },
        .width = 130,
        .height = 54,
        .priority = WINDOW_PRIORITY_0,
        .fpDrawContents = &filemenu_draw_contents_file_3_info,
        .tab = NULL,
        .parentID = WIN_FILES_MAIN,
        .fpUpdate = { .func = &filemenu_update_show_options_right },
        .extraFlags = 0,
        .style = { .customStyle = &filemenu_windowStyles[10] }
    },
    {
        .windowID = WIN_FILES_SLOT4_TITLE,
        .unk_01 = 0,
        .pos = { .x = 3, .y = -8 },
        .width = 124 + OFFSET_WIDTH,
        .height = 15,
        .priority = WINDOW_PRIORITY_0,
        .fpDrawContents = &filemenu_draw_contents_file_3_title,
        .tab = NULL,
        .parentID = WIN_FILES_SLOT4_BODY,
        .fpUpdate = { WINDOW_UPDATE_SHOW },
        .extraFlags = 0,
        .style = { .customStyle = &filemenu_windowStyles[14] }
    },
    {
        .windowID = WIN_FILES_SLOT1_BODY,
        .unk_01 = 0,
        .pos = { .x = 9, .y = 41 },
        .width = 130 + OFFSET_WIDTH,
        .height = 54,
        .priority = WINDOW_PRIORITY_0,
        .fpDrawContents = &filemenu_draw_contents_file_0_info,
        .tab = NULL,
        .parentID = WIN_FILES_MAIN,
        .fpUpdate = { .func = &filemenu_update_show_options_left },
        .extraFlags = 0,
        .style = { .customStyle = &filemenu_windowStyles[7] }
    },
    {
        .windowID = WIN_FILES_SLOT1_TITLE,
        .unk_01 = 0,
        .pos = { .x = 3, .y = -8 },
        .width = 124 + OFFSET_WIDTH,
        .height = 15,
        .priority = WINDOW_PRIORITY_0,
        .fpDrawContents = &filemenu_draw_contents_file_0_title,
        .tab = NULL,
        .parentID = WIN_FILES_SLOT1_BODY,
        .fpUpdate = { WINDOW_UPDATE_SHOW },
        .extraFlags = 0,
        .style = { .customStyle = &filemenu_windowStyles[11] }
    },
    {
        .windowID = WIN_FILES_SLOT2_BODY,
        .unk_01 = 0,
        .pos = { .x = 149, .y = 41 },
        .width = 130,
        .height = 54,
        .priority = WINDOW_PRIORITY_0,
        .fpDrawContents = &filemenu_draw_contents_file_1_info,
        .tab = NULL,
        .parentID = WIN_FILES_MAIN,
        .fpUpdate = { .func = &filemenu_update_show_options_right },
        .extraFlags = 0,
        .style = { .customStyle = &filemenu_windowStyles[8] }
    },
    {
        .windowID = WIN_FILES_SLOT2_TITLE,
        .unk_01 = 0,
        .pos = { .x = 3, .y = -8 },
        .width = 124 + OFFSET_WIDTH,
        .height = 15,
        .priority = WINDOW_PRIORITY_0,
        .fpDrawContents = &filemenu_draw_contents_file_1_title,
        .tab = NULL,
        .parentID = WIN_FILES_SLOT2_BODY,
        .fpUpdate = { WINDOW_UPDATE_SHOW },
        .extraFlags = 0,
        .style = { .customStyle = &filemenu_windowStyles[12] }
    },
};

MenuPanel filemenu_main_menuBP = {
    .initialized = false,
    .col = 0,
    .row = 0,
    .selected = 0,
    .state = 0,
    .numCols = 3,
    .numRows = 3,
    .numPages = 0,
    .gridData = filemenu_main_gridData,
    .fpInit = &filemenu_main_init,
    .fpHandleInput = &filemenu_main_handle_input,
    .fpUpdate = &filemenu_main_update,
    .fpCleanup = &filemenu_main_cleanup
};

void filemenu_draw_contents_title(
    MenuPanel* menu,
    s32 baseX, s32 baseY,
    s32 width, s32 height,
    s32 opacity, s32 darkening
) {
    s32 msgIdx;
    s32 xOffset;
    s32 yOffset;

    switch (menu->state) {
        case FM_MAIN_SELECT_FILE:
            msgIdx = FILE_MESSAGE_SELECT_FILE_TO_START;
            xOffset = 9;
            yOffset = 4;
            break;
        case FM_MAIN_SELECT_DELETE:
            msgIdx = FILE_MESSAGE_SELECT_FILE_TO_DELETE;
            xOffset = DELETE_OFFSET_X;
            yOffset = 4;
            break;
        case FM_MAIN_SELECT_COPY_FROM:
            msgIdx = FILE_MESSAGE_COPY_WHICH_FILE;
            xOffset = SAVE_OFFSET_X;
            yOffset = 4;
            break;
        case FM_MAIN_SELECT_COPY_TO:
            msgIdx = FILE_MESSAGE_COPY_TO_WHICH_FILE;
            xOffset = COPY_OFFSET_X;
            yOffset = 4;
            break;
#if !VERSION_PAL
        case FM_MAIN_SELECT_LANG_DUMMY:
        default:
            msgIdx = FILE_MESSAGE_SELECT_FILE_TO_SAVE;
            xOffset = 10;
            yOffset = 4;
            break;
#endif
    }

    filemenu_draw_message(filemenu_get_menu_message(msgIdx), baseX + xOffset, baseY + yOffset, 255, 0, 0);
}

void filemenu_draw_contents_stereo(
    MenuPanel* menu,
    s32 baseX, s32 baseY,
    s32 width, s32 height,
    s32 opacity, s32 darkening
) {
    if (gGameStatusPtr->soundOutputMode == SOUND_OUT_STEREO) {
        hud_element_set_render_pos(filemenu_mainHIDs[18], baseX + 34, baseY + 10);
        hud_element_draw_without_clipping(filemenu_mainHIDs[18]);
    } else {
        hud_element_set_render_pos(filemenu_mainHIDs[19], baseX + 34, baseY + 10);
        hud_element_draw_without_clipping(filemenu_mainHIDs[19]);
    }
}

void filemenu_draw_contents_mono(
    MenuPanel* menu,
    s32 baseX, s32 baseY,
    s32 width, s32 height,
    s32 opacity, s32 darkening
) {
    if (gGameStatusPtr->soundOutputMode == SOUND_OUT_MONO) {
        hud_element_set_render_pos(filemenu_mainHIDs[16], baseX + 34, baseY + 10);
        hud_element_draw_without_clipping(filemenu_mainHIDs[16]);
    } else {
        hud_element_set_render_pos(filemenu_mainHIDs[17], baseX + 34, baseY + 10);
        hud_element_draw_without_clipping(filemenu_mainHIDs[17]);
    }
}

void filemenu_draw_contents_option_left(
    MenuPanel* menu,
    s32 baseX, s32 baseY,
    s32 width, s32 height,
    s32 opacity, s32 darkening
) {
#if VERSION_PAL
    s32 offset = D_filemenu_802508E8[gCurrentLanguage];

    if (menu->col == 0 && menu->row == 2) {
        filemenu_set_cursor_goal_pos(WIN_FILES_OPTION_LEFT, (baseX + offset) - 10, baseY + 8);
    }
    filemenu_draw_message(filemenu_get_menu_message(FILE_MESSAGE_DELETE_FILE), baseX + offset + OFFSET_WIDTH, baseY + 2, 255, 0, 1);
#else
    if (menu->state == FM_MAIN_SELECT_LANG_DUMMY) {
        return;
    }

    if (menu->col == 0 && menu->row == 2) {
        filemenu_set_cursor_goal_pos(WIN_FILES_OPTION_LEFT, baseX, baseY + 8);
    }
    filemenu_draw_message(filemenu_get_menu_message(FILE_MESSAGE_DELETE_FILE), baseX + 8 + OFFSET_WIDTH, baseY + 2, 255, 0, 1);
#endif
}

#if VERSION_PAL
void filemenu_draw_contents_option_center(
    MenuPanel* menu,
    s32 baseX, s32 baseY,
    s32 width, s32 height,
    s32 opacity, s32 darkening
) {
    s32 msgIdx;
    s32 xOffset;
    s32 yOffset;

    switch (menu->state) {
        case FM_MAIN_SELECT_DELETE:
        case FM_MAIN_SELECT_COPY_FROM:
        case FM_MAIN_SELECT_COPY_TO:
            msgIdx = FILE_MESSAGE_CANCEL;
            xOffset = D_filemenu_802508F4[gCurrentLanguage];
            yOffset = 0;
            if (menu->col == 1 && menu->row == 2) {
                filemenu_set_cursor_goal_pos(WIN_FILES_OPTION_CENTER, baseX + xOffset - 10, baseY + 8);
            }
            break;
        default:
            msgIdx = FILE_MESSAGE_COPY_FILE;
            xOffset = D_filemenu_802508EC[gCurrentLanguage];
            yOffset = 0;
            if (menu->col == 1 && menu->row == 2) {
                filemenu_set_cursor_goal_pos(WIN_FILES_OPTION_CENTER, baseX + xOffset - 10, baseY + 8);
            }
            break;
    }

    filemenu_draw_message(filemenu_get_menu_message(msgIdx), baseX + xOffset, baseY + yOffset + 2, 255, 0, 1);
}
#else
void filemenu_draw_contents_option_center(
    MenuPanel* menu,
    s32 baseX, s32 baseY,
    s32 width, s32 height,
    s32 opacity, s32 darkening
) {
    s32 msgIdx;
    s32 xOffset;
    s32 yOffset;

    switch (menu->state) {
        case FM_MAIN_SELECT_DELETE:
        case FM_MAIN_SELECT_LANG_DUMMY:
        case FM_MAIN_SELECT_COPY_FROM:
        case FM_MAIN_SELECT_COPY_TO:
            msgIdx = FILE_MESSAGE_CANCEL;
            xOffset = CENTER_CANCEL_X;
            yOffset = 0;
            if (menu->col == 1 && menu->row == 2) {
                filemenu_set_cursor_goal_pos(WIN_FILES_OPTION_CENTER, baseX + CENTER_CANCEL_X - 10, baseY + 8);
            }
            break;
        default:
            msgIdx = FILE_MESSAGE_COPY_FILE;
            xOffset = 14;
            yOffset = 0;
            if (menu->col == 1 && menu->row == 2) {
                filemenu_set_cursor_goal_pos(WIN_FILES_OPTION_CENTER, baseX + 4, baseY + 8);
            }
            break;
    }

    filemenu_draw_message(filemenu_get_menu_message(msgIdx), baseX + xOffset, baseY + yOffset + 2, 255, 0, 1);
}
#endif

void filemenu_draw_contents_option_right(
    MenuPanel* menu,
    s32 baseX, s32 baseY,
    s32 width, s32 height,
    s32 opacity, s32 darkening
) {
#if VERSION_PAL
    s32 offset = D_filemenu_802508F0[gCurrentLanguage];

    if (menu->col == 2 && menu->row == 2) {
        filemenu_set_cursor_goal_pos(WIN_FILES_OPTION_RIGHT, baseX + offset - 10, baseY + 8);
    }
    filemenu_draw_message(filemenu_get_menu_message(FILE_MESSAGE_CANCEL), baseX + offset, baseY + 2, 255, 0, 1);
#else
    if (menu->state != 2) {
        if (menu->col == 2 && menu->row == 2) {
            filemenu_set_cursor_goal_pos(WIN_FILES_OPTION_RIGHT, baseX + 8, baseY + 8);
        }
        filemenu_draw_message(filemenu_get_menu_message(FILE_MESSAGE_CANCEL), baseX + RIGHT_CANCEL_X, baseY + 2, 255, 0, 1);
    }
#endif
}

void filemenu_draw_contents_file_info(s32 fileIdx,
    MenuPanel* menu,
    s32 baseX, s32 baseY,
    s32 width, s32 height,
    s32 opacity, s32 darkening
) {
    s32 xOffset;
    s32 temp_s0_3;
    s32 temp_s1_2;
    s32 temp_s3_2;
    s32 temp_s3;
    s32 id;
    s32 i;
    const int MAX_DISPLAYED_TIME = 100*60*60*60 - 1; // 100 hours minus one frame at 60 fps

    if (!gSaveSlotHasData[fileIdx]) {
#if VERSION_PAL
        xOffset = D_filemenu_802508E0[gCurrentLanguage];
#else
        xOffset = 50;
#endif
        filemenu_draw_message(filemenu_get_menu_message(FILE_MESSAGE_NEW), baseX + xOffset, baseY + 20, 255, 0xA, 0);
        return;
    }

    if (gSaveSlotMetadata[fileIdx].timePlayed == 0) {
#if VERSION_PAL
        xOffset = D_filemenu_802508E4[gCurrentLanguage];
#else
        xOffset = 30;
#endif
        filemenu_draw_message(filemenu_get_menu_message(FILE_MESSAGE_FIRST_PLAY), baseX + xOffset, baseY + 20, 255, 0xA, 0);
        return;
    }

#if VERSION_PAL
    xOffset = D_filemenu_802508D8[gCurrentLanguage];
#else
    xOffset = 34;
#endif
    filemenu_draw_message(filemenu_get_menu_message(FILE_MESSAGE_LEVEL), baseX + xOffset, baseY + 10, 255, 0xA, 1);
    temp_s3_2 = gSaveSlotMetadata[fileIdx].level;
    temp_s3 = temp_s3_2;
    draw_number(temp_s3 / 10, baseX + 79, baseY + 10 + NUMBER_OFFSET_Y, DRAW_NUMBER_CHARSET_THIN, MSG_PAL_STANDARD, 255, DRAW_NUMBER_STYLE_MONOSPACE);
    draw_number(temp_s3 % 10, baseX + 88, baseY + 10 + NUMBER_OFFSET_Y, DRAW_NUMBER_CHARSET_THIN, MSG_PAL_STANDARD, 255, DRAW_NUMBER_STYLE_MONOSPACE);
#if VERSION_PAL
    xOffset = D_filemenu_802508DC[gCurrentLanguage];
#else
    xOffset = 11;
#endif
    filemenu_draw_message(filemenu_get_menu_message(FILE_MESSAGE_PLAY_TIME), baseX + xOffset, baseY + 24, 255, 0xA, 1);

    temp_s3_2 = gSaveSlotMetadata[fileIdx].timePlayed;
    if (temp_s3_2 > MAX_DISPLAYED_TIME) {
        temp_s3_2 = MAX_DISPLAYED_TIME;
    }

    draw_number((temp_s3_2 / 2160000) % 10, baseX + 76, baseY + 24 + NUMBER_OFFSET_Y, DRAW_NUMBER_CHARSET_THIN, MSG_PAL_STANDARD, 255, DRAW_NUMBER_STYLE_MONOSPACE);
    temp_s1_2 = temp_s3_2 / 216000;
    draw_number(temp_s1_2 - ((temp_s3_2 / 2160000) * 10), baseX + 85, baseY + 24 + NUMBER_OFFSET_Y, DRAW_NUMBER_CHARSET_THIN, MSG_PAL_STANDARD, 255, DRAW_NUMBER_STYLE_MONOSPACE);
    filemenu_draw_message(filemenu_get_menu_message(FILE_MESSAGE_PERIOD_13), baseX + 95, baseY + 23, 255, 0xA, 1);
    filemenu_draw_message(filemenu_get_menu_message(FILE_MESSAGE_PERIOD_13), baseX + 95, baseY + 18, 255, 0xA, 1);
    temp_s0_3 = temp_s3_2 / 36000;
    draw_number(temp_s0_3 - (temp_s1_2 * 6), baseX + 100, baseY + 24 + NUMBER_OFFSET_Y, DRAW_NUMBER_CHARSET_THIN, MSG_PAL_STANDARD, 255, DRAW_NUMBER_STYLE_MONOSPACE);
    draw_number((temp_s3_2 / 3600) - (temp_s0_3 * 10), baseX + 109, baseY + 24 + NUMBER_OFFSET_Y, DRAW_NUMBER_CHARSET_THIN, MSG_PAL_STANDARD, 255, DRAW_NUMBER_STYLE_MONOSPACE);

    for (i = 0; i < 7; i++) {
        if (i < gSaveSlotMetadata[fileIdx].spiritsRescued) {
            id = filemenu_mainHIDs[i];
        } else {
            id = filemenu_mainHIDs[i + 7];
        }
        hud_element_set_render_pos(id, baseX + 17 + (i * 16), baseY + 44);
        if (i == 0) {
            hud_element_draw_without_clipping(id);
        } else {
            hud_element_draw_next(id);
        }
    }
}

#if VERSION_PAL
void filemenu_draw_contents_file_title(
    s32 fileIdx,
    MenuPanel* menu,
    s32 baseX, s32 baseY,
    s32 width, s32 height,
    s32 opacity, s32 darkening)
{
    if (filemenu_currentMenu == FILE_MENU_MAIN && menu->selected == fileIdx) {
        filemenu_set_cursor_goal_pos(fileIdx + 60, baseX - 3, baseY + 8);
    }

    filemenu_draw_message(filemenu_get_menu_message(FILE_MESSAGE_PAL_FILE_20), baseX + FILE_X, baseY + 1, 255, 0, 1);

    if (!gSaveSlotHasData[fileIdx]) {
        filemenu_draw_message(filemenu_get_menu_message(fileIdx + FILE_MESSAGE_PAL_FILENUM_1),
            baseX + D_filemenu_802508D0[gCurrentLanguage], baseY + 1, 255, 0, 1);
    } else {
        s32 tmp = D_filemenu_802508D0[gCurrentLanguage];

        filemenu_draw_message(filemenu_get_menu_message(fileIdx + FILE_MESSAGE_PAL_FILENUM_1),
            baseX + tmp, baseY + 1, 255, 0, 1);

        tmp += D_filemenu_802508D4[gCurrentLanguage];
        tmp += 6;
        filemenu_draw_file_name(
            gSaveSlotMetadata[fileIdx].filename,
            ARRAY_COUNT(gSaveSlotMetadata[fileIdx].filename),
            baseX + tmp,
            baseY + 1, 255, 0, 1, 9);
        }
}
#else
void filemenu_draw_contents_file_title(
    s32 fileIdx,
    MenuPanel* menu,
    s32 baseX, s32 baseY,
    s32 width, s32 height,
    s32 opacity, s32 darkening)
{
    if (filemenu_currentMenu == FILE_MENU_MAIN && menu->selected == fileIdx) {
        filemenu_set_cursor_goal_pos(fileIdx + 60, baseX - 3, baseY + 8);
    }

    filemenu_draw_message(filemenu_get_menu_message(FILE_MESSAGE_FILE_26), baseX + FILE_X, baseY + 1, 255, 0, 1);

    if (!gSaveSlotHasData[fileIdx]) {
        draw_number(fileIdx + 1, baseX + FILE_NUMBER_X, baseY + 1 + NUMBER_OFFSET_Y, DRAW_NUMBER_CHARSET_THIN, MSG_PAL_WHITE, 255, DRAW_NUMBER_STYLE_MONOSPACE);
    } else {
        draw_number(fileIdx + 1, baseX + FILE_NUMBER_X, baseY + 1 + NUMBER_OFFSET_Y, DRAW_NUMBER_CHARSET_THIN, MSG_PAL_WHITE, 255, DRAW_NUMBER_STYLE_MONOSPACE);
        filemenu_draw_file_name(
            gSaveSlotMetadata[fileIdx].filename,
            ARRAY_COUNT(gSaveSlotMetadata[fileIdx].filename),
            baseX + FILE_NAME_X, baseY + 1, 255, 0, 1, 9);
    }
}
#endif

void filemenu_draw_contents_file_0_info(
    MenuPanel* menu,
    s32 baseX, s32 baseY,
    s32 width, s32 height,
    s32 opacity, s32 darkening
) {
    filemenu_draw_contents_file_info(0, menu, baseX, baseY, width, height, opacity, darkening);
}

void filemenu_draw_contents_file_1_info(
    MenuPanel* menu,
    s32 baseX, s32 baseY,
    s32 width, s32 height,
    s32 opacity, s32 darkening
) {
    filemenu_draw_contents_file_info(1, menu, baseX, baseY, width, height, opacity, darkening);
}

void filemenu_draw_contents_file_2_info(
    MenuPanel* menu,
    s32 baseX, s32 baseY,
    s32 width, s32 height,
    s32 opacity, s32 darkening
) {
    filemenu_draw_contents_file_info(2, menu, baseX, baseY, width, height, opacity, darkening);
}

void filemenu_draw_contents_file_3_info(
    MenuPanel* menu,
    s32 baseX, s32 baseY,
    s32 width, s32 height,
    s32 opacity, s32 darkening
) {
    filemenu_draw_contents_file_info(3, menu, baseX, baseY, width, height, opacity, darkening);
}

void filemenu_draw_contents_file_0_title(
    MenuPanel* menu,
    s32 baseX, s32 baseY,
    s32 width, s32 height,
    s32 opacity, s32 darkening
) {
    filemenu_draw_contents_file_title(0, menu, baseX, baseY, width, height, opacity, darkening);
}

void filemenu_draw_contents_file_1_title(
    MenuPanel* menu,
    s32 baseX, s32 baseY,
    s32 width, s32 height,
    s32 opacity, s32 darkening
) {
    filemenu_draw_contents_file_title(1, menu, baseX, baseY, width, height, opacity, darkening);
}

void filemenu_draw_contents_file_2_title(
    MenuPanel* menu,
    s32 baseX, s32 baseY,
    s32 width, s32 height,
    s32 opacity, s32 darkening
) {
    filemenu_draw_contents_file_title(2, menu, baseX, baseY, width, height, opacity, darkening);
}

void filemenu_draw_contents_file_3_title(
    MenuPanel* menu,
    s32 baseX, s32 baseY,
    s32 width, s32 height,
    s32 opacity, s32 darkening
) {
    filemenu_draw_contents_file_title(3, menu, baseX, baseY, width, height, opacity, darkening);
}

void filemenu_main_init(MenuPanel* menu) {
    s32 i;

    for (i = 0; i < ARRAY_COUNT(filemenu_mainHIDs); i++) {
        filemenu_mainHIDs[i] = hud_element_create(filemenu_main_hudScripts[gCurrentLanguage][i]);
        hud_element_set_flags(filemenu_mainHIDs[i], HUD_ELEMENT_FLAG_80);
    }

    for (i = 0; i < ARRAY_COUNT(filemenu_main_windowBPs); i++) {
        filemenu_main_windowBPs[i].tab = menu;
    }

    setup_pause_menu_tab(filemenu_main_windowBPs, ARRAY_COUNT(filemenu_main_windowBPs));
    menu->selected = MENU_PANEL_SELECTED_GRID_DATA(menu);

#if VERSION_PAL
    gWindows[WIN_FILES_TITLE].pos.y = 1;
    gWindows[WIN_FILES_TITLE].width = 162;
    gWindows[WIN_FILES_TITLE].height = 25;
#else
    if (menu->state == FM_MAIN_SELECT_LANG_DUMMY) {
        gWindows[WIN_FILES_TITLE].pos.y = 1;
        gWindows[WIN_FILES_TITLE].width = 211;
        gWindows[WIN_FILES_TITLE].height = 25;
    } else {
        gWindows[WIN_FILES_TITLE].pos.y = 1;
        gWindows[WIN_FILES_TITLE].width = 162;
        gWindows[WIN_FILES_TITLE].height = 25;
    }
#endif
    gWindows[WIN_FILES_TITLE].pos.x = CENTER_WINDOW_X(WIN_FILES_TITLE);
    gWindows[WIN_FILES_OPTION_CENTER].pos.x = CENTER_WINDOW_X(WIN_FILES_OPTION_CENTER);

    if (menu->state != FM_MAIN_SELECT_FILE) {
        set_window_update(WIN_FILES_OPTION_LEFT, WINDOW_UPDATE_HIDE);
        set_window_update(WIN_FILES_OPTION_RIGHT, WINDOW_UPDATE_HIDE);
        set_window_update(WIN_FILES_STEREO, WINDOW_UPDATE_HIDE);
        set_window_update(WIN_FILES_MONO, WINDOW_UPDATE_HIDE);
    }
    menu->initialized = true;
}

void filemenu_main_handle_input(MenuPanel* menu) {
    s32 originalSelected = menu->selected;
    s32 i;

    if (filemenu_heldButtons & BUTTON_STICK_LEFT) {
        while (true) {
            menu->col--;
            if (menu->col < 0) {
                menu->col = 0;
                break;
            }
            if (menu->selected != MENU_PANEL_SELECTED_GRID_DATA(menu)) {
                break;
            }
        }
    }

    if (filemenu_heldButtons & BUTTON_STICK_RIGHT) {
        while (true) {
            menu->col++;
            if (menu->col >= menu->numCols) {
                menu->col = menu->numCols - 1;
                break;
            }

            if (menu->selected != MENU_PANEL_SELECTED_GRID_DATA(menu)) {
                break;
            }
        }
    }

    if (filemenu_heldButtons & BUTTON_STICK_UP) {
        menu->row--;
        if (menu->row < 0) {
            menu->row = 0;
        }
    }

    if (filemenu_heldButtons & BUTTON_STICK_DOWN) {
        menu->row++;
        if (menu->row >= menu->numRows) {
            menu->row = menu->numRows - 1;
        }
    }

    switch (menu->state) {
        case FM_MAIN_SELECT_FILE:
            // force selection to column 0 when navigating up from bottom row
            if (menu->col == 1 && (u8) menu->row < 2) {
                menu->col = 0;
            }
            break;
        case FM_MAIN_SELECT_DELETE:
#if !VERSION_PAL
            // force selection to column 0 when navigating up from bottom row
            if (menu->col == 1 && (u8) menu->row < 2) {
                menu->col = 0;
            }
            // force selection to column 1 when navigating down to bottom row
            if (menu->row == 2) {
                menu->col = 1;
            }
            break;
        case FM_MAIN_SELECT_LANG_DUMMY:
            // force selection to column 0 when navigating up from bottom row
            if (menu->col == 1 && (u8) menu->row < 2) {
                menu->col = 0;
            }
            // force selection to column 1 when navigating down to bottom row
            if (menu->row == 2) {
                menu->col = 1;
            }
            break;
#endif
        case FM_MAIN_SELECT_COPY_FROM:
#if !VERSION_PAL
            // force selection to column 0 when navigating up from bottom row
            if (menu->col == 1 && (u8) menu->row < 2) {
                menu->col = 0;
            }
            // force selection to column 1 when navigating down to bottom row
            if (menu->row == 2) {
                menu->col = 1;
            }
            break;
#endif
        case FM_MAIN_SELECT_COPY_TO:
            // force selection to column 0 when navigating up from bottom row
            if (menu->col == 1 && (u8) menu->row < 2) {
                menu->col = 0;
            }
            // force selection to column 1 when navigating down to bottom row
            if (menu->row == 2) {
                menu->col = 1;
            }
            break;
    }

    menu->selected = MENU_PANEL_SELECTED_GRID_DATA(menu);
    if (originalSelected != menu->selected) {
        sfx_play_sound(SOUND_FILE_MENU_MOVE_CURSOR);
    }

    if (menu->state == FM_MAIN_SELECT_FILE) {
        s32 originalOutputMode = gGameStatusPtr->soundOutputMode;

        if (filemenu_pressedButtons & BUTTON_Z) {
            gGameStatusPtr->soundOutputMode = SOUND_OUT_STEREO;
        }
        if (filemenu_pressedButtons & BUTTON_R) {
            gGameStatusPtr->soundOutputMode = SOUND_OUT_MONO;
        }

        if (originalOutputMode != gGameStatusPtr->soundOutputMode) {
            sfx_play_sound(SOUND_MENU_CHANGE_TAB);
            if (gGameStatusPtr->soundOutputMode != SOUND_OUT_MONO) {
                snd_set_stereo();
            } else {
                snd_set_mono();
            }
        }
    }

    if ((filemenu_pressedButtons & BUTTON_START) && menu->state == FM_MAIN_SELECT_FILE && menu->selected <= FM_MAIN_OPT_FILE_4) {
        filemenu_pressedButtons = BUTTON_A;
    }

    if (filemenu_pressedButtons & BUTTON_A) {
        s32 cond = false;

        switch (menu->state) {
            case FM_MAIN_SELECT_FILE:
                if (menu->selected <= FM_MAIN_OPT_FILE_4 && !gSaveSlotHasData[menu->selected]) {
                    cond = true;
                }

                if (cond) {
                    // selected a file with no data
                    MenuPanel* inputMenu;
                    for (i = 0; i < ARRAY_COUNT(filemenu_filename); i++) {
                        filemenu_filename[i] = MSG_CHAR_READ_SPACE;
                    }
                    filemenu_filename_pos = 0;
                    set_window_update(WIN_FILES_INPUT_FIELD, (s32)filemenu_update_show_name_input);
                    set_window_update(WIN_FILES_INPUT_KEYBOARD, (s32)filemenu_update_show_name_input);
                    set_window_update(WIN_FILES_TITLE, (s32)filemenu_update_hidden_with_rotation);
                    set_window_update(WIN_FILES_STEREO, (s32)filemenu_update_hidden_with_rotation);
                    set_window_update(WIN_FILES_MONO, (s32)filemenu_update_hidden_with_rotation);
                    set_window_update(WIN_FILES_OPTION_LEFT, (s32)filemenu_update_hidden_with_rotation);
                    set_window_update(WIN_FILES_OPTION_CENTER, (s32)filemenu_update_hidden_with_rotation);
                    set_window_update(WIN_FILES_OPTION_RIGHT, (s32)filemenu_update_hidden_with_rotation);
                    set_window_update(WIN_FILES_OPTION_LEFT, (s32)filemenu_update_hidden_with_rotation);
                    set_window_update(WIN_FILES_SLOT1_BODY, (s32)filemenu_update_hidden_with_rotation);
                    set_window_update(WIN_FILES_SLOT2_BODY, (s32)filemenu_update_hidden_with_rotation);
                    set_window_update(WIN_FILES_SLOT3_BODY, (s32)filemenu_update_hidden_with_rotation);
                    set_window_update(WIN_FILES_SLOT4_BODY, (s32)filemenu_update_hidden_with_rotation);
                    sfx_play_sound(SOUND_MENU_NEXT);
                    filemenu_currentMenu = FILE_MENU_INPUT_NAME;
                    inputMenu = filemenu_menus[FILE_MENU_INPUT_NAME];
                    inputMenu->state = FM_INPUT_CHARSET_A;
                    filemenu_set_selected(inputMenu, 0, 0);
                    break;
                }

                if (menu->selected == FM_MAIN_OPT_CANCEL) {
                    // selected "Cancel" button
                    set_window_update(WIN_FILES_STEREO, (s32)filemenu_update_hidden_options_left);
                    set_window_update(WIN_FILES_MONO, (s32)filemenu_update_hidden_options_right);
                    set_window_update(WIN_FILES_OPTION_LEFT, (s32)filemenu_update_hidden_options_bottom);
                    set_window_update(WIN_FILES_OPTION_CENTER, (s32)filemenu_update_hidden_options_bottom);
                    set_window_update(WIN_FILES_OPTION_RIGHT, (s32)filemenu_update_hidden_options_bottom);
                    set_window_update(WIN_FILES_SLOT1_BODY, (s32)filemenu_update_hidden_options_left);
                    set_window_update(WIN_FILES_SLOT2_BODY, (s32)filemenu_update_hidden_options_right);
                    set_window_update(WIN_FILES_SLOT3_BODY, (s32)filemenu_update_hidden_options_left);
                    set_window_update(WIN_FILES_SLOT4_BODY, (s32)filemenu_update_hidden_options_right);
                    set_window_update(WIN_FILES_TITLE, (s32)filemenu_update_hidden_title);
                    sfx_play_sound(SOUND_FILE_MENU_OUT);
                    set_game_mode(GAME_MODE_END_FILE_SELECT);
                } else if (menu->selected == FM_MAIN_OPT_DELETE) {
                    // selected "Delete File" button
                    sfx_play_sound(SOUND_MENU_NEXT);
                    menu->state = FM_MAIN_SELECT_DELETE;
                    filemenu_set_selected(menu, 1, 2);
                    set_window_update(WIN_FILES_STEREO, (s32)filemenu_update_hidden_options_left);
                    set_window_update(WIN_FILES_MONO, (s32)filemenu_update_hidden_options_right);
                    set_window_update(WIN_FILES_OPTION_LEFT, (s32)filemenu_update_hidden_options_bottom);
                    set_window_update(WIN_FILES_OPTION_RIGHT, (s32)filemenu_update_hidden_options_bottom);
                } else if (menu->selected == FM_MAIN_OPT_COPY) {
                    sfx_play_sound(SOUND_MENU_NEXT);
                    menu->state = FM_MAIN_SELECT_COPY_FROM;
                    filemenu_set_selected(menu, 1, 2);
                    set_window_update(WIN_FILES_STEREO, (s32)filemenu_update_hidden_options_left);
                    set_window_update(WIN_FILES_MONO, (s32)filemenu_update_hidden_options_right);
                    set_window_update(WIN_FILES_OPTION_LEFT, (s32)filemenu_update_hidden_options_bottom);
                    set_window_update(WIN_FILES_OPTION_RIGHT, (s32)filemenu_update_hidden_options_bottom);
                } else if (menu->selected <= FM_MAIN_OPT_FILE_4) {
                    MenuPanel* confirmMenu;

                    sfx_play_sound(SOUND_MENU_NEXT);
                    set_window_update(WIN_FILES_TITLE, (s32)filemenu_update_hidden_with_rotation);
                    set_window_update(WIN_FILES_STEREO, (s32)filemenu_update_hidden_with_rotation);
                    set_window_update(WIN_FILES_MONO, (s32)filemenu_update_hidden_with_rotation);
                    set_window_update(WIN_FILES_OPTION_LEFT, (s32)filemenu_update_hidden_with_rotation);
                    set_window_update(WIN_FILES_OPTION_CENTER, (s32)filemenu_update_hidden_with_rotation);
                    set_window_update(WIN_FILES_OPTION_RIGHT, (s32)filemenu_update_hidden_with_rotation);
                    set_window_update(WIN_FILES_OPTION_LEFT, (s32)filemenu_update_hidden_with_rotation);
                    set_window_update(WIN_FILES_SLOT1_BODY, (s32)filemenu_update_hidden_with_rotation);
                    set_window_update(WIN_FILES_SLOT2_BODY, (s32)filemenu_update_hidden_with_rotation);
                    set_window_update(WIN_FILES_SLOT3_BODY, (s32)filemenu_update_hidden_with_rotation);
                    set_window_update(WIN_FILES_SLOT4_BODY, (s32)filemenu_update_hidden_with_rotation);
                    set_window_update(menu->selected + WIN_FILES_SLOT1_BODY, (s32)filemenu_update_select_file);
                    sfx_play_sound(SOUND_MENU_NEXT);
                    set_window_update(WIN_FILES_CONFIRM_OPTIONS, WINDOW_UPDATE_SHOW);

                    gWindows[WIN_FILES_CONFIRM_OPTIONS].pos.y = 143;
                    gWindows[WIN_FILES_CONFIRM_OPTIONS].width = 69;
                    gWindows[WIN_FILES_CONFIRM_OPTIONS].height = 44;
                    gWindows[WIN_FILES_CONFIRM_OPTIONS].pos.x = CENTER_WINDOW_X(WIN_FILES_CONFIRM_OPTIONS);

                    gWindows[WIN_FILES_CONFIRM_PROMPT].pos.y = -29;
                    gWindows[WIN_FILES_CONFIRM_PROMPT].width = SELECT_CONFIRM_PROMPT_WIDTH;
                    gWindows[WIN_FILES_CONFIRM_PROMPT].height = 25;
                    gWindows[WIN_FILES_CONFIRM_PROMPT].pos.x = CENTER_WINDOW_X(WIN_FILES_CONFIRM_PROMPT);

                    filemenu_currentMenu = FILE_MENU_CONFIRM;
                    confirmMenu = filemenu_menus[FILE_MENU_CONFIRM];
                    confirmMenu->state = FM_CONFIRM_START;
                    filemenu_set_selected(confirmMenu, 0, 0);
                }
                break;
            case FM_MAIN_SELECT_DELETE:
#if VERSION_PAL
                gWindows[WIN_FILES_TITLE].width = 211;
#endif
                if (menu->selected == FM_MAIN_OPT_CANCEL) {
                    // selected "Cancel" button
                    sfx_play_sound(SOUND_MENU_NEXT);
                    menu->state = FM_MAIN_SELECT_FILE;
                    set_window_update(WIN_FILES_STEREO, (s32)filemenu_update_show_options_left);
                    set_window_update(WIN_FILES_MONO, (s32)filemenu_update_show_options_right);
                    set_window_update(WIN_FILES_OPTION_LEFT, (s32)filemenu_update_show_options_bottom);
                    set_window_update(WIN_FILES_OPTION_RIGHT, (s32)filemenu_update_show_options_bottom);
                    filemenu_set_selected(menu, 0, 2);
                } else if (menu->selected <= FM_MAIN_OPT_FILE_4) {
                    // selected a file
                    if (gSaveSlotHasData[menu->selected]) {
                        MenuPanel* confirmMenu;

                        sfx_play_sound(SOUND_MENU_NEXT);
                        set_window_update(WIN_FILES_CONFIRM_OPTIONS, (s32)filemenu_update_show_name_confirm);

                        gWindows[WIN_FILES_CONFIRM_OPTIONS].pos.y = 121;
                        gWindows[WIN_FILES_CONFIRM_OPTIONS].width = 69;
                        gWindows[WIN_FILES_CONFIRM_OPTIONS].height = 44;
                        gWindows[WIN_FILES_CONFIRM_OPTIONS].pos.x = CENTER_WINDOW_X(WIN_FILES_CONFIRM_OPTIONS);

                        gWindows[WIN_FILES_CONFIRM_PROMPT].pos.y = -29;
                        gWindows[WIN_FILES_CONFIRM_PROMPT].width = DELETE_CONFIRM_PROMPT_WIDTH;
                        gWindows[WIN_FILES_CONFIRM_PROMPT].height = 25;
                        gWindows[WIN_FILES_CONFIRM_PROMPT].pos.x = CENTER_WINDOW_X(WIN_FILES_CONFIRM_PROMPT);

                        filemenu_currentMenu = FILE_MENU_CONFIRM;
                        confirmMenu = filemenu_menus[FILE_MENU_CONFIRM];
                        confirmMenu->state = FM_CONFIRM_DELETE;
                        filemenu_set_selected(confirmMenu, 0, 1);
                    } else {
                        sfx_play_sound(SOUND_MENU_ERROR);
                    }
                }
                break;
            case FM_MAIN_SELECT_COPY_FROM:
                if (menu->selected == FM_MAIN_OPT_CANCEL) {
                    // selected "Cancel" button
                    sfx_play_sound(SOUND_MENU_NEXT);
                    menu->state = FM_MAIN_SELECT_FILE;
                    set_window_update(WIN_FILES_STEREO, (s32)filemenu_update_show_options_left);
                    set_window_update(WIN_FILES_MONO, (s32)filemenu_update_show_options_right);
                    set_window_update(WIN_FILES_OPTION_LEFT, (s32)filemenu_update_show_options_bottom);
                    set_window_update(WIN_FILES_OPTION_RIGHT, (s32)filemenu_update_show_options_bottom);
                    filemenu_set_selected(menu, 0, 1);
                } else if (menu->selected <= FM_MAIN_OPT_FILE_4) {
                    // selected a file
                    if (gSaveSlotHasData[menu->selected]) {
                        sfx_play_sound(SOUND_MENU_NEXT);
                        menu->state = FM_MAIN_SELECT_COPY_TO;
                        filemenu_loadedFileIdx = menu->selected;
                    } else {
                        sfx_play_sound(SOUND_MENU_ERROR);
                    }
                }
                break;
            case FM_MAIN_SELECT_COPY_TO:
                if (menu->selected == FM_MAIN_OPT_CANCEL) {
                    // selected "Cancel" button
                    sfx_play_sound(SOUND_MENU_NEXT);
                    menu->state = FM_MAIN_SELECT_COPY_FROM;
                    filemenu_set_selected(menu, 0, 2);
                } else if (menu->selected <= FM_MAIN_OPT_FILE_4) {
                    // selected a file
                    if (filemenu_loadedFileIdx == menu->selected) {
                        sfx_play_sound(SOUND_MENU_ERROR);
                    } else {
                        filemenu_iterFileIdx = menu->selected;
                        if (!gSaveSlotHasData[menu->selected]) {
                            sfx_play_sound(SOUND_MENU_NEXT);
                            filemenu_currentMenu = FILE_MENU_MESSAGE;
                            filemenu_menus[filemenu_currentMenu]->state = FM_MESSAGE_COPIED;
                            gWindows[WIN_FILES_MESSAGE].width = FILE_COPIED_MESSAGE_WIDTH;
                            gWindows[WIN_FILES_MESSAGE].height = 39;
                            gWindows[WIN_FILES_MESSAGE].pos.x = CENTER_WINDOW_X(WIN_FILES_MESSAGE);
                            gWindows[WIN_FILES_MESSAGE].pos.y = CENTER_WINDOW_Y(WIN_FILES_MESSAGE);

                            set_window_update(WIN_FILES_MESSAGE, WINDOW_UPDATE_SHOW);
                            set_window_update(WIN_FILES_CONFIRM_OPTIONS, WINDOW_UPDATE_HIDE);
#if VERSION_PAL
                            if (fio_load_game(filemenu_loadedFileIdx)) {
                                gSaveSlotMetadata[filemenu_iterFileIdx] = gSaveSlotMetadata[filemenu_loadedFileIdx];
                                fio_save_game(filemenu_iterFileIdx);
                                gSaveSlotHasData[filemenu_iterFileIdx] = true;
                            }
#else
                            fio_load_game(filemenu_loadedFileIdx);
                            gSaveSlotMetadata[filemenu_iterFileIdx] = gSaveSlotMetadata[filemenu_loadedFileIdx];
                            fio_save_game(filemenu_iterFileIdx);
                            gSaveSlotHasData[filemenu_iterFileIdx] = true;
#endif
                        } else {
                            MenuPanel* confirmMenu;

                            sfx_play_sound(SOUND_MENU_NEXT);
                            set_window_update(WIN_FILES_CONFIRM_OPTIONS, (s32)filemenu_update_show_name_confirm);
                            gWindows[WIN_FILES_CONFIRM_OPTIONS].pos.y = 121;
                            gWindows[WIN_FILES_CONFIRM_OPTIONS].width = 69;
                            gWindows[WIN_FILES_CONFIRM_OPTIONS].height = 44;
                            gWindows[WIN_FILES_CONFIRM_OPTIONS].pos.x = CENTER_WINDOW_X(WIN_FILES_CONFIRM_OPTIONS);

                            gWindows[WIN_FILES_CONFIRM_PROMPT].pos.y = -43;
                            gWindows[WIN_FILES_CONFIRM_PROMPT].width = COPY_CONFIRM_PROMPT_WIDTH;
                            gWindows[WIN_FILES_CONFIRM_PROMPT].height = 39;
                            gWindows[WIN_FILES_CONFIRM_PROMPT].pos.x = CENTER_WINDOW_X(WIN_FILES_CONFIRM_PROMPT);

                            filemenu_currentMenu = FILE_MENU_CONFIRM;
                            confirmMenu = filemenu_menus[filemenu_currentMenu];
                            confirmMenu->state = FM_CONFIRM_COPY;
                            filemenu_set_selected(confirmMenu, 0, 1);
                        }
                    }
                }
                break;
#if !VERSION_PAL
            case FM_MAIN_SELECT_LANG_DUMMY:
                if (menu->selected == FM_MAIN_OPT_CANCEL) {
                    // selected "Cancel" button
                    sfx_play_sound(SOUND_MENU_NEXT);
                    set_game_mode(GAME_MODE_END_LANGUAGE_SELECT);
                } else if (menu->selected <= FM_MAIN_OPT_FILE_4) {
                    // selected a file
                    if (!gSaveSlotHasData[menu->selected]) {
                        sfx_play_sound(SOUND_MENU_NEXT);
                        filemenu_currentMenu = FILE_MENU_MESSAGE;
                        filemenu_menus[filemenu_currentMenu]->state = FM_MESSAGE_DUMMY;
                        set_window_update(WIN_FILES_MAIN, (s32)main_menu_window_update);
                        set_window_update(WIN_FILES_MESSAGE, WINDOW_UPDATE_SHOW);
                        set_window_update(WIN_FILES_CONFIRM_OPTIONS, WINDOW_UPDATE_HIDE);
                        fio_save_game(menu->selected);
                        gSaveSlotHasData[menu->selected] = true;
                    } else {
                        MenuPanel* confirmMenu;

                        sfx_play_sound(SOUND_MENU_NEXT);
                        set_window_update(WIN_FILES_CONFIRM_OPTIONS, (s32)filemenu_update_show_name_confirm);
                        gWindows[WIN_FILES_CONFIRM_OPTIONS].pos.y = 127;
                        gWindows[WIN_FILES_CONFIRM_OPTIONS].width = 69;
                        gWindows[WIN_FILES_CONFIRM_OPTIONS].height = 44;
                        gWindows[WIN_FILES_CONFIRM_OPTIONS].pos.x = CENTER_WINDOW_X(WIN_FILES_CONFIRM_OPTIONS);

                        gWindows[WIN_FILES_CONFIRM_PROMPT].pos.y = -50;
                        gWindows[WIN_FILES_CONFIRM_PROMPT].width = 148;
                        gWindows[WIN_FILES_CONFIRM_PROMPT].height = 38;
                        gWindows[WIN_FILES_CONFIRM_PROMPT].pos.x = CENTER_WINDOW_X(WIN_FILES_CONFIRM_PROMPT);

                        filemenu_currentMenu = FILE_MENU_CONFIRM;
                        confirmMenu = filemenu_menus[filemenu_currentMenu];
                        confirmMenu->state = FM_CONFIRM_DUMMY;
                        filemenu_set_selected(confirmMenu, 0, 1);
                    }
                }
                break;
#endif
        }
    }

    if (filemenu_pressedButtons & BUTTON_B) {
        switch (menu->state) {
            case FM_MAIN_SELECT_FILE:
                filemenu_set_selected(menu, 2, 2);
                break;
            case FM_MAIN_SELECT_DELETE:
#if !VERSION_PAL
                filemenu_set_selected(menu, 1, 2);
                break;
            case FM_MAIN_SELECT_LANG_DUMMY:
                filemenu_set_selected(menu, 1, 2);
                break;
#endif
            case FM_MAIN_SELECT_COPY_FROM:
#if !VERSION_PAL
                filemenu_set_selected(menu, 1, 2);
                break;
#endif
            case FM_MAIN_SELECT_COPY_TO:
                filemenu_set_selected(menu, 1, 2);
                break;
        }

        switch (menu->state) {
            case FM_MAIN_SELECT_FILE:
                set_window_update(WIN_FILES_STEREO, (s32)filemenu_update_hidden_options_left);
                set_window_update(WIN_FILES_MONO, (s32)filemenu_update_hidden_options_right);
                set_window_update(WIN_FILES_OPTION_LEFT, (s32)filemenu_update_hidden_options_bottom);
                set_window_update(WIN_FILES_OPTION_CENTER, (s32)filemenu_update_hidden_options_bottom);
                set_window_update(WIN_FILES_OPTION_RIGHT, (s32)filemenu_update_hidden_options_bottom);
                set_window_update(WIN_FILES_SLOT1_BODY, (s32)filemenu_update_hidden_options_left);
                set_window_update(WIN_FILES_SLOT2_BODY, (s32)filemenu_update_hidden_options_right);
                set_window_update(WIN_FILES_SLOT3_BODY, (s32)filemenu_update_hidden_options_left);
                set_window_update(WIN_FILES_SLOT4_BODY, (s32)filemenu_update_hidden_options_right);
                set_window_update(WIN_FILES_TITLE, (s32)filemenu_update_hidden_title);
                sfx_play_sound(SOUND_FILE_MENU_OUT);
                set_game_mode(GAME_MODE_END_FILE_SELECT);
                break;
            case FM_MAIN_SELECT_DELETE:
                menu->state = FM_MAIN_SELECT_FILE;
                filemenu_set_selected(menu, 0, 2);
                set_window_update(WIN_FILES_STEREO, (s32)filemenu_update_show_options_left);
                set_window_update(WIN_FILES_MONO, (s32)filemenu_update_show_options_right);
                set_window_update(WIN_FILES_OPTION_LEFT, (s32)filemenu_update_show_options_bottom);
                set_window_update(WIN_FILES_OPTION_RIGHT, (s32)filemenu_update_show_options_bottom);
                sfx_play_sound(SOUND_MENU_BACK);
                break;
            case FM_MAIN_SELECT_COPY_FROM:
                menu->state = FM_MAIN_SELECT_FILE;
                filemenu_set_selected(menu, 1, 2);
                set_window_update(WIN_FILES_STEREO, (s32)filemenu_update_show_options_left);
                set_window_update(WIN_FILES_MONO, (s32)filemenu_update_show_options_right);
                set_window_update(WIN_FILES_OPTION_LEFT, (s32)filemenu_update_show_options_bottom);
                set_window_update(WIN_FILES_OPTION_RIGHT, (s32)filemenu_update_show_options_bottom);
                sfx_play_sound(SOUND_MENU_BACK);
                break;
            case FM_MAIN_SELECT_COPY_TO:
                menu->state = FM_MAIN_SELECT_COPY_FROM;
                filemenu_set_selected(menu, (filemenu_loadedFileIdx % 2) * 2, filemenu_loadedFileIdx / 2);
                sfx_play_sound(SOUND_MENU_BACK);
                break;
#if !VERSION_PAL
            case FM_MAIN_SELECT_LANG_DUMMY:
                sfx_play_sound(SOUND_MENU_BACK);
                set_game_mode(GAME_MODE_END_LANGUAGE_SELECT);
                break;
#endif
        }
    }

#if VERSION_PAL
    gWindows[WIN_FILES_TITLE].width = D_filemenu_80250910[gCurrentLanguage + menu->state * 4];
    gWindows[WIN_FILES_TITLE].pos.x = CENTER_WINDOW_X(WIN_FILES_TITLE);
#endif
}

void filemenu_main_update(MenuPanel* menu) {
    // set default styles for all file slot panels
    gWindowStyles[WIN_FILES_SLOT1_BODY].customStyle = &filemenu_windowStyles[15];
    gWindowStyles[WIN_FILES_SLOT2_BODY].customStyle = &filemenu_windowStyles[15];
    gWindowStyles[WIN_FILES_SLOT3_BODY].customStyle = &filemenu_windowStyles[15];
    gWindowStyles[WIN_FILES_SLOT4_BODY].customStyle = &filemenu_windowStyles[15];
    gWindowStyles[WIN_FILES_SLOT1_TITLE].customStyle = &filemenu_windowStyles[17];
    gWindowStyles[WIN_FILES_SLOT2_TITLE].customStyle = &filemenu_windowStyles[17];
    gWindowStyles[WIN_FILES_SLOT3_TITLE].customStyle = &filemenu_windowStyles[17];
    gWindowStyles[WIN_FILES_SLOT4_TITLE].customStyle = &filemenu_windowStyles[17];

    // modify style for selected file slot panel to add highlight
    switch (menu->selected) {
        case FM_MAIN_OPT_FILE_1:
            gWindowStyles[WIN_FILES_SLOT1_BODY].customStyle = &filemenu_windowStyles[16];
            gWindowStyles[WIN_FILES_SLOT1_TITLE].customStyle = &filemenu_windowStyles[18];
            break;
        case FM_MAIN_OPT_FILE_2:
            gWindowStyles[WIN_FILES_SLOT2_BODY].customStyle = &filemenu_windowStyles[16];
            gWindowStyles[WIN_FILES_SLOT2_TITLE].customStyle = &filemenu_windowStyles[18];
            break;
        case FM_MAIN_OPT_FILE_3:
            gWindowStyles[WIN_FILES_SLOT3_BODY].customStyle = &filemenu_windowStyles[16];
            gWindowStyles[WIN_FILES_SLOT3_TITLE].customStyle = &filemenu_windowStyles[18];
            break;
        case FM_MAIN_OPT_FILE_4:
            gWindowStyles[WIN_FILES_SLOT4_BODY].customStyle = &filemenu_windowStyles[16];
            gWindowStyles[WIN_FILES_SLOT4_TITLE].customStyle = &filemenu_windowStyles[18];
            break;
    }

    // also add highlight to "copy to" target
    if (filemenu_menus[FILE_MENU_MAIN]->state == FM_MAIN_SELECT_COPY_TO) {
        switch (filemenu_loadedFileIdx) {
            case FM_MAIN_OPT_FILE_1:
                gWindowStyles[WIN_FILES_SLOT1_BODY].customStyle = &filemenu_windowStyles[16];
                gWindowStyles[WIN_FILES_SLOT1_TITLE].customStyle = &filemenu_windowStyles[18];
                return;
            case FM_MAIN_OPT_FILE_2:
                gWindowStyles[WIN_FILES_SLOT2_BODY].customStyle = &filemenu_windowStyles[16];
                gWindowStyles[WIN_FILES_SLOT2_TITLE].customStyle = &filemenu_windowStyles[18];
                return;
            case FM_MAIN_OPT_FILE_3:
                gWindowStyles[WIN_FILES_SLOT3_BODY].customStyle = &filemenu_windowStyles[16];
                gWindowStyles[WIN_FILES_SLOT3_TITLE].customStyle = &filemenu_windowStyles[18];
                return;
            case FM_MAIN_OPT_FILE_4:
                gWindowStyles[WIN_FILES_SLOT4_BODY].customStyle = &filemenu_windowStyles[16];
                gWindowStyles[WIN_FILES_SLOT4_TITLE].customStyle = &filemenu_windowStyles[18];
                return;
        }
    }
}

void filemenu_main_cleanup(MenuPanel* menu) {
    s32 i;

    for (i = 0; i < ARRAY_COUNT(filemenu_mainHIDs); i++) {
        hud_element_free(filemenu_mainHIDs[i]);
    }
}
