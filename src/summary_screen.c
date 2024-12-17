#include "global.h"

// Extracts the upper 16 bits of a 32-bit number
#define HIHALF(n) (((n) & 0xFFFF0000) >> 16)

// Extracts the lower 16 bits of a 32-bit number
#define LOHALF(n) ((n) & 0xFFFF)

static void PSS_PlayMonCry(void); // PokeSum_TryPlayMonCry
static void PSS_RemoveAllWindows(u8 curPageIndex); // PokeSum_RemoveWindows
static void PSS_SetHelpContext(void); // PokeSum_SetHelpContext
static void PSS_AddTextToWin3(void); // PokeSum_PrintRightPaneText
static void PSS_AddTextToWin4(void); // PokeSum_PrintBottomPaneText
static void PSS_AddTextToWin5(void); // PokeSum_PrintAbilityDataOrMoveTypes
static void PSS_DrawMonMoveIcon(void); // PokeSum_PrintMonTypeIcons
static void PSS_AddTextToWin0(const u8 * str); // PokeSum_PrintPageName
static void PSS_AddTextToWin1(const u8 * str); // PokeSum_PrintControlsString
static void PSS_AddTextToWin2(const u8 * str); // PrintMonLevelNickOnWindow2
static void PSS_SetInvisibleHpBar(u8 invisible); // ShowOrHideHpBarObjs
static void PSS_SetInvisibleExpBar(u8 invisible); // ShowOrHideExpBarObjs
static void PSS_SetInvisibleMonSprite(u8 invisible); // PokeSum_ShowOrHideMonPicSprite
static void PSS_SetInvisibleMarkings(u8 invisible); // PokeSum_ShowOrHideMonMarkingsSprite
static void PSS_SetInvisiblePokeball(u8 invisible); // ShowOrHideBallIconObj
static void PSS_SetInvisibleIconStatus(u8 invisible); // ShowOrHideStatusIcon
static void PSS_TurnOffScreen(void); // PokeSum_Setup_ResetCallbacks
static void PSS_InitDisplay(void); // PokeSum_Setup_InitGpu
static void PSS_ResetAll(void); // PokeSum_Setup_SpritesReset
static u8 PSS_LoadTilesAndMap(void); // PokeSum_HandleLoadBgGfx
static u8 PSS_LoadSpritesData(void); // PokeSum_HandleCreateSprites
static void PSS_InitWindows(void); // PokeSum_CreateWindows
static void PSS_GetDataPokemon(void); // BufferMonInfo
static void PSS_GetStatsPokemon(void); // BufferMonSkills
static u8 PSS_CheckMonStatus(u32 status); // StatusToAilment
static void PSS_GetMoveNames(u8); // BufferMonMoveI
static u16 PSS_GetMonMove(struct Pokemon * mon, u8 moveSlot); // GetMonMoveBySlotId
static u16 PSS_GetMovePP(struct Pokemon * mon, u8 moveSlot); // GetMonPpByMoveSlot
static void PSS_LoadShinyIndicator(u16, u16); // CreateShinyStarObj
static void PSS_LoadUnkPoint(u16, u16); // CreatePokerusIconObj
static void PSS_LoadMarkings(void); // PokeSum_CreateMonMarkingsSprite
static void PSS_LoadMovesSelectCursor(u16, u16); // CreateMoveSelectionCursorObjs
static void PSS_LoadIconStatus(u16, u16); // CreateMonStatusIconObj
static void PSS_LoadHpBar(u16, u16); // CreateHpBarObjs
static void PSS_LoadExpBar(u16, u16); // CreateExpBarObjs
static void PSS_LoadPokeball(void); // CreateBallIconObj
static void PSS_LoadMonIcon(void); // PokeSum_CreateMonIconSprite
static void PSS_LoadMonSprite(void); // PokeSum_CreateMonPicSprite
static void PSS_ShowInfoPokemon(void); // PrintInfoPage
static void PSS_ShowMonStats(void); // PrintSkillsPage
static void PSS_PrintMoveNamesOrCancel(void); // PrintMovesPage
static void PSS_PrintMoveNamesAndPP(u8 i); // PokeSum_PrintMoveName
static void PSS_CheckIfMonIsEgg(void); // PokeSum_PrintTrainerMemo
static void PSS_PrintExpPointAndNextLvTexts(void); // PokeSum_PrintExpPoints_NextLv
static void PSS_ShowAttackInfo(void); // PokeSum_PrintSelectedMoveStats
static void PSS_ShowEggInfo(void); // PokeSum_PrintTrainerMemo_Mon
static void PSS_ShowMonInfo(void); // PokeSum_PrintTrainerMemo_Egg
static bool32 PSS_IsNatureBoldOrGentle(u8 nature); // PokeSum_IsMonBoldOrGentle
static void PSS_ShowTrainerMemo(void); // PokeSum_PrintTrainerMemo_Mon_NotHeldByOT
static bool32 PSS_IsMonFromGenIII(void); // CurrentMonIsFromGBA
static void PSS_PrintAbilityNameAndDescription(void); // PokeSum_PrintAbilityNameAndDesc
static void PSS_DrawMoveIcon(void); // PokeSum_DrawMoveTypeIcons
static void PSS_ShowIconStatus(void); // UpdateMonStatusIconObj
static void PSS_SetupHpBar(void); // UpdateHpBarObjs
static void PSS_SetupExpBar(void); // UpdateExpBarObjs
static void PSS_UnloadMarkings(void); // PokeSum_DestroyMonMarkingsSprite
static void PSS_GetMarkings(void); // PokeSum_UpdateMonMarkingsAnim
static void PSS_ScrollPSSBackground(void);

struct PokemonSummaryScreenData
{
    u8 window[7];

    u8 spriteId_0;
    u8 spriteId_1;
    u8 spriteId_2;

    u8 task;
    u8 unk301C;

    u8 unk3020;

    bool32 isEnemyParty; /* 0x3024 */

    struct PokeSummary
    {
        u8 nickname[POKEMON_NAME_LENGTH + 1];
        u8 specieName[POKEMON_NAME_LENGTH + 1];
        u8 ot_name[12];
        u8 unk304C[2][12];

        u8 dexNum[5];
        u8 ot_id[7];
        u8 heldItem[ITEM_NAME_LENGTH + 1];

        u8 genderSymbol[3];
        u8 level[7];
        u8 unk3090[9];
        u8 unk309C[5][5];

        u8 unk30B8[5][11];
        u8 unk30F0[5][11];
        u8 moveName[5][MOVE_NAME_LENGTH + 1];
        u8 power[5][5];
        u8 unk3188[5][5];

        u8 unk31A4[9];
        u8 unk31B0[9];

        u8 abilityName[13];
        u8 abilityDescription[52];
    } summary;

    u8 isEgg; /* 0x3200 */
    u8 isBadEgg; /* 0x3204 */

    u8 mode; /* 0x3208 */
    u8 unk320C; /* 0x320C */
    u8 lastIndex; /* 0x3210 */
    u8 curPageIndex; /* 0x3214 */
    u8 unk3218; /* 0x3218 */
    u8 isBoxMon; /* 0x321C */
    u8 typeIcons[2]; /* 0x3220 */

    u8 unk3224; /* 0x3224 */
    u8 unk3228; /* 0x3228 */
    u8 unk322C; /* 0x322C */
    u8 unk3230; /* 0x3230 */

    u8 lockMovesFlag; /* 0x3234 */

    u8 unk3238; /* 0x3238 */
    u8 unk323C; /* 0x323C */
    u8 unk3240; /* 0x3240 */
    u8 unk3244; /* 0x3244 */
    u8 unk3248; /* 0x3248 */
    s16 unk324C; /* 0x324C */

    u16 move[5]; /* 0x3250 */
    u16 currentMove[5]; /* 0x325A */
    u8 unk3264; /* 0x3264 */
    u8 unk3268; /* 0x3268 */

    u8 monStatus; /* 0x326C */

    u8 state3270; /* 0x3270 */
    u8 state3274; /* 0x3274 */
    u8 unk3278; /* 0x3278 */
    u8 state; /* 0x327C */
    u8 unk3280; /* 0x3280 */
    u8 unk3284; /* 0x3284 */
    u8 unk3288; /* 0x3288 */
    u8 unk328C; /* 0x328C */

    struct Pokemon currentMon; /* 0x3290 */

    union
    {
        struct Pokemon * mons;
        struct BoxPokemon * boxMons;
    } monList;

    MainCallback savedCallback;
    struct Sprite * markingSprite;

    u8 unk3300[2]; /* 0x3300 */
    u8 unk3304[3]; /* 0x3304 */
    u8 unused[200]; /* 0x3304 */
};

struct Struct203B144
{
    u16 unk00;
    u16 unk02;
    u16 tileTag;
    u16 palTag;
    u16 unk08;
    u16 unk0A;
    u16 unk0C;
    u16 unk0E;
    u16 unk10;

    u16 unk12[5];
    u16 unk1C[5];

    u16 unk26;
};

struct Struct203B160
{
    struct Sprite * sprites[11]; /* 0x00 */
    u16 cordX[11]; /* 0x2c */
    u16 tileTag; /* 0x42 */
    u16 palTag; /* 0x44 */
};

struct Struct203B15C
{
    struct Sprite * sprites[10]; /* 0x00 */
    u16 cordX[10]; /* 0x28 */
    u16 unk3C; /* 0x3c */
    u16 unk3E; /* 0x3e */
};

struct Struct203B170
{
    u8 unk00; /* 0x00 */
    u8 tileTag; /* 0x04 */
    u8 unk08; /* 0x08 */
};

struct Struct203B148
{
    struct Sprite * sprite; /* 0x00 */
    u16 tileTag; /* 0x04 */
    u16 palTag; /* 0x06 */
    u16 unk08; /* 0x08 */
};

struct Struct203B158
{
    struct Sprite * sprite; /* 0x00 */
    u16 tileTag; /* 0x04 */
    u16 palTag; /* 0x06 */
};

struct Struct203B164
{
    struct Sprite * sprite; /* 0x00 */
    u16 tileTag; /* 0x04 */
    u16 palTag; /* 0x06 */
};

struct Struct203B168
{
    struct Sprite * sprite; /* 0x00 */
    u16 tileTag; /* 0x04 */
    u16 palTag; /* 0x06 */
};

extern struct PokemonSummaryScreenData *sMonSummaryScreen;
extern struct Struct203B144 *sUnknown_203B144;
extern struct Struct203B148 *sUnknown_203B148[4];
extern struct Struct203B158 *sStatusIconSummaryScreen;
extern struct Struct203B15C *sHpBarSummaryScreen;
extern struct Struct203B160 *sExpBarSummaryScreen;
extern struct Struct203B164 *sUnknown_203B164;
extern struct Struct203B168 *sUnknown_203B168;
extern struct Struct203B170 *sUnknown_203B170;
extern u8 sLastViewedMonIndex;
extern u8 sUnknown_203B16D;
extern u8 sUnknown_203B16E;

static const struct BgTemplate SummayScreenBgTemplate[] = 
{
	{ //Text
		.bg = 0,
		.charBaseIndex = 1,
		.mapBaseIndex = 29,
		.screenSize = 0,
		.paletteMode = 0,
		.priority = 0,
		.baseTile = 0	   
	},
	{ //Page Showed
	 	.bg = 1,
	 	.charBaseIndex = 0,
	 	.mapBaseIndex = 30,
	 	.screenSize = 0,
	 	.paletteMode = 0,
	 	.priority = 2,
	 	.baseTile = 0
	 },
	 { // BG
	 	.bg = 2,
	 	.charBaseIndex = 0,
	 	.mapBaseIndex = 31,
	 	.screenSize = 0,
	 	.paletteMode = 0,
	 	.priority = 3,
	 	.baseTile = 0
	 },
	 { //Moves
	 	.bg = 3,
	 	.charBaseIndex = 0,
	 	.mapBaseIndex = 28,
	 	.screenSize = 0,
	 	.paletteMode = 0,
	 	.priority = 1,
	 	.baseTile = 0
	 }
};

static const struct WindowTemplate WindowTemplatePokemonInfo[] =
{
    { //PAGE TITLE
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 13,
        .height = 2,
        .paletteNum = 7,
        .baseBlock = 0x0258
    },
    { //INSTRUCTION
        .bg = 0,
        .tilemapLeft = 19,
        .tilemapTop = 0,
        .width = 11,
        .height = 2,
        .paletteNum = 7,
        .baseBlock = 0x0272
    },
    { // NICKNAME & LV OR TYPES
        .bg = 0,
        .tilemapLeft = 21,
        .tilemapTop = 2,
        .width = 10,
        .height = 4,
        .paletteNum = 6,
        .baseBlock = 0x0288
    }
};

static const struct WindowTemplate sDataMonAndNatureWindowTemplate[] = 
{
    { //Data Pokemon
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 2,
        .width = 21,
        .height = 10,
        .paletteNum = 6,
        .baseBlock = 0
    },
    { // Nature and Found place
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 12,
        .width = 28,
        .height = 8,
        .paletteNum = 6,
        .baseBlock = 220
    },
    { //DUMMY_WIN_TEMPLATE
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 0,
        .height = 0,
        .paletteNum = 0,
        .baseBlock = 0
    },
    { //DUMMY_WIN_TEMPLATE
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 0,
        .height = 0,
        .paletteNum = 0,
        .baseBlock = 0
    },
};

static const struct WindowTemplate sMonStatsAndAbilityWindowTemplate[] = 
{
    { // Pokemon Stats
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 2,
        .width = 17,
        .height = 13,
        .paletteNum = 6,
        .baseBlock = 1
    },
    { //Exp Point & Next Lv
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 0,
        .height = 0,
        .paletteNum = 0,
        .baseBlock = 0
    },
    { // Ability & Description
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 15,
        .width = 29,
        .height = 5,
        .paletteNum = 6,
        .baseBlock = 292
    },
    { // DUMMY_WIN_TEMPLATE
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 0,
        .height = 0,
        .paletteNum = 0,
        .baseBlock = 0
    },
};

static const struct WindowTemplate sMovesInfoWindowTemplate[] = 
{
    { //Move Names
        .bg = 0,
        .tilemapLeft = 5,
        .tilemapTop = 2,
        .width = 10,
        .height = 18,
        .paletteNum = 6,
        .baseBlock = 1
    },
    { //Move Data & Description
        .bg = 0,
        .tilemapLeft = 15,
        .tilemapTop = 6,
        .width = 15,
        .height = 14,
        .paletteNum = 6,
        .baseBlock = 181
    },
    { //Move Icons
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 2,
        .width = 5,
        .height = 18,
        .paletteNum = 6,
        .baseBlock = 452
    },
    { //DUMMY_WIN_TEMPLATE
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 0,
        .height = 0,
        .paletteNum = 0,
        .baseBlock = 0
    },
};

static const struct WindowTemplate sUnknown_8463F9C[] = 
{
    {
        .bg = 255,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 0,
        .height = 0,
        .paletteNum = 0,
        .baseBlock = 0x0000
    },
};

static const s8 sPSSNatureStatTable[][5] =
{
    // Atk Def Spd Sp.Atk Sp.Def
    {    0,  0,  0,     0,     0}, // Hardy
    {   +1, -1,  0,     0,     0}, // Lonely
    {   +1,  0, -1,     0,     0}, // Brave
    {   +1,  0,  0,    -1,     0}, // Adamant
    {   +1,  0,  0,     0,    -1}, // Naughty
    {   -1, +1,  0,     0,     0}, // Bold
    {    0,  0,  0,     0,     0}, // Docile
    {    0, +1, -1,     0,     0}, // Relaxed
    {    0, +1,  0,    -1,     0}, // Impish
    {    0, +1,  0,     0,    -1}, // Lax
    {   -1,  0, +1,     0,     0}, // Timid
    {    0, -1, +1,     0,     0}, // Hasty
    {    0,  0,  0,     0,     0}, // Serious
    {    0,  0, +1,    -1,     0}, // Jolly
    {    0,  0, +1,     0,    -1}, // Naive
    {   -1,  0,  0,    +1,     0}, // Modest
    {    0, -1,  0,    +1,     0}, // Mild
    {    0,  0, -1,    +1,     0}, // Quiet
    {    0,  0,  0,     0,     0}, // Bashful
    {    0,  0,  0,    +1,    -1}, // Rash
    {   -1,  0,  0,     0,    +1}, // Calm
    {    0, -1,  0,     0,    +1}, // Gentle
    {    0,  0, -1,     0,    +1}, // Sassy
    {    0,  0,  0,    -1,    +1}, // Careful
    {    0,  0,  0,     0,     0}, // Quirky
};

enum
{
	DARK,
	WHITE_TITLE,
	BLUE_SHADOW,
	WHITE,
	RED_SHADOW,
	BLUE,
	RED,
	ORANGE,
	RED_2,
};

enum
{
	tTimer,
	Offset,
};

static const u8 sPSSTextColours[][3] =
{
    [DARK] = {0, 14, 13},		//Text Dark
    [WHITE_TITLE] = {0, 1, 2}, 	//Text White (title) 
    [BLUE_SHADOW] = {0, 15, 7}, //Red Shadow
    [WHITE] = {0, 15, 14},		//Text White
    [RED_SHADOW] = {0, 15, 1}, 	//Blue Shadow
    [BLUE] = {0, 7, 6}, 		//Male Symbol
    [RED] = {0, 1, 2}, 			//Female Symbol (red)
    [ORANGE] = {0, 3, 14}, 		//Low PP
    [RED_2] = {0, 1, 14}, 		//Whithout PP
};

static const u16 * const sHpBarPalettes[] =
{
    gPal0SummaryScreen,
    gPal1SummaryScreen,
    gPal2SummaryScreen,
};

// The functions below must be hooked probably

static void PSS_GetDataPokemon(void)
{
    u8 tempStr[20];
    u16 dexNum;
    u16 gender;
    u16 heldItem;
    u32 otId;

    dexNum = SpeciesToPokedexNum(GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPECIES));
    if (dexNum == 0xffff)
        StringCopy(sMonSummaryScreen->summary.dexNum, gText_8416202);
    else
        ConvertIntToDecimalStringN(sMonSummaryScreen->summary.dexNum, dexNum, STR_CONV_MODE_LEADING_ZEROS, 3);

    sUnknown_203B144->unk00 = 0;

    if (!sMonSummaryScreen->isEgg)
    {
        dexNum = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPECIES);
        GetSpeciesName(sMonSummaryScreen->summary.specieName, dexNum);
    }
    else
    {
        StringCopy(sMonSummaryScreen->summary.specieName, gText_EggNickname);
        if (gSaveBlock2Ptr->optionsLanguage == SPA)
			StringCopy(sMonSummaryScreen->summary.specieName, gText_EggNicknameSpa);
		return;
    }

    sMonSummaryScreen->typeIcons[0] = gBaseStats[dexNum].type1;
    sMonSummaryScreen->typeIcons[1] = gBaseStats[dexNum].type2;

    GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_NICKNAME, tempStr);
    StringCopyN_Multibyte(sMonSummaryScreen->summary.nickname, tempStr, POKEMON_NAME_LENGTH);
    StringGetEnd10(sMonSummaryScreen->summary.nickname);

    gender = GetMonGender(&sMonSummaryScreen->currentMon);
    dexNum = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPECIES2);

    if (gender == MON_FEMALE)
        StringCopy(sMonSummaryScreen->summary.genderSymbol, gText_FemaleSymbol);
    else if (gender == MON_MALE)
        StringCopy(sMonSummaryScreen->summary.genderSymbol, gText_MaleSymbol);
    else
        StringCopy(sMonSummaryScreen->summary.genderSymbol, gText_StringDummy);

    if (dexNum == SPECIES_NIDORAN_M || dexNum == SPECIES_NIDORAN_F)
        if (StringCompare(sMonSummaryScreen->summary.nickname, gSpeciesNames[dexNum]) == 0)
            StringCopy(sMonSummaryScreen->summary.genderSymbol, gText_StringDummy);

    GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_OT_NAME, tempStr);
    StringCopyN_Multibyte(sMonSummaryScreen->summary.ot_name, tempStr, OT_NAME_LENGTH);

    ConvertInternationalString(sMonSummaryScreen->summary.ot_name, GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_LANGUAGE));

    otId = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_OT_ID) & 0xffff;
    ConvertIntToDecimalStringN(sMonSummaryScreen->summary.ot_id, otId, STR_CONV_MODE_LEADING_ZEROS, 5);

    ConvertIntToDecimalStringN(tempStr, GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_LEVEL), STR_CONV_MODE_LEFT_ALIGN, 3);
    if (gSaveBlock2Ptr->optionsLanguage == ENG)
		StringCopy(sMonSummaryScreen->summary.level, gText_Lv);
    if (gSaveBlock2Ptr->optionsLanguage == SPA)
		StringCopy(sMonSummaryScreen->summary.level, gText_LvSpa);
    StringAppendN(sMonSummaryScreen->summary.level, tempStr, 4);

    heldItem = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_HELD_ITEM);

    if (heldItem == ITEM_NONE)
	{
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
			StringCopy(sMonSummaryScreen->summary.heldItem, gText_84161EF);
        if (gSaveBlock2Ptr->optionsLanguage == SPA)
			StringCopy(sMonSummaryScreen->summary.heldItem, gText_84161EFSpa);
	}
    else
        CopyItemName(heldItem, sMonSummaryScreen->summary.heldItem);
}

static void PSS_InitDisplay(void)
{
    DmaClearLarge16(3, (void *)VRAM, VRAM_SIZE, 0x1000);
    DmaClear32(3, (void *)OAM, OAM_SIZE);
    DmaClear16(3, (void *)PLTT, PLTT_SIZE);

    SetGpuReg(REG_OFFSET_DISPCNT, 0);

    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, SummayScreenBgTemplate, NELEMS(SummayScreenBgTemplate));

    ChangeBgX(0, 0, 0);
    ChangeBgY(0, 0, 0);
    ChangeBgX(1, 0, 0);
    ChangeBgY(1, 0, 0);
    ChangeBgX(2, 0, 0);
    ChangeBgY(2, 0, 0);
    ChangeBgX(3, 0, 0);
    ChangeBgY(3, 0, 0);

    DeactivateAllTextPrinters();
//Transparencia
	SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_EFFECT_BLEND | BLDCNT_TGT1_BG1 | BLDCNT_TGT2_BG2);
	SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(13, 16));
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON);

    ShowBg(0);
    ShowBg(1);
    ShowBg(2);
    ShowBg(3);
}

static void PSS_AddTextToWin0(const u8 * str)
{
    FillWindowPixelBuffer(sMonSummaryScreen->window[0], 0);
    AddTextPrinterParameterized3(sMonSummaryScreen->window[0], 2, 4, 1, sPSSTextColours[WHITE_TITLE], 0, str);
    PutWindowTilemap(sMonSummaryScreen->window[0]);
}

static void PSS_AddTextToWin1(const u8 * str)
{
    u8 v0;
    s32 width;
    u8 r1;

    FillWindowPixelBuffer(sMonSummaryScreen->window[1], 0);
    width = GetStringWidth(0, str, 0);
    r1 = sMonSummaryScreen->window[1];
    AddTextPrinterParameterized3(r1, 0, 0x54 - width, 0, sPSSTextColours[WHITE_TITLE], 0, str);
    PutWindowTilemap(sMonSummaryScreen->window[1]);
}

static void PSS_AddTextToWin2(const u8 * msg)
{
    FillWindowPixelBuffer(sMonSummaryScreen->window[2], 0);

    if (!sMonSummaryScreen->isEgg)
    {
        if (sMonSummaryScreen->curPageIndex != PSS_PAGE_MOVES_INFO)
		{
			AddTextPrinterParameterized3(sMonSummaryScreen->window[2], 2, 0, 14, sPSSTextColours[DARK], 0xff, sMonSummaryScreen->summary.level);
		}
		else
		{
			if (gSaveBlock2Ptr->optionsLanguage == ENG)
			{
				BlitMoveInfoIcon(sMonSummaryScreen->window[2], sMonSummaryScreen->typeIcons[0] + 1, 6, 16);
				if (sMonSummaryScreen->typeIcons[0] != sMonSummaryScreen->typeIcons[1])
					BlitMoveInfoIcon(sMonSummaryScreen->window[2], sMonSummaryScreen->typeIcons[1] + 1, 38, 16);
			}
			if (gSaveBlock2Ptr->optionsLanguage == SPA)
			{
				BlitMoveInfoIcon(sMonSummaryScreen->window[2], sMonSummaryScreen->typeIcons[0] + 24, 6, 16);
				if (sMonSummaryScreen->typeIcons[0] != sMonSummaryScreen->typeIcons[1])
					BlitMoveInfoIcon(sMonSummaryScreen->window[2], sMonSummaryScreen->typeIcons[1] + 24, 38, 16);
			}
		}
        AddTextPrinterParameterized3(sMonSummaryScreen->window[2], 2, 0, 2, sPSSTextColours[DARK], 0xff, sMonSummaryScreen->summary.nickname);
			
		if (GetMonGender(&sMonSummaryScreen->currentMon) == MON_FEMALE)
			AddTextPrinterParameterized3(sMonSummaryScreen->window[2], 2, 62, 2, sPSSTextColours[RED], 0, sMonSummaryScreen->summary.genderSymbol);
		else
			AddTextPrinterParameterized3(sMonSummaryScreen->window[2], 2, 62, 2, sPSSTextColours[BLUE], 0, sMonSummaryScreen->summary.genderSymbol);
	}

    PutWindowTilemap(sMonSummaryScreen->window[2]);
}

static void PSS_ShowInfoPokemon(void)
{
    

    if (!sMonSummaryScreen->isEgg)
    {
        AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 80, 20, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.specieName);
		
		if (gSaveBlock2Ptr->optionsLanguage == ENG)
		{
			AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 8 - 1, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_DexNumber);
			AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 20 - 1, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_Name);
			AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 32 - 1, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_Type);
			AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 44 - 1, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_OT);
			AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 56 - 1, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_IDNumber);
			AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 68 - 1, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_Item);
		}
        if (gSaveBlock2Ptr->optionsLanguage == SPA)
		{
			AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 8 - 1, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_DexNumberSpa);
			AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 20 - 1, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_NameSpa);
			AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 32 - 1, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_TypeSpa);
			AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 44 - 1, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_OTSpa);
			AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 56 - 1, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_IDNumberSpa);
			AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 68 - 1, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_ItemSpa);
		}
		
		if ((HIHALF(GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_OT_ID)) ^ LOHALF(GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_OT_ID)) ^ HIHALF(GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_PERSONALITY)) ^ LOHALF(GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_PERSONALITY))) < 8)
			AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 80 + sUnknown_203B144->unk00, 8, sPSSTextColours[RED], TEXT_SPEED_FF, sMonSummaryScreen->summary.dexNum);
		else	
			AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 80 + sUnknown_203B144->unk00, 8, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.dexNum);
		if ((gSaveBlock2Ptr->playerTrainerId[0] | (gSaveBlock2Ptr->playerTrainerId[1] << 8) | (gSaveBlock2Ptr->playerTrainerId[2] << 16) | (gSaveBlock2Ptr->playerTrainerId[3] << 24)) == GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_OT_ID))
		{
			if (gSaveBlock2Ptr->playerGender == FEMALE)
				AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 80, 44, sPSSTextColours[RED], TEXT_SPEED_FF, sMonSummaryScreen->summary.ot_name);
			if (gSaveBlock2Ptr->playerGender == MALE)
				AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 80, 44, sPSSTextColours[BLUE], TEXT_SPEED_FF, sMonSummaryScreen->summary.ot_name);
		}
		else
			AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 80, 44, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.ot_name);
		AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 80, 56, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.ot_id);
		AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 80, 68, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.heldItem);
    }
    else
    {
        u8 eggCycles;
        u8 hatchMsgIndex;

        eggCycles = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_FRIENDSHIP);

        if (eggCycles <= 5)
            hatchMsgIndex = 3;
        else if (eggCycles <= 10)
            hatchMsgIndex = 2;
        else if (eggCycles <= 40)
            hatchMsgIndex = 1;
        else
            hatchMsgIndex = 0;

        if (sMonSummaryScreen->isBadEgg)
            hatchMsgIndex = 0;

        if (gSaveBlock2Ptr->optionsLanguage == ENG)
		{
			AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 7, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_NameSpa);
			AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 31, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_Status);
			AddTextPrinterParameterized4(sMonSummaryScreen->window[3], 2, 80, 32, 0, -2, sPSSTextColours[DARK], TEXT_SPEED_FF, sUnknown_8463EC4[hatchMsgIndex]);
        }
		if (gSaveBlock2Ptr->optionsLanguage == SPA)
		{
			AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 7, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_NameSpa);
			AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 31, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_StatusSpa);
			AddTextPrinterParameterized4(sMonSummaryScreen->window[3], 2, 80, 32, 0, -2, sPSSTextColours[DARK], TEXT_SPEED_FF, sUnknown_8463EC4[hatchMsgIndex + 4]);
		}
		AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 80, 8, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.specieName);
    }
}

static void PSS_ShowMonStats(void)
{
    u8 nature = GetNature(&sMonSummaryScreen->currentMon);
	
	if (gSaveBlock2Ptr->optionsLanguage == ENG)
	{
		AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 44, 0, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_HP);
		AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 79, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_ExpPoints);
		AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 91, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_ToNextLv);
		AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 19, sPSSTextColours[WHITE + sPSSNatureStatTable[nature][0]], TEXT_SPEED_FF, gText_PSS_Attack);
		AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 31, sPSSTextColours[WHITE + sPSSNatureStatTable[nature][1]], TEXT_SPEED_FF, gText_PSS_Defense);
		AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 43, sPSSTextColours[WHITE + sPSSNatureStatTable[nature][3]], TEXT_SPEED_FF, gText_PSS_SpAtk);
		AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 55, sPSSTextColours[WHITE + sPSSNatureStatTable[nature][4]], TEXT_SPEED_FF, gText_PSS_SpDef);
		AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 67, sPSSTextColours[WHITE + sPSSNatureStatTable[nature][2]], TEXT_SPEED_FF, gText_PSS_Speed);
	}
    if (gSaveBlock2Ptr->optionsLanguage == SPA)
	{
		AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 44, 0, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_HPSpa);
		AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 79, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_ExpPointsSpa);
		AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 91, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_ToNextLvSpa);
		AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 19, sPSSTextColours[WHITE + sPSSNatureStatTable[nature][0]], TEXT_SPEED_FF, gText_PSS_AttackSpa);
		AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 31, sPSSTextColours[WHITE + sPSSNatureStatTable[nature][1]], TEXT_SPEED_FF, gText_PSS_DefenseSpa);
		AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 43, sPSSTextColours[WHITE + sPSSNatureStatTable[nature][3]], TEXT_SPEED_FF, gText_PSS_SpAtkSpa);
		AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 55, sPSSTextColours[WHITE + sPSSNatureStatTable[nature][4]], TEXT_SPEED_FF, gText_PSS_SpDefSpa);
		AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 10, 67, sPSSTextColours[WHITE + sPSSNatureStatTable[nature][2]], TEXT_SPEED_FF, gText_PSS_SpeedSpa);
	}
	
	AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 68 + sUnknown_203B144->unk02, 0, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.unk3090);
    AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 90 + sUnknown_203B144->tileTag, 20, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.unk309C[PSS_STAT_ATK]);
    AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 90 + sUnknown_203B144->palTag, 32, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.unk309C[PSS_STAT_DEF]);
    AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 90 + sUnknown_203B144->unk08, 44, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.unk309C[PSS_STAT_SPA]);
    AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 90 + sUnknown_203B144->unk0A, 56, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.unk309C[PSS_STAT_SPD]);
    AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 90 + sUnknown_203B144->unk0C, 68, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.unk309C[PSS_STAT_SPE]);
    AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 55 + sUnknown_203B144->unk0E, 80, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.unk31A4);
    AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 55 + sUnknown_203B144->unk10, 92, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.unk31B0);
}

static void PSS_PrintMoveNamesOrCancel(void)
{
    u8 i;

    for (i = 0; i < 4; i++)
        PSS_PrintMoveNamesAndPP(i);

    if (sMonSummaryScreen->curPageIndex == PSS_PAGE_MOVES_INFO)
    {
        if (sMonSummaryScreen->mode == PSS_MODE_SELECT_MOVE)
            PSS_PrintMoveNamesAndPP(4);
        else
		{
            if (gSaveBlock2Ptr->optionsLanguage == ENG)
				AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 3, MACRO_8137270(4), sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_FameChecker_Cancel);
            if (gSaveBlock2Ptr->optionsLanguage == SPA)
				AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 3, MACRO_8137270(4), sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_FameChecker_CancelSpa);
		}
    }
}

static void PSS_PrintMoveNamesAndPP(u8 i)
{
    u8 color = WHITE;
    u8 curPP = PSS_GetMovePP(&sMonSummaryScreen->currentMon, i);
    u16 move = sMonSummaryScreen->currentMove[i];
    u8 ppBonuses = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_PP_BONUSES);
    u8 maxPP = CalculatePPWithBonus(move, ppBonuses, i);

    if (i == 4)
        curPP = maxPP;
	//Add Move Names
    AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 3, MACRO_8137270(i) - 2, sPSSTextColours[WHITE], TEXT_SPEED_FF, sMonSummaryScreen->summary.moveName[i]);

    if (sMonSummaryScreen->currentMove[i] == 0 || (curPP == maxPP))
        color = WHITE;
    else if (curPP == 0)
        color = RED_2;
    else if (maxPP == 3)
    {
        if (curPP == 2)
            color = ORANGE;
        else if (curPP == 1)
            color = ORANGE;
    }
    else if (maxPP == 2)
    {
        if (curPP == 1)
            color = ORANGE;
    }
    else
    {
        if (curPP <= (maxPP / 4))
            color = ORANGE;
        else if (curPP <= (maxPP / 2))
            color = ORANGE;
    }
	//Add PP text
    AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 20, MACRO_81372E4(i), sPSSTextColours[color], TEXT_SPEED_FF, gText_8416238);
	// Add PP counter
    AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 40 + sUnknown_203B144->unk12[i], MACRO_81372E4(i), sPSSTextColours[color], TEXT_SPEED_FF, sMonSummaryScreen->summary.unk30B8[i]);

    if (sMonSummaryScreen->currentMove[i] != MOVE_NONE)
    {
		// Add Slash
        AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 52, MACRO_81372E4(i), sPSSTextColours[color], TEXT_SPEED_FF, gText_Slash);
        // Add PP Max
		AddTextPrinterParameterized3(sMonSummaryScreen->window[3], 2, 58 + sUnknown_203B144->unk1C[i], MACRO_81372E4(i), sPSSTextColours[color], TEXT_SPEED_FF, sMonSummaryScreen->summary.unk30F0[i]);
    }
}

static void sub_8137578(void)
{
    u8 nature;
    u8 level;
    u8 metLocation;
    u8 levelStr[5];
    u8 mapNameStr[32];
    u8 natureMetOrHatchedAtLevelStr[152];

    DynamicPlaceholderTextUtil_Reset();
    nature = GetNature(&sMonSummaryScreen->currentMon);
	if (gSaveBlock2Ptr->optionsLanguage == ENG)
		DynamicPlaceholderTextUtil_SetPlaceholderPtr(0, gNatureNamePointers[nature]);
	else if (gSaveBlock2Ptr->optionsLanguage == SPA)
		DynamicPlaceholderTextUtil_SetPlaceholderPtr(0, gNatureNameSpaPointers[nature]);

    level = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_MET_LEVEL);

    if (level == 0)
        level = 1;

    ConvertIntToDecimalStringN(levelStr, level, STR_CONV_MODE_LEFT_ALIGN, 3);
    DynamicPlaceholderTextUtil_SetPlaceholderPtr(1, levelStr);

    metLocation = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_MET_LOCATION);

    if (sub_813B838(metLocation) == TRUE)
        GetMapNameGeneric_(mapNameStr, metLocation);
    else
    {
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
		{
			if (sMonSummaryScreen->isEnemyParty == TRUE || sub_8138B4C() == TRUE)
				StringCopy(mapNameStr, gText_8419C13);
			else
				StringCopy(mapNameStr, gText_8419C0B);
		}
        if (gSaveBlock2Ptr->optionsLanguage == SPA)
		{
			if (sMonSummaryScreen->isEnemyParty == TRUE || sub_8138B4C() == TRUE)
				StringCopy(mapNameStr, gText_8419C13Spa);
			else
				StringCopy(mapNameStr, gText_8419C0BSpa);
		}
    }

    DynamicPlaceholderTextUtil_SetPlaceholderPtr(2, mapNameStr);

    if (GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_MET_LEVEL) == 0)
    {
        if (GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_EVENT_LEGAL) == 1)
        {
            if (gSaveBlock2Ptr->optionsLanguage == ENG)
			{
				if (PSS_IsNatureBoldOrGentle(nature))
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_841996D);
				else
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_841992F);
			}
            if (gSaveBlock2Ptr->optionsLanguage == SPA)
			{
				if (PSS_IsNatureBoldOrGentle(nature))
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_841996DSpa);
				else
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_841992FSpa);
			}
        }
        else
        {
            if (gSaveBlock2Ptr->optionsLanguage == ENG)
			{
				if (PSS_IsNatureBoldOrGentle(nature))
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84198D5);
				else
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84198B4);
			}
            if (gSaveBlock2Ptr->optionsLanguage == SPA)
			{
				if (PSS_IsNatureBoldOrGentle(nature))
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84198D5Spa);
				else
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84198B4Spa);
			}
        }
    }
    else
    {
        if (metLocation == METLOC_FATEFUL_ENCOUNTER)
        {
            if (gSaveBlock2Ptr->optionsLanguage == ENG)
			{
				if (PSS_IsNatureBoldOrGentle(nature))
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84197ED);
				else
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84197B8);
			}
            if (gSaveBlock2Ptr->optionsLanguage == SPA)
			{
				if (PSS_IsNatureBoldOrGentle(nature))
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84197EDSpa);
				else
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84197B8Spa);
			}
        }
        else
        {
            if (gSaveBlock2Ptr->optionsLanguage == ENG)
			{
				if (PSS_IsNatureBoldOrGentle(nature))
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_8419841);
				else
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_8419822);
			}
            if (gSaveBlock2Ptr->optionsLanguage == SPA)
			{
				if (PSS_IsNatureBoldOrGentle(nature))
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_8419841Spa);
				else
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_8419822Spa);
			}
        }
    }

	if (gSaveBlock2Ptr->optionsLanguage == ENG)
		AddTextPrinterParameterized3(sMonSummaryScreen->window[4], 2, 6, 0, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_TrainerMemo);
	if (gSaveBlock2Ptr->optionsLanguage == SPA)
		AddTextPrinterParameterized3(sMonSummaryScreen->window[4], 2, 6, 0, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_TrainerMemoSpa);
    AddTextPrinterParameterized4(sMonSummaryScreen->window[4], 2, 16, 12, 0, -2, sPSSTextColours[DARK], TEXT_SPEED_FF, natureMetOrHatchedAtLevelStr);
}

static void PSS_ShowTrainerMemo(void)
{
    u8 nature;
    u8 level;
    u8 metLocation;
    u8 levelStr[5];
    u8 mapNameStr[32];
    u8 natureMetOrHatchedAtLevelStr[152];

    DynamicPlaceholderTextUtil_Reset();
    nature = GetNature(&sMonSummaryScreen->currentMon);
	if (gSaveBlock2Ptr->optionsLanguage == ENG)
		DynamicPlaceholderTextUtil_SetPlaceholderPtr(0, gNatureNamePointers[nature]);
	else if (gSaveBlock2Ptr->optionsLanguage == SPA)
		DynamicPlaceholderTextUtil_SetPlaceholderPtr(0, gNatureNameSpaPointers[nature]);

    level = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_MET_LEVEL);

    if (level == 0)
        level = 1;

    ConvertIntToDecimalStringN(levelStr, level, STR_CONV_MODE_LEFT_ALIGN, 3);
    DynamicPlaceholderTextUtil_SetPlaceholderPtr(1, levelStr);

    metLocation = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_MET_LOCATION);

    if (!sub_813B838(metLocation) || !PSS_IsMonFromGenIII())
    {
        if (sub_8138B4C() == TRUE)
        {
            sub_8137578();
            return;
        }

        if (metLocation == METLOC_FATEFUL_ENCOUNTER)
        {
            if (gSaveBlock2Ptr->optionsLanguage == ENG)
			{
				if (PSS_IsNatureBoldOrGentle(nature))
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84197ED);
				else
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84197B8);
			}
            if (gSaveBlock2Ptr->optionsLanguage == SPA)
			{
				if (PSS_IsNatureBoldOrGentle(nature))
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84197EDSpa);
				else
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84197B8Spa);
			}
        }
        else
        {
            if (gSaveBlock2Ptr->optionsLanguage == ENG)
			{
				if (PSS_IsNatureBoldOrGentle(nature))
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_841979D);
				else
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_8419782);
			}
            if (gSaveBlock2Ptr->optionsLanguage == SPA)
			{
				if (PSS_IsNatureBoldOrGentle(nature))
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_841979DSpa);
				else
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_8419782Spa);
			}
        }
		if (gSaveBlock2Ptr->optionsLanguage == ENG)
			AddTextPrinterParameterized3(sMonSummaryScreen->window[4], 2, 6, 0, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_TrainerMemo);
		if (gSaveBlock2Ptr->optionsLanguage == SPA)
			AddTextPrinterParameterized3(sMonSummaryScreen->window[4], 2, 6, 0, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_TrainerMemoSpa);

        AddTextPrinterParameterized4(sMonSummaryScreen->window[4], 2, 16, 12, 0, -2, sPSSTextColours[DARK], TEXT_SPEED_FF, natureMetOrHatchedAtLevelStr);
        return;
    }

    if (sub_813B838(metLocation) == TRUE)
        GetMapNameGeneric_(mapNameStr, metLocation);
    else
	{
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
			StringCopy(mapNameStr, gText_8419C0B);
        if (gSaveBlock2Ptr->optionsLanguage == SPA)
			StringCopy(mapNameStr, gText_8419C0BSpa);
	}		

    DynamicPlaceholderTextUtil_SetPlaceholderPtr(2, mapNameStr);

    if (GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_MET_LEVEL) == 0)
    {
        if (GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_EVENT_LEGAL) == 1)
        {
            if (PSS_IsNatureBoldOrGentle(nature))
			{                
				if (gSaveBlock2Ptr->optionsLanguage == ENG)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84199F4);
				if (gSaveBlock2Ptr->optionsLanguage == SPA)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84199F4Spa);
			}
            else
			{
                if (gSaveBlock2Ptr->optionsLanguage == ENG)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84199AB);
                if (gSaveBlock2Ptr->optionsLanguage == SPA)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84199ABSpa);
			}
        }
        else
        {
            if (PSS_IsNatureBoldOrGentle(nature))
			{
                if (gSaveBlock2Ptr->optionsLanguage == ENG)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_841988A);
                if (gSaveBlock2Ptr->optionsLanguage == SPA)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_841988ASpa);
			}
            else
			{
                if (gSaveBlock2Ptr->optionsLanguage == ENG)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_8419860);
                if (gSaveBlock2Ptr->optionsLanguage == SPA)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_8419860Spa);
			}
        }
    }
    else
    {
        if (metLocation == METLOC_FATEFUL_ENCOUNTER)
        {
            if (PSS_IsNatureBoldOrGentle(nature))
			{
                if (gSaveBlock2Ptr->optionsLanguage == ENG)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84197ED);
                if (gSaveBlock2Ptr->optionsLanguage == SPA)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84197EDSpa);
			}
            else
			{
                if (gSaveBlock2Ptr->optionsLanguage == ENG)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84197B8);
                if (gSaveBlock2Ptr->optionsLanguage == SPA)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_84197B8Spa);
			}
        }
        else
        {
            if (PSS_IsNatureBoldOrGentle(nature))
			{
                if (gSaveBlock2Ptr->optionsLanguage == ENG)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_841988A);
                if (gSaveBlock2Ptr->optionsLanguage == SPA)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_841988ASpa);
			}
            else
			{
                if (gSaveBlock2Ptr->optionsLanguage == ENG)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_8419860);
                if (gSaveBlock2Ptr->optionsLanguage == SPA)
					DynamicPlaceholderTextUtil_ExpandPlaceholders(natureMetOrHatchedAtLevelStr, gText_8419860Spa);
			}
        }
    }
	if (gSaveBlock2Ptr->optionsLanguage == ENG)
		AddTextPrinterParameterized3(sMonSummaryScreen->window[4], 2, 6, 0, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_TrainerMemo);
	if (gSaveBlock2Ptr->optionsLanguage == SPA)
		AddTextPrinterParameterized3(sMonSummaryScreen->window[4], 2, 6, 0, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_TrainerMemoSpa);

    AddTextPrinterParameterized4(sMonSummaryScreen->window[4], 2, 16, 12, 0, -2, sPSSTextColours[DARK], TEXT_SPEED_FF, natureMetOrHatchedAtLevelStr);
}

static void PSS_ShowMonInfo(void)
{
    u8 metLocation;
    u8 version;
    u8 chosenStrIndex = 0;

    metLocation = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_MET_LOCATION);

    if (sMonSummaryScreen->monList.mons != gEnemyParty)
    {
        if (metLocation == METLOC_FATEFUL_ENCOUNTER || GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_EVENT_LEGAL) == 1)
            chosenStrIndex = 4;
        else
        {
            version = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_MET_GAME);

            if (version != VERSION_LEAF_GREEN && version != VERSION_FIRE_RED)
                chosenStrIndex = 1;
            else if (metLocation == METLOC_SPECIAL_EGG)
                chosenStrIndex = 2;

            if (chosenStrIndex == 0 || chosenStrIndex == 2)
                if (sub_813847C(&sMonSummaryScreen->currentMon) == FALSE)
                    chosenStrIndex++;
        }
    }
    else
    {
        if (metLocation == METLOC_FATEFUL_ENCOUNTER || GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_EVENT_LEGAL) == 1)
            chosenStrIndex = 4;
        else
        {
            version = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_MET_GAME);

            if (version != VERSION_LEAF_GREEN && version != VERSION_FIRE_RED)
            {
                if (metLocation == METLOC_SPECIAL_EGG)
                    chosenStrIndex = 5;
            }
            else if (metLocation == METLOC_SPECIAL_EGG)
                chosenStrIndex = 2;

            if (sub_813847C(&sMonSummaryScreen->currentMon) == FALSE)
                chosenStrIndex++;
        }
    }

    if (sMonSummaryScreen->isBadEgg)
        chosenStrIndex = 0;

	if (gSaveBlock2Ptr->optionsLanguage == ENG)
	{
		AddTextPrinterParameterized4(sMonSummaryScreen->window[4], 2, 16, 12, 0, -2, sPSSTextColours[DARK], TEXT_SPEED_FF, sUnknown_8463ED4[chosenStrIndex]);
		AddTextPrinterParameterized3(sMonSummaryScreen->window[4], 2, 6, 0, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_TrainerMemo);
    }
	if (gSaveBlock2Ptr->optionsLanguage == SPA)
	{
		AddTextPrinterParameterized3(sMonSummaryScreen->window[4], 2, 6, 0, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_TrainerMemoSpa);
		AddTextPrinterParameterized4(sMonSummaryScreen->window[4], 2, 16, 12, 0, -2, sPSSTextColours[DARK], TEXT_SPEED_FF, sUnknown_8463ED4[chosenStrIndex + 7]);
	}
}

static void PSS_PrintExpPointAndNextLvTexts(void)
{
    if (gSaveBlock2Ptr->optionsLanguage == ENG)
	{
		AddTextPrinterParameterized3(sMonSummaryScreen->window[4], 2, 26,  7, sPSSTextColours[DARK], TEXT_SPEED_FF, gText_8419C4D);
		AddTextPrinterParameterized3(sMonSummaryScreen->window[4], 2, 26, 20, sPSSTextColours[DARK], TEXT_SPEED_FF, gText_8419C59);
	}
    if (gSaveBlock2Ptr->optionsLanguage == SPA)
	{
		AddTextPrinterParameterized3(sMonSummaryScreen->window[4], 2, 26,  7, sPSSTextColours[DARK], TEXT_SPEED_FF, gText_8419C4DSpa);
		AddTextPrinterParameterized3(sMonSummaryScreen->window[4], 2, 26, 20, sPSSTextColours[DARK], TEXT_SPEED_FF, gText_8419C59Spa);
	}
}

static void PSS_ShowAttackInfo(void)
{
    if (sUnknown_203B16D < 5)
    {
        if (sMonSummaryScreen->mode != PSS_MODE_SELECT_MOVE && sUnknown_203B16D == 4)
            return;
		//Add Category Icon
	//	BlitMoveInfoIcon(sMonSummaryScreen->window[4], gBattleMoves[sMonSummaryScreen->currentMove[sUnknown_203B16D]].category + 47, 85, 0);
		//Add Power
        AddTextPrinterParameterized3(sMonSummaryScreen->window[4], 2, 91, 18, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.power[sUnknown_203B16D]);
		//Add Accuracy
        AddTextPrinterParameterized3(sMonSummaryScreen->window[4], 2, 91, 33, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.unk3188[sUnknown_203B16D]);
		//Add Move Description
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
		{
			AddTextPrinterParameterized4(sMonSummaryScreen->window[4], 2, 14,  2, 0, -2, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_Category);
			AddTextPrinterParameterized4(sMonSummaryScreen->window[4], 2, 14, 17, 0, -2, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_Power);
			AddTextPrinterParameterized4(sMonSummaryScreen->window[4], 2, 14, 32, 0, -2, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_Accuracy);
			AddTextPrinterParameterized4(sMonSummaryScreen->window[4], 2, 10, 48, 0, -2, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_Effect);
			AddTextPrinterParameterized4(sMonSummaryScreen->window[4], 2,  7, 63, 0, -2, sPSSTextColours[DARK], TEXT_SPEED_FF, gMoveDescriptionPointers[sMonSummaryScreen->currentMove[sUnknown_203B16D] - 1]);
        }
		if (gSaveBlock2Ptr->optionsLanguage == SPA)
		{
			AddTextPrinterParameterized4(sMonSummaryScreen->window[4], 2, 14,  2, 0, -2, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_CategorySpa);
			AddTextPrinterParameterized4(sMonSummaryScreen->window[4], 2, 14, 17, 0, -2, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_PowerSpa);
			AddTextPrinterParameterized4(sMonSummaryScreen->window[4], 2, 14, 32, 0, -2, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_AccuracySpa);
			AddTextPrinterParameterized4(sMonSummaryScreen->window[4], 2, 10, 48, 0, -2, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_EffectSpa);
			AddTextPrinterParameterized4(sMonSummaryScreen->window[4], 2,  7, 63, 0, -2, sPSSTextColours[DARK], TEXT_SPEED_FF, gMoveDescriptionPointers[sMonSummaryScreen->currentMove[sUnknown_203B16D] + MOVES_COUNT - 2]);
		}
        PutWindowTilemap(sMonSummaryScreen->window[4]);
    }
}

static void PSS_PrintAbilityNameAndDescription(void)
{
    FillWindowPixelBuffer(sMonSummaryScreen->window[5], 0);
	if (gSaveBlock2Ptr->optionsLanguage == ENG)
		AddTextPrinterParameterized3(sMonSummaryScreen->window[5], 2, 11,  4, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_Ability);
	if (gSaveBlock2Ptr->optionsLanguage == SPA)
		AddTextPrinterParameterized3(sMonSummaryScreen->window[5], 2, 5,  4, sPSSTextColours[WHITE], TEXT_SPEED_FF, gText_PSS_AbilitySpa);
    AddTextPrinterParameterized3(sMonSummaryScreen->window[5], 2, 60,  4, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.abilityName);
    AddTextPrinterParameterized3(sMonSummaryScreen->window[5], 2, 20, 23, sPSSTextColours[DARK], TEXT_SPEED_FF, sMonSummaryScreen->summary.abilityDescription);

}

static void PSS_DrawMoveIcon(void)
{
    u8 i;

    FillWindowPixelBuffer(sMonSummaryScreen->window[5], 0);

    if (gSaveBlock2Ptr->optionsLanguage == ENG)
	{
		for (i = 0; i < 4; i++)
		{
			if (sMonSummaryScreen->currentMove[i] == MOVE_NONE)
				continue;

			BlitMoveInfoIcon(sMonSummaryScreen->window[5], sMonSummaryScreen->move[i] + 1, 8, MACRO_8137270(i) - 2);
		}

		if (sMonSummaryScreen->mode == PSS_MODE_SELECT_MOVE)
			BlitMoveInfoIcon(sMonSummaryScreen->window[5], sMonSummaryScreen->move[4] + 1, 8, MACRO_8137270(4) - 2);
	}
    if (gSaveBlock2Ptr->optionsLanguage == SPA)
	{
		for (i = 0; i < 4; i++)
		{
			if (sMonSummaryScreen->currentMove[i] == MOVE_NONE)
				continue;

			BlitMoveInfoIcon(sMonSummaryScreen->window[5], sMonSummaryScreen->move[i] + 24, 8, MACRO_8137270(i) - 2);
		}

		if (sMonSummaryScreen->mode == PSS_MODE_SELECT_MOVE)
			BlitMoveInfoIcon(sMonSummaryScreen->window[5], sMonSummaryScreen->move[4] + 24, 8, MACRO_8137270(4) - 2);
	}
}

static void sub_8137EE8(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
	PSS_ScrollPSSBackground();
}

static void sub_8137FF4(void)
{
    if (sMonSummaryScreen->unk324C < 240)
    {
        sMonSummaryScreen->unk324C += 240;
        if (sMonSummaryScreen->unk324C > 240)
            sMonSummaryScreen->unk324C = 240;
    }
}

static void sub_813805C(void)
{
    if (sMonSummaryScreen->unk324C >= 240)
    {
        sMonSummaryScreen->unk324C -= 240;
        if (sMonSummaryScreen->unk324C < 0)
            sMonSummaryScreen->unk324C = 0;
    }
}

static void sub_813817C(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();

    if (sMonSummaryScreen->unk3244 == FALSE)
        return;

    sub_81380F0();
}

static void sub_8138280(u8 curPageIndex)
{
    u8 i;
    for (i = 0; i < 3; i++)
		sMonSummaryScreen->window[i] = AddWindow(&WindowTemplatePokemonInfo[i]);
    for (i = 0; i < 4; i++)
        switch (curPageIndex)
        {
        case PSS_PAGE_INFO:
            sMonSummaryScreen->window[i + 3] = AddWindow(&sDataMonAndNatureWindowTemplate[i]);
            break;
        case PSS_PAGE_SKILLS:
        default:
            sMonSummaryScreen->window[i + 3] = AddWindow(&sMonStatsAndAbilityWindowTemplate[i]);
            break;
        case PSS_PAGE_MOVES:
        case PSS_PAGE_MOVES_INFO:
            sMonSummaryScreen->window[i + 3] = AddWindow(&sMovesInfoWindowTemplate[i]);
            break;
        }
}

static void sub_8138538(void)
{
    switch (sMonSummaryScreen->curPageIndex)
    {
    case PSS_PAGE_INFO:
        if (!sMonSummaryScreen->isEgg)
        {
            LZ77UnCompVram(gMapSummaryScreenPokemonInfo, (void *)(VRAM + 0xF000));
        }
        else
        {
            LZ77UnCompVram(gMapSummaryScreenEgg, (void *)(VRAM + 0xF000));
        }
		PSS_SetInvisibleHpBar(1);
		PSS_SetInvisibleExpBar(1);
        break;
    case PSS_PAGE_SKILLS:
        LZ77UnCompVram(gMapSummaryScreenPokemonSkills, (void *)(VRAM + 0xF000));
		PSS_SetInvisibleHpBar(0);
		PSS_SetInvisibleExpBar(0);
		HideBg(3);
        break;
    case PSS_PAGE_MOVES:
        LZ77UnCompVram(gMapSummaryScreenKnownMoves, (void *)(VRAM + 0xF000));
        LZ77UnCompVram(gMapSummaryScreenMoves, (void *)(VRAM + 0xE000));
		PSS_SetInvisibleHpBar(1);
		PSS_SetInvisibleExpBar(1);
		ShowBg(3);
        break;
    case PSS_PAGE_MOVES_INFO:
		LZ77UnCompVram(gMapSummaryScreenMovesInfo, (void *)(VRAM + 0xF000));
        LZ77UnCompVram(gMapSummaryScreenMoves, (void *)(VRAM + 0xE000));
		PSS_SetInvisibleHpBar(1);
		PSS_SetInvisibleExpBar(1);
		ShowBg(3);
        break;
    }
}

static void PSS_DrawMonMoveIcon(void)
{
    switch (sMonSummaryScreen->curPageIndex)
    {
    case PSS_PAGE_INFO:
        if (gSaveBlock2Ptr->optionsLanguage == ENG)
		{
			if (!sMonSummaryScreen->isEgg)
			{
				BlitMoveInfoIcon(sMonSummaryScreen->window[3], sMonSummaryScreen->typeIcons[0] + 1, 78, 33);

				if (sMonSummaryScreen->typeIcons[0] != sMonSummaryScreen->typeIcons[1])
                BlitMoveInfoIcon(sMonSummaryScreen->window[3], sMonSummaryScreen->typeIcons[1] + 1, 110, 33);
			}
		}
        if (gSaveBlock2Ptr->optionsLanguage == SPA)
		{
			if (!sMonSummaryScreen->isEgg)
			{
				BlitMoveInfoIcon(sMonSummaryScreen->window[3], sMonSummaryScreen->typeIcons[0] + 24, 78, 33);

				if (sMonSummaryScreen->typeIcons[0] != sMonSummaryScreen->typeIcons[1])
                BlitMoveInfoIcon(sMonSummaryScreen->window[3], sMonSummaryScreen->typeIcons[1] + 24, 110, 33);
			}
		}
        break;
    case PSS_PAGE_SKILLS:
        break;
    case PSS_PAGE_MOVES:
        break;
    case PSS_PAGE_MOVES_INFO:
        break;
    }
}

static void sub_8138CD8(u8 id)
{
    u8 i;

    switch (sMonSummaryScreen->unk3288)
    {
    case 0:
        if (MenuHelpers_CallLinkSomething() == TRUE || sub_800B270() == TRUE)
            return;

        if (JOY_NEW(DPAD_UP))
        {
            if (sUnknown_203B16D > 0)
            {
                sMonSummaryScreen->unk3288 = 2;
                PlaySE(SE_SELECT);

                for (i = sUnknown_203B16D; i > 0; i--)
                    if (sMonSummaryScreen->currentMove[i - 1] != 0)
                    {
                        PlaySE(SE_SELECT);
                        sUnknown_203B16D = i - 1;
                        return;
                    }
            }
            else
            {
                sUnknown_203B16D = 4;
                sMonSummaryScreen->unk3288 = 2;
                PlaySE(SE_SELECT);

                if (sMonSummaryScreen->unk3268 == TRUE)
                    for (i = sUnknown_203B16D; i > 0; i--)
                        if (sMonSummaryScreen->currentMove[i - 1] != 0)
                        {
                            PlaySE(SE_SELECT);
                            sUnknown_203B16D = i - 1;
                            return;
                        }
            }
        }
        else if (JOY_NEW(DPAD_DOWN))
        {
            if (sUnknown_203B16D < 4)
            {
                u8 v0 = 4;

                sMonSummaryScreen->unk3288 = 2;

                if (sMonSummaryScreen->unk3268 == TRUE)
                {
                    if (sUnknown_203B16D == 5 - 2)
                    {
                        sUnknown_203B16D = 0;
                        sMonSummaryScreen->unk3288 = 2;
                        PlaySE(SE_SELECT);
                        return;
                    }
                    v0--;
                }

                for (i = sUnknown_203B16D; i < v0; i++)
                    if (sMonSummaryScreen->currentMove[i + 1] != 0)
                    {
                        PlaySE(SE_SELECT);
                        sUnknown_203B16D = i + 1;
                        return;
                    }

                if (!sMonSummaryScreen->unk3268)
                {
                    PlaySE(SE_SELECT);
                    sUnknown_203B16D = i;
                }
                else
                {
                    PlaySE(SE_SELECT);
                    sUnknown_203B16D = 0;
                }

                return;
            }
            else if (sUnknown_203B16D == 4)
            {
                sUnknown_203B16D = 0;
                sMonSummaryScreen->unk3288 = 2;
                PlaySE(SE_SELECT);
                return;
            }
        }
        else if (JOY_NEW(A_BUTTON))
        {
            PlaySE(SE_SELECT);
            if (sUnknown_203B16D == 4)
            {
                sUnknown_203B16D = 0;
                sUnknown_203B16E = 0;
                sMonSummaryScreen->unk3268 = FALSE;
                sub_813A0E8(1);
                sMonSummaryScreen->unk3224 = 0;
                PSS_RemoveAllWindows(sMonSummaryScreen->curPageIndex);
                sMonSummaryScreen->curPageIndex--;
                sMonSummaryScreen->unk3288 = 1;
                return;
            }

            if (sMonSummaryScreen->unk3268 != TRUE)
            {
                if (sMonSummaryScreen->isEnemyParty == FALSE
                    && gMain.inBattle == 0
                    && gReceivedRemoteLinkPlayers == 0)
                {
                    sUnknown_203B16E = sUnknown_203B16D;
                    sMonSummaryScreen->unk3268 = TRUE;
                }
                return;
            }
            else
            {
                sMonSummaryScreen->unk3268 = FALSE;

                if (sUnknown_203B16D == sUnknown_203B16E)
                    return;

                if (sMonSummaryScreen->isBoxMon == 0)
                    sub_81390B0();
                else
                    sub_81391EC();

                sub_8139328(&sMonSummaryScreen->currentMon);
                sub_81367B0();
                sMonSummaryScreen->unk3288 = 2;
                return;
            }
        }
        else if (JOY_NEW(B_BUTTON))
        {
            if (sMonSummaryScreen->unk3268 == TRUE)
            {
                sUnknown_203B16E = sUnknown_203B16D;
                sMonSummaryScreen->unk3268 = FALSE;
                return;
            }

            if (sUnknown_203B16D == 4)
            {
                sUnknown_203B16D = 0;
                sUnknown_203B16E = 0;
            }

            sub_813A0E8(1);
            sMonSummaryScreen->unk3224 = 0;
            PSS_RemoveAllWindows(sMonSummaryScreen->curPageIndex);
            sMonSummaryScreen->curPageIndex--;
            sMonSummaryScreen->unk3288 = 1;
        }
        break;
    case 1:
        gTasks[sMonSummaryScreen->task].func = sub_81351A0;
        sMonSummaryScreen->unk3288 = 0;
        break;
    case 2:
        PSS_AddTextToWin3();
        PSS_AddTextToWin4();
        PSS_AddTextToWin5();
        sMonSummaryScreen->unk3288 = 3;
        break;
    case 3:
        if (MenuHelpers_CallLinkSomething() == TRUE || sub_800B270() == TRUE)
            return;

        CopyWindowToVram(sMonSummaryScreen->window[3], 2);
        CopyWindowToVram(sMonSummaryScreen->window[4], 2);
        CopyWindowToVram(sMonSummaryScreen->window[5], 2);
        CopyWindowToVram(sMonSummaryScreen->window[6], 2);
        CopyBgTilemapBufferToVram(0);
        sMonSummaryScreen->unk3288 = 0;
        break;
    default:
        break;
    }
}

static void sub_81393D4(u8 taskId)
{
    u8 i;

    switch (sMonSummaryScreen->unk3288)
    {
    case 0:
        BeginNormalPaletteFade(0xffffffff, 0, 16, 0, 0);
        sMonSummaryScreen->unk3288++;
        break;
    case 1:
        if (!gPaletteFade.active)
        {
            PSS_PlayMonCry();
            sMonSummaryScreen->unk3288++;
        }
        break;
    case 2:
        if (JOY_NEW(DPAD_UP))
        {
            if (sUnknown_203B16D > 0)
            {
                sMonSummaryScreen->unk3288 = 3;
                PlaySE(SE_SELECT);
                for (i = sUnknown_203B16D; i > 0; i--)
                    if (sMonSummaryScreen->currentMove[i - 1] != 0)
                    {
                        PlaySE(SE_SELECT);
                        sUnknown_203B16D = i - 1;
                        return;
                    }
            }
            else
            {
                sUnknown_203B16D = 4;
                sMonSummaryScreen->unk3288 = 3;
                PlaySE(SE_SELECT);
                return;
            }
        }
        else if (JOY_NEW(DPAD_DOWN))
        {
            if (sUnknown_203B16D < 4)
            {
                u8 v0 = 4;

                sMonSummaryScreen->unk3288 = 3;

                if (sMonSummaryScreen->unk3268 == TRUE)
                    v0--;

                for (i = sUnknown_203B16D; i < v0; i++)
                    if (sMonSummaryScreen->currentMove[i + 1] != 0)
                    {
                        PlaySE(SE_SELECT);
                        sUnknown_203B16D = i + 1;
                        return;
                    }

                if (!sMonSummaryScreen->unk3268)
                {
                    PlaySE(SE_SELECT);
                    sUnknown_203B16D = i;
                }

                return;
            }
            else if (sUnknown_203B16D == 4)
            {
                sUnknown_203B16D = 0;
                sMonSummaryScreen->unk3288 = 3;
                PlaySE(SE_SELECT);
                return;
            }
        }
        else if (JOY_NEW(A_BUTTON))
        {
            if (sub_8139388() == TRUE || sUnknown_203B16D == 4)
            {
                PlaySE(SE_SELECT);
                sUnknown_203B16E = sUnknown_203B16D;
                gSpecialVar_0x8005 = sUnknown_203B16E;
                sMonSummaryScreen->unk3288 = 6;
            }
            else
            {
                PlaySE(SE_FAILURE);
                sMonSummaryScreen->unk3288 = 5;
            }
        }
        else if (JOY_NEW(B_BUTTON))
        {
            sUnknown_203B16E = 4;
            gSpecialVar_0x8005 = (u16)sUnknown_203B16E;
            sMonSummaryScreen->unk3288 = 6;
        }
        break;
    case 3:
        PSS_AddTextToWin3();
        PSS_AddTextToWin4();
        PSS_AddTextToWin5();
        sMonSummaryScreen->unk3288 = 4;
        break;
    case 4:
        if (MenuHelpers_CallLinkSomething() == TRUE || sub_800B270() == TRUE)
            return;

        CopyWindowToVram(sMonSummaryScreen->window[3], 2);
        CopyWindowToVram(sMonSummaryScreen->window[4], 2);
        CopyWindowToVram(sMonSummaryScreen->window[5], 2);
        CopyWindowToVram(sMonSummaryScreen->window[6], 2);
        CopyBgTilemapBufferToVram(0);
        sMonSummaryScreen->unk3288 = 2;
        break;
    case 5:
        FillWindowPixelBuffer(sMonSummaryScreen->window[4], 0);
        AddTextPrinterParameterized4(sMonSummaryScreen->window[4], 2,
                                     7, 42,
                                     0, 0,
                                     sPSSTextColours[DARK], TEXT_SPEED_FF,
                                     gText_8419CB9);
        CopyWindowToVram(sMonSummaryScreen->window[4], 2);
        CopyBgTilemapBufferToVram(0);
        sMonSummaryScreen->unk3288 = 2;
        break;
    case 6:
        BeginNormalPaletteFade(0xffffffff, 0, 0, 16, 0);
        sMonSummaryScreen->unk3288++;
        break;
    default:
        if (!gPaletteFade.active)
            sub_8137E64(taskId);
        break;
    }
}

static void PSS_LoadMonSprite(void)
{
    u16 spriteId;
    u16 species;
    u32 personality;
    u32 trainerId;

    sUnknown_203B170 = AllocZeroed(sizeof(struct Struct203B170));

    species = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPECIES2);
    personality = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_PERSONALITY);
    trainerId = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_OT_ID);

    if (sMonSummaryScreen->savedCallback == CB2_ReturnToTradeMenuFromSummary)
    {
        if (sMonSummaryScreen->isEnemyParty == TRUE)
            spriteId = CreateMonPicSprite(species, trainerId, personality, 1, 204, 78, 12, 0xffff, 1);
        else
            spriteId = CreateMonPicSprite_HandleDeoxys(species, trainerId, personality, 1, 204, 78, 12, 0xffff);
    }
    else
    {
        if (ShouldIgnoreDeoxysForm(3, sLastViewedMonIndex))
            spriteId = CreateMonPicSprite(species, trainerId, personality, 1, 204, 78, 12, 0xffff, 1);
        else
            spriteId = CreateMonPicSprite_HandleDeoxys(species, trainerId, personality, 1, 204, 78, 12, 0xffff);
    }

    FreeSpriteOamMatrix(&gSprites[spriteId]);
    sMonSummaryScreen->spriteId_1 = spriteId;
    PSS_SetInvisibleMonSprite(1);
    sub_8139AAC(spriteId);
}

static void PSS_LoadPokeball(void)
{
    u16 ballItemId;
    u8 ballId;

    if (!sMonSummaryScreen->isEgg)
        ballItemId = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_POKEBALL);
    else
        ballItemId = 0;

    ballId = ItemIdToBallId(ballItemId);
    LoadBallGfx(ballId);

    sMonSummaryScreen->spriteId_0 = CreateSprite(&gBallSpriteTemplates[ballId], 232, 39, 0);
    gSprites[sMonSummaryScreen->spriteId_0].callback = SpriteCallbackDummy;
    gSprites[sMonSummaryScreen->spriteId_0].oam.priority = 0;

    PSS_SetInvisiblePokeball(1);
}

static void PSS_LoadMonIcon(void)
{
    u16 species;
    u32 personality;

    species = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SPECIES2);
    personality = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_PERSONALITY);

    SafeLoadMonIconPalette(species);

    if (sMonSummaryScreen->savedCallback == CB2_ReturnToTradeMenuFromSummary)
    {
        if (sMonSummaryScreen->isEnemyParty == TRUE)
            sMonSummaryScreen->spriteId_2 = CreateMonIcon(species, SpriteCallbackDummy, 140, 28, 0, personality, 0);
        else
            sMonSummaryScreen->spriteId_2 = CreateMonIcon(species, SpriteCallbackDummy, 140, 28, 0, personality, 1);
    }
    else
    {
        if (ShouldIgnoreDeoxysForm(3, sLastViewedMonIndex))
            sMonSummaryScreen->spriteId_2 = CreateMonIcon(species, SpriteCallbackDummy, 140, 28, 0, personality, 0);
        else
            sMonSummaryScreen->spriteId_2 = CreateMonIcon(species, SpriteCallbackDummy, 140, 28, 0, personality, 1);
    }

    if (!IsPokeSpriteNotFlipped(species))
        gSprites[sMonSummaryScreen->spriteId_2].hFlip = FALSE;
    else
        gSprites[sMonSummaryScreen->spriteId_2].hFlip = TRUE;

    sub_8139EE4(1);
}

static void PSS_LoadMovesSelectCursor(u16 tileTag, u16 palTag)
{
    u8 i;
    u8 spriteId;
    void * gfxBufferPtrs[2];
    gfxBufferPtrs[0] = AllocZeroed(0x20 * 64);
    gfxBufferPtrs[1] = AllocZeroed(0x20 * 64);

    sUnknown_203B148[0] = AllocZeroed(sizeof(struct Struct203B148));
    sUnknown_203B148[1] = AllocZeroed(sizeof(struct Struct203B148));
    sUnknown_203B148[2] = AllocZeroed(sizeof(struct Struct203B148));
    sUnknown_203B148[3] = AllocZeroed(sizeof(struct Struct203B148));

    LZ77UnCompWram(gSelectCursorGfxLeft, gfxBufferPtrs[0]);
    LZ77UnCompWram(gSelectCursorGfxRight, gfxBufferPtrs[1]);

    for (i = 0; i < 4; i++)
    {
        struct SpriteSheet sheet = {
            .data = gfxBufferPtrs[i % 2],
            .size = 0x20 * 64,
            .tag = tileTag + i
        };

        struct SpritePalette palette = {.data = gSelectCursorPalette, .tag = palTag};
        struct SpriteTemplate template = {
            .tileTag = tileTag + i,
            .paletteTag = palTag,
            .oam = &sUnknown_846398C,
            .anims = sUnknown_84639A4,
            .images = NULL,
            .affineAnims = gDummySpriteAffineAnimTable,
            .callback = sub_813A124,
        };

        LoadSpriteSheet(&sheet);
        LoadSpritePalette(&palette);

        spriteId = CreateSprite(&template, 64 * (i % 2) + 32, sUnknown_203B16D * 28 + 34, i % 2);
        sUnknown_203B148[i]->sprite = &gSprites[spriteId];
        sUnknown_203B148[i]->tileTag = i;
        sUnknown_203B148[i]->palTag = tileTag + i;
        sUnknown_203B148[i]->unk08 = palTag;
        sUnknown_203B148[i]->sprite->subpriority = i;

        if (i > 1)
            StartSpriteAnim(sUnknown_203B148[i]->sprite, 1);
    }

    sub_813A0E8(1);

    FREE_AND_SET_NULL_IF_SET(gfxBufferPtrs[0]);
    FREE_AND_SET_NULL_IF_SET(gfxBufferPtrs[1]);
}

static void PSS_ShowIconStatus(void)
{
    sMonSummaryScreen->monStatus = PSS_CheckMonStatus(GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_STATUS));

    if (sMonSummaryScreen->monStatus == AILMENT_NONE)
    {
        PSS_SetInvisibleIconStatus(1);
        return;
    }

    StartSpriteAnim(sStatusIconSummaryScreen->sprite, sMonSummaryScreen->monStatus - 1);
    PSS_SetInvisibleIconStatus(0);
}

static void PSS_SetInvisibleIconStatus(u8 invisible)
{
    if (sMonSummaryScreen->monStatus == AILMENT_NONE || sMonSummaryScreen->isEgg)
        sStatusIconSummaryScreen->sprite->invisible = TRUE;
    else
        sStatusIconSummaryScreen->sprite->invisible = invisible;

    if (sMonSummaryScreen->curPageIndex == PSS_PAGE_MOVES_INFO)
    {
        if (sStatusIconSummaryScreen->sprite->pos1.y != 45)
        {
            sStatusIconSummaryScreen->sprite->pos1.x = 16;
            sStatusIconSummaryScreen->sprite->pos1.y = 45;
            return;
        }
    }
    else if (sStatusIconSummaryScreen->sprite->pos1.y != 38)
    {
        sStatusIconSummaryScreen->sprite->pos1.x = 214;
        sStatusIconSummaryScreen->sprite->pos1.y = 38;
        return;
    }
}

static void PSS_LoadHpBar(u16 tileTag, u16 palTag)
{
    u8 i;
    u8 spriteId;
    void * gfxBufferPtr;
    u32 curHp;
    u32 maxHp;
    u8 hpBarPalTagOffset = 0;

    sHpBarSummaryScreen = AllocZeroed(sizeof(struct Struct203B15C));
    gfxBufferPtr = AllocZeroed(0x20 * 12);
    if (gSaveBlock2Ptr->optionsLanguage == ENG)
		LZ77UnCompWram(gHpBarSummaryScreen, gfxBufferPtr);
    if (gSaveBlock2Ptr->optionsLanguage == SPA)
		LZ77UnCompWram(gHpBarSummaryScreenSpa, gfxBufferPtr);

    curHp = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_HP);
    maxHp = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_MAX_HP);

    if (maxHp / 4 > curHp)
        hpBarPalTagOffset = 2;
    else if (maxHp / 2 > curHp)
        hpBarPalTagOffset = 1;

    if (gfxBufferPtr != NULL)
    {
        struct SpriteSheet sheet = {
            .data = gfxBufferPtr,
            .size = 0x20 * 12,
            .tag = tileTag
        };

        struct SpritePalette palette1 = {.data = sHpBarPalettes[0], .tag = palTag};
        struct SpritePalette palette2 = {.data = sHpBarPalettes[1], .tag = palTag + 1};
        struct SpritePalette palette3 = {.data = sHpBarPalettes[2], .tag = palTag + 2};

        LoadSpriteSheet(&sheet);
        LoadSpritePalette(&palette1);
        LoadSpritePalette(&palette2);
        LoadSpritePalette(&palette3);
    }

    for (i = 0; i < 9; i++)
    {
        struct SpriteTemplate template = {
            .tileTag = tileTag,
            .paletteTag = palTag + hpBarPalTagOffset,
            .oam = &sUnknown_8463A14,
            .anims = sUnknown_8463A7C,
            .images = NULL,
            .affineAnims = gDummySpriteAffineAnimTable,
            .callback = SpriteCallbackDummy,
        };

        sHpBarSummaryScreen->cordX[i] = i * 8 + 81;
        spriteId = CreateSprite(&template, sHpBarSummaryScreen->cordX[i], 33, 0);
        sHpBarSummaryScreen->sprites[i] = &gSprites[spriteId];
        sHpBarSummaryScreen->sprites[i]->invisible = FALSE;
        sHpBarSummaryScreen->sprites[i]->oam.priority = 2;
        sHpBarSummaryScreen->unk3C = tileTag;
        sHpBarSummaryScreen->unk3E = palTag;
        StartSpriteAnim(sHpBarSummaryScreen->sprites[i], 8);
    }

    PSS_SetupHpBar();
    PSS_SetInvisibleHpBar(1);

    FREE_AND_SET_NULL_IF_SET(gfxBufferPtr);
}

// The change in this one is minimal, may it doesn't need to be hooked
static void PSS_LoadExpBar(u16 tileTag, u16 palTag)
{
    u8 i;
    u8 spriteId;
    void * gfxBufferPtr;

    sExpBarSummaryScreen = AllocZeroed(sizeof(struct Struct203B160));
    gfxBufferPtr = AllocZeroed(0x20 * 12);

    LZ77UnCompWram(gExpBarSummaryScreen, gfxBufferPtr);
    if (gfxBufferPtr != NULL)
    {
        struct SpriteSheet sheet = {
            .data = gfxBufferPtr,
            .size = 0x20 * 12,
            .tag = tileTag
        };

        struct SpritePalette palette = {.data = gPal0SummaryScreen, .tag = palTag};
        LoadSpriteSheet(&sheet);
        LoadSpritePalette(&palette);
    }

    for (i = 0; i < 11; i++)
    {
        struct SpriteTemplate template = {
            .tileTag = tileTag,
            .paletteTag = palTag,
            .oam = &sUnknown_8463A14,
            .anims = sUnknown_8463A7C,
            .images = NULL,
            .affineAnims = gDummySpriteAffineAnimTable,
            .callback = SpriteCallbackDummy,
        };

        sExpBarSummaryScreen->cordX[i] = i * 8 + 156;
        spriteId = CreateSprite(&template, sExpBarSummaryScreen->cordX[i], 130, 0);
        sExpBarSummaryScreen->sprites[i] = &gSprites[spriteId];
        sExpBarSummaryScreen->sprites[i]->oam.priority = 2;
        sExpBarSummaryScreen->tileTag = tileTag;
        sExpBarSummaryScreen->palTag = palTag;
    }

    PSS_SetupExpBar();
    PSS_SetInvisibleExpBar(1);

    FREE_AND_SET_NULL_IF_SET(gfxBufferPtr);
}

static void PSS_LoadShinyIndicator(u16 tileTag, u16 palTag)
{
    u16 spriteId;
    void * gfxBufferPtr;

    sUnknown_203B168 = AllocZeroed(sizeof(struct Struct203B168));
    gfxBufferPtr = AllocZeroed(0x20 * 2);

    LZ77UnCompWram(gShinyIndicatorGfx, gfxBufferPtr);

    if (sUnknown_203B168 != NULL)
    {
        struct SpriteSheet sheet = {
            .data = gfxBufferPtr,
            .size = 0x20 * 2,
            .tag = tileTag
        };

        struct SpritePalette palette = {.data = gShinyIndicatorPalette, .tag = palTag};
        struct SpriteTemplate template = {
            .tileTag = tileTag,
            .paletteTag = palTag,
            .oam = &sUnknown_8463B30,
            .anims = sUnknown_8463B40,
            .images = NULL,
            .affineAnims = gDummySpriteAffineAnimTable,
            .callback = SpriteCallbackDummy,
        };

        LoadSpriteSheet(&sheet);
        LoadSpritePalette(&palette);
        spriteId = CreateSprite(&template, 166, 50, 0);
        sUnknown_203B168->sprite = &gSprites[spriteId];
        sUnknown_203B168->tileTag = tileTag;
        sUnknown_203B168->palTag = palTag;
    }

    sub_813AEB0(1);
    sub_813AF50();

    FREE_AND_SET_NULL_IF_SET(gfxBufferPtr);
}

static void sub_813AEB0(u8 invisible)
{
    if (IsMonShiny(&sMonSummaryScreen->currentMon) == TRUE
        && !sMonSummaryScreen->isEgg)
        sUnknown_203B168->sprite->invisible = invisible;
    else
        sUnknown_203B168->sprite->invisible = TRUE;

    if (sMonSummaryScreen->curPageIndex == PSS_PAGE_MOVES_INFO)
    {
        sUnknown_203B168->sprite->pos1.x = 126;
        sUnknown_203B168->sprite->pos1.y = 20;
    }
    else
    {
        sUnknown_203B168->sprite->pos1.x = 166;
        sUnknown_203B168->sprite->pos1.y = 50;
    }
}

static void PSS_LoadMarkings(void)
{
    u32 markings = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_MARKINGS);

    DestroySpriteAndFreeResources(sMonSummaryScreen->markingSprite);
    sMonSummaryScreen->markingSprite = CreateMonMarkingSprite_SelectCombo(TAG_PSS_UNK_8C, TAG_PSS_UNK_8C, sUnknown_84636E0);

    if (sMonSummaryScreen->markingSprite != NULL)
    {
        StartSpriteAnim(sMonSummaryScreen->markingSprite, markings);
        sMonSummaryScreen->markingSprite->pos1.x = 208;
        sMonSummaryScreen->markingSprite->pos1.y = 114;
    }

    PSS_SetInvisibleMarkings(1);
}

static void sub_813B3F0(u8 id)
{
    switch (sMonSummaryScreen->unk328C)
    {
    case 0:
        StopCryAndClearCrySongs();
        sUnknown_203B16D = 0;
        sUnknown_203B16E = 0;
        sMonSummaryScreen->unk328C++;
        break;
    case 1:
        PSS_UnloadMonSprite();
        sub_8139F20();
        sub_8139D90();
        sMonSummaryScreen->unk328C++;
        break;
    case 2:
        sub_8138B8C(&sMonSummaryScreen->currentMon);

        sMonSummaryScreen->isEgg = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_IS_EGG);
        sMonSummaryScreen->isBadEgg = GetMonData(&sMonSummaryScreen->currentMon, MON_DATA_SANITY_IS_BAD_EGG);

        if (sMonSummaryScreen->isBadEgg == TRUE)
            sMonSummaryScreen->isEgg = TRUE;

        sMonSummaryScreen->unk328C++;
        break;
    case 3:
        sMonSummaryScreen->unk328C++;
        break;
    case 4:
        sMonSummaryScreen->unk328C++;
        break;
    case 5:
        PSS_GetDataPokemon();
        sMonSummaryScreen->unk328C++;
        break;
    case 6:
        if (!sMonSummaryScreen->isEgg)
            PSS_GetStatsPokemon();

        sMonSummaryScreen->unk328C++;
        break;
    case 7:
        if (!sMonSummaryScreen->isEgg)
            sub_81367B0();

        sMonSummaryScreen->unk328C++;
        break;
    case 8:
        PSS_AddTextToWin3();
        PSS_AddTextToWin4();
        PSS_AddTextToWin5();
        sMonSummaryScreen->unk328C++;
        break;
    case 9:
        PSS_DrawMonMoveIcon();
        sub_8138538();
        sub_8137D28(sMonSummaryScreen->curPageIndex);
        sMonSummaryScreen->unk328C++;
        break;
    case 10:
        CopyWindowToVram(sMonSummaryScreen->window[0], 2);
        CopyWindowToVram(sMonSummaryScreen->window[1], 2);
        CopyWindowToVram(sMonSummaryScreen->window[2], 2);
        CopyWindowToVram(sMonSummaryScreen->window[3], 2);
        CopyWindowToVram(sMonSummaryScreen->window[4], 2);
        CopyWindowToVram(sMonSummaryScreen->window[5], 2);
        CopyWindowToVram(sMonSummaryScreen->window[6], 2);
        CopyBgTilemapBufferToVram(0);
        sMonSummaryScreen->unk328C++;
        break;
    case 11:
        if (!Overworld_LinkRecvQueueLengthMoreThan2() && !sub_800B270())
        {
            sub_813AFC4();
            PSS_PlayMonCry();
            sMonSummaryScreen->unk328C++;
        }
        break;
    default:
        sMonSummaryScreen->unk328C = 0;
        DestroyTask(id);
        break;
    }
}

static void PSS_ScrollPSSBackground(void)
{
    ChangeBgX(2, 80, 1);
    ChangeBgY(2, 80, 1);
}
